# generSQL
To preface: I have no idea if this has been done before.

- This is an in-memory DBMS constantly backing up to storage
- I imagine its use with bursty traffic and slow (but fast enough over time) I/O
> - I hope to make I/O operations as close to sequential as possible to optimize for most real forms of I/O
- Further, I plan to have a SQL terminal that -- when a write query is run -- authenticates the query's success twice:
> - first, when the write is committed to memory
> - second, when the write is committed to storage