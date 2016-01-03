#include "shq.h"
#include "sem.h"

#include <iostream>

const size_t SHMEM_SIZE = 1 << 16;

void PrintMessages(const char* msg) {
    std::cout << "[msg]:" << msg << std::endl;
}

int main() {
    NShQ::TShQ queue("/dev/urandom", SHMEM_SIZE, true);
    NSem::TSem msgWaitingSem("/dev/random", true);
    while (true) {
        msgWaitingSem.Dec();
        size_t msgRead = queue.ProcessAndClean(PrintMessages);
        if (msgRead > 1) {
            msgWaitingSem.Dec(msgRead - 1);
        }
    }
    return 0;
}