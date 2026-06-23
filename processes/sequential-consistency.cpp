#include <bits/stdc++.h>
#include<thread>
#include<atomic>

#define ll long long
#define fastio \
    ios_base::sync_with_stdio(false); \
    cin.tie(NULL)

#define endl "\n"

using namespace std;

atomic<int> x{0};
atomic<int> y{0};
int r1 = 0;
int r2 = 0;

void t1() {
  x.store(1, memory_order_seq_cst);
  r1 = y.load(memory_order_seq_cst);
}

void t2() {
  y.store(1, memory_order_seq_cst);
  r2 = x.load(memory_order_seq_cst);
}

int main() {
    fastio;
    int d = 0;
    for(int i = 0; i < 1'000'000; i++) {
      x = 0, y = 0, r1 = 0, r2 = 0;
      thread ta(t1);
      thread tb(t2);
      if(ta.joinable()) ta.join();
      if(tb.joinable()) tb.join();
      if(r1 == 0 && r2 == 0) d++;
    }
    cout << "how much times hardware lier compiler and volatile: " << d << endl;
    return 0;
}
