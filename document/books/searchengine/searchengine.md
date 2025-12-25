# Search Engine Project Documentation

---

**PROJECT:** Command-Line Search Engine  
**AUTHOR:** Adarsh Singh  
**DATE:** December 2025

---

## 1. Understanding Command Line Arguments in C++

### 1.1 Introduction

This search engine project accepts input through command-line arguments. To understand how the program receives and processes these inputs, we need to understand `argc` and `argv`.

### 1.2 The main() Function Signature

**Syntax:**
```cpp
int main(int argc, char *argv[])
```

**Where:**
- `argc` = **Argument Count** (integer)
- `argv` = **Argument Vector** (array of character pointers)

### 1.3 What is argc?

`argc` represents the **NUMBER** of command-line arguments passed to the program, including the program name itself.

**Example:**
```bash
./searchengine word1 word2 word3
```
In this case: `argc = 4` (program name + 3 arguments)

### 1.4 What is argv?

`argv` is an **ARRAY** of strings (character pointers) that holds all command-line arguments.

**Structure:**
- `argv[0]` = Program name (e.g., `"./searchengine"`)
- `argv[1]` = First argument (e.g., `"word1"`)
- `argv[2]` = Second argument (e.g., `"word2"`)
- ...and so on

### 1.5 Technical Details

The parameter `argv` has type `char**` (pointer to pointer to char).

These two declarations are **EQUIVALENT:**
```cpp
int main(int argc, char *argv[]);  // Array notation
int main(int argc, char **argv);   // Pointer notation
```

**Why?** Because in C/C++, array parameters decay to pointers.

**Memory Structure:**
- `argv` points to the first element of an array of `char*` pointers
- Each `argv[i]` points to a null-terminated C-string
- Each string represents one command-line argument

#### Visual Diagram

![argc and argv memory structure](../../pic/argv+argc.png)

*Figure 1: Memory layout of argc and argv showing how command-line arguments are stored*

### 1.6 Naming Convention

While `argc` and `argv` are standard conventions, you can use any valid identifiers:

```cpp
int main(int num_args, char** arg_strings)  // Valid alternative
```

However, using `argc`/`argv` is **HIGHLY RECOMMENDED** for code readability.

### 1.7 How This Applies to Our Search Engine

Our search engine uses command-line arguments to:

1. Accept search queries from the user
2. Specify which documents to search
3. Configure search options (if implemented)

**Example Usage:**
```bash
./searchengine "search term" doc1.txt doc2.txt
```

In this case:
- `argc = 4`
- `argv[0] = "./searchengine"`
- `argv[1] = "search term"`
- `argv[2] = "doc1.txt"`
- `argv[3] = "doc2.txt"`

---

## 2. Understanding String Comparison with strcmp()

### 2.1 Introduction

When working with C-style strings in C++, comparing strings is a fundamental operation. Our search engine needs to compare user input with stored data, and understanding how string comparison works is crucial for correct functionality.

### 2.2 The Problem: Why Can't We Use == ?

In C++, when dealing with C-style strings (character arrays), you might be tempted to compare them using the `==` operator:

```cpp
char* string_a = "hello";
char* string_b = "hello";

if (string_a == string_b) {  // ❌ This doesn't work as expected!
    // This compares memory addresses, not content!
}
```

**The Issue:**  
The `==` operator between two `char*` pointers compares **memory addresses**, not the actual string content.

### 2.3 Memory Addresses vs String Content

When you use `==` with pointers, you're asking:  
> "Do these two pointers point to the **same memory location**?"

When you want to compare strings, you're actually asking:  
> "Do these two strings have the **same characters** in the same order?"

#### Visual Example

