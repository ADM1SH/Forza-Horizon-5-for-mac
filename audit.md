# Audit

This repo is a research repo with no website, server, database, payment or user login. Items from the global checklists are N/A with a reason unless they apply.

## Protect (20 items)

| Items | Status | Reason |
|---|---|---|
| 1-3 Rate limits, API limits, spending caps | N/A | No service or paid API |
| 4, 7, 8 Error handling, failed requests, timeouts | N/A | No code yet. Apply when a tool exists |
| 5, 6 Loading and empty states | N/A | No UI |
| 9, 10 Duplicate submissions, duplicate payments | N/A | No forms or payments |
| 11-13 DB queries, indexes, pagination | N/A | No database |
| 14, 15 Upload compression and size | N/A | No uploads |
| 16 Cache repeat requests | N/A | No service |
| 17 Uptime monitoring | N/A | No service |
| 18 Error logging | Planned | Log rules in SECURITY.md |
| 19 Simultaneous users | N/A | No service |
| 20 Backup restoration | N/A | Nothing to restore. The bottle is not changed |

## Security (20 items)

| Item | Status | Reason |
|---|---|---|
| 1 Hide API keys | Pass | No keys in repo. .gitignore blocks .env |
| 2 Purge Git secrets | Open | Run gitleaks before the first push |
| 3-8 DB key, RLS, PII encryption, server auth, IDOR, mass assignment | N/A | No backend |
| 9-11 Cookies, password hashing, login rate limit | N/A | No login |
| 12 Bot protection | N/A | No forms |
| 13-15 SQL, input schema, XSS | N/A | No backend or HTML |
| 16 Upload restrictions | N/A | No uploads |
| 17 Trim API responses | N/A | No API |
| 18, 19 Headers, HTTPS | N/A | No site |
| 20 Dependency scan | N/A | No dependencies yet |

## Site polish (20 items)

All N/A. There is no website. README.md renders on GitHub.

## Repo checks

| Check | Status |
|---|---|
| LICENSE present (MIT) | Pass |
| No game, Apple or Wine binaries in repo | Pass (blocked by .gitignore) |
| No account names or SteamIDs in docs | Open: scan before the first commit |
| No em dashes in .md files | Open: scan before the first commit |
