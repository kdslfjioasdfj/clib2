# CLib2

## What is CLib2?
**CLib2** is a modern C library designed to fill the gaps in the C standard library.
C is minimal by design, leaving developers to implement common utilities themselves.
**CLib2** provides safe, reusable, and convenient abstractions for things C does not implement natively.

---

## History / Motivation
The name **CLib2** comes from a rework of my previous project **CLib**.
The original version had design and usability issues.
**CLib2** is a complete overhaul — no code is shared with the original — with the same goal: making C development safer and more convenient.

---

## Getting Started

### 1. Clone the repo
```bash
git clone git@github.com:YOUR_USERNAME/clib2.git
cd clib2
```
### 2. Build with CMake
```bash
mkdir build
cd build
cmake .. -G ninja
ninja
```
Here are some notes on this step that might help:
1. You might want to check the `CMakeLists.txt` in the project directory and customize it!
2. The build config does not (yet) support running tests. You'll need to compile and link those separately!
3. The config (by default) already handles for a static AND dynamic library at the same time.

## Contributing
Not open for contributions just yet — it’s just me for now.
When I’m ready, I’ll put up my requirements here!