```
Memory Layout:
┌─────────────────────────────────────┐
│ Address: 0x1000                     │
│ Content: 'h' 'e' 'l' 'l' 'o' '\0'   │  ← string_a points here
└─────────────────────────────────────┘

┌─────────────────────────────────────┐
│ Address: 0x2000                     │
│ Content: 'h' 'e' 'l' 'l' 'o' '\0'   │  ← string_b points here
└─────────────────────────────────────┘

string_a == string_b  →  0x1000 == 0x2000  →  false ❌
strcmp(string_a, string_b)  →  compares content  →  0 (equal) ✓
```

### 2.4 The Solution: strcmp()

The `strcmp()` function compares two C-style strings **character by character**, examining their actual content rather than memory addresses.

**Function Signature:**
```cpp
#include <cstring>

int strcmp(const char* str1, const char* str2);
```

**Return Values:**
- **0** → Strings are **equal**
- **< 0** (negative) → str1 comes **before** str2 (lexicographically)
- **> 0** (positive) → str1 comes **after** str2 (lexicographically)

### 2.5 How strcmp() Works

The function compares strings **character by character** using their ASCII values:

```cpp
strcmp("apple", "apple")   // Returns 0  (equal)
strcmp("apple", "banana")  // Returns <0 (apple < banana)
strcmp("zebra", "apple")   // Returns >0 (zebra > apple)
```

**Algorithm:**
1. Start at the first character of both strings
2. Compare character by character
3. If a difference is found, return the difference
4. If one string ends (reaches '\0'), it's considered "less than"
5. If both strings end simultaneously, they're equal (return 0)

### 2.6 Practical Examples

#### Example 1: Basic String Comparison
```cpp
#include <cstring>
#include <iostream>

int main() {
    char str1[] = "hello";
    char str2[] = "hello";
    char str3[] = "world";
    
    // Compare strings with identical content
    if (strcmp(str1, str2) == 0) {
        std::cout << "str1 and str2 are equal\n";  // ✓ This will execute
    }
    
    // Compare different strings
    if (strcmp(str1, str3) != 0) {
        std::cout << "str1 and str3 are different\n";  // ✓ This will execute
    }
    
    return 0;
}
```

#### Example 2: Why == Fails with String Literals
```cpp
char* string_a = "search";
char* string_b = "search";

// These are two SEPARATE string literals in memory
// Even though content is same, they may have different addresses

if (string_a == string_b) {
    std::cout << "Same address\n";  // ❌ Might not execute!
}

if (strcmp(string_a, string_b) == 0) {
    std::cout << "Same content\n";  // ✓ Will execute!
}
```

### 2.7 How This Applies to Our Search Engine

Our search engine uses `strcmp()` in several critical operations:

#### 1. **Comparing Search Queries**
```cpp
// Check if user's search term matches a keyword
if (strcmp(argv[1], "search") == 0) {
    // Execute search operation
}
```

#### 2. **Matching File Names**
```cpp
// Verify which document to search
if (strcmp(filename, "doc1.txt") == 0) {
    // Process doc1.txt
}
```

#### 3. **Finding Exact Matches**
```cpp
// Compare search term with words in documents
if (strcmp(search_term, word_from_file) == 0) {
    // Match found!
    match_count++;
}
```

### 2.8 Common Pitfalls to Avoid

#### ❌ Wrong: Using == for string comparison
```cpp
if (str1 == str2) {  // Compares addresses, not content!
    // Unreliable behavior
}
```

#### ✓ Correct: Using strcmp()
```cpp
if (strcmp(str1, str2) == 0) {  // Compares content
    // Reliable comparison
}
```

#### ⚠️ Important Notes:
1. **Case Sensitivity:** `strcmp()` is case-sensitive
   - `strcmp("Hello", "hello")` ≠ 0
   - Use `strcasecmp()` or `_stricmp()` for case-insensitive comparison

2. **Null Termination:** Both strings must be null-terminated (`'\0'`)
   - Otherwise, undefined behavior occurs

3. **NULL Pointers:** Never pass NULL pointers to `strcmp()`
   - This causes a crash

### 2.9 Alternative: C++ std::string

