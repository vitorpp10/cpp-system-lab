#include <bits/stdc++.h>
#include<mutex>
#include<thread>
#include<chrono>

#define ll long long
#define fastio \
    ios_base::sync_with_stdio(false); \
    cin.tie(NULL)

#define endl "\n"

using namespace std;
using namespace chrono;

mutex mtx;
int c=0;
void in() { for(int i = 0; i < 10'000'000; i++) c++; }
void s() {
  for(int i = 0; i < 10'000'000; i++) {
    unique_lock<mutex> lock(mtx);
    c++;
  }
}
int main() {
    fastio;
    auto si = high_resolution_clock::now();
    vector<thread> pi;
    for(int i = 0; i < 2; i++) pi.emplace_back(in);
    for(auto& pii : pi) if(pii.joinable()) pii.join();
    auto fi = high_resolution_clock::now();
    duration<double> elapsed1 = fi - si;
    cout << "NO SYNC: "<< endl << "value:"<< c << endl << "time: " << elapsed1.count() << endl << endl;
    c=0;
    auto ss = high_resolution_clock::now();
    vector<thread> ps;
    for(int i = 0; i < 2; i++) ps.emplace_back(s);
    for(auto& pss : ps) if(pss.joinable()) pss.join();
    auto fs = high_resolution_clock::now();
    duration<double> elapsed2 = fs - ss;
    cout << "SYNC: "<< endl << "value:"<< c << endl << "time: " << elapsed2.count() << endl;
    return 0;
}
