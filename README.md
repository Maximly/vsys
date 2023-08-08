# vsys: kernel modules in C++
**Cross-platform kernel module/driver development in C++/CMake**
- Linux kernel C++ support is being implemented. Ubuntu arm64 is supported already!
- Mac/Windows kernel is next.

**What's done so far:**
- User mode modules are built and run under Linux/Mac/Windows (could be broken from time to time while kernel stuff is added).
- Linux kernel headers are patched automatically to be included from C++ sources (tested on Ubuntu/Raspberry arm64, x86 does not work yet - simply because I prefer arm but I promise to support x86 as well).
- Linux kernel modules can now build and link correctly under Ubuntu arm64.
- Linux kernel modules can now load/unload correctly under Ubuntu arm64.
<img width="637" alt="image" src="https://github.com/Maximly/vsys/assets/98682581/5f5dc0e3-7ccc-41af-992e-fa18fe135025">

**Currently working on:**
- Polishing implementation of Module class

**Next steps:**
- Make it working under x86/x64
- Add Win kernel support before moving further? Your ideas/requests are welcome!
