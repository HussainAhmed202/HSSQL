# Devlog

# 2026-10-12

**Problem** - I can write a TableSchema successfully to a binary file. But cannot read correctly. Turns out, when structus are stored, the compiler adds padding to make it easier for the cpu to access. For example,
Struct = {
char a; //1 Byte
int b; //4 Byte
char c; //1 Byte
}

Compiler will 0 pad the regions such that each member of the struct occupies the same number of bytes. In this struct, the largest object is the second member b which is an integer; it takes 4Bytes. The total size of the struct must be a multiple of the largest member of the struct. Therefore, the min largest possible size is 12 Bytes.
Bytes 0-3 occupied by member a; 4-7 by b; 8-11 by c.
0 a
1 0
2 0
3 0
4 b
5 b
6 b
7 b
8 c
9 0
10 0
11 0

=== DUMPING RAW MEMORY FOR TableSchema (1540 bytes) ===

Offset 0000 [0x000]: E8 2F 81 00 03 00 00 00 00 00 00 00 00 00 00 00
Offset 0016 [0x010]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0032 [0x020]: 00 00 00 00 00 00 00 69 64 00 00 00 00 00 00 00
Offset 0048 [0x030]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0064 [0x040]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0080 [0x050]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0096 [0x060]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0112 [0x070]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0128 [0x080]: 00 00 00 00 00 00 00 00 00 00 00 4E 55 4D 42 45
Offset 0144 [0x090]: 52 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0160 [0x0A0]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0176 [0x0B0]: 00 00 00 00 00 00 00 00 00 00 00 00 00 75 73 65
Offset 0192 [0x0C0]: 72 5F 69 64 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0208 [0x0D0]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0224 [0x0E0]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0240 [0x0F0]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0256 [0x100]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0272 [0x110]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0288 [0x120]: 00 4E 55 4D 42 45 52 00 00 00 00 00 00 00 00 00
Offset 0304 [0x130]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0320 [0x140]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0336 [0x150]: 00 00 00 61 6D 6F 75 6E 74 00 00 00 00 00 00 00
Offset 0352 [0x160]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0368 [0x170]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0384 [0x180]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0400 [0x190]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0416 [0x1A0]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0432 [0x1B0]: 00 00 00 00 00 00 00 44 4F 55 42 4C 45 00 00 00
Offset 0448 [0x1C0]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0464 [0x1D0]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0480 [0x1E0]: 00 00 00 00 00 00 00 00 00 6E 61 6D 65 00 00 00
Offset 0496 [0x1F0]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0512 [0x200]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0528 [0x210]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0544 [0x220]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0560 [0x230]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0576 [0x240]: 00 00 00 00 00 00 00 00 00 00 00 00 00 56 41 52
Offset 0592 [0x250]: 43 48 41 52 32 00 00 00 00 00 00 00 00 00 00 00
Offset 0608 [0x260]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0624 [0x270]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0640 [0x280]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0656 [0x290]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0672 [0x2A0]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0688 [0x2B0]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0704 [0x2C0]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0720 [0x2D0]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0736 [0x2E0]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0752 [0x2F0]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0768 [0x300]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0784 [0x310]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0800 [0x320]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0816 [0x330]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0832 [0x340]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0848 [0x350]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0864 [0x360]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0880 [0x370]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0896 [0x380]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0912 [0x390]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0928 [0x3A0]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0944 [0x3B0]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0960 [0x3C0]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0976 [0x3D0]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 0992 [0x3E0]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1008 [0x3F0]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1024 [0x400]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1040 [0x410]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1056 [0x420]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1072 [0x430]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1088 [0x440]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1104 [0x450]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1120 [0x460]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1136 [0x470]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1152 [0x480]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1168 [0x490]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1184 [0x4A0]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1200 [0x4B0]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1216 [0x4C0]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1232 [0x4D0]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1248 [0x4E0]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1264 [0x4F0]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1280 [0x500]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1296 [0x510]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1312 [0x520]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1328 [0x530]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1344 [0x540]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1360 [0x550]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1376 [0x560]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1392 [0x570]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1408 [0x580]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1424 [0x590]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1440 [0x5A0]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1456 [0x5B0]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1472 [0x5C0]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1488 [0x5D0]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1504 [0x5E0]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1520 [0x5F0]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Offset 1536 [0x600]: 00 00 00 00

TableSchema
char table_name[35];
Column columns[10];
int num_columns;

Column
char col_name[100];
char col_type[50];

Size taken by one TableSchema object in memory
= 35 + 10 X (sizeof(Column)) + 4 [int take 4 Bytes]
= 35 + 10 X (100 + 50) + 4
= 1540 Bytes.

# 2026-10-07

