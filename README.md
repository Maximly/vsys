# vsys
**Cross-platform kernel module/driver development in C++/Cmake**
- Linux kernel support is being implemented. 
- Mac/Windows kernel is next.

**What's done so far:**
- User mode modules are built and run under Linux/Mac/Windows
- Linux kernel modules can build and link correctly

**Current challenges:**
- Undefined symbols implementation in Linux kernel (used by STL string, vector, etc)
