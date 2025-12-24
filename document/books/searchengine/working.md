# Search Engine Implementation - Code Explanation

This document provides a **detailed step-by-step explanation** of the `Searchengine.cpp` file implementation. It explains how the code works, what each line does, and the execution flow of the main program.

---

## File Information

**File Name**: `Searchengine.cpp`  
**Location**: `src/Searchengine.cpp`  
**Purpose**: Main entry point for the search engine - handles command-line arguments, validates input, processes document files, and manages the document storage container  
**Header File**: `header/Searchengine.hpp`

---

## Table of Contents

1. [Complete Source Code](#complete-source-code)
2. [Header Files and Dependencies](#header-files-and-dependencies)
3. [Main Function Overview](#main-function-overview)
4. [Command-Line Argument Parsing](#command-line-argument-parsing)
5. [Argument Validation](#argument-validation)
6. [Integer Conversion with Error Handling](#integer-conversion-with-error-handling)
7. [File Size Analysis](#file-size-analysis)
8. [Mymap Object Creation](#mymap-object-creation)
9. [Reading Documents into Memory](#reading-documents-into-memory)
10. [Success Output and Data Display](#success-output-and-data-display)
11. [Document Printing Loop](#document-printing-loop)
12. [Memory Cleanup](#memory-cleanup)
13. [Execution Flow Diagram](#execution-flow-diagram)
14. [Example Usage Scenarios](#example-usage-scenarios)

---

## Complete Source Code

```cpp
#include "Searchengine.hpp"

using namespace std;

// read document/books/searchengine.md for more information
int main(int argc, char** argv) {
    if (argc != 5 ||
        strcmp(argv[1], "-d") != 0 ||
        strcmp(argv[3], "-k") != 0) {
        cout << "Wrong arguments. Usage: -d <file> -k <number>" << endl;
        return -1;
    }

    cout << "Please wait..." << endl;
    int linecounter = 0;
    int maxlength = -1;
    int k;
    try {
        k = stoi(argv[4]); 
    } catch (...) {
        cout << "Invalid value for -k (must be an integer)" << endl;
        return -1;
    }
    
    if(read_sizes(&linecounter, &maxlength, argv[2]) == -1){
        return -1;
    }

    Mymap *mymap=new Mymap(linecounter, maxlength);

    if(read_input(mymap, argv[2]) == -1){
        delete (mymap);
        return -1;
    }

    cout<<"File read successfully. Lines: " << linecounter << ", Max Length: " << maxlength << endl;
    for(int i=0;i<mymap->get_size();i++){
        mymap->print(i);
    }
    delete (mymap);
    return 1;
}
```

---

## Header Files and Dependencies

### Searchengine.hpp Contents

```cpp
#include <iostream>
#include <cstring>
#include "./Document_store.hpp"
```

**Line-by-Line Breakdown:**

#### Line 1: `#include <iostream>`

**What it provides:**
- `cout` - Console output stream
- `endl` - End line manipulator
- Input/output stream functionality

**Used in our code:**
```cpp
cout << "Please wait..." << endl;
cout << "Wrong arguments. Usage: -d <file> -k <number>" << endl;
```

---

#### Line 2: `#include <cstring>`

**What it provides:**
- `strcmp()` - String comparison function
- C-style string manipulation functions

**Used in our code:**
```cpp
strcmp(argv[1], "-d") != 0
strcmp(argv[3], "-k") != 0
```

**Why `strcmp()`?**
- `argv` contains C-style strings (`char*`)
- Cannot use `==` to compare C-style strings (compares pointers, not content)
- `strcmp()` compares actual string content

---

#### Line 3: `#include "./Document_store.hpp"`

**What it provides:**
- `read_sizes()` function declaration
- Access to document processing functionality

**Used in our code:**
```cpp
read_sizes(&linecounter, &maxlength, argv[2])
```

---

### Line 1-2: Include and Namespace

```cpp
#include "Searchengine.hpp"

using namespace std;
```

**Line 1: `#include "Searchengine.hpp"`**
- Includes the header file with all dependencies
- Brings in `iostream`, `cstring`, and `Document_store.hpp`

**Line 2: `using namespace std;`**
- Allows using `cout` instead of `std::cout`
- Allows using `endl` instead of `std::endl`
- Makes code more readable

---

## Main Function Overview

### Function Signature

```cpp
int main(int argc, char** argv)
```

**Return Type**: `int`
- Returns `1` on success
- Returns `-1` on failure (wrong arguments, invalid input, file error)

**Parameters:**

| Parameter | Type | Purpose |
|-----------|------|---------|
| `argc` | `int` | **Arg**ument **C**ount - Number of command-line arguments |
| `argv` | `char**` | **Arg**ument **V**ector - Array of C-style strings containing arguments |

---

### Understanding argc and argv

**Visual Representation:**

```
Command: ./searchengine -d doc1.txt -k 10

argc = 5

argv[0] = "./searchengine"  ← Program name
argv[1] = "-d"              ← Flag
argv[2] = "doc1.txt"        ← File name
argv[3] = "-k"              ← Flag
argv[4] = "10"              ← Number (as string)
```

**Memory Layout:**

```
argv (array of pointers):
+--------+--------+--------+--------+--------+
| ptr[0] | ptr[1] | ptr[2] | ptr[3] | ptr[4] |
+--------+--------+--------+--------+--------+
    ↓        ↓        ↓        ↓        ↓
  "prog"   "-d"   "doc.txt"  "-k"    "10"
```

**Important:** 
- `argv` elements are C-style strings (`char*`)
- All arguments are strings (even numbers)
- Must convert "10" to integer using `stoi()`

---

## Command-Line Argument Parsing

### Lines 6-11: Argument Validation

```cpp
if (argc != 5 ||
    strcmp(argv[1], "-d") != 0 ||
    strcmp(argv[3], "-k") != 0) {
    cout << "Wrong arguments. Usage: -d <file> -k <number>" << endl;
    return -1;
}
```

**This is a compound conditional checking THREE things!**

---

### Condition 1: `argc != 5`

```cpp
argc != 5
```

**What it checks:**
- Verifies exactly 5 arguments are provided
- 5 arguments = program name + 4 user arguments

**Example:**

```
✓ VALID:
./searchengine -d doc1.txt -k 10
argc = 5 → Condition is FALSE → Continue

✗ INVALID (too few):
./searchengine -d doc1.txt
argc = 3 → Condition is TRUE → Error!

✗ INVALID (too many):
./searchengine -d doc1.txt -k 10 extra
argc = 6 → Condition is TRUE → Error!
```

**Why exactly 5?**

| Index | Content | Example |
|-------|---------|---------|
| argv[0] | Program name | `./searchengine` |
| argv[1] | `-d` flag | `-d` |
| argv[2] | File path | `doc1.txt` |
| argv[3] | `-k` flag | `-k` |
| argv[4] | Number | `10` |

---

### Condition 2: `strcmp(argv[1], "-d") != 0`

```cpp
strcmp(argv[1], "-d") != 0
```

**What it checks:**
- First argument must be exactly "-d"
- Uses `strcmp()` to compare string content

**How strcmp() works:**

```cpp
strcmp(str1, str2) returns:
    0   if strings are equal
    < 0 if str1 < str2 (alphabetically)
    > 0 if str1 > str2 (alphabetically)
```

**Examples:**

```
✓ VALID:
argv[1] = "-d"
strcmp("-d", "-d") = 0
0 != 0 → FALSE → Continue

✗ INVALID:
argv[1] = "-f"
strcmp("-f", "-d") ≠ 0
non-zero != 0 → TRUE → Error!

✗ INVALID:
argv[1] = "d"
strcmp("d", "-d") ≠ 0
non-zero != 0 → TRUE → Error!
```

**Why not use `==`?**

```cpp
// ❌ WRONG: Compares pointer addresses, not content
if(argv[1] == "-d")  // Always false!

// ✅ CORRECT: Compares string content
if(strcmp(argv[1], "-d") == 0)
```

---

### Condition 3: `strcmp(argv[3], "-k") != 0`

```cpp
strcmp(argv[3], "-k") != 0
```

**What it checks:**
- Third argument must be exactly "-k"
- Same logic as condition 2

**Examples:**

```
✓ VALID:
argv[3] = "-k"
strcmp("-k", "-k") = 0
0 != 0 → FALSE → Continue

✗ INVALID:
argv[3] = "-n"
strcmp("-n", "-k") ≠ 0
TRUE → Error!
```

---

### The OR Logic (||)

```cpp
if (argc != 5 ||
    strcmp(argv[1], "-d") != 0 ||
    strcmp(argv[3], "-k") != 0)
```

**How OR works:**
- If **ANY** condition is TRUE → Enter error block
- If **ALL** conditions are FALSE → Continue execution

**Truth Table:**

| argc != 5 | strcmp argv[1] | strcmp argv[3] | Result | Action |
|-----------|----------------|----------------|--------|--------|
| TRUE | - | - | TRUE | Error |
| FALSE | TRUE | - | TRUE | Error |
| FALSE | FALSE | TRUE | TRUE | Error |
| FALSE | FALSE | FALSE | FALSE | Continue ✓ |

**Visual Flow:**

```
Check argc != 5
    ↓
  TRUE? → Error → Exit
    ↓ NO
Check argv[1] != "-d"
    ↓
  TRUE? → Error → Exit
    ↓ NO
Check argv[3] != "-k"
    ↓
  TRUE? → Error → Exit
    ↓ NO
All checks passed → Continue
```

---

### Lines 9-11: Error Message and Exit

```cpp
cout << "Wrong arguments. Usage: -d <file> -k <number>" << endl;
return -1;
```

**Line 9: Error Message**

```cpp
cout << "Wrong arguments. Usage: -d <file> -k <number>" << endl;
```

**What it does:**
- Prints error message to console
- Shows correct usage format
- Helps user understand how to use the program

**Example Output:**
```
Wrong arguments. Usage: -d <file> -k <number>
```

**Line 10: Return Error Code**

```cpp
return -1;
```

**What it does:**
- Exits `main()` function immediately
- Returns `-1` to operating system (indicates error)
- Program terminates

**Error code convention:**
```
 0 or positive = Success
-1 or negative = Error
```

**Usage by system:**
```bash
./searchengine -d wrong
echo $?  # Prints: 255 (or -1 depending on system)
```

---

## Argument Validation

### Line 13: User Feedback

```cpp
cout << "Please wait..." << endl;
```

**What it does:**
- Informs user that processing has started
- Provides feedback while file is being read
- Good user experience practice

**Why here?**
- Placed **after** argument validation
- Only shown when arguments are correct
- Appears before potentially slow file operations

**Visual Timeline:**

```
User runs: ./searchengine -d large_file.txt -k 10
    ↓
Arguments validated ✓
    ↓
Output: "Please wait..."
    ↓
File processing starts (may take time)
    ↓
Output: "File read successfully..."
```

---

### Lines 14-16: Variable Initialization

```cpp
int linecounter = 0;
int maxlength = -1;
int k;
```

**Line-by-Line Breakdown:**

#### Line 14: `int linecounter = 0;`

**Purpose:**
- Will store the total number of lines in the file
- Initialized to `0` (no lines counted yet)
- Will be modified by `read_sizes()` function

**Why initialize to 0?**
- Safe default value
- Prevents using uninitialized memory
- Makes sense: 0 lines before reading

---

#### Line 15: `int maxlength = -1;`

**Purpose:**
- Will store the maximum line length in the file
- Initialized to `-1` (no maximum found yet)

**Why initialize to -1 instead of 0?**
- `-1` indicates "not yet calculated"
- Valid line lengths are ≥ 0
- Allows detection if file reading failed
- After successful read, will be ≥ 0

**Example:**
```cpp
// Before reading
maxlength = -1  (means: unknown)

// After reading file with lines of length 10, 25, 15
maxlength = 25  (longest line)
```

---

#### Line 16: `int k;`

**Purpose:**
- Will store the number parameter from command line
- Not initialized (will be assigned in try block)

**Why not initialized?**
- Will be assigned immediately in next block
- If assignment fails, program exits (no need for default)

**Will hold:**
- Value from `argv[4]` converted to integer
- Example: If `argv[4] = "10"`, then `k = 10`

---

## Integer Conversion with Error Handling

### Lines 17-22: Try-Catch Block

```cpp
try {
    k = stoi(argv[4]); 
} catch (...) {
    cout << "Invalid value for -k (must be an integer)" << endl;
    return -1;
}
```

**This is EXCEPTION HANDLING in C++!**

---

### Line 17: Try Block

```cpp
try {
```

**What it does:**
- Starts a protected code block
- Code inside is monitored for exceptions (errors)
- If exception occurs, jumps to `catch` block

**Why needed?**
- `stoi()` can throw exceptions
- Prevents program crash
- Allows graceful error handling

---

### Line 18: String to Integer Conversion

```cpp
k = stoi(argv[4]);
```

**Breaking it down:**

**`argv[4]`**
- Fourth command-line argument (index 4)
- Contains the number as a **string**
- Example: `"10"`, `"25"`, `"100"`

**`stoi()`**
- **St**ring **to** **I**nteger conversion
- Converts string to integer
- Example: `"10"` → `10`

**Full syntax:**
```cpp
int stoi(const string& str);
```

**Examples:**

```cpp
// ✓ Valid conversions
stoi("10")    → 10
stoi("0")     → 0
stoi("999")   → 999
stoi("-5")    → -5
stoi("  42 ") → 42 (ignores whitespace)

// ✗ Invalid - throws exception
stoi("abc")       → exception!
stoi("12.5")      → exception! (not an integer)
stoi("10 20")     → exception! (multiple values)
stoi("")          → exception! (empty string)
```

---

### Why stoi() Instead of atoi()?

**Old way (atoi):**
```cpp
k = atoi(argv[4]);  // ❌ No error detection!

// Problems:
atoi("abc")  → 0  (silently fails!)
atoi("10")   → 10 (works)
// Can't tell if "0" is valid or error!
```

**Modern way (stoi):**
```cpp
try {
    k = stoi(argv[4]);  // ✅ Throws exception on error!
} catch(...) {
    // Handle error
}

// Advantages:
stoi("abc")  → throws exception (detectable!)
stoi("10")   → 10 (works)
stoi("0")    → 0 (valid, distinguishable from error)
```

**Comparison:**

| Feature | atoi() | stoi() |
|---------|--------|--------|
| Error detection | ❌ Returns 0 | ✅ Throws exception |
| C++ standard | C-style | Modern C++ |
| Safety | Low | High |
| Distinguishes "0" from error | ❌ No | ✅ Yes |

---

### Lines 19-22: Exception Handling

```cpp
} catch (...) {
    cout << "Invalid value for -k (must be an integer)" << endl;
    return -1;
}
```

**Line 19: `catch (...)`**

**What it does:**
- Catches **any** exception thrown in try block
- `...` means "catch all exception types"

**Alternative (specific exception):**
```cpp
catch (invalid_argument& e) {
    // Only catch invalid_argument exceptions
}
catch (out_of_range& e) {
    // Only catch out_of_range exceptions
}
```

**Our code uses `(...)` because:**
- Simpler
- Catches all possible errors
- Same error message for all cases

---

**Lines 20-21: Error Handling**

```cpp
cout << "Invalid value for -k (must be an integer)" << endl;
return -1;
```

**What happens:**
1. Print error message explaining the problem
2. Exit program with error code `-1`

**Example scenarios:**

```
Scenario 1: User enters non-numeric value
Command: ./searchengine -d doc1.txt -k hello
    ↓
stoi("hello") throws exception
    ↓
Output: "Invalid value for -k (must be an integer)"
    ↓
Program exits with -1

Scenario 2: User enters decimal number
Command: ./searchengine -d doc1.txt -k 12.5
    ↓
stoi("12.5") throws exception
    ↓
Output: "Invalid value for -k (must be an integer)"
    ↓
Program exits with -1

Scenario 3: User enters valid number
Command: ./searchengine -d doc1.txt -k 10
    ↓
k = stoi("10") → k = 10 ✓
    ↓
No exception
    ↓
Continue to next line
```

---

### Complete Flow of Try-Catch

```
try {
    k = stoi(argv[4]);
}
    ↓
Is argv[4] valid integer string?
    ↓
  YES → k = integer value → Continue to line 24
    ↓
   NO → Exception thrown
        ↓
      catch (...) {
          Print error
          return -1
      }
        ↓
      Program exits
```

---

## File Processing

### Lines 24-26: Call read_sizes()

```cpp
if(read_sizes(&linecounter, &maxlength, argv[2]) == -1){
    return -1;
}
```

**This is where the actual file processing happens!**

---

### Line 24: Function Call and Return Check

```cpp
if(read_sizes(&linecounter, &maxlength, argv[2]) == -1)
```

**Breaking it down:**

#### Part 1: Function Call

```cpp
read_sizes(&linecounter, &maxlength, argv[2])
```

**Function: `read_sizes()`**
- Defined in `document_store.cpp`
- Reads file and counts lines

**Parameters:**

| Argument | Type | Value | Purpose |
|----------|------|-------|---------|
| `&linecounter` | `int*` | Address of `linecounter` | Output: stores line count |
| `&maxlength` | `int*` | Address of `maxlength` | Output: stores max length |
| `argv[2]` | `char*` | File path string | Input: file to process |

**Why `&` (address-of) operator?**
- `read_sizes()` expects pointers (`int*`)
- We have variables (`int`)
- `&` gets the memory address of variable
- Allows function to modify original variables

**Visual:**

```
Before call:
    linecounter = 0
    maxlength = -1

Call read_sizes(&linecounter, &maxlength, "doc1.txt")
    ↓
Function modifies values through pointers
    ↓
After call:
    linecounter = 150  (file has 150 lines)
    maxlength = 85     (longest line is 85 chars)
```

---

#### Part 2: Return Value Check

```cpp
... == -1
```

**What it checks:**
- `read_sizes()` returns `-1` on error
- `read_sizes()` returns `1` on success

**Possible scenarios:**

```
Return -1 → Error occurred
    - File not found
    - File empty
    - Permission denied

Return 1 → Success
    - File read successfully
    - linecounter and maxlength updated
```

---

#### Part 3: Conditional Logic

```cpp
if(... == -1)
```

**If condition is TRUE (return value is -1):**
- Enter the if block
- Execute line 25

**If condition is FALSE (return value is 1):**
- Skip the if block
- Continue to line 27

---

### Line 25: Error Exit

```cpp
return -1;
```

**What happens:**
- Exits `main()` function immediately
- Returns error code to operating system
- Program terminates

**Why no error message here?**
- `read_sizes()` already printed error message
- Examples:
  - "Cannot open file: doc1.txt"
  - "File is empty: doc1.txt"
- No need to print duplicate message

---

### Complete Flow Example

**Scenario 1: File Not Found**

```
Command: ./searchengine -d missing.txt -k 10

Line 24: read_sizes(&linecounter, &maxlength, "missing.txt")
    ↓
Inside read_sizes():
    fopen("missing.txt") returns NULL
    Prints: "Cannot open file: missing.txt"
    Returns -1
    ↓
Line 24: return value == -1 → TRUE
    ↓
Line 25: return -1
    ↓
Program exits
```

**Scenario 2: File Empty**

```
Command: ./searchengine -d empty.txt -k 10

Line 24: read_sizes(&linecounter, &maxlength, "empty.txt")
    ↓
Inside read_sizes():
    File opens successfully
    fgetc() returns EOF (empty file)
    Prints: "File is empty: empty.txt"
    Returns -1
    ↓
Line 24: return value == -1 → TRUE
    ↓
Line 25: return -1
    ↓
Program exits
```

**Scenario 3: Success**

```
Command: ./searchengine -d doc1.txt -k 10

Line 24: read_sizes(&linecounter, &maxlength, "doc1.txt")
    ↓
Inside read_sizes():
    File opens successfully
    Reads 150 lines
    Maximum line length: 85
    linecounter = 150
    maxlength = 85
    Returns 1
    ↓
Line 24: return value == 1 → 1 == -1 → FALSE
    ↓
Skip line 25
    ↓
Continue to line 27
```

---

### Lines 27-28: Success Output

```cpp
cout << "File read successfully. Lines: " << linecounter 
     << ", Max Length: " << maxlength << endl;
```

**What it does:**
- Prints success message
- Shows statistics about the file
- Provides useful information to user

**Example Outputs:**

```
File read successfully. Lines: 150, Max Length: 85
File read successfully. Lines: 42, Max Length: 120
File read successfully. Lines: 1, Max Length: 10
```

**Breaking down the output:**

```cpp
cout << "File read successfully. Lines: "  // Static text
     << linecounter                         // Variable (e.g., 150)
     << ", Max Length: "                    // Static text
     << maxlength                           // Variable (e.g., 85)
     << endl;                               // New line
```

**Why this output is useful:**
- Confirms file was processed
- Shows number of lines (for validation)
- Shows longest line (for buffer allocation planning)
- Helps debug if values seem wrong

---

---

## Mymap Object Creation

### Line 27: Dynamic Memory Allocation for Document Container

```cpp
Mymap *mymap=new Mymap(linecounter, maxlength);
```

**This line creates the main data structure that will store all documents!**

---

### Breaking It Down

#### Part 1: Variable Declaration

```cpp
Mymap *mymap
```

**What it means:**
- `Mymap` - Class type (defined in Map.cpp)
- `*` - Pointer (stores memory address)
- `mymap` - Variable name

**Visual:**

```
Stack:
  mymap (Mymap*) = [will point to heap object]
```

---

#### Part 2: Dynamic Allocation

```cpp
new Mymap(linecounter, maxlength)
```

**What `new` does:**
1. Allocates memory on heap for `Mymap` object
2. Calls `Mymap` constructor with parameters
3. Returns pointer to allocated object

**Parameters passed to constructor:**

| Parameter | Value | Purpose |
|-----------|-------|---------|
| `linecounter` | 150 (example) | Number of documents to store |
| `maxlength` | 85 (example) | Maximum length of any document |

---

#### Part 3: Memory Layout

**After line 27 executes:**

```
Stack:
  linecounter = 150
  maxlength = 85
  mymap (Mymap*) → [heap address]

Heap:
  Mymap object:
    - size = 150
    - max_length = 85
    - documents** → [array of 150 char* pointers]
```

---

### Why Use new?

**Heap allocation is necessary because:**
1. **Size determined at runtime** - Don't know `linecounter` until file is read
2. **Large data** - Could be thousands of documents
3. **Persistent until deleted** - Stays alive until we explicitly free it

---

### Constructor Execution

**When `new Mymap(linecounter, maxlength)` is called:**

```cpp
// Inside Map.cpp constructor
Mymap::Mymap(int size, int max_length) : size(size), max_length(max_length) {
    documents = new char*[size];  // Allocate array of pointers
    for(int i=0; i<size; i++){
        documents[i] = nullptr;    // Initialize to null
    }
}
```

**Step by step:**

```
1. Allocate Mymap object
2. Initialize size = 150
3. Initialize max_length = 85
4. Allocate array of 150 char* pointers
5. Set all pointers to nullptr
6. Return address of object
7. Store address in mymap variable
```

---

### Example with Different Sizes

**Small file (3 lines, max 20 chars):**

```cpp
linecounter = 3
maxlength = 20
Mymap *mymap = new Mymap(3, 20);

Heap:
  documents → [ptr0][ptr1][ptr2]  (3 pointers)
```

**Large file (1000 lines, max 500 chars):**

```cpp
linecounter = 1000
maxlength = 500
Mymap *mymap = new Mymap(1000, 500);

Heap:
  documents → [ptr0][ptr1][ptr2]...[ptr999]  (1000 pointers)
```

---

## Reading Documents into Memory

### Lines 29-32: read_input() Function Call with Error Handling

```cpp
if(read_input(mymap, argv[2]) == -1){
    delete (mymap);
    return -1;
}
```

**This is where actual document content is loaded into memory!**

---

### Line 29: Function Call and Error Check

```cpp
if(read_input(mymap, argv[2]) == -1)
```

**Breaking it down:**

#### Function Call

```cpp
read_input(mymap, argv[2])
```

**Function: `read_input()`**
- Defined in `document_store.cpp`
- Reads all lines from file
- Stores each line as a document in mymap

**Parameters:**

| Argument | Type | Value | Purpose |
|----------|------|-------|---------|
| `mymap` | `Mymap*` | Pointer to container | Where to store documents |
| `argv[2]` | `char*` | File path | Which file to read |

**What it does inside:**

```
1. Opens file
2. For each line in file:
   - Allocate memory for line
   - Copy line content
   - Store in mymap using insert()
3. Close file
4. Return success/error code
```

---

#### Return Value Check

```cpp
... == -1
```

**Possible return values:**

```
Return -1 → Error
    - File couldn't be opened again
    - Memory allocation failed
    - Read error occurred

Return 1 → Success
    - All documents loaded
    - mymap now contains all data
```

---

### Lines 30-31: Cleanup on Error

```cpp
delete (mymap);
return -1;
```

**Why cleanup is crucial:**

**Without cleanup:**

```cpp
if(read_input(mymap, argv[2]) == -1){
    return -1;  // ❌ Memory leak! mymap still allocated!
}
```

**With cleanup:**

```cpp
if(read_input(mymap, argv[2]) == -1){
    delete (mymap);  // ✅ Free memory first
    return -1;       // Then exit
}
```

---

### What delete (mymap) Does

**Line 30: `delete (mymap);`**

```
1. Calls Mymap destructor
2. Destructor frees documents array
3. Destructor frees each individual document
4. Frees the Mymap object itself
```

**Visual:**

```
Before delete:
  Heap:
    mymap object → documents[0] → "doc content"
                   documents[1] → nullptr
                   ...

After delete:
  Heap:
    [all memory freed]
  
  Stack:
    mymap still exists but points to freed memory
```

---

### Execution Flow Example

**Success scenario:**

```
Command: ./searchengine -d doc1.txt -k 10

Line 29: read_input(mymap, "doc1.txt")
    ↓
Inside read_input():
    Opens doc1.txt successfully
    Reads line 1: "First document"
    mymap->insert("First document", 0)
    Reads line 2: "Second document"
    mymap->insert("Second document", 1)
    ...
    Reads line 150: "Last document"
    mymap->insert("Last document", 149)
    Returns 1
    ↓
Line 29: return value == 1 → 1 == -1 → FALSE
    ↓
Skip lines 30-31
    ↓
Continue to line 34
```

**Error scenario:**

```
Command: ./searchengine -d locked.txt -k 10

Line 29: read_input(mymap, "locked.txt")
    ↓
Inside read_input():
    fopen() fails (permission denied)
    Prints: "Error opening file: locked.txt"
    Returns -1
    ↓
Line 29: return value == -1 → -1 == -1 → TRUE
    ↓
Line 30: delete (mymap)
    ↓
Mymap destructor runs:
    Frees documents array
    Frees Mymap object
    ↓
Line 31: return -1
    ↓
Program exits
```

---

### Memory State After Successful read_input()

```
Stack:
  linecounter = 150
  maxlength = 85
  mymap → [heap address]

Heap:
  Mymap object:
    size = 150
    max_length = 85
    documents → [0] → "First document\0"
                [1] → "Second document\0"
                [2] → "Third document\0"
                ...
                [149] → "Last document\0"
```

---

## Success Output and Data Display

### Line 34: Success Message

```cpp
cout<<"File read successfully. Lines: " << linecounter << ", Max Length: " << maxlength << endl;
```

**This confirms everything loaded successfully!**

---

### Breaking Down the Output

```cpp
cout << "File read successfully. Lines: "  // Static text
     << linecounter                         // Variable value
     << ", Max Length: "                    // Static text
     << maxlength                           // Variable value
     << endl;                               // Newline
```

---

### Example Outputs

**Small file:**
```
File read successfully. Lines: 3, Max Length: 25
```

**Medium file:**
```
File read successfully. Lines: 150, Max Length: 85
```

**Large file:**
```
File read successfully. Lines: 10000, Max Length: 512
```

---

### Why This Output is Important

**User information:**
- Confirms successful processing
- Shows file statistics
- Provides verification

**Debugging:**
- Can verify line count is correct
- Can check max length is reasonable
- Helps identify issues

---

## Document Printing Loop

### Lines 35-37: Print All Documents

```cpp
for(int i=0;i<mymap->get_size();i++){
    mymap->print(i);
}
```

**This loops through all documents and displays them!**

---

### Line 35: Loop Declaration

```cpp
for(int i=0; i<mymap->get_size(); i++)
```

**Breaking it down:**

#### Part 1: Initialization

```cpp
int i=0
```

**Creates loop counter:**
- Starts at 0 (first document index)
- Used to access each document

---

#### Part 2: Condition

```cpp
i<mymap->get_size()
```

**What `get_size()` returns:**
- Number of documents in mymap
- Same as `linecounter`

**How condition works:**

```
If mymap has 150 documents:
  i=0: 0 < 150 → TRUE → continue
  i=1: 1 < 150 → TRUE → continue
  ...
  i=149: 149 < 150 → TRUE → continue
  i=150: 150 < 150 → FALSE → stop
```

---

#### Part 3: Increment

```cpp
i++
```

**After each iteration:**
- Increments i by 1
- Moves to next document

---

### Line 36: Print Function Call

```cpp
mymap->print(i);
```

**What it does:**

```cpp
// Inside Map.cpp
void Mymap::print(int index) const {
    if(index >= 0 && index < size && documents[index] != nullptr){
        cout << documents[index] << endl;
    }
}
```

**Process:**
1. Check if index is valid
2. Check if document exists
3. Print document content
4. Add newline

---

### Complete Loop Execution Example

**File contents:**
```
Hello World
C++ Programming
Search Engine
```

**Execution:**

```
Line 35: i=0
    ↓
Line 36: mymap->print(0)
    ↓
Output: "Hello World"
    ↓
Line 35: i++ → i=1
    ↓
Line 35: i=1, 1 < 3 → TRUE
    ↓
Line 36: mymap->print(1)
    ↓
Output: "C++ Programming"
    ↓
Line 35: i++ → i=2
    ↓
Line 35: i=2, 2 < 3 → TRUE
    ↓
Line 36: mymap->print(2)
    ↓
Output: "Search Engine"
    ↓
Line 35: i++ → i=3
    ↓
Line 35: i=3, 3 < 3 → FALSE
    ↓
Exit loop
```

**Complete output:**
```
Hello World
C++ Programming
Search Engine
```

---

### Visual Flow

```
mymap contains:
  [0] → "Hello World"
  [1] → "C++ Programming"  
  [2] → "Search Engine"

Loop iterations:
  i=0 → print("Hello World")
  i=1 → print("C++ Programming")
  i=2 → print("Search Engine")
  i=3 → condition false, exit

Output:
  Hello World
  C++ Programming
  Search Engine
```

---

## Memory Cleanup

### Line 38: Freeing Dynamic Memory

```cpp
delete (mymap);
```

**This is CRITICAL for preventing memory leaks!**

---

### Why We Need delete

**Problem without delete:**

```cpp
int main() {
    Mymap *mymap = new Mymap(150, 85);
    // ... use mymap ...
    return 1;  // ❌ Memory leak! mymap never freed!
}
```

**When program ends:**
- Stack memory automatically freed
- **Heap memory stays allocated** ❌
- Operating system eventually reclaims it
- But it's a memory leak!

**Solution with delete:**

```cpp
int main() {
    Mymap *mymap = new Mymap(150, 85);
    // ... use mymap ...
    delete (mymap);  // ✅ Properly freed!
    return 1;
}
```

---

### What delete Does

**Step-by-step execution:**

```
1. Call Mymap destructor
2. Destructor executes:
   - Loop through all documents
   - delete[] each document string
   - delete[] documents array
3. Free Mymap object itself
```

---

### Destructor Code

```cpp
// Inside Map.cpp
Mymap::~Mymap(){
    for(int i=0; i<size; i++){
        if(documents[i] != nullptr){
            delete[] documents[i];  // Free each document
        }
    }
    delete[] documents;  // Free array of pointers
}
```

---

### Memory State Before and After

**Before delete (line 38):**

```
Stack:
  mymap → [heap address]

Heap:
  Mymap object:
    documents → [0] → "First document"
                [1] → "Second document"
                ...
                [149] → "Last document"
  
  Total memory used: ~100KB (example)
```

**After delete:**

```
Stack:
  mymap (dangling pointer - don't use!)

Heap:
  [all memory freed]
  
  Total memory used: 0 bytes
```

---

### Matching new and delete

**CRITICAL RULE: Every `new` must have a matching `delete`**

```cpp
// ✅ CORRECT
Mymap *mymap = new Mymap(150, 85);
delete (mymap);

// ❌ WRONG - Memory leak
Mymap *mymap = new Mymap(150, 85);
// No delete!

// ❌ WRONG - Double free (crash!)
Mymap *mymap = new Mymap(150, 85);
delete (mymap);
delete (mymap);  // Already freed!
```

---

### Why Use Parentheses?

```cpp
delete (mymap);   // With parentheses
delete mymap;     // Without parentheses
```

**Both are valid!**
- Parentheses are optional for `delete`
- Some prefer them for clarity
- Consistent with function call syntax

---

### Complete Memory Lifecycle

```
Line 27: Mymap *mymap = new Mymap(150, 85);
    ↓
  Heap: Mymap allocated, documents array allocated
    ↓
Lines 29-37: Use mymap
    ↓
  Heap: Document strings allocated and stored
    ↓
Line 38: delete (mymap);
    ↓
  Heap: All documents freed
  Heap: documents array freed
  Heap: Mymap object freed
    ↓
Line 39: return 1;
    ↓
  Stack: All local variables cleaned up
  Program exits cleanly
```

---

### Line 39: Program Exit

```cpp
return 1;
```

**What it does:**
- Exits `main()` function
- Returns success code `1` to operating system
- Program terminates successfully

**At this point:**
- All documents have been printed
- All memory has been freed
- No memory leaks
- Clean exit

---

## Execution Flow Diagram

### Complete Flow Chart

```
START
  ↓
[1] main(argc, argv) called
  ↓
[2] Check: argc == 5?
  ├─ NO → Print error → return -1 → END
  └─ YES → Continue
  ↓
[3] Check: argv[1] == "-d"?
  ├─ NO → Print error → return -1 → END
  └─ YES → Continue
  ↓
[4] Check: argv[3] == "-k"?
  ├─ NO → Print error → return -1 → END
  └─ YES → Continue
  ↓
[5] Print "Please wait..."
  ↓
[6] Initialize: linecounter=0, maxlength=-1
  ↓
[7] TRY: k = stoi(argv[4])
  ↓
[8] Success?
  ├─ NO → CATCH → Print error → return -1 → END
  └─ YES → Continue
  ↓
[9] Call: read_sizes(&linecounter, &maxlength, argv[2])
  ↓
[10] Return value == -1?
  ├─ YES → return -1 → END
  └─ NO → Continue
  ↓
[11] Create: Mymap *mymap = new Mymap(linecounter, maxlength)
  ↓
[12] Call: read_input(mymap, argv[2])
  ↓
[13] Return value == -1?
  ├─ YES → delete (mymap) → return -1 → END
  └─ NO → Continue
  ↓
[14] Print success message with statistics
  ↓
[15] For i=0 to mymap->get_size()-1:
      mymap->print(i)
  ↓
[16] delete (mymap)
  ↓
[17] return 1
  ↓
END
```

---

### Step-by-Step Execution Example

**Command:**
```bash
./searchengine -d data/doc1.txt -k 10
```

**Execution Trace:**

```
Step 1: Program starts
    argc = 5
    argv[0] = "./searchengine"
    argv[1] = "-d"
    argv[2] = "data/doc1.txt"
    argv[3] = "-k"
    argv[4] = "10"

Step 2: Check argc != 5
    5 != 5 → FALSE ✓

Step 3: Check strcmp(argv[1], "-d") != 0
    strcmp("-d", "-d") = 0
    0 != 0 → FALSE ✓

Step 4: Check strcmp(argv[3], "-k") != 0
    strcmp("-k", "-k") = 0
    0 != 0 → FALSE ✓

Step 5: All conditions FALSE → Skip error block
    Continue to line 13

Step 6: Print "Please wait..."
    Output: Please wait...

Step 7: Initialize variables
    linecounter = 0
    maxlength = -1

Step 8: TRY block - Convert string to integer
    k = stoi("10")
    k = 10 ✓

Step 9: No exception thrown
    Skip CATCH block

Step 10: Call read_sizes()
    read_sizes(&linecounter, &maxlength, "data/doc1.txt")
    
    Inside read_sizes():
        Opens file successfully
        Reads lines:
            Line 1: "Hello World" (11 chars)
            Line 2: "This is a test document" (23 chars)
            Line 3: "Testing" (7 chars)
        linecounter = 3
        maxlength = 23
        Returns 1
    
    Back in main():
        linecounter = 3
        maxlength = 23

Step 11: Check return value == -1
    1 == -1 → FALSE
    Skip return -1

Step 12: Print success message
    Output: File read successfully. Lines: 3, Max Length: 23

Step 13: return 1
    Program exits with success code

END

Final Output:
    Please wait...
    File read successfully. Lines: 3, Max Length: 23
```

---

## Example Usage Scenarios

### Scenario 1: Correct Usage with Full Execution ✓

**File: doc1.txt**
```
Line 1: Hello World
Line 2: C++ Programming
Line 3: Search Engine
```

**Command:**
```bash
./searchengine -d doc1.txt -k 5
```

**Complete Execution:**
```
Arguments validated ✓
    argc = 5
    argv[1] = "-d"
    argv[2] = "doc1.txt"
    argv[3] = "-k"
    argv[4] = "5"

Output: Please wait...

read_sizes() called:
    linecounter = 3
    maxlength = 18
    Returns 1

Mymap created:
    mymap = new Mymap(3, 18)
    Memory allocated for 3 documents

read_input() called:
    Opens doc1.txt
    Reads line 0: "Hello World"
    Reads line 1: "C++ Programming"
    Reads line 2: "Search Engine"
    Returns 1

Output: File read successfully. Lines: 3, Max Length: 18

Printing all documents:
    Hello World
    C++ Programming
    Search Engine

Memory cleanup:
    delete (mymap) - all memory freed

Exit code: 1 (success)
```

---

### Scenario 2: Wrong Number of Arguments ✗

**Command:**
```bash
./searchengine -d myfile.txt
```

**Execution:**
```
argc = 3
Check: argc != 5
    3 != 5 → TRUE
    
Output:
    Wrong arguments. Usage: -d <file> -k <number>

Exit code: -1 (error)
```

---

### Scenario 3: Wrong Flag Order ✗

**Command:**
```bash
./searchengine -k 10 -d myfile.txt
```

**Execution:**
```
argc = 5 ✓
Check: argv[1] == "-d"
    argv[1] = "-k"
    strcmp("-k", "-d") ≠ 0 → TRUE
    
Output:
    Wrong arguments. Usage: -d <file> -k <number>

Exit code: -1 (error)
```

---

### Scenario 4: Invalid Number ✗

**Command:**
```bash
./searchengine -d myfile.txt -k abc
```

**Execution:**
```
Arguments validated ✓

Output: Please wait...

TRY: k = stoi("abc")
    Exception thrown!

CATCH:
    Output: Invalid value for -k (must be an integer)

Exit code: -1 (error)
```

---

### Scenario 5: File Not Found ✗

**Command:**
```bash
./searchengine -d missing.txt -k 10
```

**Execution:**
```
Arguments validated ✓
Number parsed ✓ (k = 10)

Output: Please wait...

read_sizes() called:
    fopen("missing.txt") returns NULL
    Output: Cannot open file: missing.txt
    Returns -1

Back in main():
    Return value == -1 → TRUE
    return -1

Exit code: -1 (error)

Note: Mymap was never created (failed before line 27)
```

---

### Scenario 6: File Error During read_input ✗

**Command:**
```bash
./searchengine -d corrupted.txt -k 10
```

**Execution:**
```
Arguments validated ✓
Number parsed ✓

Output: Please wait...

read_sizes() successful:
    linecounter = 100
    maxlength = 50
    Returns 1

Mymap created:
    mymap = new Mymap(100, 50)

read_input() called:
    File cannot be opened (permission error)
    Output: Error opening file: corrupted.txt
    Returns -1

Error handling:
    delete (mymap) - memory freed
    return -1

Exit code: -1 (error)

Note: Proper cleanup performed before exit!
```

---

### Scenario 7: Large File Success ✓

**Command:**
```bash
./searchengine -d bigfile.txt -k 1000
```

**Execution:**
```
Arguments validated ✓
k = stoi("1000") = 1000 ✓

Output: Please wait...

read_sizes() processes file:
    linecounter = 50000
    maxlength = 2048
    Returns 1

Mymap created:
    mymap = new Mymap(50000, 2048)
    Memory allocated for 50000 documents

read_input() loads all documents:
    Processes 50000 lines
    Returns 1

Output: File read successfully. Lines: 50000, Max Length: 2048

Printing loop:
    for(i=0; i<50000; i++)
        mymap->print(i)
    Output: [50000 lines printed]

Memory cleanup:
    delete (mymap) - all documents freed

Exit code: 1 (success)
```

---

### Scenario 8: Empty File ✗

**Command:**
```bash
./searchengine -d empty.txt -k 10
```

**Execution:**
```
Arguments validated ✓
Number parsed ✓

Output: Please wait...

read_sizes() called:
    File opened successfully
    fgetc() returns EOF (empty file)
    Output: File is empty: empty.txt
    Returns -1

Back in main():
    Return value == -1 → TRUE
    return -1

Exit code: -1 (error)

Note: Mymap was never created (failed before line 27)
```

---

### Scenario 9: Single Line File ✓

**File: single.txt**
```
Only one line here
```

**Command:**
```bash
./searchengine -d single.txt -k 10
```

**Execution:**
```
read_sizes() result:
    linecounter = 1
    maxlength = 18

Mymap created:
    mymap = new Mymap(1, 18)

read_input() loads:
    documents[0] = "Only one line here"

Output: File read successfully. Lines: 1, Max Length: 18

Printing loop:
    i=0: "Only one line here"
    i=1: condition false, exit

Memory cleanup:
    delete (mymap)

Exit code: 1 (success)
```

---

## Error Handling Summary

### All Error Cases

| Error Type | Detection Point | Error Message | Cleanup Action | Exit Code |
|------------|----------------|---------------|----------------|-----------|
| Wrong argument count | Line 6 | "Wrong arguments. Usage: -d <file> -k <number>" | None needed | -1 |
| Missing `-d` flag | Line 7 | "Wrong arguments. Usage: -d <file> -k <number>" | None needed | -1 |
| Missing `-k` flag | Line 8 | "Wrong arguments. Usage: -d <file> -k <number>" | None needed | -1 |
| Invalid integer | Line 19 | "Invalid value for -k (must be an integer)" | None needed | -1 |
| File error (read_sizes) | Line 24 | From document_store.cpp | None needed | -1 |
| File error (read_input) | Line 29 | From document_store.cpp | delete (mymap) | -1 |

---

### Error Detection Flow

```
Command-Line Errors (Caught immediately)
    ↓
    ├─ Argument count wrong → return -1
    ├─ Flag position wrong → return -1
    └─ Flag spelling wrong → return -1
    ↓
Integer Conversion Errors (Caught in try-catch)
    ↓
    ├─ Non-numeric input → return -1
    ├─ Decimal numbers → return -1
    └─ Empty string → return -1
    ↓
File Size Analysis Errors (Caught in read_sizes)
    ↓
    ├─ File doesn't exist → return -1
    ├─ File empty → return -1
    └─ Permission denied → return -1
    ↓
Memory Allocation
    ↓
    new Mymap() → Success
    ↓
Document Loading Errors (Caught in read_input)
    ↓
    ├─ File cannot be opened → delete mymap → return -1
    ├─ Memory allocation failed → delete mymap → return -1
    └─ Read error → delete mymap → return -1
    ↓
Success Path
    ↓
    Print documents → delete mymap → return 1
```

---

## Variable Tracking Table

### Variable States Throughout Execution

**Successful Execution Example (3-line file):**

| Line | linecounter | maxlength | k | mymap | Notes |
|------|-------------|-----------|---|-------|-------|
| 14 | 0 | - | - | - | Initialized |
| 15 | 0 | -1 | - | - | Initialized |
| 16 | 0 | -1 | <uninit> | - | Declared |
| 18 | 0 | -1 | 5 | - | Converted from argv[4] |
| 24 (after) | 3 | 18 | 5 | - | Updated by read_sizes() |
| 27 | 3 | 18 | 5 | [heap addr] | Mymap created |
| 29-32 | 3 | 18 | 5 | [has data] | Documents loaded |
| 35-37 | 3 | 18 | 5 | [has data] | Documents printed |
| 38 | 3 | 18 | 5 | [freed] | Memory cleaned up |

---

## Memory State Through Program

### Memory Evolution

**Start of program:**
```
Stack: [empty]
Heap:  [empty]
```

**After line 16 (variables declared):**
```
Stack:
  linecounter = 0
  maxlength = -1
  k = <uninitialized>

Heap: [empty]
```

**After line 27 (Mymap created):**
```
Stack:
  linecounter = 3
  maxlength = 18
  k = 5
  mymap → [heap address]

Heap:
  Mymap object:
    size = 3
    max_length = 18
    documents → [ptr0][ptr1][ptr2] (all nullptr)
```

**After line 32 (documents loaded):**
```
Stack:
  linecounter = 3
  maxlength = 18
  k = 5
  mymap → [heap address]

Heap:
  Mymap object:
    size = 3
    max_length = 18
    documents → [0] → "Hello World\0"
                [1] → "C++ Programming\0"
                [2] → "Search Engine\0"
```

**After line 38 (cleanup):**
```
Stack:
  linecounter = 3
  maxlength = 18
  k = 5
  mymap → [dangling pointer]

Heap: [all freed]
```

**After line 39 (program exit):**
```
Stack: [cleaned up by OS]
Heap:  [cleaned up by OS]
```

---

## Program Structure Analysis

### Code Organization

**Layers:**
```
main() [Searchengine.cpp]
    ↓
├─ Argument Validation
├─ User Interface (cout messages)
├─ Memory Management (new/delete)
    ↓
    ├─ read_sizes() [document_store.cpp]
    │   ↓
    │   File I/O (count lines)
    │
    └─ read_input() [document_store.cpp]
        ↓
        File I/O (load documents)
        Mymap interaction
```

**Separation of Concerns:**

| Component | File | Responsibility |
|-----------|------|----------------|
| Main Program | `Searchengine.cpp` | CLI, flow control, memory management |
| Document Storage | `document_store.cpp` | File I/O, document loading |
| Data Structure | `Map.cpp` | Document container, memory management |

**Benefits:**
- ✅ Clean separation
- ✅ Easy to test each component
- ✅ Reusable functions
- ✅ Clear responsibilities
- ✅ Proper memory management

---

## Key Takeaways

### Important Concepts

1. **Command-Line Arguments**
   - `argc` = argument count
   - `argv` = argument vector (array of strings)
   - Always includes program name as argv[0]

2. **String Comparison**
   - Use `strcmp()` for C-style strings
   - Cannot use `==` (compares addresses, not content)
   - Return value: 0 = equal, non-zero = different

3. **Exception Handling**
   - `try-catch` blocks protect against runtime errors
   - `stoi()` throws exceptions on invalid input
   - Better than `atoi()` which silently fails

4. **Pointer Parameters**
   - Use `&` to pass address of variable
   - Allows function to modify original variable
   - Common pattern for "returning" multiple values

5. **Dynamic Memory**
   - `new` allocates on heap
   - `delete` frees memory
   - Must match every `new` with `delete`
   - Clean up even on error paths!

6. **Error Codes**
   - `-1` indicates error
   - `1` indicates success
   - Consistent throughout program

---

### Program Flow Summary

```
1. Validate command-line arguments (argc, flags)
2. Notify user processing started
3. Initialize counters
4. Convert string parameter to integer (with error handling)
5. Call read_sizes() to analyze file
6. Create Mymap object with appropriate size
7. Call read_input() to load all documents
8. Display success message
9. Print all loaded documents
10. Clean up memory
11. Return success code
```

---

### Current Functionality

**What the program does:**
- ✅ Validates command-line arguments
- ✅ Parses file path and number parameter
- ✅ Counts lines in file
- ✅ Finds maximum line length
- ✅ Creates dynamic container
- ✅ Loads all documents into memory
- ✅ Displays all documents
- ✅ Proper memory management

**What's next (future development):**
- 🔲 Use `k` parameter for search results
- 🔲 Implement search functionality
- 🔲 Read search queries
- 🔲 Return top k results
- 🔲 Index documents for fast searching
- 🔲 Implement ranking algorithm

---

### Code Quality

**Strengths:**
- ✅ Clear error messages
- ✅ Comprehensive validation
- ✅ Exception handling
- ✅ Good user feedback
- ✅ Separation of concerns
- ✅ Proper memory cleanup on ALL paths
- ✅ Defensive programming

**Potential Improvements:**
```cpp
// Add validation for k value
if(k <= 0) {
    cout << "Error: k must be positive" << endl;
    return -1;
}

// Add progress indicator for large files
if(linecounter > 10000) {
    cout << "Loading large file..." << endl;
}

// More specific exception handling
catch(invalid_argument& e) {
    cout << "Invalid integer format" << endl;
}
catch(out_of_range& e) {
    cout << "Number too large" << endl;
}

// Use k parameter (currently parsed but unused)
// Will be used when search functionality is implemented
```

---

## Summary

This `Searchengine.cpp` file serves as the **main entry point** for the search engine application. It handles:

1. **Command-line interface** - Parses and validates user input
2. **Error handling** - Comprehensive checks at multiple levels
3. **Memory management** - Dynamic allocation and cleanup
4. **File processing coordination** - Orchestrates document loading
5. **User feedback** - Clear messages for all scenarios
6. **Data display** - Prints all loaded documents

**Current State:**
- ✅ Complete document loading system
- ✅ Dynamic memory management
- ✅ Error handling on all paths
- ✅ Ready for search implementation

**Key Functions Used:**
- `main()` - Entry point
- `strcmp()` - String comparison
- `stoi()` - String to integer conversion
- `read_sizes()` - File analysis (from document_store)
- `read_input()` - Document loading (from document_store)
- `new` / `delete` - Dynamic memory management

**Key Classes Used:**
- `Mymap` - Document container (from Map.cpp)

---

**Document Version**: 2.0  
**Last Updated**: December 24, 2025  
**Author**: High-Performance Search Engine Project  
**Repository**: github.com/adarshpheonix2810/high-performance-search-engine-cpp

---

### Scenario 2: Wrong Number of Arguments ✗

**Command:**
```bash
./searchengine -d myfile.txt
```

**Execution:**
```
argc = 3
Check: argc != 5
    3 != 5 → TRUE
    
Output:
    Wrong arguments. Usage: -d <file> -k <number>

Exit code: -1 (error)
```

---

### Scenario 3: Wrong Flag Order ✗

**Command:**
```bash
./searchengine -k 10 -d myfile.txt
```

**Execution:**
```
argc = 5 ✓
Check: argv[1] == "-d"
    argv[1] = "-k"
    strcmp("-k", "-d") ≠ 0 → TRUE
    
Output:
    Wrong arguments. Usage: -d <file> -k <number>

Exit code: -1 (error)
```

---

### Scenario 4: Invalid Number ✗

**Command:**
```bash
./searchengine -d myfile.txt -k abc
```

**Execution:**
```
Arguments validated ✓

Output: Please wait...

TRY: k = stoi("abc")
    Exception thrown!

CATCH:
    Output: Invalid value for -k (must be an integer)

Exit code: -1 (error)
```

---

### Scenario 5: File Not Found ✗

**Command:**
```bash
./searchengine -d missing.txt -k 10
```

**Execution:**
```
Arguments validated ✓
Number parsed ✓ (k = 10)

Output: Please wait...

read_sizes() called
    fopen("missing.txt") returns NULL
    Output: Cannot open file: missing.txt
    Returns -1

Back in main():
    Return value == -1 → TRUE
    return -1

Exit code: -1 (error)
```

---

### Scenario 6: Empty File ✗

**Command:**
```bash
./searchengine -d empty.txt -k 10
```

**Execution:**
```
Arguments validated ✓
Number parsed ✓

Output: Please wait...

read_sizes() called
    File opened successfully
    fgetc() returns EOF (empty file)
    Output: File is empty: empty.txt
    Returns -1

Back in main():
    Return value == -1 → TRUE
    return -1

Exit code: -1 (error)
```

---

### Scenario 7: Large Number ✓

**Command:**
```bash
./searchengine -d bigfile.txt -k 1000000
```

**Execution:**
```
Arguments validated ✓
k = stoi("1000000") = 1000000 ✓

Output: Please wait...

read_sizes() processes file successfully
    linecounter = 50000
    maxlength = 2048
    Returns 1

Output:
    File read successfully. Lines: 50000, Max Length: 2048

Exit code: 1 (success)
```

---

### Scenario 8: Negative Number ✓

**Command:**
```bash
./searchengine -d data.txt -k -5
```

**Execution:**
```
Arguments validated ✓
k = stoi("-5") = -5 ✓

Output: Please wait...
File read successfully. Lines: 100, Max Length: 75

Exit code: 1 (success)

Note: Program accepts negative numbers!
      Validation logic for k value can be added if needed
```

---

## Error Handling Summary

### All Error Cases

| Error Type | Detection Point | Error Message | Exit Code |
|------------|----------------|---------------|-----------|
| Wrong argument count | Line 6 | "Wrong arguments. Usage: -d <file> -k <number>" | -1 |
| Missing `-d` flag | Line 7 | "Wrong arguments. Usage: -d <file> -k <number>" | -1 |
| Missing `-k` flag | Line 8 | "Wrong arguments. Usage: -d <file> -k <number>" | -1 |
| Invalid integer | Line 19 | "Invalid value for -k (must be an integer)" | -1 |
| File not found | `read_sizes()` | "Cannot open file: <filename>" | -1 |
| File empty | `read_sizes()` | "File is empty: <filename>" | -1 |

---

### Error Detection Flow

```
Command-Line Errors (Caught immediately)
    ↓
    ├─ Argument count wrong
    ├─ Flag position wrong
    └─ Flag spelling wrong
    ↓
Integer Conversion Errors (Caught in try-catch)
    ↓
    ├─ Non-numeric input
    ├─ Decimal numbers
    └─ Empty string
    ↓
File Processing Errors (Caught in read_sizes)
    ↓
    ├─ File doesn't exist
    ├─ File empty
    └─ Permission denied
```

---

## Variable Tracking Table

### Variable States Throughout Execution

**Successful Execution Example:**

| Line | linecounter | maxlength | k | Notes |
|------|-------------|-----------|---|-------|
| 14 | 0 | - | - | Initialized |
| 15 | 0 | -1 | - | Initialized |
| 16 | 0 | -1 | <uninitialized> | Declared |
| 18 | 0 | -1 | 10 | Converted from argv[4] |
| 24 (after) | 150 | 85 | 10 | Updated by read_sizes() |
| 27 | 150 | 85 | 10 | Used in output |

---

## Program Structure Analysis

### Code Organization

**Layers:**
```
main() [Searchengine.cpp]
    ↓
Argument Validation
    ↓
User Interface (cout messages)
    ↓
read_sizes() [document_store.cpp]
    ↓
File I/O Operations (fopen, getline, etc.)
```

**Separation of Concerns:**

| File | Responsibility |
|------|----------------|
| `Searchengine.cpp` | Command-line interface, argument parsing, user interaction |
| `document_store.cpp` | File processing, line counting, low-level I/O |

**Benefits:**
- Clean separation
- Easy to test each component
- Reusable `read_sizes()` function
- Main function stays simple and readable

---

## Key Takeaways

### Important Concepts

1. **Command-Line Arguments**
   - `argc` = argument count
   - `argv` = argument vector (array of strings)
   - Always includes program name as argv[0]

2. **String Comparison**
   - Use `strcmp()` for C-style strings
   - Cannot use `==` (compares addresses, not content)
   - Return value: 0 = equal, non-zero = different

3. **Exception Handling**
   - `try-catch` blocks protect against runtime errors
   - `stoi()` throws exceptions on invalid input
   - Better than `atoi()` which silently fails

4. **Pointer Parameters**
   - Use `&` to pass address of variable
   - Allows function to modify original variable
   - Common pattern for "returning" multiple values

5. **Error Codes**
   - `-1` indicates error
   - `1` indicates success
   - Consistent throughout program

---

### Program Flow Summary

```
1. Validate command-line arguments (argc, flags)
2. Notify user processing started
3. Initialize counters
4. Convert string parameter to integer (with error handling)
5. Call read_sizes() to process file
6. Check for errors from read_sizes()
7. Display results
8. Return success code
```

---

### Current Functionality

**What the program does:**
- ✅ Validates command-line arguments
- ✅ Parses file path and number parameter
- ✅ Counts lines in file
- ✅ Finds maximum line length
- ✅ Displays results

**What's next (future development):**
- 🔲 Use `k` parameter (currently unused!)
- 🔲 Implement search functionality
- 🔲 Read search queries
- 🔲 Return top k results
- 🔲 Index document for fast searching

---

### Code Quality

**Strengths:**
- ✅ Clear error messages
- ✅ Comprehensive validation
- ✅ Exception handling
- ✅ Good user feedback
- ✅ Separation of concerns

**Potential Improvements:**
```cpp
// Add validation for k value
if(k <= 0) {
    cout << "Error: k must be positive" << endl;
    return -1;
}

// Use k parameter (currently unused)
// Future: implement actual search with k results

// Add more specific error handling
catch(invalid_argument& e) {
    cout << "Invalid integer format" << endl;
}
catch(out_of_range& e) {
    cout << "Number too large" << endl;
}
```

---

## Summary

This `Searchengine.cpp` file serves as the **main entry point** for the search engine application. It handles:

1. **Command-line interface** - Parses and validates user input
2. **Error handling** - Comprehensive checks at multiple levels
3. **File processing coordination** - Calls `read_sizes()` from document_store
4. **User feedback** - Clear messages for all scenarios

**Current State:**
- Foundation for search engine
- Argument parsing complete
- File reading complete
- Ready for search implementation

**Key Functions:**
- `main()` - Entry point
- `strcmp()` - String comparison
- `stoi()` - String to integer conversion
- `read_sizes()` - File processing (from document_store)

---

**Document Version**: 1.0  
**Last Updated**: December 23, 2025  
**Author**: High-Performance Search Engine Project  
**Repository**: github.com/adarshpheonix2810/high-performance-search-engine-cpp