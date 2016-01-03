#include "shq.h"
#include "sem.h"

#include <cstring>
#include <iostream>
#include <exception>

const size_t SHMEM_SIZE = 1 << 16;

void PrintMessages(const char* msg) {
    std::cout << "[msg]:" << msg << std::endl;
    if (strcmp(msg, "exit") == 0) {
        throw std::exception(); // escape from infinite loop
    }
}

int main() {
    NShQ::TShQ queue("/dev/urandom", SHMEM_SIZE, /* own = */ true);
    NSem::TSem msgWaitingSem("/dev/random", /* own = */ true);
    try {
        while (true) {
            msgWaitingSem.Dec();
            size_t msgRead = queue.ProcessAndClean(PrintMessages);
            if (msgRead > 1) {
                msgWaitingSem.Dec(msgRead - 1);
            }
        }
    } catch (...) {
        std::cout << "Shutdown..." << std::endl;
    }
    return 0;
}
