# Search - Concepts Documentation

This document explains the **concepts and theory** behind search operations and query handling. For code implementation details, see `working.md`.

---

## Table of Contents

1. [What is Search Module?](#1-what-is-search-module)
2. [What is TF (Term Frequency)?](#2-what-is-tf-term-frequency)
3. [What is DF (Document Frequency)?](#3-what-is-df-document-frequency)
4. [What is strtok?](#4-what-is-strtok)
5. [What is isdigit?](#5-what-is-isdigit)
6. [What is atoi?](#6-what-is-atoi)
7. [Why Not Use stoi?](#7-why-not-use-stoi)
8. [Query Command System](#8-query-command-system)

---

## 1. What is Search Module?

### Definition

The **Search module** is responsible for executing user queries and calculating relevance scores for documents. It provides three main operations:

1. **`/search`** - Search for documents containing query words
2. **`/tf`** - Get Term Frequency (word count in specific document)
3. **`/df`** - Get Document Frequency (how many documents contain word)

### Purpose

The search module bridges the **user interface** (command-line queries) with the **backend data structures** (Trie, Map, Listnode) to provide meaningful search results.

---

## 2. What is TF (Term Frequency)?

### Definition

**Term Frequency (TF)** is the count of how many times a specific word appears in a specific document.

### Example

```
Document 0: "hello world hello earth hello"

TF("hello", doc=0) = 3
TF("world", doc=0) = 1
TF("earth", doc=0) = 1
TF("python", doc=0) = 0  (not present)
```

### Why TF Matters

**Relevance Scoring:**
- Document with word appearing 5 times is MORE relevant than document with word appearing once
- Higher TF → Higher BM25 score

**Query:**
```bash
/tf 0 hello
# Returns: 3 (appears 3 times in document 0)
```

### Formula in BM25

```
TF Component = (TF × (k1 + 1)) / (TF + k1 × (1 - b + b × (doc_len / avg_doc_len)))

Where:
- k1 = 1.5 (tuning parameter)
- b = 0.75 (length normalization)
```

---

## 3. What is DF (Document Frequency)?

### Definition

**Document Frequency (DF)** is the count of how many documents contain a specific word.

### Example

```
Document 0: "hello world"
Document 1: "hello earth"
Document 2: "python code"

DF("hello") = 2  (in doc 0 and 1)
DF("world") = 1  (only in doc 0)
DF("python") = 1  (only in doc 2)
DF("java") = 0   (not present)
```

### Why DF Matters

**Inverse Relationship:**
- **Common words** (high DF) → Less important (e.g., "the", "is")
- **Rare words** (low DF) → More important (e.g., "quantum", "blockchain")

**IDF Calculation:**
```
IDF = log((N - DF + 0.5) / (DF + 0.5))

Where:
- N = total number of documents
- DF = document frequency
```

**Query:**
```bash
/df hello
# Returns: 2 (present in 2 documents)
```

### Example Impact

```
Query: "quantum physics"

DF("quantum") = 5 out of 1000 documents  → High IDF (rare, important!)
DF("the") = 998 out of 1000 documents   → Low IDF (common, less important)
```

---

## 4. What is strtok?

### Definition

`strtok` is a C function that **tokenizes** (splits) a string into smaller tokens based on delimiters.

### Syntax

```cpp
char* strtok(char* str, const char* delimiters);
```

### How It Works

```cpp
char input[] = "/tf 0 hello";
char* token = strtok(input, " \t\n");  // First call: returns "/tf"

token = strtok(NULL, " \t\n");  // Second call: returns "0"
token = strtok(NULL, " \t\n");  // Third call: returns "hello"
token = strtok(NULL, " \t\n");  // Fourth call: returns NULL (end)
```

### Key Points

**1. Modifies Original String:**
```cpp
char input[] = "hello world";
strtok(input, " ");
// input is now: "hello\0world"  (inserted null terminator)
```

**2. Returns Pointer to Original String:**
```cpp
char* token = strtok(input, " ");
// token points to somewhere inside input[]
// NOT a new allocated memory!
```

**3. Stateful Function:**
```cpp
strtok(input, " ");   // First call with string
strtok(NULL, " ");    // Subsequent calls with NULL
// Remembers position internally (static variable)
```

### Critical Memory Rule

**NEVER call free() on strtok result:**
```cpp
char* token = strtok(input, " ");
free(token);  // ❌ CRASH! token points to input[], not malloc'd memory
```

### Alternatives

| Function | Thread-Safe | Modifies String | Allocates Memory |
|----------|-------------|-----------------|------------------|
| `strtok` | ❌ No | ✅ Yes | ❌ No |
| `strtok_r` | ✅ Yes | ✅ Yes | ❌ No |
| `strsep` | ✅ Yes | ✅ Yes | ❌ No |
| Manual parsing | ✅ Yes | ❌ No | ✅ Optional |

---

## 5. What is isdigit?

### Definition

`isdigit` checks if a character is a decimal digit (0-9).

### Syntax

```cpp
#include <cctype>
int isdigit(int ch);
```

### How It Works

```cpp
isdigit('5')   // Returns: non-zero (true)
isdigit('a')   // Returns: 0 (false)
isdigit(' ')   // Returns: 0 (false)
isdigit('\n')  // Returns: 0 (false)
```

### Use Case in Our Code

**Validating document ID:**
```cpp
char* id_str = "123";
for(int i = 0; i < strlen(id_str); i++){
    if(!isdigit(id_str[i])){
        cout << "Error: ID must be a number" << endl;
        return -1;
    }
}
// All characters are digits ✅
int id = atoi(id_str);
```

### Why Not Just atoi?

```cpp
atoi("123")    // Returns: 123 ✅
atoi("abc")    // Returns: 0 (silent failure) ❌
atoi("12abc")  // Returns: 12 (partial parse) ❌
```

**Problem:** `atoi` doesn't tell you if input was valid!

**Solution:** Validate first with `isdigit`:
```cpp
// Step 1: Validate
if(!isdigit(str[0])){
    return -1;  // Invalid input
}

// Step 2: Convert
int num = atoi(str);  // Safe now
```

### Alternatives

| Function | Purpose | Example |
|----------|---------|---------|
| `isdigit` | Check if digit | `isdigit('5')` |
| `isalpha` | Check if letter | `isalpha('a')` |
| `isalnum` | Check if alphanumeric | `isalnum('a')` |
| `isspace` | Check if whitespace | `isspace(' ')` |

---

## 6. What is atoi?

### Definition

`atoi` converts a string to an integer (ASCII to Integer).

### Syntax

```cpp
#include <cstdlib>
int atoi(const char* str);
```

### How It Works

```cpp
atoi("123")      // Returns: 123
atoi("-456")     // Returns: -456
atoi("  789")    // Returns: 789 (skips leading spaces)
atoi("12.34")    // Returns: 12 (stops at non-digit)
atoi("abc")      // Returns: 0 (invalid input)
atoi("")         // Returns: 0 (empty string)
```

### Conversion Process

```
String: "123"
  ↓
'1' → 1
'2' → 10 + 2 = 12
'3' → 120 + 3 = 123
  ↓
Integer: 123
```

### Problems with atoi

**1. No Error Reporting:**
```cpp
int x = atoi("abc");  // x = 0 (no error!)
int y = atoi("0");    // y = 0 (valid zero)
// Can't tell the difference! ❌
```

**2. Silent Partial Conversion:**
```cpp
int x = atoi("123abc");  // x = 123 (stops at 'a')
// No warning about "abc" ❌
```

**3. No Overflow Detection:**
```cpp
int x = atoi("999999999999999");  // Undefined behavior!
```

### When to Use atoi

✅ **Good for:**
- Trusted input (already validated)
- Simple cases where error handling not critical
- Quick prototypes

❌ **Bad for:**
- User input validation
- Production code requiring error handling
- Large numbers (use strtol)

---

## 7. Why Not Use stoi?

### Comparison

| Feature | atoi | stoi |
|---------|------|------|
| Language | C | C++ |
| Header | `<cstdlib>` | `<string>` |
| Input | `const char*` | `std::string` |
| Error handling | None | Throws exception |
| Overflow | Undefined | Throws exception |

### Example: stoi

```cpp
#include <string>
try {
    int x = stoi("123");        // ✅ Works
    int y = stoi("abc");        // ❌ Throws invalid_argument
    int z = stoi("999999999");  // ❌ Throws out_of_range
} catch(const std::exception& e) {
    cout << "Error: " << e.what() << endl;
}
```

### Why We Use atoi Instead

**1. Our Code Uses C-Style Strings:**
```cpp
char* token = strtok(input, " ");
// token is char*, not std::string
// Would need conversion: std::string(token)
```

**2. Already Validated Input:**
```cpp
// We validate BEFORE converting
for(int i=0; i<strlen(token); i++){
    if(!isdigit(token[i])){
        return -1;  // Validated ✅
    }
}
int id = atoi(token);  // Safe to use atoi
```

**3. Simpler Error Handling:**
```cpp
// atoi approach (our code)
if(!isdigit(token[0])) return -1;
int x = atoi(token);

// stoi approach
try {
    int x = stoi(std::string(token));
} catch(...) {
    return -1;
}
// More code, same result
```

**4. C-Style Project:**
- Using `char*`, `strtok`, `malloc/free`
- Mixing C++ std::string would be inconsistent
- atoi fits the C-style design

### When stoi is Better

✅ **Use stoi when:**
- Working with `std::string` throughout
- Need strong exception-based error handling
- Using modern C++ (C++11+)
- Want automatic overflow detection

---

## 8. Query Command System

### Available Commands

```bash
/search <query>     # Search for documents (not implemented)
/tf <doc_id> <word> # Get term frequency
/df <word>          # Get document frequency  
/exit               # Exit program
```

### Command Format

**TF Query:**
```bash
/tf 0 hello
│   │ │
│   │ └─ word to search
│   └─ document ID (must be number)
└─ command
```

**DF Query:**
```bash
/df hello
│   │
│   └─ word to search
└─ command
```

### Input Flow

```
User types: "/tf 0 hello\n"
    ↓
getline() reads entire line
    ↓
strtok splits: ["/tf", "0", "hello"]
    ↓
inputmanager() identifies command
    ↓
tf() function executes
    ↓
Result displayed
```

### Error Handling

**Unknown Command:**
```bash
> hello
Unknown command: hello
Available commands: /search, /df, /tf, /exit
```

**Missing Arguments:**
```bash
> /tf 0
Error: Missing word. Usage: /tf <doc_id> <word>
```

**Invalid Document ID:**
```bash
> /tf abc hello
Error: Document ID must be a number
```

---

## 9. What is #include <windows.h>?

### Definition

**`#include <windows.h>`** is a **Windows-specific header file** that provides access to Windows API functions, including terminal/console manipulation functions.

### What Does It Provide?

```cpp
#include <windows.h>
```

**Windows API functions available:**
- `GetConsoleScreenBufferInfo()` - Get terminal dimensions
- `CONSOLE_SCREEN_BUFFER_INFO` - Structure for console data
- `GetStdHandle(STD_OUTPUT_HANDLE)` - Get console handle
- `SetConsoleTextAttribute()` - Change text colors
- `system("cls")` - Clear screen

### Why Do We Use It?

**In Search.hpp:**
```cpp
#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/ioctl.h>
    #include <unistd.h>
#endif
```

**Purpose:** Get terminal width for formatted output (line separators, etc.)

**Windows Example:**
```cpp
CONSOLE_SCREEN_BUFFER_INFO csbi;
GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
```

**Linux Alternative:**
```cpp
struct winsize w;
ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
int width = w.ws_col;
```

### Cross-Platform Approach

**Our Implementation:**
```cpp
#ifdef _WIN32       // Windows
    #include <windows.h>
#else               // Linux/Unix/Mac
    #include <sys/ioctl.h>
    #include <unistd.h>
#endif
```

**Explanation:**
- `#ifdef _WIN32` - Checks if compiling on Windows
- Include Windows headers if Windows
- Include POSIX headers if Unix-like systems
- Allows same code to compile on different platforms

### Q: What Are Alternatives to windows.h?

**1. Hardcode Width**
```cpp
int width = 80;  // Assume 80 columns
```
✅ Simple, works everywhere
❌ Not accurate for all terminals
❌ Users with different screen sizes see bad formatting

**2. Use Cross-Platform Library**
```cpp
#include <ncurses.h>  // Terminal manipulation library
initscr();
int width = COLS;
```
✅ Works on all platforms
❌ Requires external library installation
❌ More complex API

**3. Use Boost.Program_options**
```cpp
#include <boost/program_options.hpp>
```
✅ Professional-grade
❌ Heavy dependency
❌ Overkill for simple terminal width

**4. Conditional Compilation (Our Choice)**
```cpp
#ifdef _WIN32
    // Windows code
#else
    // Linux code
#endif
```
✅ No external dependencies
✅ Native platform functions (fastest)
✅ Minimal code
❌ Must maintain two code paths

### Q: Why Not Always Include windows.h?

**Problem with always including:**
```cpp
#include <windows.h>  // On Linux: ERROR!
```

**Error on Linux:**
```
fatal error: windows.h: No such file or directory
```

**Reason:** 
- `windows.h` only exists on Windows
- Linux uses different system headers
- Must use conditional compilation

---

## 10. What is BM25 Ranking Algorithm?

### Definition

**BM25 (Best Matching 25)** is a probabilistic ranking function used to score documents based on query term relevance.

### Full Name

**Okapi BM25** - Developed at City University, London (Okapi information retrieval system)

### Why Use BM25?

**Problem with simple word counting:**
```
Query: "machine learning"
Doc 1: "machine" appears 5 times → Score = 5
Doc 2: "machine" appears 100 times → Score = 100
```

❌ **Issue:** Doc 2 might just be repeating "machine" without real content!

**BM25 Solution:**
- **Diminishing returns** - Each occurrence matters less
- **Document length normalization** - Penalize long documents
- **IDF weighting** - Rare words score higher

### BM25 Formula

```
score = Σ IDF(qi) × (f(qi,D) × (k1+1)) / (f(qi,D) + k1×(1 - b + b×(|D|/avgdl)))

Where:
  qi      = query term i
  IDF     = Inverse Document Frequency
  f(qi,D) = Term frequency in document D (TF)
  |D|     = Document length (word count)
  avgdl   = Average document length
  k1      = Term frequency saturation (typically 1.2)
  b       = Length normalization (typically 0.75)
```

### Components Explained

**1. IDF (Inverse Document Frequency)**
```cpp
IDF = log((N - df + 0.5) / (df + 0.5))

Where:
  N  = Total number of documents
  df = Documents containing the term
```

**Example:**
```
Total docs: 100
Term "the": appears in 95 docs → IDF = log((100-95+0.5)/(95+0.5)) = -2.94 (low)
Term "quantum": appears in 2 docs → IDF = log((100-2+0.5)/(2+0.5)) = 3.68 (high)
```

**Interpretation:**
- Common words ("the", "is") → Low IDF → Less important
- Rare words ("quantum", "algorithm") → High IDF → More important

**2. Term Frequency Saturation (k1)**
```cpp
k1 = 1.2  // Standard value

tf_component = (tf × (k1+1)) / (tf + k1×normalization)
```

**Example with k1=1.2:**
```
TF=1  → tf_component ≈ 0.92
TF=2  → tf_component ≈ 1.47
TF=5  → tf_component ≈ 1.96
TF=10 → tf_component ≈ 2.04
TF=100→ tf_component ≈ 2.18  (barely increases!)
```

**Effect:** After ~10 occurrences, additional occurrences barely increase score.

**3. Length Normalization (b)**
```cpp
b = 0.75  // Standard value (0 = no normalization, 1 = full)

normalization = 1 - b + b×(docLength/avgLength)
```

**Example with b=0.75:**
```
Doc length = avg length → norm = 1.0 (no penalty)
Doc length = 2× avg    → norm = 1.75 (score reduced)
Doc length = 0.5× avg  → norm = 0.625 (score boosted)
```

**Effect:** Longer documents need higher TF to match short documents.

### Complete BM25 Example

```
Query: "search engine"
Total docs: 10
Average doc length: 100 words

Term "search":
  - Appears in 5 docs → IDF = log((10-5+0.5)/(5+0.5)) = 0.0
  
Term "engine":
  - Appears in 2 docs → IDF = log((10-2+0.5)/(2+0.5)) = 1.22

Document 5:
  - Length: 120 words
  - TF("search") = 3
  - TF("engine") = 2
  
Score calculation:
  normalization = 1 - 0.75 + 0.75×(120/100) = 1.15
  
  score("search") = 0.0 × (3×2.2)/(3 + 1.2×1.15) = 0.0
  score("engine") = 1.22 × (2×2.2)/(2 + 1.2×1.15) = 1.27
  
  Total BM25 score = 0.0 + 1.27 = 1.27
```

### Q: Why k1=1.2 and b=0.75?

**Answer:** Empirically determined optimal values.

**Research findings:**
- Tested on TREC (Text REtrieval Conference) datasets
- k1=1.2 and b=0.75 performed best across various document collections
- Values can be tuned for specific domains

**Parameter effects:**
```
k1 higher (2.0):  More weight on term frequency
k1 lower (0.5):   Less weight on term frequency

b higher (1.0):   Full length normalization
b lower (0.0):    No length normalization
```

### Q: What Are Alternatives to BM25?

**1. TF-IDF (Term Frequency - Inverse Document Frequency)**
```cpp
score = TF × IDF
```
✅ Simpler formula
✅ Faster computation
❌ No length normalization
❌ Linear TF scaling (no saturation)

**2. BM25+ (Extended BM25)**
```cpp
score = BM25 + delta×(|D|/avgdl)
```
✅ Better for verbose queries
✅ Prevents zero scores
❌ More complex
❌ Extra parameter (delta)

**3. Language Models**
```cpp
score = P(query|document)
```
✅ Theoretically elegant
✅ Handles phrase queries better
❌ More computational overhead
❌ Requires smoothing techniques

**4. Neural Ranking (BERT, etc.)**
```cpp
score = NeuralNetwork(query, document)
```
✅ State-of-the-art accuracy
✅ Understands semantics
❌ Extremely slow
❌ Requires GPU
❌ Needs training data

**Our choice:** BM25 is the **industry standard** for fast, effective text search.

---

## 11. Memory Management in Search Function

### Q: Why Do We Need delete heap and delete scorelist?

**Problem: Memory Leaks**
```cpp
void search(...) {
    Maxheap* heap = new Maxheap(k);      // Allocates memory
    Scorelist* scorelist = new Scorelist();  // Allocates memory
    
    // ... use heap and scorelist ...
    
    return;  // ❌ Memory never freed! LEAK!
}
```

**Each search call leaks:**
- Heap object: ~100 bytes + (k × 12 bytes for scores)
- Scorelist chain: ~16 bytes × number of documents

**After 1000 searches:** Megabytes of leaked memory!

**Solution (Jan 2, 2026 Fix):**
```cpp
void search(...) {
    Maxheap* heap = new Maxheap(k);
    Scorelist* scorelist = new Scorelist();
    
    // ... use them ...
    
    // Cleanup before returning
    delete heap;       // Frees heap arrays
    delete scorelist;  // Frees entire chain recursively
}
```

### Q: Why Not Use Stack Allocation?

**Alternative approach:**
```cpp
void search(...) {
    Maxheap heap(k);        // Stack allocation
    Scorelist scorelist;    // Stack allocation
    
    // ... use them ...
    
    // Automatic cleanup when function exits
}
```

**Advantages:**
✅ No manual delete needed
✅ Automatic cleanup
✅ Exception-safe

**Why we don't use it:**
❌ Maxheap uses malloc internally (expects heap allocation)
❌ Stack size limited (~1MB) - large k might overflow
❌ C-style codebase convention (new/delete)

**Modern C++ would prefer stack allocation!**

---

## 12. Input Validation

### Q: Why Check if(token == NULL)?

**Added: January 2, 2026**

```cpp
void search(char* token, ...) {
    if(token == NULL) {
        cout << "Error: No search query provided" << endl;
        return;
    }
    // ... rest of function ...
}
```

**Problem without validation:**
```
User types: /search
             ↓
token = NULL (strtok returns NULL for empty input)
             ↓
strtok(NULL, " ")  // Undefined behavior!
             ↓
CRASH or garbage results
```

**With validation:**
```
User types: /search
             ↓
token = NULL
             ↓
Check: token == NULL? YES
             ↓
Display error message
Return safely ✅
```

### Q: What Other Validation Could We Add?

**1. Maximum query length**
```cpp
int wordCount = 0;
while(token != NULL && wordCount < MAX_QUERY_WORDS) {
    // Prevents overflow if user pastes huge query
}
```

**2. Empty word validation**
```cpp
if(strlen(token) == 0) continue;  // Skip empty tokens
```

**3. Special character handling**
```cpp
// Remove punctuation, convert to lowercase
for(int i=0; token[i]; i++){
    if(!isalnum(token[i])) token[i] = ' ';
    token[i] = tolower(token[i]);
}
```

**4. Result count validation**
```cpp
if(k <= 0 || k > 100) {
    cout << "Error: k must be between 1 and 100" << endl;
    return;
}
```

**Currently implemented:** Basic NULL check (sufficient for controlled input).

---

## Summary

### Key Concepts

1. **TF** - Word count in specific document (relevance indicator)
2. **DF** - Document count containing word (rarity indicator)
3. **BM25** - Probabilistic ranking with TF saturation and length normalization
4. **strtok** - String tokenizer (NEVER free its result!)
5. **isdigit** - Validates numeric input (prevents atoi errors)
6. **atoi** - Converts string to int (simple, no error reporting)
7. **stoi** - Modern C++ alternative (exceptions, slower for C-style code)
8. **windows.h** - Windows API for terminal functions (cross-platform with ifdef)

### Why These Choices?

✅ **strtok** - Fast, simple, fits C-style design  
✅ **isdigit** - Validates before conversion  
✅ **atoi** - Simple conversion after validation  
✅ **BM25** - Industry standard ranking algorithm  
✅ **windows.h** - Native terminal access (conditional compilation)  
✅ **delete heap/scorelist** - Prevents memory leaks  
✅ **Input validation** - Prevents crashes on empty queries  

### BM25 Parameters

| Parameter | Value | Purpose |
|-----------|-------|---------|
| **k1** | 1.2 | TF saturation (diminishing returns) |
| **b** | 0.75 | Length normalization factor |
| **IDF** | log((N-df+0.5)/(df+0.5)) | Rare word boosting |

### Integration with Other Modules

```
User Query → Search.cpp → Parse → Trie (TF/DF lookup)
                      ↓
              Calculate BM25 → Scorelist (track docs)
                      ↓
              Insert to Maxheap → Get top-k results
                      ↓
              Display documents → Map (get content)
                      ↓
              Cleanup (delete heap, scorelist)
```

---

**Document Version**: 1.1  
**Last Updated**: January 2, 2026  
**Changes**: Added BM25 explanation, windows.h Q&A, memory management, input validation  
**Author**: High-Performance Search Engine Project
