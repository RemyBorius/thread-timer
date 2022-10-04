# thread-timer
Two simple timer class,  with a thread safe one !

## Content

This little header only file contain 2 classes of timer.

* The `Timer` class : compute an interval of time
* The `Thread Timer` : a thread safe timer, used to compute the cumulative time spent in a partion of code, in all threads.

## How to

The classes has 4 methods :

* `start()` : start the timer
* `storp()` : stop the timer
* `ms()` : return the time recorded between start and stop
* `reset()` : reset the timer

The thread timer is a bit more tricky to use : 
Compared to the "basic" timer, the thread timer can be used INSIDE threads to compute the cumulative time spent in a portion of code (for exemple, in a parallèle for loop). To use it :
     * declare the thread timer as a global variable
     * when started, the timer return a token
     * feed this token to the stop method
     * enjoy the result of your benchmark !
     *
     *  The timer can be set back to ZERO thanks to the reboot() function
     
## Exemple

```cpp
#include "timer.h"
#include "thread"
#include "iostream"

ThreadTimer myTimer;

int doStuff(int myInt) {
    int token = myTimer.start();
    // do things
    myTimer.stop(token);
    return myInt;
}

int main(int argc, char **argv) {

    std::thread thread1(doStuff, 1);
    std::thread thread2(doStuff, 2);

    thread1.join();
    thread2.join();
    
    std::cout << "Cumulative time in threads : " << myTimer.ms() << endl; 

}
```

## Thanks

Thanks to https://github.com/bshoshany/thread-pool for the original template I completed
