#ifndef ITERATIONS
#define ITERATIONS 50
#endif

#include <new> // for placement new
#include <iostream>
#include <sys/mman.h>

using namespace std;

struct DV { 
    int64_t a; 
    int64_t b; 
};

class PoolAlloc {
    public:
        PoolAlloc() {
            int requiredMemory = (ITERATIONS * sizeof(DV)) + (ITERATIONS * sizeof(DV*));
            this->pool = mmap(0,requiredMemory , PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
            this->startOfPool = this->pool;
            if (pool == MAP_FAILED) {
                cout << "mmap failed to allocate required memory size" << endl;
                exit(1);
            }    
            cout<<"Memory Pool Allocated Using mmap"<<endl;
        }

        ~PoolAlloc() {
            munmap(this->startOfPool, ITERATIONS * sizeof(DV));
            cout<<"Memory Pool Deallocated Using munmap"<<endl;
        }

        void* alloc(size_t size) {
            void *p = operator new(size, pool);
            if (p == nullptr) {
                std::cout << "operator new failed to allocate required memory size" << std::endl;
                exit(1);
            }
            this->pool = static_cast<char*>(this->pool) + size;
            return p;
        }

        void free(void* ptr) {
            return;
        }

    private:
        void* pool;
        void* startOfPool;
};

PoolAlloc* getPoolAlloc() {
    PoolAlloc *p = new PoolAlloc();
    return p;
}

void releasePoolAlloc(PoolAlloc* p) {
    delete p;
}

int64_t dsum(int64_t c)
{
    PoolAlloc* MP = getPoolAlloc();
    DV** arr = static_cast<DV**>(MP->alloc(c * sizeof(DV*)));
    for(int64_t i = 0; i < c; ++i) {
        arr[i] = static_cast<DV*>(MP->alloc(sizeof(DV)));
        arr[i]->a = i;
        arr[i]->b = i;
    }
    int64_t res = 0;
    for(int64_t i = 0; i < c; ++i) {
        res += arr[i]->a + arr[i]->b;
    }
    for(int64_t i = 0; i < c; ++i) {
        MP->free(arr[i]);
    }
    MP->free(arr);
    releasePoolAlloc(MP);
    return res;
}

int main() {
    cout << dsum(ITERATIONS) << endl;
}


