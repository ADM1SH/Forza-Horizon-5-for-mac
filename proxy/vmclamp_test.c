/*
 * Host test for vmclamp.h. Runs on the Mac. No bottle, no Wine.
 * Build and run:
 *   clang -std=c11 -Wall -Wextra -Werror -o /tmp/vmclamp_test proxy/vmclamp_test.c && /tmp/vmclamp_test
 * License: MIT (see LICENSE).
 */
#include <stdio.h>
#include <stdint.h>
#include "vmclamp.h"

#define GIB ((uint64_t)1 << 30)
#define PHYS (24 * GIB)
#define MARGIN (2 * GIB)

static int failures;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); failures++; } } while (0)

static vmc_values sample(void)
{
    vmc_values v = { 0 };
    v.private_usage = 6 * GIB; v.pagefile_usage = 6 * GIB; v.peak_pagefile_usage = 7 * GIB;
    v.virtual_size = 10 * GIB; v.peak_virtual_size = 11 * GIB;
    v.working_set = 4 * GIB; v.peak_working_set = 5 * GIB;
    v.has_private = 1;
    return v;
}

static void test_normal_values_unchanged(void)
{
    vmc_values v = sample(), w = sample();
    CHECK(vmclamp(&v, PHYS, MARGIN) == 0);
    CHECK(v.private_usage == w.private_usage && v.virtual_size == w.virtual_size);
    CHECK(v.peak_pagefile_usage == w.peak_pagefile_usage && v.working_set == w.working_set);
}

static void test_huge_private_is_clamped_below_ram(void)
{
    vmc_values v = sample();
    v.private_usage = 149 * GIB; v.pagefile_usage = 150 * GIB; v.peak_pagefile_usage = 151 * GIB;
    CHECK(vmclamp(&v, PHYS, MARGIN) == 3);
    CHECK(v.private_usage < PHYS && v.pagefile_usage < PHYS && v.peak_pagefile_usage < PHYS);
    CHECK(v.private_usage == v.working_set);
}

static void test_huge_virtual_is_clamped(void)
{
    vmc_values v = sample();
    v.virtual_size = 140 * GIB; v.peak_virtual_size = 141 * GIB;
    CHECK(vmclamp(&v, PHYS, MARGIN) == 2);
    CHECK(v.virtual_size < PHYS && v.peak_virtual_size < PHYS);
}

static void test_working_set_larger_than_ram(void)
{
    vmc_values v = sample();
    v.working_set = 30 * GIB; v.peak_working_set = 31 * GIB; v.private_usage = 30 * GIB;
    CHECK(vmclamp(&v, PHYS, MARGIN) >= 3);
    CHECK(v.working_set == PHYS - MARGIN && v.private_usage == PHYS - MARGIN);
    CHECK(v.working_set < PHYS);
}

static void test_peaks_not_below_current(void)
{
    vmc_values v = sample();
    v.private_usage = 149 * GIB; v.pagefile_usage = 150 * GIB; v.peak_pagefile_usage = 1 * GIB;
    v.virtual_size = 140 * GIB; v.peak_virtual_size = 1 * GIB;
    vmclamp(&v, PHYS, MARGIN);
    CHECK(v.peak_pagefile_usage >= v.pagefile_usage);
    CHECK(v.peak_virtual_size >= v.virtual_size);
    CHECK(v.peak_working_set >= v.working_set);
}

static void test_zero_working_set_uses_limit(void)
{
    vmc_values v = sample();
    v.working_set = 0; v.peak_working_set = 0; v.private_usage = 149 * GIB;
    CHECK(vmclamp(&v, PHYS, MARGIN) == 1);
    CHECK(v.private_usage == PHYS - MARGIN);
}

static void test_private_absent_is_not_touched(void)
{
    vmc_values v = sample();
    v.has_private = 0; v.private_usage = 149 * GIB;
    CHECK(vmclamp(&v, PHYS, MARGIN) == 0);
    CHECK(v.private_usage == 149 * GIB);
}

static void test_all_zero_is_unchanged(void)
{
    vmc_values v = { 0 };
    CHECK(vmclamp(&v, PHYS, MARGIN) == 0);
    CHECK(v.private_usage == 0 && v.working_set == 0);
}

static void test_value_equal_to_limit_is_unchanged(void)
{
    vmc_values v = sample();
    v.private_usage = PHYS - MARGIN;
    CHECK(vmclamp(&v, PHYS, MARGIN) == 0);
}

static void test_invalid_arguments_fail_fast(void)
{
    vmc_values v = sample(), w = sample();
    CHECK(vmclamp(&v, 0, 0) < 0);
    CHECK(vmclamp(&v, PHYS, PHYS) < 0);
    CHECK(vmclamp(&v, PHYS, PHYS + 1) < 0);
    CHECK(vmclamp(&v, PHYS, 0) < 0); /* a zero margin could leave a value equal to RAM */
    CHECK(vmclamp(0, PHYS, MARGIN) < 0);
    CHECK(v.private_usage == w.private_usage && v.working_set == w.working_set);
}

static void test_second_call_changes_nothing(void)
{
    vmc_values v = sample();
    v.private_usage = 149 * GIB; v.virtual_size = 140 * GIB;
    vmclamp(&v, PHYS, MARGIN);
    CHECK(vmclamp(&v, PHYS, MARGIN) == 0);
}

int main(void)
{
    test_normal_values_unchanged();
    test_huge_private_is_clamped_below_ram();
    test_huge_virtual_is_clamped();
    test_working_set_larger_than_ram();
    test_peaks_not_below_current();
    test_zero_working_set_uses_limit();
    test_private_absent_is_not_touched();
    test_all_zero_is_unchanged();
    test_value_equal_to_limit_is_unchanged();
    test_invalid_arguments_fail_fast();
    test_second_call_changes_nothing();
    printf(failures ? "%d check(s) failed\n" : "all checks passed\n", failures);
    return failures != 0;
}
