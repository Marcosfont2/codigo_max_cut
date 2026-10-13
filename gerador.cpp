#include <cstdlib>
#include <iostream>
#include <random>
#include <set>
using namespace std;
#define ll long long

// gera grafos esparsos aleatorios com arestas de peso 1
int main(int argc, char** argv) {
    if (argc < 4) {
        cerr << "Uso: " << argv[0] << "[n] [m] [seed]\n";
        return 1;
    }

    ll n = atoll(argv[1]);
    ll m = atoll(argv[2]);
    mt19937_64 rng(atoll(argv[3]));

    set<pair<ll, ll>> e;
    while (e.size() < m) {
        ll u = rng() % n + 1;
        ll v = rng() % n + 1;
        if (u == v) {
            continue;
        }
        if (u > v) {
            swap(u, v);
        }
        e.insert({u, v});
    }

    cout << n << ' ' << m << '\n';
    for (auto [u, v] : e) {
        cout << u << ' ' << v << " 1\n";
    }
}
