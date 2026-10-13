# Corte Máximo em grafos esparsos

Heurística gulosa + busca local 1-flip para o problema do Corte Máximo (Max-Cut).

Arquivos:
- `maxcut.cpp`: algoritmo.
- `gerador.cpp`: gerador de grafos esparsos aleatórios.
- `instancias/`: instâncias do [G-set](https://web.stanford.edu/~yyye/yyye/Gset/) utilizadas e `R100k` (gerada).
- `resultados/`: resultados para as instancias incluidas.

Formato das instâncias: a primeira linha tem `n m`, depois vêm `m` linhas especificando as arestas (`u v peso`), com vértices numerados de 1 a `n`.

# Compilando

```
g++ -std=c++17 -O2 -o maxcut maxcut.cpp
g++ -std=c++17 -O2 -o gerador gerador.cpp
```

# Rodando

## maxcut

```
./maxcut <instancia> [saida]
```

Argumentos:
- `<instancia>`: caminho do arquivo do grafo, por exemplo `instancias/G55`.
- `[saida]`: arquivo onde salvar o relatório e a solução. Caso não especificado, será utiliazdo `<instancia>.sol`.

Exemplo:

```
./maxcut instancias/G55 resultados/G55.txt
```

## gerador

```
./gerador <n> <m> <semente> > <arquivo>
```

Argumentos:
- `<n>`: número de vértices.
- `<m>`: número de arestas distintas, escolhidas aleatóriamente. Deve ser no máximo `n(n-1)/2`.
- `<semente>`: semente do gerador (O gerador é determinístico).

Saída: o grafo no formato especificado, com pesos 1, escrito na saída padrão (use `>` para salvar em arquivo).

Exemplo:

```
./gerador 100000 200000 1 > instancias/R100k
```
