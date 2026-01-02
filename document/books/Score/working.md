# Score (Scorelist) - Working Documentation

This document provides **step-by-step implementation** details for the Scorelist class used in BM25 ranking. For conceptual understanding, see `score.md`.

---

## Table of Contents

1. [Complete Source Code](#1-complete-source-code)
2. [Constructor Implementation](#2-constructor-implementation)
3. [insert() Function Breakdown](#3-insert-function-breakdown)
4. [Getter Methods](#4-getter-methods)
5. [Destructor and Memory Management](#5-destructor-and-memory-management)
6. [Usage in Search Function](#6-usage-in-search-function)
7. [January 2, 2026 Updates](#7-january-2-2026-updates)

---

## 1. Complete Source Code

### Score.hpp (Header File)

```cpp
#include <iostream>
#include <cstdlib>
#ifndef SCORE_HPP
#define SCORE_HPP
using namespace std;
class Scorelist
{
    int id;
    Scorelist *next;
    public:
        Scorelist(int docId=-1): id(docId){next=NULL;};
        ~Scorelist();
        void insert(int docId);
        Scorelist* get_next(){return next;};
        int get_id(){return id;};
};
#endif
```

### Score.cpp (Implementation)

```cpp
#include "Score.hpp"
using namespace std;

Scorelist::~Scorelist()
{
    if(next!=NULL)
        delete(next);
}

void Scorelist::insert(int docId)
{
    if(id==docId){
        return;
    }
    else
    if(id==-1){
        id=docId;
    }
    else{
        if(next==NULL){
            next=new Scorelist(docId);
        }
        else{
            next->insert(docId);
        }
    }
}
```

---

## 2. Constructor Implementation

```cpp
Scorelist(int docId=-1): id(docId){next=NULL;};
```

### Line-by-Line Breakdown

**Constructor with Default Parameter**
```cpp
Scorelist(int docId=-1)
```
- `docId`: Document ID to store (default = -1)
- Default value -1 indicates "empty" or uninitialized node
- Allows creation without parameters: `new Scorelist()`

**Initialization List**
```cpp
: id(docId)
```
- Sets member variable `id` to the passed `docId`
- More efficient than assignment in constructor body

**Constructor Body**
```cpp
{next=NULL;}
```
- Initializes `next` pointer to NULL
- Indicates this is currently the last node in the list

### Usage Examples

```cpp
// Create head node (empty)
Scorelist* head = new Scorelist();
// id = -1, next = NULL

// Create node with document ID
Scorelist* node = new Scorelist(5);
// id = 5, next = NULL
```

---

## 3. insert() Function Breakdown

```cpp
void Scorelist::insert(int docId)
{
    if(id==docId){
        return;
    }
    else
    if(id==-1){
        id=docId;
    }
    else{
        if(next==NULL){
            next=new Scorelist(docId);
        }
        else{
            next->insert(docId);
        }
    }
}
```

### Three Cases Handled

**Case 1: Duplicate Document**
```cpp
if(id==docId){
    return;
}
```
- Document already in list
- Ignore and return (no duplicates allowed)
- Prevents counting same document multiple times

**Example:**
```
List: [5] → [8] → [12]
Insert 8: Already exists, do nothing
Result: [5] → [8] → [12]
```

**Case 2: Empty Node**
```cpp
else if(id==-1){
    id=docId;
}
```
- Current node is empty (id = -1)
- Fill this node with the new document ID
- No need to allocate new node

**Example:**
```
List: [-1] (empty head)
Insert 5:
Result: [5]
```

**Case 3: Add to End**
```cpp
else{
    if(next==NULL){
        next=new Scorelist(docId);
    }
    else{
        next->insert(docId);
    }
}
```

**Sub-case 3a: End of list reached**
```cpp
if(next==NULL){
    next=new Scorelist(docId);
}
```
- Create new node with document ID
- Link current node to new node

**Sub-case 3b: Continue traversal**
```cpp
else{
    next->insert(docId);
}
```
- Recursively call insert on next node
- Continues until empty slot or end found

### Complete Insertion Example

```
Initial: scorelist = [-1] (empty)

Insert 5:
  Current id=-1 → Fill it
  Result: [5]

Insert 12:
  Current id=5 (not -1)
  next=NULL → Create new node
  Result: [5] → [12]

Insert 3:
  At node 5: id≠3, next≠NULL → Recurse
  At node 12: id≠3, next=NULL → Create
  Result: [5] → [12] → [3]

Insert 12 (duplicate):
  At node 5: id≠12 → Continue
  At node 12: id==12 → Return (ignore)
  Result: [5] → [12] → [3] (unchanged)
```

---

## 4. Getter Methods

### get_id()

```cpp
int get_id(){return id;};
```

**Purpose:** Return document ID stored in current node
**Return value:** 
- Valid document ID (0, 1, 2, ...)
- -1 if node is empty

**Usage:**
```cpp
int docId = currentDoc->get_id();
if(docId != -1){
    // Valid document, process it
}
```

### get_next()

```cpp
Scorelist* get_next(){return next;};
```

**Purpose:** Return pointer to next node in list
**Return value:**
- Pointer to next Scorelist node
- NULL if this is the last node

**Usage:**
```cpp
Scorelist* current = scorelist;
while(current != NULL){
    int id = current->get_id();
    // Process document id
    current = current->get_next();  // Move to next
}
```

---

## 5. Destructor and Memory Management

```cpp
Scorelist::~Scorelist()
{
    if(next!=NULL)
        delete(next);
}
```

### Recursive Deletion

**How it works:**
1. Check if there's a next node
2. If yes, delete it
3. Deleting next calls ITS destructor
4. Chain reaction deletes entire list

**Example:**
```
List: [5] → [12] → [3]

delete scorelist;  // Delete head node [5]
  ↓
  Destructor of [5] called
  next != NULL, so delete [12]
    ↓
    Destructor of [12] called
    next != NULL, so delete [3]
      ↓
      Destructor of [3] called
      next == NULL, stop
      
All nodes freed! ✅
```

**Critical:** Only delete the head node, destructor handles rest!

```cpp
// CORRECT
Scorelist* list = new Scorelist();
// ... use list ...
delete list;  // Frees entire chain

// WRONG - Double free!
delete list;
delete list->get_next();  // ❌ Already freed!
```

---

## 6. Usage in Search Function

### Purpose in BM25 Scoring

**Problem:** Need to track which documents contain query terms
**Solution:** Scorelist stores unique document IDs

### Integration Example (from Search.cpp)

```cpp
void search(char *token, TrieNode *trie, Mymap *map, int k) {
    // Create scorelist to track documents
    Scorelist *scorelist = new Scorelist();
    
    // Parse query words
    char *queryWords[MAX_QUERY_WORDS];
    int wordCount = 0;
    // ... tokenization code ...
    
    // For each query word, find documents
    for(int i = 0; i < wordCount; i++){
        listnode *wordDocs = trie->dfsearchword(...);
        
        // Insert all documents containing this word
        while(wordDocs != NULL){
            int docId = wordDocs->get_id();
            scorelist->insert(docId);  // ← Uses Scorelist
            wordDocs = wordDocs->get_next();
        }
    }
    
    // Now scorelist contains ALL unique documents
    // Iterate through to calculate BM25 scores
    Scorelist *currentDoc = scorelist;
    while(currentDoc != NULL){
        int docId = currentDoc->get_id();
        
        if(docId != -1){  // Skip empty nodes
            double score = 0.0;
            // Calculate BM25 score for this document
            // ...
            heap->insert(score, docId);
        }
        
        currentDoc = currentDoc->get_next();
    }
    
    // Cleanup
    delete scorelist;  // Frees entire list
}
```

### Why Use Scorelist?

**Alternative 1: Array**
```cpp
int docIds[1000];  // Fixed size - might overflow!
```
❌ Limited capacity
❌ Wasted memory if few documents

**Alternative 2: Dynamic array**
```cpp
int* docIds = (int*)malloc(size * sizeof(int));
// Need to track size, resize when full
```
❌ Complex resizing logic
❌ Must track capacity and count separately

**Scorelist (Linked List):**
```cpp
Scorelist* list = new Scorelist();
list->insert(5);   // Grows automatically
list->insert(12);  // No size limit
```
✅ Grows dynamically as needed
✅ Simple insertion logic
✅ No wasted memory
✅ Easy traversal

---

## 7. January 2, 2026 Updates

### Parameter Renaming: did → docId

**Changed in:** Score.hpp, Score.cpp

**Before:**
```cpp
// Old parameter name
Scorelist(int did=-1): id(did){next=NULL;};
void insert(int did);
```

**After:**
```cpp
// New parameter name (Jan 2, 2026)
Scorelist(int docId=-1): id(docId){next=NULL;};
void insert(int docId);
```

**Reason for change:**
- `docId` is more descriptive than `did`
- Matches naming convention in other files
- Improves code readability
- "docId" = "document identifier" is self-documenting

**Impact:**
- No functional changes
- Code quality improvement only
- Better maintainability

### Testing Results (Jan 2, 2026)

**Test 1: Single word query**
```
Query: "search"
Scorelist inserts: 5, 12, 8, 3, 15
Result: 5 unique documents tracked ✅
```

**Test 2: Multi-word query**
```
Query: "search engine"
Word "search": docs 5, 12, 8
Word "engine": docs 8, 15, 12
Scorelist final: 5, 12, 8, 15 (4 unique) ✅
Duplicate 8 and 12 ignored correctly ✅
```

**Test 3: Empty result**
```
Query: "xyzabc" (no matches)
Scorelist: [-1] (empty head only)
get_id() returns -1 → Skipped in scoring ✅
```

---

## Complete Memory Flow

```
1. Creation:
   Scorelist* list = new Scorelist();
   Memory: [HEAD: id=-1, next=NULL]

2. First Insert:
   list->insert(5);
   Memory: [HEAD: id=5, next=NULL]

3. Second Insert:
   list->insert(12);
   Memory: [HEAD: id=5, next→] → [NODE: id=12, next=NULL]

4. Third Insert:
   list->insert(3);
   Memory: [HEAD: id=5, next→] → [NODE: id=12, next→] → [NODE: id=3, next=NULL]

5. Traversal:
   current = list;
   while(current != NULL){
       process(current->get_id());
       current = current->get_next();
   }
   
6. Cleanup:
   delete list;
   Memory: All nodes freed recursively ✅
```

---

**Document Version**: 1.0  
**Created**: January 2, 2026  
**Purpose**: Document Scorelist implementation for document tracking in BM25 ranking  
**Updates**: Parameter renamed did→docId for better code quality  
**Author**: High-Performance Search Engine Project  
**Status**: Fully operational ✅
