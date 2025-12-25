# Listnode - Concepts Documentation

This document explains the **concepts and theory** behind the Listnode (linked list) data structure used for tracking document information in our search engine. For detailed code explanation, see `working.md`.

---

## Table of Contents

1. [What is Listnode?](#1-what-is-listnode)
2. [Why Use Linked List?](#2-why-use-linked-list)
3. [Listnode Structure](#3-listnode-structure)
4. [Linked List vs Array](#4-linked-list-vs-array)
5. [Recursive Operations](#5-recursive-operations)
6. [Integration with Trie](#6-integration-with-trie)
7. [Term Frequency Tracking](#7-term-frequency-tracking)
8. [Future Usage in Search](#8-future-usage-in-search)

---

## 1. What is Listnode?

### Definition

**Listnode** is a linked list node that stores information about which documents contain a specific word and how many times the word appears in each document.

### Purpose in Search Engine

When a word appears in multiple documents, we need to track:
1. **Document IDs** - Which documents contain this word
2. **Frequency** - How many times the word appears in each document

### Example

```
Word: "hello"

Document 1: "hello world hello"     → appears 2 times
Document 3: "hello earth"           → appears 1 time
Document 7: "hello hello hello"     → appears 3 times

Listnode chain:
[id:1, times:2] → [id:3, times:1] → [id:7, times:3] → NULL
```

---

## 2. Why Use Linked List?

### Problem: Unknown Number of Documents

**Challenge:**
- We don't know in advance how many documents will contain each word
- Array would require fixed size or expensive reallocation
- Need dynamic structure that grows as needed

### Linked List Advantages

**1. Dynamic Size**
```cpp
// Start with nothing
listnode* list = NULL;

// Add documents as they appear
list->add(5);   // First document
list->add(12);  // Second document
list->add(3);   // Third document
// No size limit, grows automatically
```

**2. Efficient Insertion**
```cpp
// O(1) insertion at head
// O(n) insertion at tail (but sequential access is fine)
```

**3. Memory Efficiency**
```cpp
// Only allocates memory for documents that actually contain the word
// Rare words: small list (1-2 nodes)
// Common words: larger list (many nodes)
```

### Comparison with Alternatives

| Approach | Memory | Insert Time | Search Time |
|----------|--------|-------------|-------------|
| Fixed Array | Wastes space | O(1) | O(1) with index |
| Dynamic Array | Reallocation cost | O(n) resize | O(1) |
| **Linked List** | **Minimal** | **O(1)** | **O(n) but small** |
| Hash Map | Overhead | O(1) | O(1) but complex |

For our use case (small lists, sequential access), linked list is optimal.

---

## 3. Listnode Structure

### Class Definition

```cpp
class listnode {
    listnode *next;  // Pointer to next document
    int id;          // Document ID
    int times;       // Occurrence count
public:
    listnode(int did);
    ~listnode();
    void add(int did);
};
```

### Member Variables

**1. next**
```cpp
listnode *next;
```
- Points to the next node in the list
- NULL for last node
- Enables chaining of document records

**2. id**
```cpp
int id;
```
- Stores the document ID
- Identifies which document contains the word
- Used for search queries: "Does document 5 contain 'hello'?"

**3. times**
```cpp
int times;
```
- Counts how many times the word appears in this document
- Initialized to 0, incremented with each occurrence
- Used for Term Frequency (TF) in BM25 algorithm

### Visual Representation

**Single Node:**
```
+------------------+
| id: 5            |  ← Document ID
| times: 3         |  ← Appears 3 times
| next: 0x2000 ----|--> Next node
+------------------+
```

**Linked Chain:**
```
Word: "search"

  HEAD
   ↓
[id:1, times:2] → [id:4, times:1] → [id:9, times:5] → NULL
   Doc 1             Doc 4             Doc 9
  2 times           1 time           5 times
```

---

## 4. Linked List vs Array

### Why Not Use Array?

**Option 1: Fixed-Size Array**
```cpp
int document_ids[100];    // ❌ What if > 100 documents?
int frequencies[100];     // ❌ Wastes memory if word rare
```

**Problems:**
- Fixed size limit
- Wastes memory for rare words
- No flexibility

**Option 2: Dynamic Array (vector)**
```cpp
vector<int> document_ids;     // ✅ Dynamic
vector<int> frequencies;      // ✅ Dynamic
```

**Problems:**
- Two separate arrays to manage
- Reallocation overhead
- More complex than needed

### Why Linked List Wins

**Simplicity:**
```cpp
listnode* list = NULL;    // Start empty
list->add(5);              // Add document 5
list->add(3);              // Add document 3
// Simple, clean, self-contained
```

**Memory Efficiency:**
```
Word appears in 2 documents:
Array:  100 × 8 bytes = 800 bytes (even if only 2 used)
List:   2 nodes × 16 bytes = 32 bytes ✅
```

**No Reallocation:**
- Array needs reallocation when full
- Linked list grows one node at a time
- No expensive copy operations

---

## 5. Recursive Operations

### Why Recursion?

Linked list operations naturally fit recursive patterns:

**Iterative (Complex):**
```cpp
void add(int did) {
    listnode* current = this;
    listnode* prev = NULL;
    
    while(current != NULL) {
        if(current->id == did) {
            current->times++;
            return;
        }
        prev = current;
        current = current->next;
    }
    
    prev->next = new listnode(did);
}
```

**Recursive (Elegant):**
```cpp
void add(int did) {
    if(did == id) {
        times++;  // Found it!
    } else {
        if(next == NULL) {
            next = new listnode(did);  // Add new
        }
        next->add(did);  // Continue
    }
}
```

### Recursive Benefits

✅ **Cleaner code** - Easier to read and understand  
✅ **Less state management** - No current/prev pointers  
✅ **Natural fit** - Matches list structure  
✅ **Easier to prove correct** - Base case + recursive case  

---

## 6. Integration with Trie

### How Trie and Listnode Work Together

**Trie Structure:**
```
        h
        ↓
        e
        ↓
        l
        ↓
        l
        ↓
        o  [listnode*]
           ↓
           [id:1, times:2] → [id:3, times:1] → NULL
```

**Concept:**
- Each complete word in Trie has a listnode chain
- listnode stores which documents contain that word
- Enables fast lookup: "Which docs have 'hello'?"

### Complete Example

```
Insert "hello" in Document 1:
1. Trie insert: h→e→l→l→o
2. At 'o': list = NULL
3. Create: list = new listnode(1)
4. Result: list = [id:1, times:0]

Insert "hello" again in Document 1:
1. Trie insert: h→e→l→l→o (path exists)
2. At 'o': list exists
3. Call: list->add(1)
4. Result: list = [id:1, times:1]

Insert "hello" in Document 5:
1. Trie insert: h→e→l→l→o (path exists)
2. At 'o': list exists
3. Call: list->add(5)
4. Result: list = [id:1, times:1] → [id:5, times:0]
```

### Data Flow

```
split() function tokenizes text:
  "hello world"
   ↓      ↓
  token  token

For each token:
  trie->insert(token, documentID)
    ↓
  Navigate to word end in Trie
    ↓
  Call list->add(documentID)
    ↓
  Update or create listnode
```

---

## 7. Term Frequency Tracking

### What is Term Frequency (TF)?

**Definition:** How many times a word appears in a specific document.

### Why Track TF?

**1. Relevance Scoring**
- Document with "hello" 5 times is MORE relevant than document with "hello" once
- BM25 algorithm uses TF for scoring

**2. Document Statistics**
```cpp
// Query: How many times does "hello" appear in document 3?
int frequency = trie->tfsearchword(3, "hello");
// Returns: number from listnode
```

### How Listnode Tracks TF

**Initial State:**
```cpp
listnode(int did) : id(did), times(0) {
    next = NULL;
}
```
- New node created with `times = 0`
- Represents first occurrence (will be incremented)

**Incrementing:**
```cpp
void add(int did) {
    if(did == id) {
        times++;  // Same document - increment count
    }
    // ...
}
```

**Example Timeline:**
```
Document 1 text: "hello world hello earth hello"

Insert "hello", doc 1:
  list = [id:1, times:0]

Insert "hello", doc 1 again:
  list = [id:1, times:1]  ← incremented

Insert "hello", doc 1 third time:
  list = [id:1, times:2]  ← incremented again

Final: Document 1 has "hello" 3 times ✅
```

---

## 8. Future Usage in Search

### Search Operations (Not Yet Implemented)

**1. Term Frequency Search**
```cpp
// How many times does "hello" appear in document 5?
int count = listnode->search(5);
```

**Implementation:**
```cpp
int search(int did) {
    if(did == id) {
        return times;  // Found it
    } else {
        if(next == NULL) {
            return 0;  // Not found
        }
        return next->search(did);  // Keep looking
    }
}
```

**2. Document Frequency Count**
```cpp
// How many documents contain "hello"?
int docCount = listnode->volume();
```

**Implementation:**
```cpp
int volume() {
    if(next != NULL) {
        return 1 + next->volume();  // Count this + rest
    } else {
        return 1;  // Last node
    }
}
```

**3. Pass Documents to Scorelist**
```cpp
// Get all documents containing "hello" for ranking
listnode->passdocuments(scorelist);
```

**Implementation:**
```cpp
void passdocuments(Scorelist* scorelist) {
    scorelist->insert(id);  // Add this document
    if(next != NULL) {
        next->passdocuments(scorelist);  // Add rest
    }
}
```

### BM25 Algorithm Usage

**Full Search Flow:**

```
User Query: "hello world"

1. Tokenize: ["hello", "world"]

2. For "hello":
   - Navigate Trie to h→e→l→l→o
   - Get listnode chain
   - Extract documents: [1, 3, 5]
   - Get TF for each: [2, 1, 4]

3. For "world":
   - Navigate Trie to w→o→r→l→d
   - Get listnode chain
   - Extract documents: [1, 5, 7]
   - Get TF for each: [1, 2, 1]

4. Combine results:
   - Documents with both words: [1, 5]
   - Calculate BM25 scores using TF values
   - Rank by score

5. Return top K results
```

### Why listnode is Critical

✅ **Fast TF lookup** - O(n) where n = documents with word  
✅ **Fast DF calculation** - Count nodes in list  
✅ **Memory efficient** - Only store what's needed  
✅ **Simple integration** - Works naturally with Trie  
✅ **BM25 ready** - Provides all necessary statistics  

---

## Summary

### Key Concepts

1. **Listnode** - Linked list node for document tracking
2. **Dynamic Size** - Grows as documents added
3. **TF Tracking** - Counts word occurrences per document
4. **Trie Integration** - Attached at word endpoints
5. **Recursive Operations** - Clean, elegant implementation
6. **BM25 Foundation** - Provides TF/DF statistics

### Why Listnode?

✅ **Flexible** - Unknown number of documents  
✅ **Efficient** - Only allocates what's needed  
✅ **Simple** - Clean recursive implementation  
✅ **Powerful** - Enables advanced search features  
✅ **Standard** - Common pattern in search engines  

### Current Status

✅ **Complete Implementation** - Class fully coded  
✅ **Methods Working** - add(), search(), volume()  
⏳ **Not Yet Integrated** - Commented out in Trie  
⏳ **Not Yet Used** - Waiting for search features  

### Future Integration

When search is implemented:
```cpp
// In Trie.hpp:
listnode* list;  // ✅ Uncomment

// In Trie insert():
if(strlen(token)==1){
    if(list==NULL)
        list=new listnode(id);
    list->add(id);  // ✅ Use listnode
}

// Search operations:
int tf = trie->tfsearchword(docId, "hello");
int df = trie->dsearchword("hello");
```

---

**Document Version**: 1.0  
**Last Updated**: December 26, 2025  
**Author**: High-Performance Search Engine Project  
**Repository**: github.com/adarshpheonix2810/high-performance-search-engine-cpp
