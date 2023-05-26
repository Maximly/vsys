rmdir /s /q build
cmake . -B build -DVSYS_USER=1 -DCMAKE_BUILD_TYPE=Debug
cmake --build build