**Did** - I have switched to dynamic memory allocation. I was avoiding it for sometime on purpose. I am trying to implement a functionality. Read the table catalog when the db engine starts. When saving table to disk, the catalog will be updated. When reading catalog from a binary file, how to decide what should be the size of the catalog. The catalog is an arrya of TableSchema objects. How many objects are there?? Easy solution. Check the size of the entire catalog file. That is the number of bytes used to store the entire catalog. We can use sizeof to get the sizeof the TableSchema object. Divide total/element_size, we get the number of elemetns. Brilliant....Now I shoot myself in the foot. Apparently, I cannot define an array of size = some variable that will hold that value. That is variable length array or VLA. No No No No No No. There probably a way to do this, but I am not smart enough for this. Life is too short. Time to embrace this new journey. Dynamic Memory allocation. Malloc and stuff. Excited. It does make life easy. No long need to define max length of columns, catalog, string lengths etc.

# 2026-10-06

**Did** - Been some time. I did work in between but forget to add the logs. Today, I printed a NULL pointer. It was printed as (null). Thats awesome. Integrated the save.c file functions into the main.c

# 2026-09-30

**Problem** - How to read a specific table. What to store in the binary file. A table? The entire catalog??
What is the size of the TableSchema in binary file? How to read specific table. How is a array of TableSchema stored?For this I visited some CS. An array is a fixed contigous block of memory. An array of struct is still a fixed contigous block of memory. Therefore, when i store a array of struct, the data is stored linearly. How to read it. Similar to how we iterate over a array element. Number of elements X Size of one element.In this case, it is the number of tables in the catalog X size of a TableSchema object.
**Did** - Late at night. Laptop battery is also low. I need to quickly draft functions that save a table struct as a binary object and a function that reads this binary object. Both these functions are saved in the save.c file.

# 2026-09-26

**Did** - Creating a separate file repl.c for the REPL. Making main.c cleaner. The challenge here is how to separate the REPL from the rest. The main.c is currently my REPL sort of. I have modularized the code a bit. The EOF, input lenght, exit commands have been created into separate functions. The REPL function will return 1 or 0. 1 means exit the program.It takes the input and prepares it for tokenzier.

# 2026-09-21

**Problem** - Logging back in after a while. There is one issue right now.
Welcome to HSSQL
hssql> create table customer_accounts
create table customer_accounts
hssql> create table orders
create table orderser_accounts
hssql> del table this
delate table thisrser_accounts

There is an issue in the tokenization of the user input. When I assign a value in the tokens array, I missed the intialization step. So the new input overides the previous input. This is fine. However, when the new input is smaller than the previous input, then it is a problem. In the above output, the first input table was customer_accounts which is 18 chars long. The next input is orders which is 7 chars long. This means at
tokens[][2] = customer_accounts
next iteration
tokens[][2] = orderser_accounts
There are two ways to resolve this issue

1. Re-intialize the array everytime in the REPL. This would zero the values held in the array. However, there is a better way
2. This is what I go with. In C, strings are whatever characters encountered before **_\0_** which is the NULL terminator. Currently, I was not adding the null terminator to my tokens.
   **Did:** - Modified tokenizer.c. Added null terminator at the end of a token to avoid buffer overflow in the REPL. I also added the find_table_by_name function defined in the catalog file. Now main will call this function to check if a table exists before adding it to the catalog
   **Learned** - C strings are weird. Learned about null terminators importance.

# 2026-09-10

**Did:** - Built new file - catalog.c which stores informations of all Tables in the database
I did my first **stack smashing** - The catalog array was fixed size of 2 TableSchema structs. Added another. The program behaved weirdly and existed abruptly
**Learned:** - Passing int pointers. This way a variables value can be changed from inside the function.
To tackle the stack smashing, used directive MAX_TABLES to specify how many tables allowed in the catalog.
**Next:** Improving the main.c to integrate all the components.

# 2026-09-05

**Did:** Refactored parser.c file - Now standalone function that can be called by including its declaration defined in the header file. The main file has the REPL. It now takes in the user input. The parser only parses CREATE statement so it expects a create statement; Otherwis it prints and error message.
**Learned** Passing pointers to a function - Parses uses Query Struct. I initially passed it as value. Turns out, when passing as value, the value is copied into the local scope of that function. To actually change it, we pass in the memory address of the object. Then the object is changed in global scope. I didnt get this problem previously because I was working wih arrays. We you pass an array, it decays into a pointer. So it indirectly, passes as a pointer - not as a value.
**Next** My REPL can parse CREATE statements. There is another limitation that Im currently using fix size memory objects. Not getting into C dynamic memory allocation stuff - will do that in the future obv. My REPL can take in a CREATE statement, parse it. I cannot do any othe DML or DDL statement. I want to work on how to actually execute the command first. Then will work on dynamic memory allocation and supporting other DDL and DML commands.

# 2026-09-04

**Did:** Refactored tokenizer.c file - Now standalone function that can be called by including its declaration defined in the header file. This tokenizer function is called in the main.c file. The main.c file has the REPL. It now takes in the user input, and prints out the tokens.
**Learned** Header files - these contain the function declarations or prototypes of a module. This allows other files to call and use these functions without needing to know or implement them. Header files mask the implementation details and act kind of like an API.

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
