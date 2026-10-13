#include <algorithm>
#include <chrono>
#include <deque>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <sstream>
#include <vector>
using namespace std;
#define ll long long

struct Grafo {
    int n = 0;
    ll m = 0;
    ll pesoTotal = 0;
    vector<vector<pair<int, int>>> adj;
};

//  Lê um arquivo que representa um grafo. A primeira linha deve ser "n m", o numero de vertices e arestas,
//  respectivamente, seguido por m linhas no formato "u v peso", sendo 'u' e 'v' os vertices e 'peso' o custo dessa
//  aresta
bool lerGrafo(const string &caminho, Grafo &g) {
    ifstream in(caminho);
    if (!in) {
        return false;
    }

    in >> g.n >> g.m;
    g.adj.resize(g.n, {});
    for (ll i = 0; i < g.m; i++) {
        int u, v, w;
        in >> u >> v >> w;

        // -1 pois 0 indexado
        u--;
        v--;

        g.adj[u].push_back({v, w});
        g.adj[v].push_back({u, w});
        g.pesoTotal += w;
    }
    return true;
}

// Valor do corte, soma dos pesos das arestas cujas pontas estao em lados diferentes
ll valorCorte(const Grafo &g, const vector<int> &lado) {
    ll corte = 0;
    for (int v = 0; v < g.n; v++) {
        for (auto [u, w] : g.adj[v]) {
            if (u > v && lado[u] != lado[v]) { // u > v para garantir que cada aresta é contada apenas uma vez
                corte += w;
            }
        }
    }
    return corte;
}

// Vertices ordenados por grau decrescente
vector<int> ordemPorGrau(const Grafo &g) {
    vector<int> ordem(g.n);
    iota(ordem.begin(), ordem.end(), 0);
    sort(ordem.begin(), ordem.end(), [&](int a, int b) { return g.adj[a].size() > g.adj[b].size(); });
    return ordem;
}

// constroi de maneira gulosa, cada vertice vai para o lado oposto do de maior
// peso entre os vizinhos ja colocados, de modo a maximiza o corte local. complexidade O(n + m).
vector<int> construcaoGulosa(const Grafo &g, const vector<int> &ordem) {
    vector<int> lado(g.n, -1); // -1 = ainda nao colocado
    for (int v : ordem) {
        ll peso0 = 0, peso1 = 0; // peso das arestas para vizinhos nos lados 0 e 1
        for (auto [u, w] : g.adj[v]) {
            if (lado[u] == 0) {
                peso0 += w;
            } else if (lado[u] == 1) {
                peso1 += w;
            }
        }
        lado[v] = (peso0 >= peso1) ? 1 : 0;
    }
    return lado;
}

// retorna o ganho no corte se v trocar de lado, ou seja, o custo se arestas internas passam a cortadas (+w)
// e arestas cortadas passam a internas (-w).
ll ganhoDeTroca(const Grafo &g, const vector<int> &lado, int v) {
    ll ganho = 0;
    for (auto [u, w] : g.adj[v]) {
        ganho += (lado[u] == lado[v]) ? w : -w;
    }
    return ganho;
}

// realiza uma busca local, troca de lado todo vertice com ganho positivo.
void buscaLocal(const Grafo &g, vector<int> &lado, const vector<int> &ordem) {
    deque<int> fila(ordem.begin(), ordem.end());
    vector<char> naFila(g.n, 1);
    while (!fila.empty()) {
        int v = fila.front();
        fila.pop_front();
        naFila[v] = 0;
        if (ganhoDeTroca(g, lado, v) <= 0) {
            continue;
        }
        lado[v] ^= 1;
        for (auto [u, w] : g.adj[v])
            if (!naFila[u]) {
                naFila[u] = 1;
                fila.push_back(u);
            }
    }
}

// serve de referencia para comparacao. faz particoes aleatorias, o valor esperado é  pesoTotal / 2.
ll corteAleatorio(const Grafo &g) {
    mt19937 rng(1);
    vector<int> lado(g.n);
    for (int &l : lado)
        l = rng() & 1;
    return valorCorte(g, lado);
}

// Salva os resultados da execução
void salvarSolucao(const string &caminho, const string &relatorio, const vector<int> &lado) {
    ofstream f(caminho);
    f << relatorio << "Particao (vertice: lado):\n";
    for (size_t v = 0; v < lado.size(); v++) {
        f << v + 1 << ' ' << lado[v] << '\n';
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " instancia [saida]\n";
        return 1;
    }

    Grafo g;
    if (!lerGrafo(argv[1], g)) {
        cerr << "Não foi possível ler o arquivo de entrada\n";
        return 1;
    }

    auto t0 = chrono::steady_clock::now();
    vector<int> ordem = ordemPorGrau(g);
    vector<int> lado = construcaoGulosa(g, ordem);
    auto t1 = chrono::steady_clock::now();
    ll corteGuloso = valorCorte(g, lado);

    buscaLocal(g, lado, ordem);
    auto t2 = chrono::steady_clock::now();
    ll corteFinal = valorCorte(g, lado);

    ostringstream r;
    r << "Instancia: " << argv[1] << "\nn=" << g.n << " m=" << g.m << " peso_total=" << g.pesoTotal
      << "\nAleatorio: " << corteAleatorio(g) << "\nGuloso: " << corteGuloso << " ("
      << chrono::duration<double>(t1 - t0).count() << " s)"
      << "\nGuloso+BuscaLocal: " << corteFinal << " (" << chrono::duration<double>(t2 - t0).count() << " s)\n";
    cout << r.str();

    string saida = argc > 2 ? argv[2] : string(argv[1]) + ".sol";
    salvarSolucao(saida, r.str(), lado);
    cout << "Relatorio e solucao salvos em " << saida << endl;
}
