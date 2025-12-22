# 🚀 High-Performance Search Engine in C++

A **from-scratch implementation of a high-performance search engine in C++**, inspired by real-world systems like Google and Elasticsearch.  
This project demonstrates **core systems + backend engineering concepts** such as indexing, ranking, concurrency, caching, and networking.

---

## 📌 Overview

This project builds a **mini search engine** that can:

- Load documents from disk (recursively)
- Crawl web pages and store them locally
- Tokenize and parse text (including HTML)
- Build a **positional inverted index**
- Rank documents using **BM25**
- Support **phrase search**
- Provide **autocomplete suggestions**
- Cache frequent queries using **LRU cache**
- Expose search via a **REST-style HTTP API**
- Run efficiently using **multithreading**

The goal of this project is to understand **how real search engines work internally** while writing clean, modular, and high-performance C++ code.

---

## ✨ Features

### 🔍 Search & Indexing
- Inverted index with positional postings
- Fast keyword search
- Phrase search using token positions
- BM25 and TF-IDF ranking models
- Snippet generation from documents

### ⚡ Performance
- Multithreaded document indexing
- Efficient memory usage
- Binary index save/load
- Optional memory-mapped index loading

### 🧠 Intelligence
- Autocomplete using Trie data structure
- LRU cache for repeated queries

---
