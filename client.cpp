#include "shq.h"
#include "sem.h"
#include <string.h>

#include <iostream>

const size_t SHMEM_SIZE = 1 << 16;

int main() {
    NShQ::TShQ queue("/dev/urandom", SHMEM_SIZE, false);
    NSem::TSem msgWaitingSem("/dev/random", false);
    while (true) {
        std::cout << "$>:";
        std::string msg;
        getline(std::cin, msg);
        queue.AppendMsg(msg.c_str());
        msgWaitingSem.Inc();
        std::cout << "[sent]: " << msg << std::endl;
    }
    return 0;
}