Modern C++ offers `std::string` which allows direct comparison:

```cpp
#include <string>

std::string str1 = "hello";
std::string str2 = "hello";

if (str1 == str2) {  // ✓ This works with std::string!
    std::cout << "Strings are equal\n";
}
```

However, our search engine uses C-style strings (char arrays) because:
- Command-line arguments come as `char*` arrays
- Lower-level control for educational purposes
- Demonstrates fundamental C/C++ concepts

### 2.10 Quick Reference Table

| Operation | C-Style Strings (char*) | C++ std::string |
|-----------|------------------------|-----------------|
| Declaration | `char str[] = "text";` | `std::string str = "text";` |
| Comparison | `strcmp(str1, str2) == 0` | `str1 == str2` |
| Address Check | `str1 == str2` | N/A |
| Case-Insensitive | `strcasecmp(str1, str2)` | Custom function needed |
| Length | `strlen(str)` | `str.length()` |

### 2.11 Summary

- **`strcmp()`** compares string **content**, not memory addresses
- **`==`** operator compares **memory addresses** for pointers
- `strcmp()` returns **0** when strings are equal
- This is essential for our search engine to match search terms with document content
- Always use `strcmp()` for C-style string comparisons

---

## 3. Converting Strings to Integers: std::stoi() vs atoi()

### 3.1 Introduction

In many applications, including our search engine, we need to convert string representations of numbers into actual integer values. For example, converting a string "123" into the integer 123 for numerical operations or comparisons.

C++ provides two main functions for this purpose:
- **`std::stoi()`** - Modern C++ function (C++11 and later)
- **`atoi()`** - Legacy C function

Understanding the differences between these functions is crucial for writing safe and robust code.

### 3.2 What is std::stoi()?

**Definition:**  
`std::stoi()` (string to integer) is a C++11 standard library function that safely converts a string to an integer with robust error handling.

**Function Signature:**
```cpp
#include <string>

int std::stoi(const std::string& str, size_t* pos = 0, int base = 10);
```

**Parameters:**
- `str` - The string to convert
- `pos` - (Optional) Pointer to store the position of the first unconverted character
- `base` - (Optional) Numerical base (2-36), default is 10 (decimal)

**Requirements:**
- Requires `#include <string>` header
- Compiler must support C++11 or later standard
- Part of modern C++ standard library

### 3.3 What is atoi()?

**Definition:**  
`atoi()` (ASCII to integer) is a legacy C function that converts a C-style string to an integer.

**Function Signature:**
```cpp
#include <cstdlib>

int atoi(const char* str);
```

**Characteristics:**
- Legacy function from C standard library
- Simple but lacks error handling
- Returns 0 on error (ambiguous!)
- Cannot distinguish between valid "0" and conversion failure

### 3.4 Key Differences: std::stoi() vs atoi()

| Feature | std::stoi() | atoi() |
|---------|-------------|--------|
| **Error Handling** | Throws exceptions | Returns 0 (ambiguous) |
| **Invalid Input** | `std::invalid_argument` exception | Returns 0 |
| **Out of Range** | `std::out_of_range` exception | Undefined behavior |
| **Input Type** | `std::string` | `const char*` |
| **Standard** | C++11 and later | C89/C++98 |
| **Safety** | ✅ Safe with exceptions | ❌ Unsafe, no error info |
| **Base Conversion** | Supports multiple bases | Only base-10 |
| **Debugging** | ✅ Clear error messages | ❌ Silent failures |

### 3.5 Why Use std::stoi() Instead of atoi()?

#### Problem 1: atoi() Cannot Detect Errors

```cpp
#include <cstdlib>
#include <iostream>

int main() {
    int result1 = atoi("0");      // Valid input: "0"
    int result2 = atoi("abc");    // Invalid input!
    int result3 = atoi("");       // Empty string!
    
    std::cout << result1 << "\n";  // Prints: 0 ✓ Correct
    std::cout << result2 << "\n";  // Prints: 0 ❌ Wrong! Should be error
    std::cout << result3 << "\n";  // Prints: 0 ❌ Wrong! Should be error
    
    // How do you know if 0 is valid or an error? YOU CAN'T!
    return 0;
}
```

