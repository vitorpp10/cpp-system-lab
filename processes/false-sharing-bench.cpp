#include <bits/stdc++.h>
#include <cstdint>
#include <new>
#include<thread>
#include<chrono>

#define ll long long
#define fastio \
    ios_base::sync_with_stdio(false); \
    cin.tie(NULL)

#define endl "\n"

using namespace std;
using namespace chrono;

struct __attribute__((packed)) NormalData {
    volatile uint64_t c1{0};
    volatile uint64_t c2{0};
};

struct AligData {
    alignas(hardware_destructive_interference_size) volatile uint64_t c1{0};
    alignas(hardware_destructive_interference_size) volatile uint64_t c2{0};
};

void n() {
    NormalData d{0,0};
    auto s = high_resolution_clock::now();
    thread t1([&]() {
        for(int i = 0; i < 100'000'000; i++) d.c1++;
    });
    thread t2([&]() {
        for(int i = 0; i < 100'000'000; i++) d.c2++;
    });
    if(t1.joinable()) t1.join();
    if(t2.joinable()) t2.join();
    auto f = high_resolution_clock::now();
    duration<double> e = f - s;
    double throughput = (d.c1 + d.c2) / e.count();
    cout << "[TIME] without alignas: " << e.count() << endl;
    cout << "[THROUGHPUT]: " << throughput << endl;
}

void m() {
    AligData d{0,0};
    auto s = high_resolution_clock::now();
    thread t1([&]() {
        for(int i = 0; i < 100'000'000; i++) d.c1++;
    });
    thread t2([&]() {
        for(int i = 0; i < 100'000'000; i++) d.c2++;
    });
    if(t1.joinable()) t1.join();
    if(t2.joinable()) t2.join();
    auto f = high_resolution_clock::now();
    duration<double> e = f - s;
    double throughput = (d.c1 + d.c2) / e.count();
    cout << endl << "[TIME] with alignas: " << e.count() << endl;
    cout << "[THROUGHPUT]: " << throughput << endl;
}

int main() {
    fastio;
    n();
    m();
    return 0;
}
