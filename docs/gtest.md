# Google Test (gtest) as our C++ testing framework, as it's popular, powerful, and actively maintained.

Installing Google Test: Before you can use Google Test, you need to install it. The way you install it depends on your operating system:

Ubuntu:
```bash 
    sudo apt-get install libgtest-dev && cd /usr/src/gtest && sudo cmake CMakeLists.txt && sudo make && sudo cp lib/*.a /usr/lib
```

macOS:
```bash
brew install googletest
```

Windows:
```bash  
The installation process on Windows is a bit more complex. You might need to download the Google Test source from GitHub and compile it manually.
```    
