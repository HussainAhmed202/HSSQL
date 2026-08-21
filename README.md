# HSSQL

A tiny relational database engine, written from scratch in C.

This is a learning project: the goal is to understand how a database
actually works under the hood — parsing, storage, persistence — by
building one piece at a time, in small daily sessions.

Not for production use. It's deliberately simple so it stays easy to
reason about, but structured so each piece can be swapped out or
upgraded independently as the project grows.

## What it can do right now

```sql
CREATE TABLE users (id INT, name TEXT(20), age INT);
INSERT INTO users VALUES (1, "Alice", 30);
INSERT INTO users VALUES (2, "Bob", 25);
SELECT * FROM users;
SELECT name, age FROM users WHERE age = 25;
DELETE FROM users WHERE id = 2;
```

Data is written to disk and survives restarts.

## Quickstart

Requires `gcc` and `make`.

```bash
make run
```

This builds the `HSSQL` binary and drops you into a REPL. Type SQL
statements ending in `;`, or `.exit` to quit.

You can also run a `.sql` file non-interactively:

```bash
make
./HSSQL path/to/script.sql
```

## Project structure

```
src/
  db.h / db.c        core types (Column, TableSchema, Value, Row) + layout math
  catalog.h / .c      table schema persistence (data/catalog.db)
  table.h / .c         row storage engine (data/<table>.tbl) - the actual "disk" layer
  sql.h / .c            tokenizer + parser + executor for SQL statements
  main.c                 REPL / entry point
data/                   generated at runtime, gitignored (schema + table files live here)
docs/
  DEVLOG.md            running log of each work session
  ROADMAP.md         what's built, what's next, in rough priority order
```

The layering is intentional:

```
main.c  →  sql.c  →  table.c / catalog.c  →  raw files on disk
(REPL)     (parser)   (storage engine)
```

Each layer only talks to the one below it. `sql.c` doesn't know how
rows are laid out on disk, and `table.c` doesn't know anything about
SQL syntax. This is what makes it "expandable" — e.g. the storage
layer can be rewritten (flat file → B-tree pages) without touching
the parser at all.

## Current design (and its deliberate limitations)

- **Storage:** one binary file per table, fixed-width rows, linear
  scan for every read. No indexes yet — every `WHERE` is O(n).
- **Types:** `INT` (64-bit) and `TEXT(n)` (fixed-width, no variable
  length strings yet).
- **Deletes:** rows are tombstoned (flagged dead), not removed —
  the file only grows until we add compaction.
- **No transactions, no concurrency, no crash recovery.**
- **No joins yet** — single table queries only.

These aren't oversights, they're the starting point. See
[`docs/ROADMAP.md`](docs/ROADMAP.md) for what's planned next and why.

## Why this structure

Working in short daily sessions means picking up context fast
matters more than usual. That's the reasoning behind:

- Small, single-purpose files (`table.c` only knows about
  reading/writing rows, `catalog.c` only knows about schemas)
- A [devlog](docs/DEVLOG.md) so "what was I doing yesterday" has an answer
- A [roadmap](docs/ROADMAP.md) so "what's next" has an answer too

## License

MIT — see [LICENSE](LICENSE).
