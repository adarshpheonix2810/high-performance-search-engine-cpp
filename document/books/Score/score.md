# Score (Scorelist) - Conceptual Documentation

This document explains **WHAT** the Scorelist class is, **WHY** we use it, and **ALTERNATIVES**. For implementation details, see `working.md`.

---

## Table of Contents

1. [What is Scorelist?](#1-what-is-scorelist)
2. [Why Do We Need Scorelist?](#2-why-do-we-need-scorelist)
3. [Data Structure Choices](#3-data-structure-choices)
4. [Memory Management Strategies](#4-memory-management-strategies)
5. [Algorithm Analysis](#5-algorithm-analysis)
6. [Design Decisions Q&A](#6-design-decisions-qa)

---

## 1. What is Scorelist?

### Simple Definition

**Scorelist is a singly-linked list that stores unique document IDs.**

### Purpose

In the search engine, when a user searches for "search engine optimization":
1. Find all documents containing "search"
2. Find all documents containing "engine"  
3. Find all documents containing "optimization"
4. **Merge these lists** → Get unique documents

**Scorelist does step 4:** Maintains a list of unique documents that match ANY query term.

### Structure

```
Scorelist Node:
┌──────────────┐
│ id: int      │  ← Document ID
│ next: ptr    │  ← Pointer to next node
└──────────────┘

Example list:
[5] → [12] → [8] → [15] → NULL
```

---

## 2. Why Do We Need Scorelist?

### Problem: Document Tracking in BM25

**Scenario:**
```
Query: "machine learning"
Term "machine": Found in docs [1, 5, 8, 12]
Term "learning": Found in docs [5, 8, 15, 20]

Need to score: [1, 5, 8, 12, 15, 20]
            (all UNIQUE documents)
```

**Without Scorelist:**
```cpp
// Process "machine" matches
for each doc in [1, 5, 8, 12]:
    calculate_score(doc)

// Process "learning" matches  
for each doc in [5, 8, 15, 20]:
    calculate_score(doc)  // ❌ Recalculates 5 and 8!
```

**With Scorelist:**
```cpp
Scorelist* docs = new Scorelist();
// Insert from "machine"
docs->insert(1); docs->insert(5); docs->insert(8); docs->insert(12);
// Insert from "learning"
docs->insert(5);  // Ignored (duplicate)
docs->insert(8);  // Ignored (duplicate)
docs->insert(15); docs->insert(20);

// Now iterate once through unique docs
Result: [1, 5, 8, 12, 15, 20] ✅ No duplicates
```

### Key Benefits

1. **Uniqueness**: Automatically prevents duplicate document IDs
2. **Dynamic Growth**: No fixed size limit
3. **Simple Interface**: insert(), get_id(), get_next()
4. **Memory Efficiency**: Only allocates what's needed

---

## 3. Data Structure Choices

### Q: Why Linked List Instead of Array?

**Arrays:**
```cpp
int docIds[1000];  // Fixed size
int count = 0;

// Insert
docIds[count++] = 5;
docIds[count++] = 12;
```

**Advantages:**
✅ Fast random access: O(1)
✅ Cache-friendly (contiguous memory)
✅ Simple indexing

**Disadvantages:**
❌ Fixed size (must pre-allocate)
❌ Wasted memory if underused
❌ Overflow if exceeds capacity
❌ Need manual duplicate checking

**Linked List:**
```cpp
Scorelist* list = new Scorelist();
list->insert(5);
list->insert(12);
```

**Advantages:**
✅ Dynamic size (grows as needed)
✅ No wasted memory
✅ Never overflows
✅ Built-in duplicate checking

**Disadvantages:**
❌ Slower random access: O(n)
❌ Extra memory for pointers
❌ Not cache-friendly

### Q: Why Not Use STL std::set?

**std::set (C++ Standard Library):**
```cpp
#include <set>
std::set<int> docIds;
docIds.insert(5);   // Auto-sorted, unique
```

**Advantages:**
✅ Automatic uniqueness
✅ Sorted order
✅ O(log n) insertion/search
✅ Well-tested implementation

**Why we DON'T use it:**
❌ Project constraint: **No STL allowed**
❌ Goal: Build from scratch for learning
❌ std::set uses red-black tree (complex)
❌ Overkill for simple list storage

### Q: Why Not Hash Table?

**Hash Table:**
```cpp
bool seen[MAX_DOCS] = {false};
seen[5] = true;
seen[12] = true;
```

**Advantages:**
✅ O(1) insertion and lookup
✅ Very fast duplicate checking

**Disadvantages:**
❌ Requires MAX_DOCS size known in advance
❌ Wastes memory (sparse array)
❌ More complex implementation
❌ Need to iterate to find all documents

**Verdict:** Linked list is simpler and sufficient for this use case.

---

## 4. Memory Management Strategies

### Q: Why Recursive Destructor?

**Our Implementation:**
```cpp
~Scorelist(){
    if(next!=NULL)
        delete(next);  // Recursive deletion
}
```

**How it works:**
```
delete head;
  → Destructor of head
    → delete next (node 2)
      → Destructor of node 2
        → delete next (node 3)
          → Destructor of node 3
            → next==NULL, stop
```

**Advantages:**
✅ Simple code (3 lines!)
✅ Automatically frees entire chain
✅ Can't forget to free nodes

**Disadvantages:**
❌ Stack overflow if list is VERY long (>1000 nodes)
❌ Not tail-recursive (can't be optimized)

### Alternative: Iterative Destructor

```cpp
~Scorelist(){
    Scorelist* current = next;
    while(current != NULL){
        Scorelist* temp = current->next;
        delete current;
        current = temp;
    }
}
```

**Advantages:**
✅ No stack overflow risk
✅ Works for any list length

**Disadvantages:**
❌ More complex code
❌ Easy to introduce bugs

**Our choice:** Recursive is fine for typical search results (<100 documents).

### Q: Why new/delete Instead of malloc/free?

**new/delete (C++):**
```cpp
Scorelist* node = new Scorelist(5);
delete node;  // Calls destructor automatically
```

**malloc/free (C):**
```cpp
Scorelist* node = (Scorelist*)malloc(sizeof(Scorelist));
// Must manually initialize!
node->id = 5;
node->next = NULL;
free(node);  // Does NOT call destructor
```

**Why new/delete:**
✅ Calls constructor automatically
✅ Calls destructor automatically
✅ Type-safe (no casting needed)
✅ Proper for C++ classes

**When malloc/free used:**
- Maxheap.cpp uses malloc for raw arrays (legacy C code)
- Not recommended for C++ objects

---

## 5. Algorithm Analysis

### Time Complexity

**insert(int docId):**
- Best case: O(1) - insert at head
- Average case: O(n) - traverse entire list
- Worst case: O(n) - duplicate at end

**get_next():**
- O(1) - direct pointer access

**get_id():**
- O(1) - direct member access

**Traversal (entire list):**
- O(n) - must visit each node

### Space Complexity

**Per node:**
- `int id`: 4 bytes
- `Scorelist* next`: 8 bytes (64-bit pointer)
- Total: 12 bytes + padding ≈ 16 bytes

**Total for n documents:**
- O(n) space
- Example: 100 documents = ~1.6 KB

### Comparison with Alternatives

| Data Structure | Insert Time | Search Time | Space | Ordered? |
|----------------|-------------|-------------|-------|----------|
| **Linked List** | O(n) | O(n) | O(n) | No |
| Array | O(1) | O(n) | O(capacity) | No |
| Sorted Array | O(n) | O(log n) | O(capacity) | Yes |
| Hash Table | O(1) | O(1) | O(capacity) | No |
| std::set | O(log n) | O(log n) | O(n) | Yes |

**Our choice justified:** Simple insertion, no search needed, order irrelevant.

---

## 6. Design Decisions Q&A

### Q: Why Allow id=-1 (Empty Nodes)?

**Answer:** Head node initialization convenience.

```cpp
// Can create empty head
Scorelist* list = new Scorelist();  // id=-1
list->insert(5);  // Fills head instead of allocating new node
```

**Alternative:** Always allocate new node for first insertion
```cpp
Scorelist* list = NULL;
list = new Scorelist(5);  // More allocations
```

**Our approach:** Saves one allocation if head can be reused.

### Q: Why No Remove Function?

**Answer:** Not needed for BM25 scoring workflow.

**Workflow:**
1. Create scorelist
2. Insert all matching documents
3. Iterate once through list
4. Delete entire list

**Never need to remove individual documents** → No remove() method.

### Q: Why Inline Getters?

```cpp
int get_id(){return id;};  // Inline in header
```

**Advantages:**
✅ Faster (no function call overhead)
✅ Simple one-liners
✅ Compiler can optimize better

**Used for:** Frequently called, trivial functions.

### Q: Why Default Parameter docId=-1?

```cpp
Scorelist(int docId=-1)
```

**Allows two creation styles:**
```cpp
Scorelist* empty = new Scorelist();      // id=-1
Scorelist* filled = new Scorelist(5);    // id=5
```

**Convenience:** Can create empty head without specifying -1 explicitly.

### Q: Why Store id as int Instead of unsigned int?

**Answer:** Need -1 as sentinel value.

**signed int:**
- Range: -2,147,483,648 to 2,147,483,647
- Can use -1 for "empty"

**unsigned int:**
- Range: 0 to 4,294,967,295
- Cannot represent -1 naturally
- Would need 0xFFFFFFFF as sentinel (confusing)

**Trade-off:** Lose half the positive range, gain clearer sentinel.

### Q: Why No Error Handling in insert()?

```cpp
void insert(int docId){
    // No checks for negative docId, memory allocation failure, etc.
}
```

**Answer:** Assumptions in project:
1. Valid document IDs always non-negative
2. Memory always available (system will crash if not)
3. Simplicity over robustness (educational project)

**Production system would add:**
```cpp
void insert(int docId){
    if(docId < 0) throw std::invalid_argument("Invalid docId");
    try {
        next = new Scorelist(docId);
    } catch(std::bad_alloc& e) {
        // Handle out of memory
    }
}
```

---

## Summary

**Scorelist is:**
- A **singly-linked list** storing **unique document IDs**
- Used in **BM25 ranking** to track documents matching query terms
- **Simple implementation** (no STL) for educational purposes
- **Dynamic growth** with automatic duplicate prevention
- **Recursive deletion** for easy memory management

**Key Design Principles:**
1. **Simplicity** over performance (O(n) insert acceptable)
2. **No STL** constraint (custom implementation)
3. **Memory efficiency** (only allocate what's needed)
4. **Clear interface** (3 methods: insert, get_id, get_next)

**Best For:**
- Small to medium lists (<1000 documents)
- Insert-heavy workloads with single traversal
- Educational C++ projects
- When simplicity matters more than speed

---

**Document Version**: 1.0  
**Created**: January 2, 2026  
**Purpose**: Explain conceptual design of Scorelist class  
**Author**: High-Performance Search Engine Project
