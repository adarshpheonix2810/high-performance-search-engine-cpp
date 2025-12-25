# Map Class Implementation - Code Explanation

This document provides a **detailed step-by-step explanation** of the `Map.cpp` file implementation. It explains how the code works, what each line does, and the complete execution flow of the Mymap class.

---

## File Information

**File Name**: `Map.cpp`  
**Location**: `src/Map.cpp`  
**Purpose**: Implements the Mymap class for storing and managing multiple text documents in dynamic memory  
**Header File**: `header/Map.hpp`

---

## Table of Contents

1. [Complete Source Code](#complete-source-code)
2. [Header File Dependencies](#header-file-dependencies)
3. [Constructor Implementation](#constructor-implementation)
4. [Destructor Implementation](#destructor-implementation)
5. [insert() Function Detailed Breakdown](#insert-function-detailed-breakdown)
6. [Memory Management Flow](#memory-management-flow)
7. [Complete Usage Example](#complete-usage-example)
8. [Common Scenarios](#common-scenarios)

---

## Complete Source Code

```cpp
#include "./Map.hpp"
using namespace std;

// Constructor
Mymap::Mymap(int size, int buffersize) : size(size), buffersize(buffersize)
{
    // Allocate arrays
    documents = new char *[size];
    doc_lengths = new int[size];

    /* Initialize to nullptr/0
    Mymap map(3, 100);  // 3 documents, max length 100
    Without initialization:
    documents[0] = 0x00FF8821 (random garbage)
    documents[1] = 0xDEADBEEF (random garbage)
    documents[2] = 0x12345678 (random garbage)
    Destructor tries to delete these = CRASH! 💥

    With initialization:
    documents[0] = nullptr ✅
    documents[1] = nullptr ✅
    documents[2] = nullptr ✅
    Destructor checks nullptr first = Safe! ✅
    */
    for (int i = 0; i < size; i++)
    {
        documents[i] = nullptr;
        doc_lengths[i] = 0;
    }
}

// Destructor
Mymap::~Mymap()
{
    for (int i = 0; i < size; i++)
    {
        delete[] documents[i];
    }
    delete[] documents;
    delete[] doc_lengths;
}

int Mymap::insert(char* line, int i){
    if(line == nullptr || i < 0 || i >= size){
        return -1;
    }
    
    // Remove newline if present
    int len = strlen(line);
    if(len > 0 && line[len-1] == '\n'){
        line[len-1] = '\0';
        len--;
    }
    
    // Trim leading spaces
    char* start = line;
    while(*start == ' ' || *start == '\t'){
        start++;
        len--;
    }
    
    // Trim trailing spaces
    char* end = start + len - 1;
    while(end > start && (*end == ' ' || *end == '\t')){
        *end = '\0';
        end--;
        len--;
    }
    
    // Allocate memory for this document
    documents[i] = new char[len + 1];
    strcpy(documents[i], start);
    doc_lengths[i] = len;
    
    return 1;
}
```

---

## Header File Dependencies

### Line 1: Include Header

```cpp
#include "./Map.hpp"
```

**What it includes:**
- Class declaration for `Mymap`
- `<iostream>` for input/output
- `<cstdlib>` for general utilities
- `<cstring>` for string functions

**Why `./Map.hpp`?**
- `./` means current directory
- Header is in same folder structure
- Could also use `"../header/Map.hpp"` if following strict structure

---

### Line 2: Namespace

```cpp
using namespace std;
```

**What it does:**
- Allows using `cout` instead of `std::cout`
- Makes code more readable
- Standard practice for implementation files

---

## Constructor Implementation

### Constructor Signature

```cpp
Mymap::Mymap(int size, int buffersize) : size(size), buffersize(buffersize)
```

**Breaking it down:**

#### Part 1: `Mymap::Mymap`

- `Mymap::` - Scope resolution operator (this is a member of Mymap class)
- `Mymap` - Constructor name (same as class name)

---

#### Part 2: Parameters

```cpp
(int size, int buffersize)
```

| Parameter | Type | Purpose |
|-----------|------|---------|
| `size` | `int` | Number of documents to store |
| `buffersize` | `int` | Maximum length of any document |

**Example call:**
```cpp
Mymap map(100, 500);
// Can store 100 documents
// Longest document can be 500 characters
```

---

#### Part 3: Member Initializer List

```cpp
: size(size), buffersize(buffersize)
```

**What this does:**
- Initializes member variables BEFORE constructor body runs
- `size(size)` means: member `size` = parameter `size`
- `buffersize(buffersize)` means: member `buffersize` = parameter `buffersize`

**Why use initializer list?**

```cpp
// ❌ Less efficient (assignment in body)
Mymap::Mymap(int size, int buffersize) {
    this->size = size;
    this->buffersize = buffersize;
}

// ✅ More efficient (initialization)
Mymap::Mymap(int size, int buffersize) : size(size), buffersize(buffersize) {
    // Members already initialized
}
```

**Visual Timeline:**

```
1. Member initializer list executes
   size = 100
   buffersize = 500

2. Constructor body runs
   { ... code ... }
```

---

### Lines 7-8: Array Allocation

```cpp
documents = new char *[size];
doc_lengths = new int[size];
```

#### Line 7: Allocate Documents Array

```cpp
documents = new char *[size];
```

**Breaking it down:**

**`char*`** - Pointer to a character (a string)  
**`char*[size]`** - Array of `size` pointers  
**`new char*[size]`** - Allocate array on heap  

**Memory layout:**
```
documents (member variable) → [heap memory]

Heap:
[ptr0][ptr1][ptr2]...[ptrN]  ← size pointers
```

**Example with size=3:**
```cpp
documents = new char*[3];

Memory:
documents → [pointer][pointer][pointer]
            (3 pointers, uninitialized)
```

---

#### Line 8: Allocate Lengths Array

```cpp
doc_lengths = new int[size];
```

**What it does:**
- Allocates array of `size` integers
- Each integer will store length of corresponding document

**Memory:**
```
doc_lengths → [int][int][int]...[int]  (size integers)
```

**Example:**
```cpp
doc_lengths = new int[3];

Memory:
doc_lengths → [?][?][?]  (3 integers, uninitialized)
```

---

### Lines 10-27: Initialization Loop

#### The Comment Block (Lines 10-26)

```cpp
/* Initialize to nullptr/0
Mymap map(3, 100);  // 3 documents, max length 100
Without initialization:
documents[0] = 0x00FF8821 (random garbage)
documents[1] = 0xDEADBEEF (random garbage)
documents[2] = 0x12345678 (random garbage)
Destructor tries to delete these = CRASH! 💥

With initialization:
documents[0] = nullptr ✅
documents[1] = nullptr ✅
documents[2] = nullptr ✅
Destructor checks nullptr first = Safe! ✅
*/
```

**Why this comment is important:**

This explains **WHY** we must initialize pointers to `nullptr`!

**Without initialization - DANGEROUS:**
```
Newly allocated memory contains random garbage values:

documents[0] = 0xDEADBEEF  ← Random memory address
documents[1] = 0x12345678  ← Random memory address
documents[2] = 0x00FF8821  ← Random memory address

When destructor runs:
delete[] documents[0];  → Tries to delete random address → 💥 CRASH!
```

**With initialization - SAFE:**
```
All pointers set to nullptr:

documents[0] = nullptr  ← Safe null pointer
documents[1] = nullptr  ← Safe null pointer
documents[2] = nullptr  ← Safe null pointer

When destructor runs:
delete[] nullptr;  → Does nothing → ✅ Safe!
```

---

#### Lines 27-31: The Initialization Loop

```cpp
for (int i = 0; i < size; i++)
{
    documents[i] = nullptr;
    doc_lengths[i] = 0;
}
```

**Line-by-line execution:**

**Line 27: Loop Header**
```cpp
for (int i = 0; i < size; i++)
```

**Example with size=3:**
```
Iteration 0: i=0, 0 < 3 → TRUE → Execute body
Iteration 1: i=1, 1 < 3 → TRUE → Execute body
Iteration 2: i=2, 2 < 3 → TRUE → Execute body
Iteration 3: i=3, 3 < 3 → FALSE → Exit loop
```

---

**Line 29: Initialize Pointer**
```cpp
documents[i] = nullptr;
```

**What is nullptr?**
- C++ keyword (introduced in C++11)
- Represents null pointer (points to nothing)
- Safe value for uninitialized pointers

**Visual progression:**
```
Before loop:
documents → [garbage][garbage][garbage]

After i=0:
documents → [nullptr][garbage][garbage]

After i=1:
documents → [nullptr][nullptr][garbage]

After i=2:
documents → [nullptr][nullptr][nullptr] ✅
```

---

**Line 30: Initialize Length**
```cpp
doc_lengths[i] = 0;
```

**Why 0?**
- Safe default value
- Means "no document yet" or "empty"
- Distinguishes from actual lengths (which are > 0)

**Visual progression:**
```
Before loop:
doc_lengths → [garbage][garbage][garbage]

After loop:
doc_lengths → [0][0][0] ✅
```

---

### Complete Constructor Execution Example

**Code:**
```cpp
Mymap map(3, 100);
```

**Step-by-step execution:**

```
Step 1: Member initializer list
  map.size = 3
  map.buffersize = 100

Step 2: Allocate documents array
  documents = new char*[3]
  
  Memory:
  documents → [ptr0][ptr1][ptr2]  (uninitialized pointers)

Step 3: Allocate lengths array
  doc_lengths = new int[3]
  
  Memory:
  doc_lengths → [int0][int1][int2]  (uninitialized integers)

Step 4: Initialize loop - i=0
  documents[0] = nullptr
  doc_lengths[0] = 0
  
  Memory:
  documents → [nullptr][ptr1][ptr2]
  doc_lengths → [0][int1][int2]

Step 5: Initialize loop - i=1
  documents[1] = nullptr
  doc_lengths[1] = 0
  
  Memory:
  documents → [nullptr][nullptr][ptr2]
  doc_lengths → [0][0][int2]

Step 6: Initialize loop - i=2
  documents[2] = nullptr
  doc_lengths[2] = 0
  
  Memory:
  documents → [nullptr][nullptr][nullptr] ✅
  doc_lengths → [0][0][0] ✅

Constructor complete!
```

---

## Destructor Implementation

### Destructor Purpose

**When called:**
- When object goes out of scope
- When `delete` is called on pointer to object

**What it must do:**
1. Free all individual document strings
2. Free the documents pointer array
3. Free the doc_lengths array

---

### Destructor Code Breakdown

```cpp
Mymap::~Mymap()
{
    for (int i = 0; i < size; i++)
    {
        delete[] documents[i];
    }
    delete[] documents;
    delete[] doc_lengths;
}
```

---

### Lines 35-37: Free Individual Documents

```cpp
for (int i = 0; i < size; i++)
{
    delete[] documents[i];
}
```

**Why loop first?**

Must free pointed-to memory BEFORE freeing the pointer array!

**Visual:**
```
Before loop:
documents → [ptr0]-→ "Hello"
            [ptr1]-→ "World"
            [ptr2]-→ "C++"

After i=0:
documents → [ptr0]  X (freed)
            [ptr1]-→ "World"
            [ptr2]-→ "C++"

After i=1:
documents → [ptr0]  X
            [ptr1]  X (freed)
            [ptr2]-→ "C++"

After i=2:
documents → [ptr0]  X
            [ptr1]  X
            [ptr2]  X (freed)
```

---

**Why `delete[] nullptr` is safe:**

```cpp
documents[0] = nullptr;
delete[] documents[0];  // Does nothing - safe!
```

**C++ rule:** Deleting a null pointer is a no-op (does nothing)

**This means:**
- If document never allocated → nullptr → safe to delete
- If document was allocated → proper cleanup
- No crashes!

---

### Line 38: Free Documents Array

```cpp
delete[] documents;
```

**What it frees:**
- The array of pointers itself
- NOT the strings (already freed in loop)

**Memory before:**
```
documents → [ptr0][ptr1][ptr2]  (array on heap)
```

**Memory after:**
```
documents → X  (array freed)
```

**⚠️ Important:** Must use `delete[]` (not `delete`) because allocated with `new[]`

---

### Line 39: Free Lengths Array

```cpp
delete[] doc_lengths;
```

**What it frees:**
- The integer array

**Memory before:**
```
doc_lengths → [5][5][3]  (array on heap)
```

**Memory after:**
```
doc_lengths → X  (array freed)
```

---

### Complete Destructor Execution Example

**Starting state:**
```
documents → [ptr0]-→ "Hello"  (6 bytes)
            [ptr1]-→ "World"  (6 bytes)
            [ptr2]-→ nullptr

doc_lengths → [5][5][0]
```

**Execution:**

```
Step 1: Loop i=0
  delete[] documents[0]
  Frees "Hello" (6 bytes freed)
  
  State:
  documents → [ptr0] X
              [ptr1]-→ "World"
              [ptr2]-→ nullptr

Step 2: Loop i=1
  delete[] documents[1]
  Frees "World" (6 bytes freed)
  
  State:
  documents → [ptr0] X
              [ptr1] X
              [ptr2]-→ nullptr

Step 3: Loop i=2
  delete[] documents[2]
  nullptr - no-op, nothing freed
  
  State:
  documents → [ptr0] X
              [ptr1] X
              [ptr2] X

Step 4: delete[] documents
  Frees pointer array (3 pointers freed)
  
  State:
  documents → X

Step 5: delete[] doc_lengths
  Frees integer array (3 ints freed)
  
  State:
  doc_lengths → X

All memory freed! ✅
```

---

## insert() Function Detailed Breakdown

### Function Signature

```cpp
int Mymap::insert(char* line, int i)
```

**Parameters:**
- `line`: Pointer to string to insert
- `i`: Index where to store the document

**Return value:**
- `1` on success
- `-1` on error

---

### Lines 42-44: Input Validation

```cpp
if(line == nullptr || i < 0 || i >= size){
    return -1;
}
```

**Three conditions checked:**

#### Condition 1: `line == nullptr`

```cpp
line == nullptr
```

**Why check?**
- Prevents crash from null pointer dereference
- `strlen(nullptr)` = CRASH!
- `strcpy(dest, nullptr)` = CRASH!

**Example:**
```cpp
map.insert(nullptr, 0);  // Returns -1 immediately
```

---

#### Condition 2: `i < 0`

```cpp
i < 0
```

**Why check?**
- Negative indices are invalid
- Would cause undefined behavior

**Example:**
```cpp
map.insert("text", -1);  // Returns -1
map.insert("text", -100);  // Returns -1
```

---

#### Condition 3: `i >= size`

```cpp
i >= size
```

**Why check?**
- Prevents array overflow
- Array indices: 0 to size-1

**Example:**
```cpp
Mymap map(3, 100);  // size = 3, valid indices: 0,1,2

map.insert("text", 0);  // ✅ Valid
map.insert("text", 2);  // ✅ Valid
map.insert("text", 3);  // ❌ Returns -1 (out of bounds)
map.insert("text", 10); // ❌ Returns -1
```

---

### Lines 47-52: Remove Newline

```cpp
// Remove newline if present
int len = strlen(line);
if(len > 0 && line[len-1] == '\n'){
    line[len-1] = '\0';
    len--;
}
```

#### Line 47: Get String Length

```cpp
int len = strlen(line);
```

**What strlen() does:**
- Counts characters until `\0`
- Does NOT include `\0` in count

**Example:**
```cpp
char* str = "Hello\n";
int len = strlen(str);  // len = 6

Memory:
['H']['e']['l']['l']['o']['\n']['\0']
  0    1    2    3    4    5    6
                              ↑
                         strlen stops here
```

---

#### Lines 48-52: Check and Remove Newline

```cpp
if(len > 0 && line[len-1] == '\n'){
    line[len-1] = '\0';
    len--;
}
```

**Breaking down the condition:**

**`len > 0`** - String is not empty  
**`line[len-1]`** - Last character  
**`== '\n'`** - Is it a newline?  

**Why check `len > 0`?**
```cpp
// If len = 0:
line[len-1]  // line[-1] → Out of bounds! CRASH!

// With check:
if(len > 0 && ...)  // len=0 → FALSE → Skip check
```

---

**What it does:**

```cpp
line[len-1] = '\0';  // Replace newline with null terminator
len--;               // Decrease length by 1
```

**Visual example:**

```
Before:
line → ['H']['e']['l']['l']['o']['\n']['\0']
        0    1    2    3    4    5    6
len = 6
line[len-1] = line[5] = '\n'

After:
line → ['H']['e']['l']['l']['o']['\0']['\0']
        0    1    2    3    4    5    6
len = 5
line[len-1] = line[4] = 'o'
```

---

### Lines 55-59: Trim Leading Whitespace

```cpp
// Trim leading spaces
char* start = line;
while(*start == ' ' || *start == '\t'){
    start++;
    len--;
}
```

#### Line 56: Initialize Start Pointer

```cpp
char* start = line;
```

**What it does:**
- Creates new pointer pointing to beginning of string
- Will be advanced past leading spaces

**Visual:**
```
line  → "   Hello"
start → "   Hello"  (same position)
```

---

#### Lines 57-59: Skip Spaces

```cpp
while(*start == ' ' || *start == '\t'){
    start++;
    len--;
}
```

**Breaking down:**

**`*start`** - Dereference pointer (get character)  
**`== ' '`** - Is it a space?  
**`|| *start == '\t'`** - OR is it a tab?  
**`start++`** - Move pointer forward  
**`len--`** - Reduce length  

**Visual execution:**

```
Initial:
line  → "   Hello World"
start → "   Hello World"
len = 14

After 1st iteration:
line  → "   Hello World"
start →  "  Hello World"  (moved 1 forward)
len = 13

After 2nd iteration:
line  → "   Hello World"
start →   " Hello World"  (moved 2 forward)
len = 12

After 3rd iteration:
line  → "   Hello World"
start →    "Hello World"  (moved 3 forward)
len = 11

4th iteration check:
*start = 'H' → Not space → Exit loop ✅
```

---

### Lines 62-67: Trim Trailing Whitespace

```cpp
// Trim trailing spaces
char* end = start + len - 1;
while(end > start && (*end == ' ' || *end == '\t')){
    *end = '\0';
    end--;
    len--;
}
```

#### Line 63: Set End Pointer

```cpp
char* end = start + len - 1;
```

**Why `len - 1`?**
- `len` is the count of characters
- Array indices start at 0
- Last character is at index `len - 1`

**Example:**
```cpp
String: "Hello"
len = 5
start → 'H' (index 0)
end → start + 5 - 1 = start + 4 → 'o' (index 4)

Array:
['H']['e']['l']['l']['o']
  0    1    2    3    4
                      ↑ end points here
```

---

#### Lines 64-67: Remove Trailing Spaces

```cpp
while(end > start && (*end == ' ' || *end == '\t')){
    *end = '\0';
    end--;
    len--;
}
```

**Conditions:**

**`end > start`** - Haven't reached beginning  
**`*end == ' '`** - Current character is space  
**`|| *end == '\t'`** - OR is tab  

**What happens:**

**`*end = '\0'`** - Replace space with null terminator  
**`end--`** - Move pointer backward  
**`len--`** - Reduce length  

**Visual execution:**

```
Initial:
start → "Hello World   "
end   →            ^  (points to last space)
len = 14

After 1st iteration:
start → "Hello World  \0"
end   →           ^
len = 13
*end was ' ' → replaced with '\0' → moved back

After 2nd iteration:
start → "Hello World \0\0"
end   →          ^
len = 12

After 3rd iteration:
start → "Hello World\0\0\0"
end   →         ^
len = 11

4th iteration check:
*end = 'd' → Not space → Exit loop ✅

Final:
start → "Hello World"  (cleaned!)
len = 11
```

---

### Lines 70-72: Allocate and Copy

```cpp
// Allocate memory for this document
documents[i] = new char[len + 1];
strcpy(documents[i], start);
doc_lengths[i] = len;
```

#### Line 71: Allocate Memory

```cpp
documents[i] = new char[len + 1];
```

**Why `len + 1`?**
- Need space for all characters
- PLUS space for null terminator `\0`

**Example:**
```cpp
String: "Hello" (5 characters)
len = 5
Allocate: len + 1 = 6 bytes

Memory:
[0][1][2][3][4][5]
↑                 ↑
characters        \0 goes here
```

**Visual:**
```
Before:
documents[i] = nullptr

After:
documents[i] → [allocated memory of len+1 bytes]
```

---

#### Line 72: Copy String

```cpp
strcpy(documents[i], start);
```

**What strcpy() does:**
- Copies characters from `start` to `documents[i]`
- Stops when it hits `\0`
- Includes the `\0` in the copy

**Example:**
```cpp
start → "Hello\0"
documents[i] → [empty memory]

After strcpy:
documents[i] → "Hello\0"
```

**Visual:**
```
Source (start):
['H']['e']['l']['l']['o']['\0']

Destination (documents[i]):
Before: [?][?][?][?][?][?]
After:  ['H']['e']['l']['l']['o']['\0']
```

---

#### Line 73: Store Length

```cpp
doc_lengths[i] = len;
```

**Why store length?**
- Fast lookup without calling `strlen()`
- Cached for performance
- Useful for buffer allocation

**Example:**
```cpp
documents[2] = "Hello World"  (allocated)
doc_lengths[2] = 11          (length stored)

// Later, fast access:
int len = doc_lengths[2];  // No need for strlen()
```

---

### Line 75: Return Success

```cpp
return 1;
```

**Indicates:** Document inserted successfully

**Convention in this codebase:**
- `1` = success
- `-1` = error

---

## Memory Management Flow

### Complete Lifecycle Example

```cpp
// Step 1: Create object
Mymap* map = new Mymap(3, 100);
```

**Memory after constructor:**
```
Heap:
  map object {
    size = 3
    buffersize = 100
    documents → [nullptr][nullptr][nullptr]
    doc_lengths → [0][0][0]
  }
```

---

```cpp
// Step 2: Insert first document
map->insert("Hello World", 0);
```

**Memory after first insert:**
```
Heap:
  documents → [ptr0]-→ "Hello World"  (12 bytes)
              [nullptr]
              [nullptr]
  doc_lengths → [11][0][0]
```

---

```cpp
// Step 3: Insert second document
map->insert("  C++ Programming  ", 1);
// (spaces trimmed → "C++ Programming")
```

**Memory after second insert:**
```
Heap:
  documents → [ptr0]-→ "Hello World"      (12 bytes)
              [ptr1]-→ "C++ Programming"  (16 bytes)
              [nullptr]
  doc_lengths → [11][15][0]
```

---

```cpp
// Step 4: Insert third document
map->insert("Search Engine\n", 2);
// (newline removed → "Search Engine")
```

**Memory after third insert:**
```
Heap:
  documents → [ptr0]-→ "Hello World"      (12 bytes)
              [ptr1]-→ "C++ Programming"  (16 bytes)
              [ptr2]-→ "Search Engine"    (14 bytes)
  doc_lengths → [11][15][13]
```

---

```cpp
// Step 5: Delete object
delete map;
```

**Memory freed in order:**

```
1. delete[] documents[0]  → Frees "Hello World"      (12 bytes)
2. delete[] documents[1]  → Frees "C++ Programming"  (16 bytes)
3. delete[] documents[2]  → Frees "Search Engine"    (14 bytes)
4. delete[] documents     → Frees pointer array      (3 pointers)
5. delete[] doc_lengths   → Frees integer array      (3 ints)

All memory freed! ✅
```

---

## Complete Usage Example

### Realistic Program Flow

```cpp
#include "Map.hpp"

int main() {
    // Step 1: Create map for 7 documents, max length 103
    Mymap* map = new Mymap(7, 103);
    
    // Step 2: Insert documents
    map->insert("Introduction to Search Engines", 0);
    map->insert("A search engine is a software system...", 1);
    map->insert("It crawls web pages, stores them...", 2);
    map->insert("When a user types a query...", 3);
    map->insert("using ranking algorithms like TF-IDF...", 4);
    map->insert("Modern search engines focus on speed...", 5);
    map->insert("inverted indexes, parallel processing...", 6);
    
    // Step 3: Access documents
    for(int i = 0; i < map->get_size(); i++) {
        map->print(i);
    }
    
    // Step 4: Get specific document
    char* doc = map->getDocument(0);
    int len = map->getlength(0);
    cout << "First document: " << doc << endl;
    cout << "Length: " << len << endl;
    
    // Step 5: Cleanup
    delete map;
    
    return 0;
}
```

**Output:**
```
Document 0: Introduction to Search Engines
Document 1: A search engine is a software system...
Document 2: It crawls web pages, stores them...
Document 3: When a user types a query...
Document 4: using ranking algorithms like TF-IDF...
Document 5: Modern search engines focus on speed...
Document 6: inverted indexes, parallel processing...
First document: Introduction to Search Engines
Length: 31
```

---

## Common Scenarios

### Scenario 1: Empty String

```cpp
map->insert("", 0);
```

**Execution:**
```
1. Validation: "" is not nullptr → OK
2. len = strlen("") = 0
3. Newline check: len = 0 → Skip
4. Leading spaces: No spaces → Skip
5. Trailing spaces: end would be at start-1 → Skip (end > start fails)
6. Allocate: new char[0 + 1] = new char[1]
7. strcpy: copies only '\0'
8. documents[0] = ""
9. doc_lengths[0] = 0
10. Return 1 ✅
```

---

### Scenario 2: Only Spaces

```cpp
map->insert("     ", 0);
```

**Execution:**
```
1. len = 5
2. Remove leading: start moves to end → len = 0
3. Trailing: end < start → Skip
4. Allocate: new char[1]
5. strcpy("") → Just '\0'
6. documents[0] = ""
7. doc_lengths[0] = 0
8. Return 1 ✅
```

---

### Scenario 3: Newline and Spaces

```cpp
map->insert("  Hello  \n", 0);
```

**Execution:**
```
1. len = strlen("  Hello  \n") = 10
2. Remove newline:
   line[9] = '\n' → Replace with '\0'
   len = 9
   line = "  Hello  "

3. Trim leading:
   start moves past 2 spaces
   start → "Hello  "
   len = 7

4. Trim trailing:
   end moves back 2 spaces
   "Hello"
   len = 5

5. Allocate: new char[6]
6. strcpy("Hello")
7. documents[0] = "Hello"
8. doc_lengths[0] = 5
9. Return 1 ✅
```

---

### Scenario 4: Invalid Index

```cpp
Mymap map(3, 100);  // size = 3
map.insert("text", 5);
```

**Execution:**
```
1. Validation: i=5, size=3
2. Check: 5 >= 3 → TRUE
3. Return -1 ❌
4. Document NOT inserted
```

---

### Scenario 5: nullptr Input

```cpp
map->insert(nullptr, 0);
```

**Execution:**
```
1. Validation: line == nullptr → TRUE
2. Return -1 ❌
3. No crash! Safe handling ✅
```

---

## Summary

### Key Functions

1. **Constructor**
   - Allocates pointer arrays
   - Initializes to nullptr/0
   - Prevents crashes

2. **Destructor**
   - Frees in correct order
   - Handles nullptr safely
   - No memory leaks

3. **insert()**
   - Validates input
   - Cleans string (trim, remove newlines)
   - Allocates exact memory needed
   - Stores document and length

---

## December 26, 2025 Update - Critical Fixes

### Changes Made Today

Today we made important fixes to the Map class to improve correctness and prepare for future BM25 search implementation.

### Fix 1: doc_lengths Now Stores Word Count

**File Changed**: `src/Map.cpp` line 71  
**What Changed**: Removed `doc_lengths[i] = len;` assignment

**Before:**
```cpp
// Allocate memory for this document
documents[i] = new char[len + 1];
strcpy(documents[i], start);
doc_lengths[i] = len;  // ❌ Stored character count
```

**After:**
```cpp
// Allocate memory for this document
documents[i] = new char[len + 1];
strcpy(documents[i], start);
// doc_lengths will be set by split() with word count for BM25
```

**Why the change?**

The `doc_lengths` array now stores **word count** instead of **character count**. This is necessary for the BM25 ranking algorithm which requires calculating average document length (avgdl) in terms of words, not characters.

**How it works now:**

1. `Map::insert()` stores the document text but doesn't set `doc_lengths[i]`
2. `split()` function in document_store.cpp counts words while tokenizing
3. `split()` calls `mymap->setlength(id, wordCount)` to store the count

**Code Flow:**
```
read_input() 
  → mymap->insert(line, i)     // Stores document text
  → split(temp, i, trie, mymap) // Counts words while building Trie
     → mymap->setlength(i, wordCount)  // Sets doc_lengths[i]
```

**Example:**
```cpp
// Document: "hello world from earth"
// Character count: 23
// Word count: 4

// Old behavior:
doc_lengths[0] = 23;  // ❌ Not useful for BM25

// New behavior:
doc_lengths[0] = 4;   // ✅ Correct for avgdl calculation
```

**Future usage in BM25:**
```cpp
// Calculate average document length
double avgdl = 0;
for(int i = 0; i < map->get_size(); i++){
    avgdl += map->getlength(i);  // Sum word counts
}
avgdl /= map->get_size();  // avgdl = 2.33 words (example)

// Use in BM25 formula
score = IDF * (tf * (k1 + 1)) / (tf + k1 * (1 - b + b * docLength / avgdl));
```

---

### Fix 2: Removed Large Comment Block

**File Changed**: `src/Map.cpp` lines 10-24  
**What Changed**: Cleaned up verbose inline comments

**Before:**
```cpp
    documents = new char *[size];
    doc_lengths = new int[size];

    /* Initialize to nullptr/0
    Mymap map(3, 100);  // 3 documents, max length 100
    Without initialization:
    documents[0] = 0x00FF8821 (random garbage)
    documents[1] = 0xDEADBEEF (random garbage)
    documents[2] = 0x12345678 (random garbage)
    Destructor tries to delete these = CRASH! 💥

    With initialization:
    documents[0] = nullptr ✅
    documents[1] = nullptr ✅
    documents[2] = nullptr ✅
    Destructor checks nullptr first = Safe! ✅
    */
    for (int i = 0; i < size; i++)
```

**After:**
```cpp
    documents = new char *[size];
    doc_lengths = new int[size];

    // Initialize to prevent undefined behavior
    for (int i = 0; i < size; i++)
```

**Why the change?**
- Long explanatory comments belong in documentation, not production code
- Keeps source code clean and readable
- The concept is already explained in detail in `map.md` documentation
- Professional codebases use concise comments in source files

**Best Practice:**
- ✅ Short comments in `.cpp` files (what the code does)
- ✅ Detailed explanations in `.md` files (why and how)
- ❌ Multi-line tutorial comments in source code

---

### Fix 3: Added const Correctness to getDocument()

**File Changed**: `header/Map.hpp` line 26  
**What Changed**: Made `getDocument()` a const method returning const pointer

**Before:**
```cpp
char* getDocument(int i){
    return documents[i];
}
```

**After:**
```cpp
const char* getDocument(int i) const {
    return documents[i];
}
```

**What this means:**

**First `const` (return type):**
- `const char*` means the **returned pointer points to const data**
- Caller cannot modify the document string through this pointer
- Enforces read-only access

**Second `const` (method):**
- `const` after parameter list means **this method doesn't modify the object**
- Can be called on const Mymap objects
- Compiler enforces no member variables are changed

**Example usage:**
```cpp
Mymap map(3, 100);
// ... load documents ...

const char* doc = map.getDocument(0);  // ✅ Get document
cout << doc;                            // ✅ Can read
doc[0] = 'X';                          // ❌ Compile ERROR - const data

// Works with const objects:
void processMap(const Mymap& map){
    const char* doc = map.getDocument(0);  // ✅ OK - const method
}
```

**Benefits:**
- ✅ Prevents accidental document modification
- ✅ Makes interface safer and clearer
- ✅ Enables const object usage
- ✅ Better compiler optimization

---

### Fix 4: Include Path Cleanup

**File Changed**: `src/Map.cpp` line 1  
**What Changed**: Removed `./` prefix for consistency

**Before:**
```cpp
#include "./Map.hpp"
```

**After:**
```cpp
#include "Map.hpp"
```

**Why the change?**
- CMakeLists.txt already includes `header/` directory
- `./` prefix is redundant and inconsistent with other files
- All other source files use direct includes
- Cleaner and more portable

**How CMake handles it:**
```cmake
include_directories(header)  # Tells compiler to look in header/

# Now all source files can use:
#include "Map.hpp"          // ✅ Clean
# Instead of:
#include "./Map.hpp"        // ❌ Redundant
#include "../header/Map.hpp" // ❌ Verbose
```

---

### Summary of Today's Changes

| File | Line | Change | Reason |
|------|------|--------|--------|
| Map.cpp | 71 | Removed `doc_lengths[i] = len` | Store word count for BM25 |
| Map.cpp | 10-24 | Removed comment block | Keep code clean |
| Map.cpp | 1 | Removed `./` from include | Consistency |
| Map.hpp | 26 | Added const correctness | Safety and const objects |

### Code Quality Improvements

✅ **Correctness** - doc_lengths now stores right data for algorithms  
✅ **Readability** - Removed clutter from source files  
✅ **Safety** - Added const correctness to prevent bugs  
✅ **Consistency** - Standardized include paths  
✅ **Future-proof** - Ready for BM25 implementation  

---

### Memory Safety Features

✅ **Null pointer initialization** - All pointers start as nullptr  
✅ **Bounds checking** - Validates array indices  
✅ **Proper cleanup** - Destructor frees all memory  
✅ **Safe deletion** - delete[] nullptr is safe  
✅ **Dynamic allocation** - Only allocates what's needed  
✅ **Const protection** - Prevents accidental data modification  

---

### Code Quality

**Strengths:**
- ✅ Clear comments explaining critical sections
- ✅ Proper error handling with return codes
- ✅ Input validation before processing
- ✅ Memory-efficient (variable-size allocation)
- ✅ Safe initialization
- ✅ const correctness for getters

**Potential Improvements:**
- Add bounds checking to `getDocument()` and `getlength()`
- Consider using `std::string` instead of `char*`
- Add copy constructor and assignment operator
- Use smart pointers (`std::unique_ptr`) for automatic cleanup

---

**Document Version**: 1.1  
**Last Updated**: December 26, 2025  
**Changes**: Added December 26 fixes documentation  
**Repository**: github.com/adarshpheonix2810/high-performance-search-engine-cpp
