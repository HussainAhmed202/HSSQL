# Devlog

Short entries, one per work session. The point is "what did I do, what's next" so picking the project back up tomorrow takes 30 seconds, not 10 minutes.

# 2026-08-31

**Did:** Created a parser that takes the tokens array that has a CREATE DDL command. It picks the columns being defined and their datatypes and stores it in an array of struct.
**Learned** Used struct for the first time. These are awesome. They are the C equivalent of classes. Sort of. This struct has two members. A col_name and col_type.
When parsing, there is an inherit structure. For example,
CREATE TABLE users (id INT, name TEXT);
At index [0]CREATE word expected
At index [1]TABLE word expected
At index [2]users name of the table being created
At index [3]( - opening bracket expected

Here we need to loop till ")" is encountered
[4]id  
 [5]INT
[6], -- Note how comma separates the column definition. 0
[7]name  
 [8]TEXT  
[9])

So I created a loop, that will use three pointers.
i which points to the current index of the token array
j which points to the current index of the column array
k which points to the member of the struct at the current column array index being populated
First iteration at i = 4 - Start after opening bracket encountered
i = 4 j = 0 k = 0 token[4] = id column[0] = {col_name -> id}
i = 5 j = 0 k = 1 token[5] = INT column[0] = {col_name -> id, col_type -> INT}
i = 6 token[6] = ,
j++ k = 0
i = 7 j = 1 k = 0 token[7] = name column[1] = {col_name -> name}
i = 8 j = 1 k = 1 token[8] = TEXT column[1] = {col_name -> name, col_type -> TEXT}

# 2026-08-26

**Did:** Set up a basic tokenizer that uses a 2D array with fixed dimenstions.
**Learned:** Using a 2D array where each index stores a string. For example, an input = SELECT _ FROM USER will be represented as
token = [
[S,E,L,E,C,T]
[_]
[F,R,O,M]
[U,S,E,R]
]
I have also added a dryrun file of my development. It is inside docs/tokenzier-dry-runs.txt
Right now, the main and tokenizer files are not integrated together. Will do that at the end of the project.
Going with a fixed size dimensions. In future, will updated it with a
more dynamic approach using malloc. Raincheck.
**Next:** Build a basic parser that will read the tokens and create some structure so that we can execute these queries.

## 2026-08-18

**Did:** Set up the main file. This file creats the CLI terminal for writing the queries.Currently, the REPL takes in input of upto 100B and echos it to the STDOUT.
**Learned** fgets() function to read input from STDIN. Handling multi-line input and buffer overflow.
**Next** Work on building the tokenizer that will parse a string input and chop it down into key words.

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
