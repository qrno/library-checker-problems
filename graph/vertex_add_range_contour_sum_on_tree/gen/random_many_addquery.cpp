#include <iostream>

#include "../params.h"
#include "random.h"

#include "../include/tree_height.hpp"
#include "../include/test_case.hpp"

std::vector<std::pair<int, int>> random_tree_edges(int n, Random &gen) {
    std::vector<std::pair<int, int>> edges;
    const auto perm = gen.perm<int>(n);
    for (int i = 1; i < n; ++i) {
        edges.emplace_back(perm[gen.uniform(0, i - 1)], perm[i]);
    }
    return edges;
}

int main(int, char* argv[]) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    long long seed = atoll(argv[1]);
    Random gen(seed);

    const int n = N_MAX;
    const int q = Q_MAX;

    std::vector<long long> a(n);
    for (auto& e : a) e = gen.uniform(A_MIN, A_MAX);

    TestCase tc(n, q, a);

    auto edges = random_tree_edges(n, gen);

    std::vector<std::vector<int>> g(n);
    for (auto& [u, v] : edges) {
        tc.add_edge(u, v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    const auto h = tree_height(g);

    for (int i = 0; i < q; ++i) {
        int query_type = gen.uniform01() < 0.9;
        int p = gen.uniform(0, n - 1);
        if (query_type == 0) {
            int x = gen.uniform(X_MIN, X_MAX);
            tc.add_addquery(p, x);
        } else {
            int l = gen.uniform(0, h[p] + 1);
            int r = gen.uniform(0, h[p] + 1);
            if (l > r) std::swap(l, r);
            tc.add_sumquery(p, l, r);
        }
    }

    std::cout << tc << std::endl;
    return 0;
}