**The Problem:** `atoi()` returns 0 for:
- Valid input: `"0"`
- Invalid input: `"abc"`
- Empty input: `""`
- Null pointer: `nullptr`

You **cannot distinguish** between a legitimate zero and an error!

#### Solution: std::stoi() Throws Exceptions

```cpp
#include <string>
#include <iostream>
#include <stdexcept>

int main() {
    try {
        int result1 = std::stoi("0");       // ✓ Valid: returns 0
        std::cout << "Result: " << result1 << "\n";
        
        int result2 = std::stoi("abc");     // ✗ Invalid!
        // This line never executes
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Error: Invalid input!\n";  // ✓ Caught the error!
    }
    catch (const std::out_of_range& e) {
        std::cout << "Error: Number too large!\n";
    }
    
    return 0;
}
```

### 3.6 Exception Types in std::stoi()

#### 1. std::invalid_argument

**Thrown when:** The string is not a valid integer representation.

```cpp
try {
    int num = std::stoi("hello");   // Not a number!
} catch (const std::invalid_argument& e) {
    std::cout << "Invalid argument: " << e.what() << "\n";
}
```

**Common Cases:**
- Non-numeric characters: `"abc"`, `"hello"`
- Empty strings: `""`
- Special characters only: `"@#$"`

#### 2. std::out_of_range

**Thrown when:** The converted value exceeds the range of `int` type.

```cpp
try {
    // Integer range: -2,147,483,648 to 2,147,483,647 (32-bit)
    int num = std::stoi("9999999999999999");  // Too large!
} catch (const std::out_of_range& e) {
    std::cout << "Out of range: " << e.what() << "\n";
}
```

**Common Cases:**
- Numbers larger than INT_MAX (2,147,483,647)
- Numbers smaller than INT_MIN (-2,147,483,648)

### 3.7 Practical Examples

#### Example 1: Safe Conversion with Error Handling

```cpp
#include <string>
#include <iostream>
#include <stdexcept>

int safeStringToInt(const std::string& input) {
    try {
        int result = std::stoi(input);
        return result;
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Error: '" << input << "' is not a valid integer\n";
        return -1;  // Or throw, or return default value
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Error: '" << input << "' is out of range\n";
        return -1;
    }
}

int main() {
    std::string input1 = "42";
    std::string input2 = "abc";
    std::string input3 = "9999999999";
    
    std::cout << "Result 1: " << safeStringToInt(input1) << "\n";  // 42
    std::cout << "Result 2: " << safeStringToInt(input2) << "\n";  // Error caught
    std::cout << "Result 3: " << safeStringToInt(input3) << "\n";  // Error caught
    
    return 0;
}
```

#### Example 2: Comparing atoi() vs std::stoi()

```cpp
#include <cstdlib>   // For atoi()
#include <string>    // For std::stoi()
#include <iostream>

int main() {
    std::string input = "invalid";
    
    // Using atoi() - UNSAFE!
    int result_atoi = atoi(input.c_str());
    std::cout << "atoi result: " << result_atoi << "\n";  // Prints: 0 (misleading!)
    // Did it fail, or was the input really "0"? We don't know!
    
    // Using std::stoi() - SAFE!
    try {
        int result_stoi = std::stoi(input);
        std::cout << "stoi result: " << result_stoi << "\n";
    }
    catch (const std::invalid_argument& e) {
        std::cout << "stoi caught error: Invalid input!\n";  // ✓ Clear error!
    }
    
    return 0;
}
```

#### Example 3: Converting with Different Bases

