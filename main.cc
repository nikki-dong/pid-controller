#include <iostream>
#include <set>
#include "pid_controller.hh"

// A simple usage example of the PidController class, using
// a lambda function.
int main() {
    // user defined timestamps.
    std::set<int64_t> timestamps{1, 2, 3, 4 ,5}; 

    nikki::PidController<int> pid(1, 2, 3, [timestamps]() mutable { 
                                                auto t = *timestamps.begin(); 
                                                timestamps.erase(t);
                                                return t;
                                            });
    // Call update.
    int i = pid.update(2, 10);
    // 1 * 8 + 2 * 8 = 24;
    std::cout << i << std::endl;

    i = pid.update(2, 10);
    // 1 * 8 + 2 * 16 + 3 * 0 = 40;
    std::cout << i << std::endl;
}
