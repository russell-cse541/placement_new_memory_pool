#define MAX_NUMBER_DVS 10

#include <sys/mman.h>
#include <iostream>

using namespace std;

struct DvSlot {
    DvSlot* next;
};

struct DV { 
    int64_t a; 
    int64_t b; 
};


class PoolAlloc
{
    public:
        void* alloc(size_t size);
        void free(void* ptr);
    private:
        int numFreeSlots;
        void *pool = 0;
        void * availableBlocks = 0;
        DvSlot* freeSlots;
        PoolAlloc();
        ~PoolAlloc();
};

PoolAlloc::PoolAlloc() {
    int requiredMemory = (sizeof(DvSlot) + sizeof(DV)) * MAX_NUMBER_DVS;
    pool = mmap(0, requiredMemory, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
        if (pool == MAP_FAILED) {
            cout << "mmap failed to allocate required memory size" << endl;
            exit(1);
        }    
        for (size_t i = 0; i < MAX_NUMBER_DVS; i++) {
            DvSlot* slot = reinterpret_cast<DvSlot*>(static_cast<char*>(pool) + i * sizeof(DV));
            slot->next = freeSlots;
            freeSlots = slot;
        }
}

PoolAlloc::~PoolAlloc() {
    munmap(pool, (sizeof(DvSlot) + sizeof(DV)) * MAX_NUMBER_DVS);
}
// PoolAlloc* getPoolAlloc() {

// }
// void releasePoolAlloc(PoolAlloc* p);

DV *alloc()


int64_t dsum(int64_t c)
{
    DV** arr = (DV**)malloc(c * sizeof(DV*));
    for(int64_t i = 0; i < c; ++i) {
        arr[i] = (DV*)malloc(sizeof(DV));
        arr[i]->a = i;
        arr[i]->b = i;
    }

    int64_t res = 0;
    for(int64_t i = 0; i < c; ++i) {
        res += arr[i]->a + arr[i]->b;
    }

    for(int64_t i = 0; i < c; ++i) {
        free(arr[i]);
    }
    free(arr);

    return res;
}

int main() {
    cout << dsum(4) << endl;
}