```cpp
#include <string>
#include <iostream>

int main() {
    // Binary (base 2)
    int binary = std::stoi("1010", nullptr, 2);
    std::cout << "Binary 1010: " << binary << "\n";  // Prints: 10
    
    // Hexadecimal (base 16)
    int hex = std::stoi("FF", nullptr, 16);
    std::cout << "Hex FF: " << hex << "\n";          // Prints: 255
    
    // Octal (base 8)
    int octal = std::stoi("17", nullptr, 8);
    std::cout << "Octal 17: " << octal << "\n";      // Prints: 15
    
    return 0;
}
```

### 3.8 How This Applies to Our Search Engine

Our search engine might use `std::stoi()` for:

#### 1. **Processing Numerical Search Parameters**
```cpp
// Convert command-line argument to integer
try {
    int max_results = std::stoi(argv[2]);  // e.g., "./searchengine query 10"
    std::cout << "Showing top " << max_results << " results\n";
}
catch (const std::invalid_argument& e) {
    std::cerr << "Error: Max results must be a number!\n";
    return 1;
}
```

#### 2. **Reading Configuration Values**
```cpp
// Parse configuration from file
std::string config_value = "100";  // Read from config file
try {
    int buffer_size = std::stoi(config_value);
    // Use buffer_size...
}
catch (const std::exception& e) {
    std::cerr << "Invalid configuration value\n";
}
```

#### 3. **Validating User Input**
```cpp
// Ensure user provides valid numerical input
std::string user_choice;
std::cout << "Select document (1-10): ";
std::getline(std::cin, user_choice);

try {
    int choice = std::stoi(user_choice);
    if (choice < 1 || choice > 10) {
        std::cout << "Choice out of range!\n";
    } else {
        // Process valid choice
    }
}
catch (const std::invalid_argument& e) {
    std::cout << "Please enter a valid number!\n";
}
```

### 3.9 Common Pitfalls to Avoid

#### ❌ Wrong: Using atoi() without validation
```cpp
int value = atoi(user_input.c_str());
if (value == 0) {
    // Is this an error or did user enter "0"? AMBIGUOUS!
}
```

#### ✓ Correct: Using std::stoi() with exception handling
```cpp
try {
    int value = std::stoi(user_input);
    // Use value safely
}
catch (const std::invalid_argument& e) {
    // Handle invalid input
}
catch (const std::out_of_range& e) {
    // Handle overflow
}
```

#### ⚠️ Important Notes:

1. **Always Use Try-Catch:** Never use `std::stoi()` without exception handling
   ```cpp
   // Bad: No exception handling
   int num = std::stoi(input);  // Can crash!
   
   // Good: With exception handling
   try {
       int num = std::stoi(input);
   } catch (...) { /* handle */ }
   ```

2. **Partial Conversions:** `std::stoi()` converts until it hits a non-digit
   ```cpp
   int num = std::stoi("123abc");  // Returns 123 (stops at 'a')
   ```

3. **Leading Whitespace:** Both functions skip leading whitespace
   ```cpp
   int num = std::stoi("  42");    // Returns 42 ✓
   ```

### 3.10 Related Functions

The C++ standard library provides similar conversion functions:

| Function | Converts To | Example |
|----------|-------------|----------|
| `std::stoi()` | `int` | `std::stoi("42")` |
| `std::stol()` | `long` | `std::stol("1000000")` |
| `std::stoll()` | `long long` | `std::stoll("999999999999")` |
| `std::stoul()` | `unsigned long` | `std::stoul("4294967295")` |
| `std::stof()` | `float` | `std::stof("3.14")` |
| `std::stod()` | `double` | `std::stod("3.14159265")` |

All follow the same exception-based error handling pattern.

### 3.11 Quick Comparison Table

