# Code style

No code exists yet. These rules apply when code is added.

## General

- Write the smallest change that solves the problem.
- Change only the lines the task needs.
- One function has one job.
- Fail fast. Return an error when a precondition fails.
- Add no new library without approval from Adam.

## C (proxy DLL)

- Keep one `.def` file. It must list ordinals 100 and 101.
- Use base-relative RVAs. Never use absolute addresses.
- Check TimeDateStamp and SizeOfImage before any in-process change. Refuse to install on a mismatch.
- Log only when an environment key turns logging on.
- Build commands go in the file header comment.

## Scripts

- Use Python 3 standard library or POSIX shell.
- Print the command before any destructive step. Never delete without a yes.

## Docs

- Use Simplified Technical English. Active voice, short sentences.
- No em dashes. Dates as DD/MM/YYYY.
- Write paths as `~`, not `/Users/<name>`.

## Git

- Commit subject: imperative, under 72 characters, format `type: summary`.
- Commit only when Adam asks. Add no AI co-author line.
- Never force-push.
