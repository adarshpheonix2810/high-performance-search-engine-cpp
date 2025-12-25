# Document Store - C File I/O Functions Documentation

This document provides comprehensive explanations of all C file I/O functions and concepts used in the `document_store.cpp` implementation of our high-performance search engine.

---

## Table of Contents
1. [fopen() - Opening Files](#topic-1-fopen---opening-files)
2. [fclose() - Closing Files](#topic-2-fclose---closing-files)
3. [fgetc() - Reading Characters](#topic-3-fgetc---reading-characters)
4. [eof() - End-of-File Detection](#topic-4-eof---end-of-file-detection)
5. [ungetc() - Pushing Characters Back](#topic-5-ungetc---pushing-characters-back)
6. [size_t - Unsigned Integer Type](#topic-6-size_t---unsigned-integer-type)
7. [getline() - Reading Lines (POSIX)](#topic-7-getline---reading-lines-posix)
8. [getline() Windows Problem & Solutions](#topic-8-getline-windows-problem--solutions)
9. [free() - Deallocating Memory](#topic-9-free---deallocating-memory)
10. [read_input() - Loading Documents into Map](#topic-10-read_input---loading-documents-into-map)
11. [strtok() and Memory Management](#topic-11-strtok-and-memory-management)
12. [getline() Return Value Checking](#topic-12-getline-return-value-checking)

---

## Topic 1: fopen() - Opening Files

### What is fopen()?
`fopen()` is a C standard library function used to **open a file** and establish a connection between the program and the file. It returns a pointer to a `FILE` object that controls the file stream.

### Syntax
```c
FILE* fopen(const char* filename, const char* mode);
```

### Parameters
- **filename**: Path to the file (relative or absolute)
- **mode**: String specifying the file access mode

### Common File Modes

| Mode | Description | File Must Exist? | Overwrites? |
|------|-------------|------------------|-------------|
| `"r"` | Read only | Yes | No |
| `"w"` | Write only | No | Yes |
| `"a"` | Append | No | No |
| `"r+"` | Read and write | Yes | No |
| `"w+"` | Read and write | No | Yes |
| `"a+"` | Read and append | No | No |
| `"rb"` | Read binary | Yes | No |
| `"wb"` | Write binary | No | Yes |

### Return Value
- **Success**: Returns a pointer to `FILE` object
- **Failure**: Returns `NULL` (file doesn't exist, permission denied, etc.)

### Practical Example from document_store.cpp
```cpp
FILE *file = fopen(file_name, "r");
if (file == NULL) {
    cout << "Error: Cannot open file " << file_name << endl;
    return -1;
}
```

### Visual Representation
```
Program Memory                    Disk Storage
+-----------+                    +-------------+
|  FILE*    |  fopen("r")       |   doc1.txt  |
|   file    | ----------------> |   Content   |
| (pointer) | <---------------- |   Lines...  |
+-----------+   Read Access     +-------------+
```

### Key Points
- Always check if `fopen()` returns `NULL` before using the file pointer
- Use `"r"` mode when you only need to read the file
- Use `"b"` suffix for binary files on Windows to prevent newline translation
- File pointer must be closed with `fclose()` after use

---

## Topic 2: fclose() - Closing Files

### What is fclose()?
`fclose()` is a C standard library function used to **close an open file stream** and release all resources associated with it. It also flushes any buffered data to the file.

### Syntax
```c
int fclose(FILE* stream);
```

### Parameters
- **stream**: Pointer to the `FILE` object to be closed

### Return Value
- **Success**: Returns `0`
- **Failure**: Returns `EOF` (typically -1) if an error occurs

### What Happens When You Call fclose()?
1. **Flushes Buffers**: Writes any unwritten data from buffer to file
2. **Releases Resources**: Frees memory allocated for the file stream
3. **Disassociates Stream**: Breaks the connection between program and file
4. **Makes Pointer Invalid**: Using the pointer after `fclose()` causes undefined behavior

### Practical Example from document_store.cpp
```cpp
FILE *file = fopen(file_name, "r");
// ... read from file ...
fclose(file);
```

### Why fclose() is Critical?

**Problem Without fclose():**
```cpp
FILE *file = fopen("data.txt", "w");
fprintf(file, "Important data");
// Program crashes or exits without fclose()
// ❌ Data might be lost! (still in buffer)
```

**Solution With fclose():**
```cpp
FILE *file = fopen("data.txt", "w");
fprintf(file, "Important data");
fclose(file);  // ✅ Data safely written to disk
```

### Memory Leak Example
```cpp
// ❌ BAD: Memory leak
for(int i = 0; i < 1000; i++) {
    FILE *f = fopen("temp.txt", "r");
    // ... use file ...
    // Missing fclose(f) - leaks resources!
}

// ✅ GOOD: Properly closes files
for(int i = 0; i < 1000; i++) {
    FILE *f = fopen("temp.txt", "r");
    // ... use file ...
    fclose(f);  // Releases resources
}
```

### Visual Representation
```
Before fclose():              After fclose():
+-------------+              +-------------+
|   Program   |              |   Program   |
|             |              |             |
|   FILE*  ---|---> [Buffer] |   FILE* = ? |
|             |       ↓      |             |
+-------------+   [Disk File]+-------------+
                              [File Closed]
```

### Key Points
- **Always** call `fclose()` for every successful `fopen()`
- Check the return value to detect write errors
- Using a closed file pointer results in undefined behavior
- OS limits the number of open files - closing prevents resource exhaustion

---

## Topic 3: fgetc() - Reading Characters

### What is fgetc()?
`fgetc()` is a C standard library function that **reads the next character** from an input stream and returns it as an integer. It's commonly used for character-by-character file reading.

### Syntax
```c
int fgetc(FILE* stream);
```

### Parameters
- **stream**: Pointer to the `FILE` object from which to read

### Return Value
- **Success**: Returns the character as an `unsigned char` cast to `int`
- **End of File or Error**: Returns `EOF` (typically -1)

### Why Return Type is int (Not char)?
```c
// EOF is typically -1 (negative value)
// char range: -128 to 127 (signed) or 0 to 255 (unsigned)
// int range: -2,147,483,648 to 2,147,483,647

int c = fgetc(file);
if (c == EOF) {  // Can distinguish EOF from valid character 255
    // End of file or error
}
```

**Problem with char return type:**
- Character with value 255 might be confused with EOF (-1)
- `int` safely holds all possible `char` values PLUS special `EOF` value

### Practical Example from document_store.cpp
```cpp
// Check if file is empty
int c = fgetc(file);
if(c == EOF){
    cout << "File is empty: " << file_name << endl;
    fclose(file);
    return -1;
}
ungetc(c, file);  // Put character back
```

### Reading File Character by Character
```cpp
FILE *file = fopen("document.txt", "r");
int c;

while((c = fgetc(file)) != EOF) {
    printf("%c", c);  // Print each character
}

fclose(file);
```

### Detecting Errors vs EOF
```cpp
int c = fgetc(file);
if (c == EOF) {
    if (feof(file)) {
        printf("End of file reached\n");
    } else if (ferror(file)) {
        printf("Read error occurred\n");
    }
}
```

### Visual Representation
```
File: "Hello"           Memory
+---+---+---+---+---+
| H | e | l | l | o |
+---+---+---+---+---+
  ↑
  File Position Indicator

1st fgetc() → Returns 'H' (72)
2nd fgetc() → Returns 'e' (101)
3rd fgetc() → Returns 'l' (108)
4th fgetc() → Returns 'l' (108)
5th fgetc() → Returns 'o' (111)
6th fgetc() → Returns EOF (-1)
```

### Performance Consideration
```cpp
// ❌ SLOW: Character-by-character reading
while((c = fgetc(file)) != EOF) {
    process(c);
}

// ✅ FASTER: Buffer reading
char buffer[4096];
while(fgets(buffer, sizeof(buffer), file) != NULL) {
    process(buffer);
}
```

### Key Points
- Returns `int` to accommodate `EOF` special value
- Advances file position indicator by 1 byte
- Can be used to peek at the first character (with `ungetc()`)
- Slower than buffered reading for large files
- Use `feof()` and `ferror()` to distinguish EOF from errors

---

## Topic 4: eof() - End-of-File Detection

### What is eof()?
`eof()` is a function used to check whether the **End-of-File (EOF) condition** has been reached during file input operations. In C, the equivalent functions are `feof()` for `FILE*` streams.

### Two Types of EOF Functions

#### 1. C++ Stream Version (std::ifstream)
```cpp
#include <fstream>

std::ifstream file("data.txt");
if (file.eof()) {
    std::cout << "End of file reached\n";
}
```

#### 2. C File Version (FILE*)
```cpp
#include <stdio.h>

FILE *file = fopen("data.txt", "r");
if (feof(file)) {
    printf("End of file reached\n");
}
```

### Important: When is eof() True?

**Critical Concept**: `eof()` returns `true` **ONLY AFTER** a read operation attempts to read **BEYOND** the end of the file.

### Common Misconception Example
```cpp
// ❌ WRONG LOGIC
FILE *file = fopen("data.txt", "r");
while(!feof(file)) {  // Checks EOF before reading
    char buffer[100];
    fgets(buffer, 100, file);  // Might read garbage on last iteration
    printf("%s", buffer);
}
```

**Problem**: `feof()` is false even at the last valid character. The loop reads one extra time, processing garbage or duplicating the last line.

### Correct Pattern
```cpp
// ✅ CORRECT LOGIC
FILE *file = fopen("data.txt", "r");
char buffer[100];

while(fgets(buffer, 100, file) != NULL) {  // Check return value
    printf("%s", buffer);
}

fclose(file);
```

### Visual Representation
```
File Content: "AB"
Position:      0  1  2
               +--+--+
               |A |B |EOF
               +--+--+
               
Read #1: fgetc() → 'A', feof() → false
Read #2: fgetc() → 'B', feof() → false
Read #3: fgetc() → EOF, feof() → TRUE ✓
```

### Detecting Read Errors vs EOF
```cpp
FILE *file = fopen("data.txt", "r");
int c;

while((c = fgetc(file)) != EOF) {
    putchar(c);
}

// Now determine WHY we stopped
if (feof(file)) {
    printf("End of file reached normally\n");
} else if (ferror(file)) {
    printf("Error occurred during reading\n");
}

fclose(file);
```

### EOF Value
```cpp
// EOF is a macro defined in <stdio.h>
#define EOF (-1)

// It's a negative integer value that cannot be a valid character
int c = fgetc(file);
if (c == EOF) {
    // Could be end-of-file OR read error
}
```

### Comparison Table

| Aspect | feof() | Return Value Check |
|--------|--------|-------------------|
| Timing | After read fails | During read |
| Best for | Error distinction | Normal reading |
| Usage | `if(feof(file))` | `if(fgets(...) != NULL)` |
| Precision | Less precise | More precise |

### Practical Example: Reading Until EOF
```cpp
FILE *file = fopen("numbers.txt", "r");
int number;

// ✅ CORRECT: Check fscanf return value
while(fscanf(file, "%d", &number) == 1) {
    printf("Read: %d\n", number);
}

// Now check why we stopped
if (feof(file)) {
    printf("Finished reading all numbers\n");
} else {
    printf("Error: Invalid data format\n");
}

fclose(file);
```

### Key Points
- `feof()` returns `true` ONLY AFTER attempting to read past EOF
- Never use `while(!feof(file))` as a loop condition
- Always check read function return values instead
- Use `feof()` and `ferror()` together to diagnose why reading stopped
- EOF is typically defined as -1

---

## Topic 5: ungetc() - Pushing Characters Back

### What is ungetc()?
`ungetc()` is a C standard library function that **pushes a character back** into an input stream, allowing it to be read again by the next read operation. It's like "un-reading" a character.

### Syntax
```c
int ungetc(int ch, FILE* stream);
```

### Parameters
- **ch**: Character to push back (as `int`)
- **stream**: Pointer to the `FILE` object

### Return Value
- **Success**: Returns the pushed character `ch`
- **Failure**: Returns `EOF` (if `ch` is `EOF` or operation fails)

### How ungetc() Works

**Visual Representation:**
```
Stream: "Hello"
Position: ↓
         H e l l o

Step 1: int c = fgetc(file);  // Reads 'H'
        Position: ↓
                 H e l l o
                   ↑

Step 2: ungetc(c, file);      // Pushes 'H' back
        Position: ↓
         H e l l o
         ↑ (back to start)

Step 3: c = fgetc(file);      // Reads 'H' again
```

### Practical Example from document_store.cpp

**Empty File Detection:**
```cpp
// Check if file is empty by peeking at first character
int c = fgetc(file);      // Read first character
if(c == EOF){             // File is empty
    cout << "File is empty: " << file_name << endl;
    fclose(file);
    return -1;
}
ungetc(c, file);          // Put character back
// Now file position is back at start
```

**Why This Pattern?**
- We want to check if file is empty WITHOUT consuming its content
- `fgetc()` tells us if there's data (returns char) or not (returns EOF)
- `ungetc()` restores the file to its original state
- Subsequent `getline()` can read from the beginning

### Another Use Case: Token Lookahead

**Parsing Numbers:**
```cpp
FILE *file = fopen("data.txt", "r");
int c;

while((c = fgetc(file)) != EOF) {
    if(isdigit(c)) {
        ungetc(c, file);  // Put digit back
        int number;
        fscanf(file, "%d", &number);  // Read full number
        printf("Found number: %d\n", number);
    }
}

fclose(file);
```

### Important Rules and Limitations

#### 1. Only One Pushback Guaranteed
```cpp
int c1 = fgetc(file);     // Read 'A'
int c2 = fgetc(file);     // Read 'B'

ungetc(c2, file);         // ✅ Push 'B' back - OK
ungetc(c1, file);         // ❌ Push 'A' back - NOT GUARANTEED
                          // Only 1 character pushback is portable
```

#### 2. Cannot Push EOF
```cpp
ungetc(EOF, file);        // ❌ Does nothing, returns EOF
```

#### 3. Affects File Position
```cpp
FILE *file = fopen("test.txt", "r");
long pos1 = ftell(file);      // Position: 0

int c = fgetc(file);          // Position: 1
long pos2 = ftell(file);      

ungetc(c, file);              // Position: back to 0
long pos3 = ftell(file);      

printf("Pos: %ld → %ld → %ld\n", pos1, pos2, pos3);
// Output: Pos: 0 → 1 → 0
```

### Comparison with Other Methods

| Method | Purpose | Modifies File? | Efficiency |
|--------|---------|----------------|------------|
| `ungetc()` | Push back 1 char | No | Fast |
| `fseek(file, -1, SEEK_CUR)` | Move back 1 byte | No | Medium |
| Read into buffer | Store multiple chars | No | Slow (memory) |

### Alternative: Buffered Reading
```cpp
// Instead of ungetc(), use larger buffer
char buffer[100];
fgets(buffer, 100, file);  // Read line into buffer
// Process buffer without needing to push back
```

### Common Pitfall: Binary Files
```cpp
// ⚠️ WARNING: ungetc() on binary files
FILE *file = fopen("image.png", "rb");
int byte = fgetc(file);
ungetc(byte, file);  // Works, but use fseek() for binary
                      // fseek() is more reliable for binary data
```

### Key Points
- `ungetc()` allows "peeking" at next character without consuming it
- Only **one character** pushback is guaranteed by C standard
- Cannot push back `EOF`
- Perfect for checking file emptiness before reading
- Updates file position indicator backwards
- Alternative: use `fseek(file, -1, SEEK_CUR)` for moving back

---

## Topic 6: size_t - Unsigned Integer Type

### What is size_t?
`size_t` is an **unsigned integer data type** defined in the C/C++ standard library specifically designed to represent the **size of objects** in bytes. It's guaranteed to be large enough to hold the size of the largest possible object on the system.

### Declaration
```cpp
#include <cstddef>  // C++
#include <stddef.h> // C

size_t size = sizeof(int);
```

### Why size_t Exists?

**Portability Problem:**
```cpp
// ❌ NON-PORTABLE: Assumes int can hold any size
int size = sizeof(large_array);  
// On 64-bit systems, arrays can be > 4GB
// int max: 2,147,483,647 (~ 2GB)
// This will overflow!

// ✅ PORTABLE: size_t adjusts to platform
size_t size = sizeof(large_array);
// On 32-bit: size_t is 32-bit (max 4GB)
// On 64-bit: size_t is 64-bit (max 16 exabytes)
```

### Platform Sizes

| Platform | size_t Size | Maximum Value |
|----------|-------------|---------------|
| 32-bit | 4 bytes | 4,294,967,295 (~ 4GB) |
| 64-bit | 8 bytes | 18,446,744,073,709,551,615 (~ 16 EB) |

### Common Uses of size_t

#### 1. sizeof Operator
```cpp
size_t int_size = sizeof(int);          // 4 bytes
size_t array_size = sizeof(int) * 100;  // 400 bytes
```

#### 2. String Length
```cpp
#include <cstring>

char str[] = "Hello";
size_t length = strlen(str);  // Returns size_t (5)
```

#### 3. Array Indexing
```cpp
int numbers[100];
for(size_t i = 0; i < 100; i++) {
    numbers[i] = i;
}
```

#### 4. Memory Allocation
```cpp
#include <cstdlib>

size_t num_bytes = 1024;
char *buffer = (char*)malloc(num_bytes);
free(buffer);
```

### Practical Example from document_store.cpp

```cpp
size_t len = 0;  // Used with getline()
char *line = NULL;
ssize_t current_length;

// getline() expects size_t* for buffer size
while((current_length = getline(&line, &len, file)) != -1) {
    // len is updated by getline() with buffer size
    // current_length is the actual line length read
}
```

### size_t vs int: Key Differences

| Aspect | size_t | int |
|--------|--------|-----|
| Signed | No (unsigned) | Yes (signed) |
| Negative values | Cannot represent | Can represent |
| Size | Platform-dependent | Usually 4 bytes |
| Purpose | Sizes, counts | General numbers |
| Example | `100` elements | `-50` temperature |

### Common size_t Mistake: Underflow

```cpp
// ❌ DANGER: Unsigned underflow
size_t a = 5;
size_t b = 10;
size_t result = a - b;  // Wraps around!
// result = 18,446,744,073,709,551,611 (not -5!)

printf("%zu\n", result);  // Huge positive number
```

**Visual Representation:**
```
Unsigned Integer Wheel (size_t):
    MAX VALUE
        ↑
  ...  │  ...
   2   │   MAX-2
   1   │   MAX-1
   0 ──┴── MAX
   
5 - 10 wraps around:
5 → 4 → 3 → 2 → 1 → 0 → MAX → MAX-1 → ... → MAX-4
                        (wraps to huge positive)
```

### Safe Comparison with Signed Integers

```cpp
int signed_num = -5;
size_t unsigned_num = 10;

// ❌ WRONG: Dangerous comparison
if(signed_num < unsigned_num) {  
    // signed_num is converted to size_t
    // -5 becomes huge positive number!
    // Condition is FALSE (unexpected)
}

// ✅ CORRECT: Cast size_t to int
if(signed_num < (int)unsigned_num) {
    // Now both are signed
    // Condition is TRUE (expected)
}
```

### Related Types

```cpp
#include <cstddef>

size_t   // Unsigned size type
ssize_t  // Signed size type (POSIX, not standard C++)
ptrdiff_t // Signed type for pointer differences

// Example:
char *start = buffer;
char *end = buffer + 100;
ptrdiff_t distance = end - start;  // 100
```

### Printf/Scanf Format Specifiers

```cpp
size_t value = 42;

// ✅ CORRECT format specifiers
printf("%zu\n", value);   // C99/C++11: size_t
printf("%zx\n", value);   // Hexadecimal size_t

// ❌ WRONG (common mistake)
printf("%d\n", value);    // Treats size_t as int - unsafe!
printf("%lu\n", value);   // Treats size_t as long - not portable!
```

### Practical Size Limits

```cpp
#include <cstddef>
#include <limits>

// Maximum value of size_t
size_t max = std::numeric_limits<size_t>::max();

// 32-bit system: 4,294,967,295 (~ 4 GB)
// 64-bit system: 18,446,744,073,709,551,615 (~ 16 EB)
```

### When NOT to Use size_t

```cpp
// ❌ Don't use for negative values
size_t temperature = -10;  // Wraps to huge positive!

// ❌ Don't use for loop counts that go backwards
for(size_t i = 10; i >= 0; i--) {  // Infinite loop!
    // i never goes negative, wraps to MAX at i-1 when i=0
}

// ✅ Use int for these cases
int temperature = -10;  // OK
for(int i = 10; i >= 0; i--) {  // OK
```

### Key Points
- `size_t` is **unsigned** - cannot represent negative values
- Automatically adjusts size on different platforms (32-bit vs 64-bit)
- Used by `sizeof`, `strlen`, `malloc`, and similar functions
- Beware of underflow: subtracting larger from smaller wraps around
- Use `%zu` format specifier with `printf`
- Use `int` or `ptrdiff_t` if negative values are possible

---

## Topic 7: getline() - Reading Lines (POSIX)

### What is getline()?
`getline()` is a **POSIX function** (not standard C) that reads an entire line from a file stream, including the newline character. It automatically allocates memory for the line, making it convenient for reading lines of unknown length.

### Syntax
```c
ssize_t getline(char **lineptr, size_t *n, FILE *stream);
```

### Parameters
- **lineptr**: Pointer to buffer (allocated automatically by `getline()`)
- **n**: Pointer to buffer size (updated by `getline()`)
- **stream**: File stream to read from

### Return Value
- **Success**: Returns number of characters read (including newline, excluding null terminator)
- **EOF or Error**: Returns -1

### How getline() Works

**Memory Management:**
```cpp
char *line = NULL;    // Start with NULL pointer
size_t len = 0;       // Start with 0 size
ssize_t read;

// First call: getline() allocates memory
read = getline(&line, &len, file);
// After: line points to malloc'd buffer, len = buffer size

// Subsequent calls: getline() reuses or expands buffer
read = getline(&line, &len, file);

// Must free when done
free(line);
```

### Visual Representation
```
Before getline():          After getline():
line = NULL               line ──→ +---+---+---+---+---+---+
len = 0                            | H | e | y |\n | \0|   |
                                   +---+---+---+---+---+---+
                          len = 6 (buffer capacity)
                          read = 4 (actual characters including \n)
```

### Practical Example from document_store.cpp

```cpp
FILE *file = fopen(file_name, "r");
char *line = NULL;
size_t len = 0;
ssize_t current_length;

// Read lines until EOF
while((current_length = getline(&line, &len, file)) != -1) {
    *linecounter = *linecounter + 1;  // Count lines
    
    // Remove newline from length calculation
    if(line[current_length-1] == '\n') {
        current_length--;
    }
    
    // Track maximum line length
    if(current_length > *maxlength) {
        *maxlength = current_length;
    }
}

free(line);   // Must free allocated memory
fclose(file);
```

### Key Features

#### 1. Automatic Memory Allocation
```cpp
// ✅ getline() manages memory automatically
char *line = NULL;
size_t len = 0;
getline(&line, &len, file);  // Allocates memory

// ❌ fgets() requires pre-allocated buffer
char buffer[256];
fgets(buffer, 256, file);  // Limited to 256 chars
```

#### 2. Handles Arbitrary Line Length
```cpp
// getline() can read lines of ANY length
// It automatically expands buffer if needed

// Read 10-character line: allocates 10+ bytes
// Read 10,000-character line: expands to 10,000+ bytes
```

#### 3. Includes Newline Character
```cpp
char *line = NULL;
size_t len = 0;
ssize_t read = getline(&line, &len, file);

// If file contains: "Hello\n"
// line = "Hello\n\0"  (newline included)
// read = 6

// To remove newline:
if(read > 0 && line[read-1] == '\n') {
    line[read-1] = '\0';  // Replace \n with \0
}
```

### getline() vs fgets()

| Feature | getline() | fgets() |
|---------|-----------|---------|
| Standard | POSIX only | C Standard |
| Buffer size | Auto-managed | Fixed size |
| Line length | Unlimited | Limited by buffer |
| Newline | Included | Included |
| Memory | Auto malloc() | Pre-allocated |
| Portability | Linux/Unix/MinGW | All platforms |

### Comparison Example

**Using fgets():**
```cpp
char buffer[100];
if(fgets(buffer, 100, file) != NULL) {
    // Can only read 99 characters
    // Long lines are truncated
}
```

**Using getline():**
```cpp
char *line = NULL;
size_t len = 0;
if(getline(&line, &len, file) != -1) {
    // Can read lines of any length
    // No truncation
}
free(line);
```

### Important Usage Rules

#### 1. Initialize Pointers Correctly
```cpp
// ✅ CORRECT
char *line = NULL;
size_t len = 0;
getline(&line, &len, file);

// ❌ WRONG: Uninitialized
char *line;        // Contains garbage
size_t len;        // Contains garbage
getline(&line, &len, file);  // Undefined behavior!
```

#### 2. Always Free Memory
```cpp
char *line = NULL;
size_t len = 0;

while(getline(&line, &len, file) != -1) {
    // Process line
}

free(line);  // ✅ Must free to avoid memory leak
```

#### 3. Reusing the Buffer
```cpp
char *line = NULL;
size_t len = 0;

// First getline: allocates new buffer
getline(&line, &len, file);  // Allocates 64 bytes

// Second getline: reuses buffer if big enough
getline(&line, &len, file);  // Reuses 64 bytes

// Third getline: expands if line is longer
getline(&line, &len, file);  // Expands to 128 bytes

free(line);  // Free once at the end
```

### Return Value Explained

```cpp
ssize_t read = getline(&line, &len, file);

// read = 5  → Read 5 characters (including \n)
// read = 0  → Empty line (just \n)
// read = -1 → EOF or error

if(read == -1) {
    if(feof(file)) {
        // End of file
    } else if(ferror(file)) {
        // Read error
    }
}
```

### Complete Working Example

```cpp
#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *file = fopen("document.txt", "r");
    if(file == NULL) {
        perror("fopen");
        return 1;
    }
    
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int line_number = 1;
    
    while((read = getline(&line, &len, file)) != -1) {
        printf("Line %d (%zd chars): %s", line_number, read, line);
        line_number++;
    }
    
    free(line);
    fclose(file);
    return 0;
}
```

### Key Points
- `getline()` is a POSIX function (available on Linux, Unix, Windows with MinGW)
- Automatically allocates and manages memory for the line buffer
- Returns number of characters read (including newline)
- Returns -1 on EOF or error
- Must initialize `lineptr` to `NULL` and `n` to `0` for first call
- Must `free()` the allocated memory when done
- Includes newline character in the returned string
- Can read lines of arbitrary length (no truncation)

---

## Topic 8: getline() Windows Problem & Solutions

### The Problem: getline() Hangs on Empty Files (Windows)

#### What Happens?
On Windows systems with POSIX compatibility layers (like MinGW, Cygwin), `getline()` has a **critical bug**: it **hangs indefinitely** when called on an **empty file** instead of returning -1.

```cpp
// ❌ PROBLEMATIC CODE (Windows)
FILE *file = fopen("empty.txt", "r");  // File has 0 bytes
char *line = NULL;
size_t len = 0;

while(getline(&line, &len, file) != -1) {  // HANGS HERE! 
    // Never executes
}
// Program freezes forever
```

### Why This Happens?

**Expected Behavior (POSIX Standard):**
```
Empty file → fgetc() returns EOF → getline() returns -1 ✓
```

**Actual Behavior (Windows POSIX layer):**
```
Empty file → getline() waits for input → never returns ❌
```

**Root Cause**: Windows implementation of `getline()` doesn't properly check for EOF condition before attempting to read, causing it to wait for data that will never arrive.

### Visual Representation

```
Linux/Unix:                      Windows (MinGW):
+-----------+                    +-----------+
| Empty File|                    | Empty File|
+-----------+                    +-----------+
      ↓                                ↓
  getline()                        getline()
      ↓                                ↓
  Check EOF                         Wait for data...
      ↓                                ↓
  Return -1 ✓                      [HANGS] ❌
```

### Real-World Impact

```cpp
// This code works fine on Linux but hangs on Windows
int read_sizes(int *linecounter, int *maxlength, char *file_name){
    *linecounter = 0;
    *maxlength = 0;
    
    FILE *file = fopen(file_name, "r");
    if(file == NULL) {
        return -1;
    }
    
    char *line = NULL;
    size_t len = 0;
    ssize_t current_length;
    
    // ❌ If file is empty, program freezes here on Windows
    while((current_length = getline(&line, &len, file)) != -1) {
        // ...
    }
    
    free(line);
    fclose(file);
    return 0;
}
```

---

## Solutions to Empty File Problem

### Solution 1: Check File Empty Before getline() ⭐ RECOMMENDED

**Method A: Using fgetc() and ungetc() (Simplest)**

```cpp
FILE *file = fopen(file_name, "r");
if(file == NULL) {
    return -1;
}

// Check if file is empty
int c = fgetc(file);
if(c == EOF){
    cout << "File is empty: " << file_name << endl;
    fclose(file);
    return -1;
}
ungetc(c, file);  // Put character back

// Now safe to use getline()
char *line = NULL;
size_t len = 0;
while(getline(&line, &len, file) != -1) {
    // Process line
}

free(line);
fclose(file);
```

**Why This Works:**
- `fgetc()` reliably returns `EOF` on empty files (even on Windows)
- `ungetc()` puts the character back so file position is unchanged
- Only 3 lines of simple code
- **Beginner-friendliness: 10/10**

---

### Solution 2: Use fseek() and ftell()

```cpp
FILE *file = fopen(file_name, "r");
if(file == NULL) {
    return -1;
}

// Check file size
fseek(file, 0, SEEK_END);    // Go to end
long size = ftell(file);     // Get position (= file size)
rewind(file);                // Go back to start

if(size == 0){
    cout << "File is empty: " << file_name << endl;
    fclose(file);
    return -1;
}

// Now safe to use getline()
char *line = NULL;
size_t len = 0;
while(getline(&line, &len, file) != -1) {
    // Process line
}

free(line);
fclose(file);
```

**Pros:**
- Reliable on all platforms
- Gets exact file size

**Cons:**
- More complex (4 function calls)
- **Beginner-friendliness: 6/10**

---

### Solution 3: Use stat() System Call

```cpp
#include <sys/stat.h>

FILE *file = fopen(file_name, "r");
if(file == NULL) {
    return -1;
}

// Get file information
struct stat file_stat;
if(stat(file_name, &file_stat) == 0) {
    if(file_stat.st_size == 0) {
        cout << "File is empty: " << file_name << endl;
        fclose(file);
        return -1;
    }
}

// Now safe to use getline()
char *line = NULL;
size_t len = 0;
while(getline(&line, &len, file) != -1) {
    // Process line
}

free(line);
fclose(file);
```

**Pros:**
- Most efficient (no file reading needed)
- Provides additional file information

**Cons:**
- Requires understanding of structs and system calls
- **Beginner-friendliness: 4/10**

---

### Solution 4: Replace getline() with fgets() ⭐ BEST for Portability

```cpp
FILE *file = fopen(file_name, "r");
if(file == NULL) {
    return -1;
}

char buffer[4096];  // Fixed-size buffer

// fgets() returns NULL on empty file (no hang)
while(fgets(buffer, sizeof(buffer), file) != NULL) {
    int current_length = strlen(buffer);
    
    // Remove newline
    if(current_length > 0 && buffer[current_length-1] == '\n') {
        current_length--;
    }
    
    *linecounter = *linecounter + 1;
    if(current_length > *maxlength) {
        *maxlength = current_length;
    }
}

fclose(file);
```

**Pros:**
- `fgets()` is standard C (works everywhere)
- No empty file bug
- No memory allocation needed

**Cons:**
- Limited buffer size (truncates very long lines)
- **Beginner-friendliness: 9/10**

---

### Solution 5: Check EOF Before Loop

```cpp
FILE *file = fopen(file_name, "r");
if(file == NULL) {
    return -1;
}

char *line = NULL;
size_t len = 0;
ssize_t current_length;

// Check if file is empty using feof()
if(feof(file)) {
    cout << "File is empty: " << file_name << endl;
    fclose(file);
    return -1;
}

// Note: This doesn't work! feof() is false until AFTER read fails
// Including for completeness only
```

**Why This Doesn't Work:**
- `feof()` returns `false` until AFTER a read fails
- Empty file has `feof() == false` before first read
- **Not a solution** - included to explain common misconception

---

### Comparison Table

| Solution | Lines of Code | Complexity | Portability | Beginner-Friendly |
|----------|---------------|------------|-------------|-------------------|
| fgetc/ungetc | 3 | Low | High | ⭐⭐⭐⭐⭐ (10/10) |
| fseek/ftell | 4 | Medium | High | ⭐⭐⭐ (6/10) |
| stat() | 5+ | High | Medium | ⭐⭐ (4/10) |
| fgets() instead | Replace function | Low | Very High | ⭐⭐⭐⭐ (9/10) |

### Our Implementation Choice

**We chose fgetc()/ungetc() method because:**
1. ✅ Simplest code (3 lines)
2. ✅ Easy to understand for beginners
3. ✅ No complex system calls or structs
4. ✅ Reliable on all platforms
5. ✅ Doesn't change file position after check
6. ✅ Perfect for our use case

```cpp
// Final implementation in document_store.cpp
int c = fgetc(file);
if(c == EOF){
    cout << "File is empty: " << file_name << endl;
    fclose(file);
    return -1;
}
ungetc(c, file);
```

### Platform Compatibility Summary

| Platform | getline() Bug | fgetc/ungetc Works | fgets() Works |
|----------|---------------|-------------------|---------------|
| Linux | ❌ No bug | ✅ Yes | ✅ Yes |
| macOS | ❌ No bug | ✅ Yes | ✅ Yes |
| Windows (MinGW) | ✅ **HAS BUG** | ✅ Yes | ✅ Yes |
| Windows (Cygwin) | ✅ **HAS BUG** | ✅ Yes | ✅ Yes |
| Windows (MSVC) | ⚠️ No getline() | N/A | ✅ Yes |

### Key Points
- `getline()` hangs on empty files in Windows POSIX implementations
- Always check for empty files BEFORE calling `getline()` on Windows
- **Best simple solution**: Use `fgetc()` to peek, then `ungetc()` to restore
- **Best portable solution**: Replace `getline()` with `fgets()`
- `feof()` doesn't work for this check (returns false before first read)
- The bug only affects Windows - Linux/Unix work correctly

---

## Topic 9: free() - Deallocating Memory

### What is free()?
`free()` is a C standard library function used to **deallocate memory** that was previously allocated dynamically using `malloc()`, `calloc()`, or `realloc()`. It returns the memory to the system for reuse.

### Syntax
```c
void free(void *ptr);
```

### Parameters
- **ptr**: Pointer to memory block to deallocate

### Return Value
- **None** (`void`)

### Why free() is Critical?

**Memory Leak Without free():**
```cpp
// ❌ MEMORY LEAK
for(int i = 0; i < 1000000; i++) {
    char *buffer = (char*)malloc(1024);  // Allocate 1 KB
    // ... use buffer ...
    // Missing free(buffer)
    // Leaks 1 KB per iteration = 1 GB total!
}
```

**Proper Memory Management:**
```cpp
// ✅ CORRECT
for(int i = 0; i < 1000000; i++) {
    char *buffer = (char*)malloc(1024);  // Allocate 1 KB
    // ... use buffer ...
    free(buffer);  // Deallocate 1 KB
    // Memory is reused
}
```

### Practical Example from document_store.cpp

```cpp
char *line = NULL;
size_t len = 0;

// getline() allocates memory for line
while(getline(&line, &len, file) != -1) {
    // line points to dynamically allocated memory
    // ... process line ...
}

// Must free the allocated memory
free(line);  // Returns memory to system
fclose(file);
```

### How Memory Allocation Works

**Visual Representation:**
```
Program Memory:
+----------------+
|  Stack         | ← Local variables (automatic)
|  int x;        |
|  char c;       |
+----------------+
|  ↓ grows down  |
|                |
|  ↑ grows up    |
+----------------+
|  Heap          | ← Dynamic memory (manual)
|  malloc()      |
|  getline()     |
+----------------+
|  Global/Static | ← Global variables
+----------------+
|  Code          | ← Program instructions
+----------------+
```

**malloc() and free():**
```
Step 1: char *ptr = malloc(100);
        Heap: [■■■■■ allocated 100 bytes ■■■■■]
              ptr → [___|___|___| ... |___]
              
Step 2: ... use ptr ...
        ptr[0] = 'A';
        ptr → [_A_|___|___| ... |___]

Step 3: free(ptr);
        Heap: [_____ freed 100 bytes _____]
        ptr still contains address but is INVALID
```

### Important Rules for free()

#### 1. Only Free Dynamically Allocated Memory

```cpp
// ✅ CORRECT: Free malloc'd memory
char *p1 = (char*)malloc(100);
free(p1);

// ❌ WRONG: Don't free stack memory
char buffer[100];
free(buffer);  // ❌ CRASH! Not from malloc()

// ❌ WRONG: Don't free string literals
char *str = "Hello";
free(str);  // ❌ CRASH! Not from malloc()
```

#### 2. Don't Use Pointer After free()

```cpp
char *buffer = (char*)malloc(100);
buffer[0] = 'A';  // ✅ OK

free(buffer);     // Memory deallocated

buffer[0] = 'B';  // ❌ UNDEFINED BEHAVIOR!
// Memory might be reused by other code
printf("%c", buffer[0]);  // ❌ UNDEFINED BEHAVIOR!
```

#### 3. Don't free() Twice

```cpp
char *buffer = (char*)malloc(100);
free(buffer);     // ✅ First free - OK
free(buffer);     // ❌ DOUBLE FREE - CRASH!
```

#### 4. free(NULL) is Safe

```cpp
char *ptr = NULL;
free(ptr);  // ✅ Does nothing, safe
```

### Complete malloc/free Lifecycle

```cpp
// 1. Allocate memory
char *line = NULL;
size_t len = 0;

// 2. getline() internally calls malloc
ssize_t read = getline(&line, &len, file);
// Now: line → [memory allocated by getline]

// 3. Use the memory
if(read != -1) {
    printf("%s", line);
}

// 4. Free when done
free(line);  // Return memory to system
line = NULL; // Good practice: set to NULL after free
```

### Memory Leak Detection Example

**Without free():**
```cpp
void process_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    char *line = NULL;
    size_t len = 0;
    
    while(getline(&line, &len, file) != -1) {
        // Process line
    }
    
    fclose(file);
    // ❌ Forgot free(line) - MEMORY LEAK!
}

// Call this 100 times → 100 memory leaks
for(int i = 0; i < 100; i++) {
    process_file("document.txt");
}
```

**With free():**
```cpp
void process_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    char *line = NULL;
    size_t len = 0;
    
    while(getline(&line, &len, file) != -1) {
        // Process line
    }
    
    free(line);  // ✅ Properly deallocate
    fclose(file);
}

// Call this 100 times → no memory leaks
for(int i = 0; i < 100; i++) {
    process_file("document.txt");
}
```

### Common Patterns

#### Pattern 1: Single Allocation
```cpp
char *buffer = (char*)malloc(256);
if(buffer != NULL) {
    // Use buffer
    strcpy(buffer, "Hello");
}
free(buffer);
```

#### Pattern 2: Reusing Buffer (getline)
```cpp
char *line = NULL;
size_t len = 0;

// Multiple calls reuse/expand the same buffer
getline(&line, &len, file);  // Allocates
getline(&line, &len, file);  // Reuses or expands
getline(&line, &len, file);  // Reuses or expands

free(line);  // Free once at end
```

#### Pattern 3: Array of Pointers
```cpp
char **lines = (char**)malloc(100 * sizeof(char*));

for(int i = 0; i < 100; i++) {
    lines[i] = (char*)malloc(256);  // Allocate each line
}

// Must free each line AND the array
for(int i = 0; i < 100; i++) {
    free(lines[i]);  // Free each line
}
free(lines);  // Free the array
```

### Best Practices

#### 1. Set Pointer to NULL After free()
```cpp
free(buffer);
buffer = NULL;  // Prevents accidental use after free
```

#### 2. Check malloc() Return Value
```cpp
char *buffer = (char*)malloc(1024);
if(buffer == NULL) {
    fprintf(stderr, "Out of memory!\n");
    return -1;
}
// Use buffer
free(buffer);
```

#### 3. Match Every malloc with free
```cpp
// ✅ CORRECT: Balanced malloc/free
void function() {
    char *p = malloc(100);  // Allocate
    // ... use p ...
    free(p);                // Deallocate
}

// ❌ WRONG: Missing free
void function() {
    char *p = malloc(100);  // Allocate
    // ... use p ...
    // Missing free(p) - MEMORY LEAK!
}
```

### C++ Alternative: Smart Pointers

In modern C++, prefer smart pointers over manual `malloc`/`free`:

```cpp
// C++ way (automatic memory management)
#include <memory>
#include <string>

void function() {
    auto buffer = std::make_unique<char[]>(100);
    // Use buffer
    // Automatically freed when function ends - no free() needed!
}
```

### Memory Leak Tools

**Linux:**
```bash
# Valgrind - detects memory leaks
valgrind --leak-check=full ./program

# Example output:
# 100 bytes in 1 blocks are definitely lost
# at malloc() (in /lib/libc.so)
# by getline() (in /lib/libc.so)
# by main() (program.cpp:42)
```

**Windows:**
```cpp
// Visual Studio: CRT Debug Heap
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

int main() {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    
    // Your code
    
    // Leaks reported at program exit
}
```

### Key Points
- `free()` deallocates memory allocated by `malloc()`, `calloc()`, or `realloc()`
- Always `free()` dynamically allocated memory to prevent memory leaks
- Only `free()` pointers that came from dynamic allocation
- Don't use memory after calling `free()` (undefined behavior)
- Don't call `free()` twice on the same pointer (double free error)
- `free(NULL)` is safe and does nothing
- `getline()` allocates memory that **must** be freed with `free()`
- Set pointers to `NULL` after freeing (best practice)
- In C++, prefer smart pointers (`std::unique_ptr`, `std::shared_ptr`)

---

## Summary

This document covered all essential C file I/O functions used in `document_store.cpp`:

1. **fopen()**: Opens files with various modes (read, write, append)
2. **fclose()**: Closes files and flushes buffers to disk
3. **fgetc()**: Reads one character at a time from files
4. **eof()**: Detects end-of-file condition (after read fails)
5. **ungetc()**: Pushes character back to stream (peek functionality)
6. **size_t**: Unsigned integer type for sizes and counts
7. **getline()**: POSIX function for reading entire lines dynamically
8. **Windows Bug**: getline() hangs on empty files - solution: check first with fgetc()
9. **free()**: Deallocates dynamically allocated memory
10. **read_input()**: Reads file line-by-line and inserts into Mymap container

### Key Takeaways for Beginners

- Always check return values (`NULL`, `-1`, `EOF`) before using results
- Match every `fopen()` with `fclose()`
- Match every `malloc()`/`getline()` with `free()`
- Initialize pointers to `NULL` before use
- Use `size_t` for sizes and counts (not `int`)
- Check for empty files before `getline()` on Windows
- Understand the difference between stack and heap memory
- Clean up resources on error (close files, free memory)

---

## Topic 10: read_input() - Loading Documents into Map

### What is read_input()?
`read_input()` is a **custom function** in our search engine that reads a file line-by-line and inserts each line as a document into a `Mymap` object. It combines file I/O operations with our custom data structure.

### Function Signature
```cpp
int read_input(Mymap* mymap, char* file_name);
```

### Parameters
- **mymap**: Pointer to Mymap object where documents will be stored
- **file_name**: Path to the file to read

### Return Value
- **1**: Success - all lines read and inserted
- **-1**: Error - file opening failed, insertion failed, or other error

---

### Code Implementation

```cpp
int read_input(Mymap* mymap, char* file_name){
    FILE *file = fopen(file_name, "r");
    char *line = NULL;
    size_t buffersize = 0;
    int current_length;
    
    for(int i=0; i<mymap->get_size(); i++){
        getline(&line, &buffersize, file);
        if (mymap->insert(line, i) == -1) {
            cout << "Error inserting line " << endl;
            free(line);
            fclose(file);
            return -1;
        }
        free(line);
        line = NULL;
        buffersize = 0;
    }
    free(line);
    fclose(file);
    return 1;
}
```

---

### Step-by-Step Breakdown

#### Step 1: Open File
```cpp
FILE *file = fopen(file_name, "r");
```

**What it does:**
- Opens file for reading
- Returns `FILE*` pointer

**Note:** ⚠️ **Missing NULL check!** Should add:
```cpp
if(file == NULL) {
    cout << "Cannot open file: " << file_name << endl;
    return -1;
}
```

---

#### Step 2: Initialize Variables
```cpp
char *line = NULL;
size_t buffersize = 0;
int current_length;
```

**Variables explained:**

| Variable | Type | Purpose |
|----------|------|---------|
| `line` | `char*` | Pointer to line buffer (allocated by getline) |
| `buffersize` | `size_t` | Size of allocated buffer |
| `current_length` | `int` | Length of line read (unused in current code) |

**Why initialize to NULL/0?**
- `getline()` needs to know if buffer is already allocated
- `NULL` tells getline to allocate new buffer
- Safe default values

---

#### Step 3: Loop Through Documents
```cpp
for(int i=0; i<mymap->get_size(); i++){
```

**What it does:**
- Iterates through all document slots in the map
- `mymap->get_size()` returns total capacity
- Each iteration reads one line

**Example:**
```cpp
Mymap has size 7
Loop runs: i=0, i=1, i=2, i=3, i=4, i=5, i=6
Reads 7 lines from file
```

---

#### Step 4: Read Line
```cpp
getline(&line, &buffersize, file);
```

**What happens:**
- Reads next line from file (including newline)
- Allocates/reallocates buffer if needed
- Stores line in `line` pointer
- Updates `buffersize` with buffer size

**Example:**
```
File content:
"Hello World\n"
"Search Engine\n"

First call: line = "Hello World\n", buffersize = 13
Second call: line = "Search Engine\n", buffersize = 15
```

**Memory management:**
- `getline()` allocates memory automatically
- We must free it later with `free(line)`

---

#### Step 5: Insert Line into Map
```cpp
if (mymap->insert(line, i) == -1) {
    cout << "Error inserting line " << endl;
    free(line);
    fclose(file);
    return -1;
}
```

**What happens:**

1. **Call insert()**: Tries to insert line at index `i`
2. **Check return value**: `-1` means error
3. **On error**:
   - Print error message
   - Free the line buffer
   - Close the file
   - Return error code

**Why cleanup on error?**
- Prevents memory leaks
- Closes file properly
- Leaves system in clean state

**Example error scenarios:**
```
- Line is NULL (shouldn't happen with getline)
- Index out of bounds (shouldn't happen with correct loop)
- Memory allocation fails in insert()
```

---

#### Step 6: Clean Up After Each Line
```cpp
free(line);
line = NULL;
buffersize = 0;
```

**Why do this?**

1. **`free(line)`**: Deallocates buffer allocated by getline
2. **`line = NULL`**: Reset pointer for next getline call
3. **`buffersize = 0`**: Reset size for next allocation

**Why reset to NULL?**
```cpp
// Without reset:
getline(&line, &buffersize, file);  // First call
free(line);
getline(&line, &buffersize, file);  // Second call
// Uses freed pointer! 💥 CRASH!

// With reset:
getline(&line, &buffersize, file);  // First call
free(line);
line = NULL;
buffersize = 0;
getline(&line, &buffersize, file);  // Second call
// Allocates new buffer ✅ Safe!
```

---

#### Step 7: Final Cleanup
```cpp
free(line);
fclose(file);
return 1;
```

**Why final free()?**
- Safety measure
- In case loop didn't run or last iteration allocated
- `free(NULL)` is safe (does nothing)

**Why fclose()?**
- Closes file handle
- Flushes buffers
- Releases system resources

**Return 1:**
- Indicates success
- All lines read and inserted successfully

---

### Visual Execution Flow

```
START
  ↓
[1] Open file for reading
  ↓
[2] Initialize: line=NULL, buffersize=0
  ↓
[3] Loop: i=0 to size-1
  ↓
  [4] Read line from file (getline allocates memory)
  ↓
  [5] Insert line into mymap[i]
  ↓
  [6] Check if insert succeeded
    ├─ Error → Clean up → Return -1
    └─ Success → Continue
  ↓
  [7] Free line buffer
  ↓
  [8] Reset line=NULL, buffersize=0
  ↓
[9] Repeat for next line
  ↓
[10] All lines processed
  ↓
[11] Final cleanup (free, fclose)
  ↓
[12] Return 1 (success)
  ↓
END
```

---

### Complete Example

**File (doc1.txt):**
```
Introduction to Search Engines
A search engine is a software system
It crawls web pages
```

**Code:**
```cpp
Mymap* map = new Mymap(3, 100);
int result = read_input(map, "doc1.txt");
```

**Execution trace:**

```
Step 1: Open "doc1.txt" ✅
Step 2: line=NULL, buffersize=0

Step 3: Loop i=0
  getline() → line = "Introduction to Search Engines\n"
              buffersize = 32
  
  insert(line, 0) → Trims, allocates, copies
                  → map->documents[0] = "Introduction to Search Engines"
                  → Returns 1 ✅
  
  free(line) → Buffer freed
  line = NULL, buffersize = 0

Step 4: Loop i=1
  getline() → line = "A search engine is a software system\n"
              buffersize = 38
  
  insert(line, 1) → map->documents[1] = "A search engine is a software system"
                  → Returns 1 ✅
  
  free(line) → Buffer freed
  line = NULL, buffersize = 0

Step 5: Loop i=2
  getline() → line = "It crawls web pages\n"
              buffersize = 21
  
  insert(line, 2) → map->documents[2] = "It crawls web pages"
                  → Returns 1 ✅
  
  free(line) → Buffer freed
  line = NULL, buffersize = 0

Step 6: Loop complete
  free(line) → (NULL, does nothing)
  fclose(file) → File closed
  return 1

Result: All 3 lines successfully loaded into map! ✅
```

---

### Memory Management Details

**Memory allocation pattern:**

```
Iteration 0:
  Before getline: line = NULL
  After getline:  line = [allocated buffer]
  After free:     line = NULL

Iteration 1:
  Before getline: line = NULL
  After getline:  line = [NEW allocated buffer]
  After free:     line = NULL

Iteration 2:
  Before getline: line = NULL
  After getline:  line = [NEW allocated buffer]
  After free:     line = NULL
```

**Why reset matters:**
- Each getline allocates a NEW buffer
- Old buffer must be freed first
- Reset ensures getline knows to allocate new memory

---

### Potential Issues and Improvements

#### Issue 1: No NULL Check on fopen

**Current code:**
```cpp
FILE *file = fopen(file_name, "r");
// No check!
for(int i=0; i<mymap->get_size(); i++){
    getline(&line, &buffersize, file);  // Crash if file is NULL!
```

**Improved code:**
```cpp
FILE *file = fopen(file_name, "r");
if(file == NULL){
    cout << "Cannot open file: " << file_name << endl;
    return -1;
}
```

---

#### Issue 2: current_length Unused

**Current code:**
```cpp
int current_length;  // Declared but never used
```

**Can remove:**
```cpp
// Remove this variable, it's not needed
```

---

#### Issue 3: No Check for getline Failure

**Current code:**
```cpp
getline(&line, &buffersize, file);  // What if this fails?
```

**Improved code:**
```cpp
if(getline(&line, &buffersize, file) == -1){
    cout << "Error reading line " << i << endl;
    free(line);
    fclose(file);
    return -1;
}
```

---

### Comparison with read_sizes()

Both functions read the same file, but for different purposes:

| Aspect | read_sizes() | read_input() |
|--------|-------------|--------------|
| **Purpose** | Count lines, find max length | Load documents into memory |
| **Returns** | Line count, max length | Success/failure code |
| **Memory** | Frees each line after measuring | Stores lines in Mymap |
| **When called** | Before creating Mymap | After creating Mymap |
| **Output** | Metadata about file | Populated Mymap object |

**Typical usage together:**
```cpp
// Step 1: Get file statistics
int linecount = 0, maxlength = -1;
read_sizes(&linecount, &maxlength, "doc1.txt");

// Step 2: Create map with correct size
Mymap* map = new Mymap(linecount, maxlength);

// Step 3: Load documents
read_input(map, "doc1.txt");

// Step 4: Use documents
for(int i=0; i<map->get_size(); i++){
    map->print(i);
}
```

---

### Key Concepts Demonstrated

1. **File I/O**: Opening, reading, closing files
2. **Dynamic Memory**: getline allocates, we must free
3. **Error Handling**: Check returns, clean up on error
4. **Resource Management**: Close files, free memory
5. **Integration**: Combining file I/O with custom data structures
6. **Memory Discipline**: Reset pointers after freeing

---

### Common Mistakes to Avoid

❌ **Forgetting to free line:**
```cpp
for(int i=0; i<size; i++){
    getline(&line, &buffersize, file);
    // Missing free(line) → MEMORY LEAK!
}
```

❌ **Not resetting to NULL:**
```cpp
free(line);
getline(&line, &buffersize, file);  // Uses freed pointer!
```

❌ **Not closing file on error:**
```cpp
if(error){
    return -1;  // File still open! → RESOURCE LEAK!
}
```

❌ **Not checking fopen return:**
```cpp
FILE* file = fopen("file.txt", "r");
getline(&line, &buffersize, file);  // CRASH if file is NULL!
```

---

### Best Practices Demonstrated

✅ **Initialize pointers to NULL**
✅ **Free memory after use**
✅ **Reset pointers after freeing**
✅ **Close files before returning**
✅ **Clean up on error paths**
✅ **Check function return values**

---

### Summary

The `read_input()` function is a crucial bridge between file I/O and our search engine's data structures. It demonstrates:

- **File reading** with `fopen()` and `getline()`
- **Memory management** with `malloc()`/`free()`
- **Error handling** with return codes and cleanup
- **Integration** of system functions with custom classes

Understanding this function is key to seeing how raw file data becomes structured, searchable information in memory.

---

## Topic 11: strtok() and Memory Management

### What is strtok()?

`strtok()` is a C standard library function used to **tokenize** (split) a string into smaller strings based on delimiters. It's commonly used to split sentences into words.

### Critical Understanding: strtok() Does NOT Allocate Memory

**This is the most important concept to understand about strtok()!**

```cpp
char text[] = "hello world earth";
char* token = strtok(text, " ");  // Returns pointer to "hello"
```

**What strtok() actually does:**
1. Finds the first delimiter (" " in this case)
2. **Replaces the delimiter with '\0'** in the original string
3. **Returns a pointer to the beginning of the token**
4. **Does NOT allocate new memory**

### Visual Representation

**Before strtok():**
```
Memory address:  [100][101][102][103][104][105][106][107][108]...
String in memory: 'h'  'e'  'l'  'l'  'o'  ' '  'w'  'o'  'r'...
                  ^
                  text points here
```

**After token = strtok(text, " "):**
```
Memory address:  [100][101][102][103][104][105][106][107][108]...
String in memory: 'h'  'e'  'l'  'l'  'o' '\0'  'w'  'o'  'r'...
                  ^                        ^
                  token points here        space replaced with \0
```

**Key Point:** `token` points to address 100 (part of original `text` array), NOT to new allocated memory!

### The Critical Bug We Fixed

**WRONG CODE (Memory Corruption):**
```cpp
void split(char* temp, int id, TrieNode* trie, Mymap* mymap){
    char* token = strtok(temp, " \t");
    while(token != NULL){
        trie->insert(token, id);
        token = strtok(NULL, " \t");
    }
    free(token);  // ❌❌❌ CRITICAL BUG! ❌❌❌
}
```

**Why this is wrong:**
1. `token` points to memory inside `temp` array
2. `temp` is allocated elsewhere (by caller)
3. `free(token)` tries to free memory that wasn't allocated by malloc!
4. **Result**: Memory corruption, undefined behavior, crashes

### Understanding the Fix

**CORRECT CODE:**
```cpp
void split(char* temp, int id, TrieNode* trie, Mymap* mymap){
    char* token = strtok(temp, " \t");
    int i = 0;
    while(token != NULL){
        i++;
        trie->insert(token, id);
        token = strtok(NULL, " \t");
    }
    mymap->setlength(id, i);
    // No free(token)! ✅ Correct - token doesn't own memory
}
```

**Why this is correct:**
- `token` is just a pointer to part of `temp`
- `temp` will be freed by the caller (read_input function)
- No memory leak because no new allocation was made

### Memory Ownership Rule

**Golden Rule:** Only free() memory that was allocated with malloc() (or realloc())

| Function | Allocates New Memory? | Need to free()? |
|----------|----------------------|-----------------|
| `malloc()` | ✅ Yes | ✅ Yes |
| `realloc()` | ✅ Yes | ✅ Yes |
| `getline()` | ✅ Yes (internally) | ✅ Yes |
| `strtok()` | ❌ No | ❌ No |
| Regular assignment | ❌ No | ❌ No |

### Complete Example

```cpp
// Memory allocation and tokenization
char* text = (char*)malloc(100);        // ✅ malloc allocates
strcpy(text, "hello world earth");

char* token = strtok(text, " ");        // token points INTO text
while(token != NULL){
    printf("%s\n", token);               // Use token
    token = strtok(NULL, " ");           // Next token
}

free(text);    // ✅ Free the malloc'd memory
// Do NOT free(token)!  ❌ token doesn't own memory
```

### Visual Memory Layout

```
Heap Memory:
+---+---+---+---+---+---+---+---+---+---+---+---+
| h | e | l | l | o |\0 | w | o | r | l | d |\0 |
+---+---+---+---+---+---+---+---+---+---+---+---+
^                       ^
|                       |
text (malloc'd)         token points here (part of text)

When you free(text):  ✅ Frees entire block
If you free(token):   ❌ Tries to free middle of block = CRASH!
```

### Key Takeaways

1. **strtok() modifies the original string** (replaces delimiters with '\0')
2. **strtok() returns pointers, doesn't allocate** new memory
3. **Never free() a strtok() return value** - it's not malloc'd
4. **Only free() the original buffer** that was tokenized
5. **strtok() is stateful** - uses internal static variable

### Real-World Impact

**Before fix:** Potential crashes when calling split()  
**After fix:** Stable, no memory corruption  
**Lesson:** Understanding memory ownership prevents critical bugs  

---

## Topic 12: getline() Return Value Checking

### What is getline() Return Value?

`getline()` is a POSIX function that reads an entire line from a file. It returns the **number of characters read**, or `-1` on failure.

### Function Signature

```c
ssize_t getline(char **lineptr, size_t *n, FILE *stream);
```

**Return Values:**
- **Positive number**: Successfully read that many characters
- **-1**: End-of-file reached OR an error occurred

### The Bug We Fixed

**WRONG CODE (No Error Checking):**
```cpp
for(int i = 0; i < mymap->get_size(); i++){
    getline(&line, &buffersize, file);  // ❌ No check!
    if (mymap->insert(line, i) == -1) {
        // ...
    }
}
```

**Problems:**
1. If file has fewer lines than expected, getline() returns -1
2. `line` might be NULL or contain garbage
3. `mymap->insert(line, i)` processes invalid data
4. No cleanup happens on error

### Why This Bug Matters

**Scenario:**
```cpp
// Expected: 10 documents
Mymap* map = new Mymap(10, 1000);

// But file only has 7 lines!
read_input(map, trie, "data.txt");
```

**What happens without check:**
```
i=0: getline() returns 50  ✅ Read line 1
i=1: getline() returns 48  ✅ Read line 2
...
i=6: getline() returns 42  ✅ Read line 7
i=7: getline() returns -1  ❌ EOF! No more lines
     → line might be NULL
     → insert(NULL, 7) crashes or undefined behavior
i=8: getline() returns -1  ❌ Still EOF
     → More bad data inserted
i=9: getline() returns -1  ❌ Still EOF
     → Even more bad data
```

### The Fix - Proper Error Handling

**CORRECT CODE:**
```cpp
for(int i = 0; i < mymap->get_size(); i++){
    if(getline(&line, &buffersize, file) == -1){  // ✅ Check return value
        cout << "Error reading line " << i << endl;
        free(line);
        fclose(file);
        free(temp);
        return -1;
    }
    if (mymap->insert(line, i) == -1) {
        // ... existing error handling
    }
}
```

**What this does:**
1. **Checks if getline() succeeded** before using the data
2. **Prints informative error** showing which line failed
3. **Cleans up all resources** (frees memory, closes file)
4. **Returns error code** to caller
5. **Prevents processing invalid data**

### Visual Flow Comparison

**Without Error Check:**
```
read_input() called
  ↓
Loop iteration i=7
  ↓
getline() → returns -1 (EOF)
  ↓
insert(garbage, 7) ❌ Undefined behavior!
  ↓
Loop continues
  ↓
More bad inserts...
```

**With Error Check:**
```
read_input() called
  ↓
Loop iteration i=7
  ↓
getline() → returns -1 (EOF)
  ↓
Check: if (getline() == -1) ✅ TRUE
  ↓
Print error message
  ↓
free(line)
  ↓
fclose(file)
  ↓
free(temp)
  ↓
return -1 (error)
  ↓
Caller handles error properly
```

### Complete Corrected Function

```cpp
int read_input(Mymap* mymap, TrieNode *trie, char* file_name){
    FILE *file = fopen(file_name, "r");
    if(file == NULL){                          // ✅ Check fopen
        cout << "Error opening file: " << file_name << endl;
        return -1;
    }
    
    char *line = NULL;
    size_t buffersize = 0;
    char *temp = (char*)malloc(mymap->get_buffersize() * sizeof(char));
    
    for(int i = 0; i < mymap->get_size(); i++){
        if(getline(&line, &buffersize, file) == -1){  // ✅ Check getline
            cout << "Error reading line " << i << endl;
            free(line);
            fclose(file);
            free(temp);
            return -1;
        }
        
        if (mymap->insert(line, i) == -1) {    // ✅ Check insert
            cout << "Error inserting line " << endl;
            free(line);
            fclose(file);
            free(temp);
            return -1;
        }
        
        strcpy(temp, mymap->getDocument(i));
        split(temp, i, trie, mymap);
        
        free(line);
        line = NULL;
        buffersize = 0;
    }
    
    free(line);
    fclose(file);
    free(temp);
    return 1;
}
```

### Error Handling Best Practices

✅ **Always check return values** of I/O functions  
✅ **Provide informative error messages** with context  
✅ **Clean up all resources** before returning from error  
✅ **Use consistent error codes** (-1 for errors, 1 for success)  
✅ **Fail fast** - return immediately on error  

### Common getline() Return Values

| Return Value | Meaning | Action |
|-------------|---------|--------|
| `> 0` | Success - number of chars read | Use the data |
| `0` | Empty line read | Valid, process it |
| `-1` | EOF or error | Check, cleanup, return error |

### Real-World Impact

**Before fix:** Silent failures, corrupted data structures  
**After fix:** Explicit errors, proper cleanup, debugging-friendly  
**Lesson:** Checking return values prevents hard-to-debug issues  

---

### Summary of All Topics

1. **fopen()**: Opens files, returns FILE* or NULL
2. **fclose()**: Closes files, flushes buffers
3. **fgetc()**: Reads single character (peek for empty file check)
4. **eof()**: Detects end-of-file condition (after read fails)
5. **ungetc()**: Pushes character back to stream (peek functionality)
6. **size_t**: Unsigned integer type for sizes and counts
7. **getline()**: POSIX function for reading entire lines dynamically
8. **Windows Bug**: getline() hangs on empty files - solution: check first with fgetc()
9. **free()**: Deallocates dynamically allocated memory
10. **read_input()**: Reads file line-by-line and inserts into Mymap container
11. **strtok() Memory**: Returns pointers, doesn't allocate - never free() them
12. **getline() Return Check**: Always validate return value before using data

### Key Takeaways for Beginners

- Always check return values (`NULL`, `-1`, `EOF`) before using results
- Match every `fopen()` with `fclose()`
- Match every `malloc()`/`getline()` with `free()`
- Initialize pointers to `NULL` before use
- Use `size_t` for sizes and counts (not `int`)
- Check for empty files before `getline()` on Windows
- Understand the difference between stack and heap memory
- Clean up resources on error (close files, free memory)
- **Never free() strtok() return values** - they don't own memory
- **Always check getline() return value** - prevents processing garbage

---

**Document Version**: 1.2  
**Last Updated**: December 26, 2025  
**Changes**: Added strtok() memory management and getline() return checking  
**Author**: High-Performance Search Engine Project  
**Repository**: github.com/adarshpheonix2810/high-performance-search-engine-cpp