| Scenario | atoi() | std::stoi() |
|----------|--------|-------------|
| Input: `"42"` | Returns 42 | Returns 42 |
| Input: `"abc"` | Returns 0 ❌ | Throws exception ✓ |
| Input: `"0"` | Returns 0 | Returns 0 |
| Input: `"999999999999"` | Undefined ❌ | Throws exception ✓ |
| Error detection | ❌ Impossible | ✓ Via exceptions |
| Safety | ❌ Unsafe | ✓ Safe |
| Modern C++ | ❌ Legacy | ✓ Recommended |

### 3.12 Summary

- **`std::stoi()`** is the **modern, safe** way to convert strings to integers
- **`atoi()`** is **legacy and unsafe** - avoid in new code
- `std::stoi()` throws **`std::invalid_argument`** for invalid input
- `std::stoi()` throws **`std::out_of_range`** for values outside int range
- `atoi()` returns **0 on error**, making it impossible to distinguish errors from valid zero
- **Always use exception handling** with `std::stoi()`
- Requires **C++11 or later** and `#include <string>`
- Essential for **robust input validation** in our search engine

---

## 4. Understanding main() Return Values

### 4.1 What Does main() Return Value Mean?

In C and C++, the `main()` function returns an integer value to the operating system indicating whether the program executed successfully or encountered an error.

### 4.2 The Standard Convention

**Unix/Linux/POSIX Standard:**
- **0** = Success (program completed without errors)
- **Non-zero** = Failure (error occurred)

```cpp
int main() {
    // ... program logic ...
    
    if (everything_went_well) {
        return 0;  // ✅ Success
    } else {
        return 1;  // ❌ Error
    }
}
```

### 4.3 Why This Matters?

**Shell Scripts and Automation:**
```bash
#!/bin/bash
./searchengine -d data.txt -k 5

if [ $? -eq 0 ]; then
    echo "Search completed successfully"
else
    echo "Search failed with error"
fi
```

The shell variable `$?` stores the return value (exit code) of the last command.

**Example:**
```bash
$ ./searchengine -d data.txt -k 5
File read successfully. Lines: 10, Max Length: 100

$ echo $?
0    # ✅ Success - returned 0

$ ./searchengine -d missing.txt -k 5
Error opening file: missing.txt

$ echo $?
1    # ❌ Error - returned non-zero
```

### 4.4 Our Fix - December 26, 2025

**Before (WRONG):**
```cpp
int main(int argc, char** argv) {
    // ... all the logic ...
    
    delete (mymap);
    delete (trie);
    return 1;  // ❌ Always returns 1 (error code)
}
```

**Problem:**
- Returns `1` even when program succeeds
- Shell scripts think the program failed
- Violates standard convention
- Confusing for automation tools

**After (CORRECT):**
```cpp
int main(int argc, char** argv) {
    // ... all the logic ...
    
    delete (mymap);
    delete (trie);
    return 0;  // ✅ Returns 0 (success)
}
```

**Impact:**
- ✅ Follows Unix/POSIX standard
- ✅ Shell scripts work correctly
- ✅ Can be used in pipelines and automation
- ✅ Professional, portable code

### 4.5 Complete Error Handling Pattern

```cpp
int main(int argc, char** argv) {
    // Validate arguments
    if (argc != 5 || strcmp(argv[1], "-d") != 0) {
        cout << "Wrong arguments" << endl;
        return -1;  // ❌ Error in arguments
    }
    
    // Allocate resources
    Mymap *mymap = new Mymap(linecounter, maxlength);
    TrieNode *trie = new TrieNode();
    
    // Read input
    if(read_input(mymap, trie, argv[2]) == -1){
        delete (mymap);
        return -1;  // ❌ Error reading file
    }
    
    // Success!
    delete (mymap);
    delete (trie);
    return 0;  // ✅ Success
}
```

### 4.6 Common Exit Codes

| Code | Meaning | Usage |
|------|---------|-------|
| 0 | Success | Everything worked |
| 1 | General error | Something went wrong |
| 2 | Misuse of command | Wrong arguments |
| 126 | Command cannot execute | Permission issue |
| 127 | Command not found | Binary missing |
| 128+n | Fatal error signal n | Crash/signal |

