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
- ✅ **Inverted Index** - Fast document lookup with positional postings
- ✅ **BM25 Ranking** - Industry-standard relevance scoring algorithm (in progress)
- ✅ **Custom Data Structures** - Hand-built Map, Trie, and Linked List implementations
- ✅ **Document Processing** - Efficient tokenization and text parsing
- ✅ **Term Frequency Tracking** - Accurate word occurrence counting per document
- ✅ **Interactive Query System** - Command-line interface with /search, /tf, /df, /exit ✨ (New!)
- ✅ **Query Commands** - Real-time term/document frequency analysis ✨ (New!)
- ✅ **Working /tf Command** - Get word count in specific documents 🎯 (Dec 31)

### ⚡ Performance Features
- 🚀 **Optimized Memory Management** - Manual memory control with no STL overhead
- 🚀 **Fast Lookups** - O(m) Trie operations for word search
- 🚀 **Efficient Storage** - Dynamic data structures that scale with content
- 🚀 **strlen() Optimization** - Called once, not in loops 🎯 (Dec 31)
- 🚀 **Linear Complexity** - O(n²) → O(n) for TF search 🎯 (Dec 31)

### 🧠 Advanced Features (Planned)
- 🔄 Phrase search using token positions
- 🔄 Autocomplete suggestions
- 🔄 Query caching with LRU
- 🔄 Multithreaded indexing
- 🔄 REST API integration

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

### Usage Examples

#### Interactive Query Mode (New! December 28, 2025)

The search engine now supports an **interactive command-line interface** for running queries:

```bash
# Start the search engine
.\searchengine.exe -d ..\data\doc1.txt -k 5

# You'll see a prompt:
Enter query (or type '/exit' to quit): 

# Available commands:
/search <query>     # Search for documents containing words
/tf <doc_id> <word> # Get term frequency of word in document
/df <word>          # Get document frequency of word
/exit               # Exit the program
```

**Example Session:**
```bash
$ .\searchengine.exe -d ..\data\doc1.txt -d ..\data\doc2.txt -k 5

Enter query (or type '/exit' to quit): /search hello world
[Search results display here]

Enter query (or type '/exit' to quit): /tf 1 hello
Term frequency of 'hello' in document 1: 3

Enter query (or type '/exit' to quit): /df hello
Document frequency of 'hello': 2 documents

Enter query (or type '/exit' to quit): /exit
Exiting program...
```

#### Command-Line Arguments

```bash
# Search a single document
.\searchengine.exe -d ..\data\doc1.txt -k 5

# Search multiple documents
.\searchengine.exe -d ..\data\doc1.txt -d ..\data\doc2.txt -k 10

# Adjust number of results
.\searchengine.exe -d ..\data\doc3.txt -k 3
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
  
- **[Search](document/books/Search/)** - Query processing module (**New!**)
  - `search.md` - TF/DF concepts, strtok, isdigit, atoi
  - `working.md` - Query parsing, memory safety, December 28 fixes
  
- **[Search Engine](document/books/searchengine/)** - Main entry point
  - `searchengine.md` - Architecture overview, input manager
  - `working.md` - Execution flow, interactive loop

Each component has:
- 📖 **Concept files** (`.md`) - Theory, "what is X", "why use Y"
- 🔧 **Working files** (`working.md`) - Code walkthrough, line-by-line explanations

---

## 🏗️ Architecture

### Project Structure
```
high-performance-search-engine-cpp/
├── header/               # Header files (.hpp)
│   ├── Map.hpp
│   ├── Trie.hpp
│   ├── Listnode.hpp
│   ├── Document_store.hpp
│   ├── Search.hpp       # ✨ New! Query processing
│   └── searchengine.hpp
├── src/                  # Implementation files (.cpp)
│   ├── Map.cpp
│   ├── Trie.cpp
│   ├── Listnode.cpp
│   ├── Document_store.cpp
│   ├── Search.cpp       # ✨ New! TF/DF/search commands
│   └── Searchengine.cpp
├── data/                 # Sample documents
├── document/             # Documentation
│   ├── books/           # Component docs
│   │   ├── Map/
│   │   ├── Trie/
│   │   ├── Listnode/
│   │   ├── Document_store/
│   │   ├── Search/      # ✨ New! Query docs
│   │   └── searchengine/
│   └── pic/             # Images and diagrams
├── CMakeLists.txt       # Build configuration
└── README.md
```

### Core Components

```
┌──────────────────────────────────────────────────────┐
│           Search Engine Entry Point                  │
│   (CLI parsing, input manager, orchestration)        │
└───────────────────┬──────────────────────────────────┘
                    │
                    ▼
         ┌──────────────────────┐
         │  Interactive Loop     │ ✨ New Dec 28!
         │  (getline, commands) │
         └──────────┬────────────┘
                    │
                    ▼
         ┌──────────────────────┐
         │   Search Module      │ ✨ New Dec 28!
         │  (/search /tf /df)   │
         └──────────┬────────────┘
                    │
         ┌──────────┴──────────┐
         │                     │
         ▼                     ▼
   ┌──────────┐          ┌──────────┐
   │   Map    │          │  Trie    │
   │ Document │◄─────────┤  Word    │
   │ Storage  │          │  Index   │
   └──────────┘          └────┬─────┘
         │                    │
         │                    ▼
         │              ┌──────────┐
         │              │ Listnode │
         └──────────────┤   TF/DF  │
                        │ Tracking │
                        └──────────┘
```

### Data Flow

**Indexing Phase:**
1. **Document Loading** → Map stores documents in dynamic array
2. **Text Processing** → Tokenization splits text into words
3. **Indexing** → Trie stores each word with character-by-character navigation
4. **TF Tracking** → Listnode chains track document IDs and frequencies

**Query Phase:** ✨ **New! December 28, 2025**
5. **User Input** → Interactive loop waits for commands (getline)
6. **Command Routing** → Input manager parses and dispatches commands
7. **Query Processing** → Search module executes /search, /tf, or /df
8. **Scoring** → BM25 algorithm ranks documents by relevance
9. **Results** → Top-K documents returned, loop continues until /exit

---

## 🛠️ Technologies Used

- **Language**: C++11
- **Build System**: CMake
- **Data Structures**: Custom implementations (no STL)
- **Algorithms**: BM25, Trie insertion, dynamic arrays
- **Memory Management**: Manual allocation/deallocation

---

## 📈 Project Status

### ✅ Completed (December 2025)
- [x] Map class with dynamic document storage
- [x] Trie data structure with insert functionality
- [x] Listnode for term frequency tracking
- [x] Document store with file I/O
- [x] Text tokenization and parsing
- [x] Basic search infrastructure
- [x] Comprehensive documentation
- [x] CMake build system
- [x] **Interactive query system** ✨ (Dec 28)
- [x] **Search module with /tf, /df commands** ✨ (Dec 28)
- [x] **Command routing with input manager** ✨ (Dec 28)
- [x] **Memory leak fixes** ✨ (Dec 28)
- [x] **Robust error handling** ✨ (Dec 28)
- [x] **Term Frequency (/tf) fully working** 🎯 (Dec 31)
- [x] **Performance optimizations (strlen)** 🎯 (Dec 31)
- [x] **Trie-Listnode integration** 🎯 (Dec 31)

### 🔄 In Progress
- [ ] Document Frequency (/df) implementation
- [ ] Full /search query processing
- [ ] BM25 scoring implementation
- [ ] Result ranking and sorting

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
