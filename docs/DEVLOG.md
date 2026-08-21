# Devlog

Short entries, one per work session. Doesn't need to be polished —
the point is "what did I do, what's next" so picking the project back
up tomorrow takes 30 seconds, not 10 minutes.

Template for a new entry:

```
## YYYY-MM-DD
**Did:** ...
**Learned:** ...
**Next:** ...
```

---

## 2026-08-21
**Did:** Set up the project as a proper git repo — README, roadmap,
devlog, .gitignore, MIT license. Reviewed `db.h` (header guards,
`#define` macros, the `enum`, and the `Column`/`TableSchema`/`Value`/
`Row` structs) as the starting point for going through the rest of
the codebase.
**Learned:** Started C from scratch — covering fundamentals before
diving deeper into the codebase.
**Next:** Finish walking through `db.h`, then `db.c`, before moving
on to `catalog.c`.
