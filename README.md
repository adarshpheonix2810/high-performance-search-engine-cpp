<div align="center">

# 🚀 High-Performance Search Engine in C++

![Search Engine Architecture](./document/pic/Searchengine.png)

[![C++](https://img.shields.io/badge/C++-11-blue.svg)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.10+-green.svg)](https://cmake.org/)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Contributions Welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg)](CONTRIBUTING.md)

**A from-scratch implementation of a high-performance search engine in C++**

Building a production-grade search system inspired by Google and Elasticsearch

[Features](#-features) • [Getting Started](#-getting-started) • [Documentation](#-documentation) • [Architecture](#-architecture) • [Contributing](#-contributing)

</div>

---

## 📌 About The Project

This project is a **complete search engine implementation** built entirely from scratch using modern C++ (C++11). It demonstrates core systems engineering concepts including:

🔍 **Indexing & Retrieval** - Inverted index with positional postings  
📊 **Ranking Algorithms** - BM25 scoring for relevance  
🌲 **Data Structures** - Custom Trie, HashMap, Linked Lists  
⚡ **Performance** - Optimized memory management and fast lookups  
📝 **Text Processing** - Tokenization and document parsing  

### Why This Project?

Understanding how search engines work is fundamental to backend engineering. This project breaks down the complexity of search systems into understandable, well-documented components - perfect for learning systems design and C++ programming.

---

## ✨ Features

### 🔍 Core Search Capabilities
- ✅ **Full-Text Search** - Complete /search command with BM25 ranking 🎉 (Jan 2)
- ✅ **BM25 Ranking** - Industry-standard relevance scoring algorithm (k1=1.2, b=0.75) 🚀 (Jan 2)
- ✅ **Inverted Index** - Fast document lookup with positional postings
- ✅ **Custom Data Structures** - Hand-built Map, Trie, Heap, and Linked List implementations
- ✅ **Document Processing** - Efficient tokenization and text parsing
- ✅ **Term Frequency Tracking** - Accurate word occurrence counting per document
- ✅ **Interactive Query System** - Command-line interface with /search, /tf, /df, /exit
- ✅ **Query Commands** - Real-time term/document frequency analysis
- ✅ **Working /tf Command** - Get word count in specific documents 🎯 (Dec 31)
- ✅ **Working /df Command** - Count documents containing words 🎉 (Jan 1)
- ✅ **Vocabulary Display** - View all indexed words with /df 🚀 (Jan 2)

### ⚡ Performance Features
- 🚀 **Optimized Memory Management** - Manual memory control with no STL overhead
- 🚀 **Fast Lookups** - O(m) Trie operations for word search
- 🚀 **Efficient Storage** - Dynamic data structures that scale with content
- 🚀 **strlen() Optimization** - Called once, not in loops 🎯 (Dec 31)
- 🚀 **Linear Complexity** - O(n²) → O(n) for TF search 🎯 (Dec 31)
- 🚀 **BM25 Optimization** - 50% performance gain by caching TF calculations 🎉 (Jan 2)
- 🚀 **Max Heap Ranking** - O(n log k) top-k document retrieval 🎉 (Jan 2)
- 🚀 **No Memory Leaks** - All dynamically allocated memory properly freed ✅ (Jan 2)

### 🧠 Advanced Features (Planned)
- 🔄 Phrase search using token positions
- 🔄 Autocomplete suggestions
- 🔄 Query caching with LRU
- 🔄 Multithreaded indexing
- 🔄 REST API integration
- 🔄 Advanced BM25+ ranking with delta parameter
- 🔄 Fuzzy matching and spell correction

---

## 🚀 Getting Started

### Prerequisites

```bash
# Required tools
- C++ Compiler (GCC/MinGW with C++11 support)
- CMake 3.10 or higher
- Git
```

### Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/adarshpheonix2810/high-performance-search-engine-cpp.git
   cd high-performance-search-engine-cpp
   ```

2. **Build the project**
   ```bash
   mkdir build
   cd build
   cmake .. -G "MinGW Makefiles"
   or
   cmake -G "MinGW Makefiles" ..
   cmake --build .
   ```

3. **Run the search engine**
   ```bash
   .\searchengine.exe -d ..\data\doc1.txt -k 5
   ```

### Quick Start Example

```bash
# Run the search engine
.\searchengine.exe -d ..\data\doc1.txt -d ..\data\doc2.txt -k 5

# Try these commands:
Enter query: /search machine learning    # Find relevant documents
Enter query: /tf 1 hello                 # Word count in doc 1
Enter query: /df algorithm               # How many docs have this word
Enter query: /exit                       # Exit program
```

---

## 📚 Documentation

Comprehensive documentation is available in the `document/books/` directory:

### Component Documentation
- **[Map](document/books/Map/)** - Dynamic array-based document storage
  - `map.md` - Concepts and theory
  - `working.md` - Implementation details
  
- **[Trie](document/books/Trie/)** - Prefix tree for word storage
  - `trie.md` - Data structure concepts
  - `working.md` - Insert algorithm breakdown
  
- **[Listnode](document/books/Listnode/)** - Linked list for TF tracking
  - `listnode.md` - Term frequency concepts
  - `working.md` - Implementation guide
  
- **[Document Store](document/books/Document_store/)** - File I/O and parsing
  - `document_store.md` - Text processing concepts
  - `working.md` - Function workflows
  
- **[Search](document/books/Search/)** - Query processing module
  - `search.md` - BM25 algorithm, windows.h, TF/DF concepts
  - `working.md` - Complete implementation with ranking
  
- **[Maxheap](document/books/Maxheap/)** - Priority queue for ranking 🎉 (Jan 2)
  - `maxheap.md` - Heap data structure, malloc vs new, alternatives
  - `working.md` - Top-k retrieval implementation
  
- **[Score](document/books/Score/)** - Document tracking list 🎉 (Jan 2)
  - `score.md` - Linked list concepts, memory management
  - `working.md` - Scorelist implementation for BM25
  
- **[Search Engine](document/books/searchengine/)** - Main entry point
  - `searchengine.md` - Architecture overview, input manager
  - `working.md` - Execution flow, interactive loop

Each component has:
- 📖 **Concept files** (`.md`) - Theory, "what is X", "why use Y"
- 🔧 **Working files** (`working.md`) - Code walkthrough, line-by-line explanations

---

## 🏗️ System Architecture & Design

### Complete System Flowchart

```
┌─────────────────────────────────────────────────────────────────────┐
│                         PROGRAM START                                │
│              ./searchengine.exe -d doc1.txt -k 5                    │
└────────────────────────────────┬────────────────────────────────────┘
                                 │
                                 ▼
        ┌────────────────────────────────────────────┐
        │    PHASE 1: INITIALIZATION & INDEXING      │
        └────────────────────────────────────────────┘
                                 │
     ┌───────────────────────────┼───────────────────────────┐
     │                           │                           │
     ▼                           ▼                           ▼
┌──────────┐           ┌────────────┐           ┌────────────┐
│   Map    │           │    Trie    │           │  Document  │
│   Init   │           │    Init    │           │   Store    │
└────┬─────┘           └──────┬─────┘           └──────┬─────┘
     │                        │                         │
     └────────────────────────┼─────────────────────────┘
                              ▼
               ┌──────────────────────────┐
               │   Read Document Files   │
               │   (doc1.txt, doc2.txt)  │
               └────────────┬─────────────┘
                            │
                            ▼
               ┌──────────────────────────┐
               │  Tokenize Text          │
               │  (Split into words)     │
               └────────────┬─────────────┘
                            │
                            ▼
               ┌──────────────────────────────┐
               │  For Each Word:              │
               │  • Insert into Trie         │
               │  • Track doc ID + TF        │
               │  • Create Listnode chain    │
               └────────────┬─────────────────┘
                            │
                            ▼
        ┌────────────────────────────────────────────┐
        │    PHASE 2: INTERACTIVE QUERY LOOP         │
        │    (User enters commands continuously)     │
        └────────────────────────────────────────────┘
                            │
                            ▼
               ┌──────────────────────────┐
               │  Display Prompt:        │
               │  "Enter query: "        │
               └────────────┬─────────────┘
                            │
                            ▼
               ┌──────────────────────────┐
               │  getline() - Read Input │
               └────────────┬─────────────┘
                            │
              ┌─────────────┴─────────────┐
              │   Parse Command Type     │
              └───┬─────────┬───────┬────┘
                  │         │       │
      ┌───────────┘         │       └───────────┐
      │                     │                   │
      ▼                     ▼                   ▼
┌──────────┐         ┌──────────┐      ┌──────────┐
│ /search  │         │   /tf    │      │   /df    │
│  query   │         │ doc word │      │  word    │
└────┬─────┘         └────┬─────┘      └────┬─────┘
     │                    │                   │
     │                    │                   │
     ▼                    ▼                   ▼
┌─────────────────────────────────────────────────────┐
│            PHASE 3: QUERY EXECUTION                 │
└─────────────────────────────────────────────────────┘

═══════════════ /search FLOW ═══════════════

     ┌──────────────────────────┐
     │ Parse Query Words       │
     │ "machine learning"      │
     └────────────┬─────────────┘
                  │
     ┌────────────▼──────────────┐
     │ For Each Word:           │
     │ • dfsearchword()        │
     │ • Get Listnode          │
     └────────────┬──────────────┘
                  │
     ┌────────────▼────────────────┐
     │ Create Scorelist           │
     │ (Track unique docs)        │
     └────────────┬────────────────┘
                  │
     ┌────────────▼────────────────┐
     │ For Each Document:         │
     │ Calculate BM25 Score       │
     │ • Compute IDF              │
     │ • Get TF via tfsearch()    │
     │ • Apply formula            │
     └────────────┬────────────────┘
                  │
     ┌────────────▼────────────────┐
     │ Insert into Maxheap        │
     │ (Keep top-k results)       │
     └────────────┬────────────────┘
                  │
     ┌────────────▼────────────────┐
     │ Extract from heap          │
     │ Display ranked docs        │
     └────────────────────────────┘

═══════════════ /tf FLOW ═══════════════════

     ┌──────────────────────────┐
     │ Parse: doc_id + word    │
     │ Example: /tf 1 hello    │
     └────────────┬─────────────┘
                  │
     ┌────────────▼────────────────┐
     │ tfsearchword(docId)        │
     │ • Search Trie for word     │
     │ • Find Listnode chain      │
     │ • Match docId              │
     │ • Return times count       │
     └────────────┬────────────────┘
                  │
     ┌────────────▼────────────────┐
     │ Display: TF = 3            │
     └────────────────────────────┘

═══════════════ /df FLOW ═══════════════════

     ┌──────────────────────────┐
     │ Parse: word             │
     │ Example: /df hello      │
     └────────────┬─────────────┘
                  │
     ┌────────────▼────────────────┐
     │ dfsearchword()             │
     │ • Search Trie for word     │
     │ • Get Listnode head        │
     │ • Call volume()            │
     │ • Count nodes in chain     │
     └────────────┬────────────────┘
                  │
     ┌────────────▼────────────────┐
     │ Display: DF = 5 docs       │
     └────────────────────────────┘

                  ▼
     ┌──────────────────────────┐
     │ Loop back to prompt     │
     │ (unless /exit typed)    │
     └──────────────────────────┘

═════════════════════════════════════════

        ┌────────────────────────────────────────────┐
        │         PHASE 4: CLEANUP & EXIT            │
        │    (When user types /exit)                 │
        └────────────────────────────────────────────┘
                            │
               ┌────────────▼──────────┐
               │ Delete Trie         │
               │ Delete Map          │
               │ Free all memory     │
               └────────────┬──────────┘
                            │
                            ▼
               ┌──────────────────────────┐
               │   PROGRAM END           │
               └──────────────────────────┘
```

### Data Structures Overview

```
┌───────────────────────────────────────────────────────┐
│                DATA STRUCTURE LAYER                   │
└───────────────────────────────────────────────────────┘

┌─────────────┐     ┌─────────────┐     ┌─────────────┐
│     MAP     │     │     TRIE    │     │  LISTNODE   │
│             │     │             │     │             │
│ docs[100]   │────▶│ root node   │────▶│ id: 5       │
│ count: 10   │     │ children[26]│     │ times: 3    │
│             │     │             │     │ next───────▶│
└─────────────┘     └─────────────┘     └─────────────┘
      │                    │                     │
      ▼                    ▼                     ▼
  Document            Word Index           TF Tracking
  Storage            (Prefix Tree)       (Per Document)

┌─────────────┐     ┌─────────────┐
│   MAXHEAP   │     │  SCORELIST  │
│             │     │             │
│ heap[k]     │     │ id: 5───────┼────▶ id: 12───────▶
│ ids[k]      │     │             │     │             │
│ count: 5    │     │             │     │             │
└─────────────┘     └─────────────┘
      │                    │
      ▼                    ▼
   Top-K Results    Unique Document IDs
   (BM25 Ranked)    (For scoring)
```

### Project Structure
```
high-performance-search-engine-cpp/
├── header/               # Header files (.hpp)
│   ├── Map.hpp          # Document storage
│   ├── Trie.hpp         # Word indexing
│   ├── Listnode.hpp     # TF/DF tracking
│   ├── Maxheap.hpp      # Top-k ranking (Jan 2)
│   ├── Score.hpp        # Document list (Jan 2)
│   ├── Search.hpp       # Query processing
│   └── searchengine.hpp # Main orchestrator
├── src/                 # Implementation files (.cpp)
├── data/                # Sample documents
├── document/            # Comprehensive documentation
│   ├── books/
│   │   ├── Map/
│   │   ├── Trie/
│   │   ├── Listnode/
│   │   ├── Maxheap/     # NEW (Jan 2)
│   │   ├── Score/       # NEW (Jan 2)
│   │   ├── Search/
│   │   └── searchengine/
│   └── pic/
└── CMakeLists.txt       # Build configuration
```

---

## 🛠️ Technologies Used

- **Language**: C++11
- **Build System**: CMake
- **Data Structures**: Custom implementations (no STL)
- **Algorithms**: BM25, Trie insertion, dynamic arrays
- **Memory Management**: Manual allocation/deallocation

---

## 📈 Project Status

### ✅ Completed

**Core Features:**
- [x] Complete BM25 search engine with ranking
- [x] All query commands working (/search, /tf, /df)
- [x] Custom data structures (Map, Trie, Heap, Listnode, Scorelist)
- [x] Interactive command-line interface
- [x] Document indexing and text processing

**Recent Updates (Jan 2, 2026):**
- [x] Full /search with BM25 ranking (k1=1.2, b=0.75)
- [x] Maxheap for O(n log k) top-k retrieval
- [x] Fixed critical bugs (constructor, infinite loop, memory leaks)
- [x] 50% BM25 performance optimization
- [x] Comprehensive documentation for all modules
- [x] Input validation and robust error handling

### 🔄 In Progress
- [ ] Additional test cases and edge case handling
- [ ] Performance benchmarking and profiling

### 📋 Planned Features
- [ ] Phrase search
- [ ] Autocomplete
- [ ] Query caching
- [ ] Multithreading
- [ ] Web crawler
- [ ] REST API
- [ ] Performance benchmarks

---

## 🤝 Contributing

Contributions are what make the open source community amazing! Any contributions you make are **greatly appreciated**.

### How to Contribute

1. **Fork the Project**
2. **Create your Feature Branch** (`git checkout -b feature/AmazingFeature`)
3. **Commit your Changes** (`git commit -m 'Add some AmazingFeature'`)
4. **Push to the Branch** (`git push origin feature/AmazingFeature`)
5. **Open a Pull Request**

### Areas for Contribution
- 🐛 Bug fixes and code optimization
- 📝 Documentation improvements
- ✨ New feature implementations
- 🧪 Test cases and benchmarks
- 🎨 Code refactoring

---

## 📝 License

Distributed under the MIT License. See `LICENSE` file for more information.

---

## 👨‍💻 Author

**Adarsh Kumar**
- GitHub: [@adarshpheonix2810](https://github.com/adarshpheonix2810)
- LinkedIn: [Connect with me](https://www.linkedin.com/in/adarsh-singh-412298273/)

---

## 🙏 Acknowledgments

- Inspired by real-world search systems (Google, Elasticsearch)
- Built as a learning project for systems programming
- Thanks to the open-source community for inspiration

---

## 📊 Project Stats

![GitHub Stars](https://img.shields.io/github/stars/adarshpheonix2810/high-performance-search-engine-cpp?style=social)
![GitHub Forks](https://img.shields.io/github/forks/adarshpheonix2810/high-performance-search-engine-cpp?style=social)
![GitHub Issues](https://img.shields.io/github/issues/adarshpheonix2810/high-performance-search-engine-cpp)
![GitHub Pull Requests](https://img.shields.io/github/issues-pr/adarshpheonix2810/high-performance-search-engine-cpp)

---

<div align="center">

### ⭐ Star this repo if you find it helpful!

**Built with ❤️ and C++**

</div>
