#define T_SIZE 3
#define BUF_SIZE 1024
#include<fcntl.h>
#include<unistd.h>
#include<iostream>
#include<mutex>
#include<queue>
#include<condition_variable>
#include<thread>
#include<cstdlib>
#include<sys/syscall.h>
#include<vector>
#include<sys/types.h>
#include<cstdint>

std::mutex mtxx;

struct dents {
    uint64_t d_ino;
    int64_t d_off;
    unsigned short d_reclen;
    unsigned char d_type;
    char d_name[];
};

class ThreadSafeQueue {
    private:
    std::mutex mtx;
    std::queue<int> f;
    std::condition_variable cv_p, cv_c;
    size_t max;
    public:
    ThreadSafeQueue(size_t m) : max(m) {}

    void push(int item) {
        std::unique_lock<std::mutex> lock(mtx);
        cv_p.wait(lock, [this]() { return f.size() < max; });
        f.push(item);
        cv_c.notify_one();
    }

    int pop() {
        std::unique_lock<std::mutex> lock(mtx);
        cv_c.wait(lock, [this]() { return !f.empty(); });
        int v = f.front();
        f.pop();
        cv_p.notify_one();
        return v;
    }
};

ThreadSafeQueue ff(20);

void p() {
    const char* path = ".";
    int fd = open(path, O_RDONLY, O_DIRECTORY);
    if (fd < 0) { perror("open"); exit(EXIT_FAILURE); }

    char buffer[BUF_SIZE];
    while (true) {
        int bpos = syscall(SYS_getdents64, fd, buffer, BUF_SIZE);
        if (bpos < 0) { perror("getdents64"); exit(EXIT_FAILURE); }
        for (int i = 0; i < bpos;) {
            struct dents* d = reinterpret_cast<struct dents*>(buffer + i);
            ff.push(d->d_ino);
            i += d->d_reclen;
        }
        if (bpos == 0) {
            for (int i = 0; i < 3; i++) { 
                ff.push(-1); 
            } 
            break;
        }
    }
    close(fd);
}

void c(int id) {
    while(true) {
        int in = ff.pop();
        if (in == -1) { 
            std::unique_lock<std::mutex> lock(mtxx);
            std::cout << "poison detected>>break\n"; 
            break;
        } else { 
            {
                std::unique_lock<std::mutex> lock(mtxx);
                std::cout << "thread[" << id << "] index inode[" << in << "]\n"; 
            }
        }
    }
}

int main() {
    std::thread tt(p);
    if (tt.joinable()) { tt.join(); }

    std::vector<std::thread> pool;
    for (int i = 0; i < T_SIZE; i++) {
        pool.emplace_back(c, i);
    }
    for (auto& t : pool) {
        if (t.joinable()) { t.join();}
    }
    return EXIT_SUCCESS;
}
