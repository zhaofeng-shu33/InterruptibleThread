# Interruptible Thread
Interruptible Thread is a thin wrapper of `std::thread` which provides the `interrupt` method.

This is a header only library.

To compile the demo code:
```shell
g++ -std=c++11 -pthread InterruptibleThread.cpp -o InterruptibleThread # linux
cl /EHsc InterruptibleThread.cpp # windows Native Tools Command Prompt
```

You need a relatively higher version of `g++`, >=5.4 works.
