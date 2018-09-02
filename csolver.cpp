# include "csolver.h"

# include <cmath>
# include <queue>
# include <cstring>
# include <cassert>

CSolver:: CSolver(int n0, std::vector<std::tuple<int, int, double> > &edges): n(n0) {
    es = edges.size();
    memset(ind, 0, sizeof(ind));
    for(int i = 0; i < maxn; ++ i) ret[i] = -1.0;
    for(int i = 0; i < maxn; ++ i) in[i].clear();
    for(int i = 0; i < maxn; ++ i) out[i].clear();

    for(unsigned i = 0; i < edges.size(); ++ i) {
        int s = std:: get<0>(edges[i]), e = std:: get<1>(edges[i]);
        double sp = std:: get<2>(edges[i]);
        if(std:: isnan(sp) || sp <= 0) continue;
        // std:: cout << s << "->" << e << ":" << sp << std:: endl;
        Edge edge(s, e, i, sp, 0);
        if(i == edges.size() - 5) edge.c = 1.0, ret[edge.id] = 1.0; // in1
        if(i == edges.size() - 4) edge.c = 0.0, ret[edge.id] = 0.0; // in2
        in[edge.v].push_back(edge);
        ++ ind[edge.v];
        out[edge.u].push_back(edge);
    }
}

void CSolver:: calcSingleNode(int x) {
    if(x == 0 || x == 1) return; // input
    if(out[x].size() == 0) return;
    assert(in[x].size() < 4);

    // std:: cout << "cur =" << x << std:: endl;

    // -> 1
    if(out[x].size() == 1) {
        double sum = 0; for(auto it: in[x]) sum += it.speed * ret[it.id];
        ret[out[x][0].id] = sum / out[x][0].speed;
        out[x][0].c = ret[out[x][0].id];
        // std:: cout << out[x][0].c << std:: endl;
        return;
    }

    // 1 -> 2,3
    if(in[x].size() == 1) {
        assert(out[x].size() == 2 || out[x].size() == 3);
        for(unsigned i = 0; i < out[x].size(); ++ i) {
            ret[out[x][i].id] = ret[in[x][0].id];
            out[x][i].c = ret[in[x][0].id];
            // std:: cout << out[x][i].id << " " << out[x][i].c << std:: endl;
        }
        return;
    }

    // 2,2
    if(in[x].size() == 2) {
        int mx = in[x][0].speed > in[x][1].speed ? 0 : 1;
        int mn = out[x][0].speed < out[x][1].speed ? 0 : 1;

        ret[out[x][mn].id] = ret[in[x][mx].id]; out[x][mn].c = ret[in[x][mx].id];
        double c4 = in[x][0].speed * ret[in[x][0].id] + in[x][1].speed * ret[in[x][1].id];
        c4 -= out[x][mn].speed * out[x][mn].c; c4 /= out[x][mn ^ 1].speed;
        ret[out[x][mn ^ 1].id] = c4; out[x][mn ^ 1].c = c4;
        // std:: cout << ret[out[x][mn].id] << " " << c4 << std:: endl;
    }

    return;
}

std:: tuple<double, double, double> CSolver:: calc() {
    // Topo Sort

    std:: queue<int> que;

    for(int i = 0; i < n; ++ i) {
        if(!ind[i]) {
            que.push(i);
        }
    }

    while(!que.empty()) {
        int x = que.front(); que.pop();
        calcSingleNode(x);
        // std:: cout << x << std:: endl;
        for(auto e: out[x]) {
            ind[e.v] --;
            if(!ind[e.v]) que.push(e.v);
        }
    }

    return std:: make_tuple(ret[es - 3], ret[es - 2], ret[es - 1]);
}
