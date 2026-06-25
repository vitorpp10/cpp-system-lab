#include <bits/stdc++.h>
#include<atomic>
#include<thread>

#define ll long long
#define fastio \
    ios_base::sync_with_stdio(false); \
    cin.tie(NULL)

#define endl "\n"

using namespace std;

atomic<bool> b{false};
int d = 0;
string s = "";

void t1() {
    d = 1;
    s = "set true";
    b.store(true, memory_order_release);
}

void t2() {
    while(!b.load(memory_order_acquire)) {}
    cout << "number: " << d << endl;
    cout << "message: " << s << endl;
}

int main() {
    fastio;
    thread ta(t1);
    thread tb(t2);
    if(ta.joinable()) ta.join();
    if(tb.joinable()) tb.join();
    return 0;
}
