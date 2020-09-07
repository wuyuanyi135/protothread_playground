#include "pt/pt-sem.h"
#include "pt/pt.h"
#include <chrono>
#include <iostream>
#include <thread>

int tick = 0;
PT_THREAD(task1(struct pt *pt)) {
    static int now;
    PT_BEGIN(pt);
    std::cout << "Task 1 is running..." << std::endl;

    now = tick;
    PT_YIELD_UNTIL(pt, tick - now > 1000);

    std::cout << "Task 1 is done" << std::endl;
    PT_END(pt);
}

PT_THREAD(task2(struct pt *pt)) {
    static int now;
    PT_BEGIN(pt);
    std::cout << "Task 2 is running..." << std::endl;

    now = tick;
    PT_YIELD_UNTIL(pt, tick - now > 3000);

    std::cout << "Task 2 is done" << std::endl;
    PT_END(pt);
}

int main() {
    struct pt pt1 {};
    struct pt pt2 {};
    int r1 = 0, r2 = 0;
    PT_INIT(&pt1);
    PT_INIT(&pt2);

    while (true) {
        if (PT_SCHEDULE(r1)) r1 = task1(&pt1);
        if (PT_SCHEDULE(r2)) r2 = task2(&pt2);
        if (r1 == PT_ENDED && r2 == PT_ENDED) {
            std::cout << "Both done!" << std::endl;
            return 0;
        }
        // Schedule every 1 millis
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        tick++;
    }
}