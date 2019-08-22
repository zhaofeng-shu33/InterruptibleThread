# Interruptible Thread
[![Travis](https://travis-ci.com/zhaofeng-shu33/InterruptibleThread.svg?branch=master)](
    https://travis-ci.com/zhaofeng-shu33/InterruptibleThread)
    
Interruptible Thread is a thin wrapper of `std::thread` which provides the `interrupt` method.



To compile the demo code:
```shell
g++ -std=c++11 -pthread InterruptibleThread.cpp -o InterruptibleThread # linux
cl /EHsc InterruptibleThread.cpp # windows Native Tools Command Prompt
```

You need a relatively higher version of `g++`, >=5.4 works.