### 4.7 Best Practices

✅ Return 0 for success  
✅ Return non-zero for errors  
✅ Use different error codes for different failures  
✅ Document error codes in README  
✅ Clean up resources before returning  

---

## 5. Understanding Header Guards and Include Paths

### 5.1 Header Guard Naming Conventions

Header guards prevent multiple inclusion of the same header file. The naming convention should be **consistent** across all header files.

### 5.2 Common Naming Patterns

**Pattern 1: _H suffix**
```cpp
#ifndef MYCLASS_H
#define MYCLASS_H
// ...
#endif
```

**Pattern 2: _HPP suffix**
```cpp
#ifndef MYCLASS_HPP
#define MYCLASS_HPP
// ...
#endif
```

**Pattern 3: Full path**
```cpp
#ifndef PROJECT_HEADER_MYCLASS_HPP
#define PROJECT_HEADER_MYCLASS_HPP
// ...
#endif
```

### 5.3 Our Standardization - December 26, 2025

**Before (INCONSISTENT):**
```cpp
// Listnode.hpp
#ifndef LISTNODE_H    // ❌ Uses _H
#define LISTNODE_H

// Map.hpp
#ifndef MAP_HPP       // ✅ Uses _HPP
#define MAP_HPP

// Trie.hpp  
#ifndef TRIE_HPP      // ✅ Uses _HPP
#define TRIE_HPP
```

**After (CONSISTENT):**
```cpp
// All files now use _HPP
#ifndef LISTNODE_HPP  // ✅ Consistent
#define LISTNODE_HPP

#ifndef MAP_HPP       // ✅ Consistent
#define MAP_HPP

#ifndef TRIE_HPP      // ✅ Consistent
#define TRIE_HPP
```

**Why standardize?**
- ✅ Cleaner codebase
- ✅ Easier to remember
- ✅ Professional appearance
- ✅ Matches C++ file extension (.hpp)

### 5.4 Include Path Best Practices

**CMake include_directories:**
```cmake
include_directories(header)  # Tell compiler where headers are
```

**Then in source files:**
```cpp
// ✅ GOOD - Clean and simple
#include "Map.hpp"
#include "Trie.hpp"

// ❌ BAD - Redundant prefix
#include "./Map.hpp"
#include "./Trie.hpp"

// ❌ BAD - Explicit path (breaks when reorganizing)
#include "../header/Map.hpp"
```

### 5.5 Our Fix - Removed ./ Prefixes

**Files Changed:**
- `src/Map.cpp` line 1
- `header/Searchengine.hpp` lines 4-5

**Before:**
```cpp
// Searchengine.hpp
#include "./Document_store.hpp"
#include "./Map.hpp"
#include "Trie.hpp"  // Inconsistent!
```

**After:**
```cpp
// Searchengine.hpp
#include "Document_store.hpp"
#include "Map.hpp"
#include "Trie.hpp"  // All consistent!
```

**Benefits:**
- ✅ Consistency across all includes
- ✅ Cleaner code
- ✅ Works with CMake include path
- ✅ More portable

---

## References

For more information, visit:  
- [What does int argc, char *argv[] mean?](https://stackoverflow.com/questions/3024197/what-does-int-argc-char-argv-mean)
- [C++ strcmp() Reference](https://cplusplus.com/reference/cstring/strcmp/)
- [String Comparison in C/C++](https://en.cppreference.com/w/cpp/string/byte/strcmp/)
- [Exit Status (Unix)](https://en.wikipedia.org/wiki/Exit_status)
- [Header Guards](https://en.cppreference.com/w/cpp/preprocessor/include)

---

**Document Version**: 1.1  
**Last Updated**: December 26, 2025  
**Changes**: Added main() return values, header guard consistency, and include path documentation  

*Documentation created for educational purposes to help understand the Search Engine project implementation.*
