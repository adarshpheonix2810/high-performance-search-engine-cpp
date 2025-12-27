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
- ✅ **BM25 Ranking** - Industry-standard relevance scoring algorithm
- ✅ **Custom Data Structures** - Hand-built Map, Trie, and Linked List implementations
- ✅ **Document Processing** - Efficient tokenization and text parsing
- ✅ **Term Frequency Tracking** - Accurate word occurrence counting per document

### ⚡ Performance Features
- 🚀 **Optimized Memory Management** - Manual memory control with no STL overhead
- 🚀 **Fast Lookups** - O(m) Trie operations for word search
- 🚀 **Efficient Storage** - Dynamic data structures that scale with content

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
   cmake -G "MinGW Makefiles" ..
   cmake --build .
   ```

3. **Run the search engine**
   ```bash
   .\searchengine.exe -d ..\data\doc1.txt -k 5
   ```

### Usage Examples

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
  
- **[Search Engine](document/books/searchengine/)** - Main entry point
  - `searchengine.md` - Architecture overview
  - `working.md` - Execution flow

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
│   └── searchengine.hpp
├── src/                  # Implementation files (.cpp)
│   ├── Map.cpp
│   ├── Trie.cpp
│   ├── Listnode.cpp
│   ├── Document_store.cpp
│   └── Searchengine.cpp
├── data/                 # Sample documents
├── document/             # Documentation
│   ├── books/           # Component docs
│   └── pic/             # Images and diagrams
├── CMakeLists.txt       # Build configuration
└── README.md
```

### Core Components

```
┌─────────────────────────────────────────────────┐
│              Search Engine Entry                 │
│         (CLI parsing, orchestration)             │
└────────────────┬────────────────────────────────┘
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

1. **Document Loading** → Map stores documents in dynamic array
2. **Text Processing** → Tokenization splits text into words
3. **Indexing** → Trie stores each word with character-by-character navigation
4. **TF Tracking** → Listnode chains track document IDs and frequencies
5. **Scoring** → BM25 algorithm ranks documents by relevance
6. **Results** → Top-K documents returned to user

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

### 🔄 In Progress
- [ ] BM25 scoring implementation
- [ ] Search query processing
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
- LinkedIn: [Connect with me](https://www.linkedin.com/in/adarsh-kumar-pheonix/)

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