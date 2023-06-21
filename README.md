# vsys: kernel modules in C++
**Cross-platform kernel module/driver development in C++/Cmake**
- Linux kernel C++ support is being implemented. 
- Mac/Windows kernel is next.

**What's done so far:**
- User mode modules are built and run under Linux/Mac/Windows.
- Linux kernel headers are patched automatically to be included from C++ sources (tested on Ubuntu/Raspberry arm64, x86 does not work yet - simply because I prefer arm but I promise to support x86 as well)
- Linux kernel modules can now build and link correctly under Ubuntu arm64

**Currently working on:**
- Undefined symbols implementation in Linux kernel (used by STL string, vector, etc)
