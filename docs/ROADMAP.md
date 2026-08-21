# Roadmap

Rough priority order. Each of these is meant to be roughly
"a few days of 30-40 min sessions," not a single sitting.

## Done

- [x] Core types: `Column`, `TableSchema`, `Value`, `Row` (`db.h/.c`)
- [x] Catalog: schema persistence to `data/catalog.db` (`catalog.h/.c`)
- [x] Storage engine: fixed-width binary rows, linear scan, tombstone
      deletes (`table.h/.c`)
- [x] SQL layer: tokenizer + parser for `CREATE TABLE`, `INSERT`,
      `SELECT` (with projection + single `WHERE col = val`), `DELETE`
      (`sql.h/.c`)
- [x] REPL + batch-file mode (`main.c`)

## Next up

- [ ] **Space reclamation (VACUUM)** — compact out tombstoned rows so
      deleted space is reused instead of the file growing forever
- [ ] **A basic index** — hash index on one column first (simplest),
      so `WHERE col = val` doesn't need a full scan
- [ ] **`UPDATE` statement** — currently only `INSERT`/`DELETE` exist
- [ ] **Multiple WHERE conditions** (`AND`) — parser currently only
      handles one `col = val`
- [ ] **Comparison operators** (`<`, `>`, `!=`) — currently only `=`

## Later / bigger jumps

- [ ] **Variable-length TEXT** — needs an overflow/heap area or
      length-prefixed records instead of fixed-width padding
- [ ] **B-tree index** — replace the hash index (or sit alongside it)
      for range queries
- [ ] **Page-based storage layer** — the real architectural jump:
      fixed-size pages with a buffer pool, instead of raw row streams.
      This is what "real" database engines look like internally.
- [ ] **JOINs** — needs the query layer to reason about more than one
      table at a time
- [ ] **Write-ahead log / crash recovery** — right now a crash
      mid-write can corrupt a row
- [ ] **Basic transactions** (BEGIN/COMMIT/ROLLBACK)

## Explicitly out of scope for now

- Concurrency / multiple connections
- Query optimizer
- Network protocol (this stays a local, single-process engine)
