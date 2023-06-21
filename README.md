# vsys
Cross-platform kernel model/driver development in C++/Cmake
Linux kernel support is being implemented. Mac/Windows kernel is next.

What's done so far:
- User mode modules are built and run under Linux/Mac/Windows
- Linux kernel modules can build and link correctly

Current challenge:
- Undefined symbols implementation (used by STL string, vector, etc)
