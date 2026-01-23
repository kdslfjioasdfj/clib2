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

## Design Choices
I made a few important design choices while making **CLib2**.
Here are the most important ones:
1. *Cross-Platform support*: I try my best to avoid platform-specific code. I also support embedded devices (by using as little memory as possible)!
2. *Simplicity*: I try my best to keep APIs simple, consistent, readable, and most importantly, understandable.
3. *Deterministic*: I try to keep the library as deterministic as possible: for the same outputs, you get the same inputs.
4. *Safe*: The library is safe: I avoid **Undefined Behavior** as much as possible. Most APIs make invalid state impossible.
5. *Opaque*: The internals of the library are opaque, even if the code is Open-Source. You don't need to worry about low-level problems!

---

## Getting Started

### 1. Clone the repo
```bash
git clone https://github.com/kdslfjioasdfj/clib2
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
