/*
 * vmclamp.h: pure function that keeps process memory counters below physical RAM.
 *
 * Why: on a 24 GB Mac the game reports a memory use of about 150 GB (research.md, section 3).
 * Which counter feeds that number is not proven. This file only holds the arithmetic,
 * so a host test can run it.
 * The adapter that copies a real counter struct into vmc_values is a later step.
 *
 * Rule: a value above (phys_ram - margin) is replaced by the working set, or by that limit
 * if the working set is zero or itself above the limit. Values at or below the limit stay.
 * Peak values never end below their current value.
 *
 * Test: clang -std=c11 -Wall -Wextra -Werror -o /tmp/vmclamp_test proxy/vmclamp_test.c && /tmp/vmclamp_test
 * License: MIT (see LICENSE).
 */
#ifndef VMCLAMP_H
#define VMCLAMP_H

#include <stdint.h>

typedef struct {
    uint64_t private_usage;        /* only valid when has_private is set */
    uint64_t pagefile_usage;
    uint64_t peak_pagefile_usage;
    uint64_t virtual_size;
    uint64_t peak_virtual_size;
    uint64_t working_set;
    uint64_t peak_working_set;
    int has_private;
} vmc_values;

/* Set *x to "to" if it is above limit. Return 1 when it changed. */
static inline int vmc_cap(uint64_t *x, uint64_t limit, uint64_t to)
{
    if (*x <= limit) return 0;
    *x = to;
    return 1;
}

/* Raise *peak to at least cur. Return 1 when it changed. */
static inline int vmc_raise(uint64_t *peak, uint64_t cur)
{
    if (*peak >= cur) return 0;
    *peak = cur;
    return 1;
}

/*
 * Return the number of fields changed, or -1 when an argument is invalid
 * (null pointer, phys_ram of zero, margin of zero or not below phys_ram). On -1 nothing changes.
 * A margin of zero is invalid because a result equal to phys_ram can still trip a used >= limit check.
 */
static inline int vmclamp(vmc_values *v, uint64_t phys_ram, uint64_t margin)
{
    if (!v || phys_ram == 0 || margin == 0 || margin >= phys_ram) return -1;

    const uint64_t limit = phys_ram - margin;
    int n = 0;

    n += vmc_cap(&v->working_set, limit, limit);
    n += vmc_cap(&v->peak_working_set, limit, limit);
    n += vmc_raise(&v->peak_working_set, v->working_set);

    const uint64_t to = (v->working_set > 0) ? v->working_set : limit;

    if (v->has_private) n += vmc_cap(&v->private_usage, limit, to);
    n += vmc_cap(&v->pagefile_usage, limit, to);
    n += vmc_cap(&v->peak_pagefile_usage, limit, to);
    n += vmc_raise(&v->peak_pagefile_usage, v->pagefile_usage);

    /* Virtual size stays at or above private usage. */
    const uint64_t vto = (v->has_private && v->private_usage > to) ? v->private_usage : to;
    n += vmc_cap(&v->virtual_size, limit, vto);
    n += vmc_cap(&v->peak_virtual_size, limit, vto);
    n += vmc_raise(&v->peak_virtual_size, v->virtual_size);

    return n;
}

#endif
