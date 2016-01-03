#include "sem.h"

#include <sys/sem.h>
#include <sys/types.h>

namespace NSem {

TSem::TSem(const char* path, bool own)
    : Owner(own)
    , SemId(semget(ftok(path, 'E'), 1, 0777 | IPC_CREAT))
{}

TSem::~TSem() {
    if (Owner) {
        semctl(SemId, 0, IPC_RMID);
    }
}

void TSem::Op(int sem_op) {
    sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = sem_op;
    sb.sem_flg = 0;
    semop(SemId, &sb, 1);
}

} // namespace NSem
