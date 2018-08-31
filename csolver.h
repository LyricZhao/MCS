# ifndef CSOLVER_H
# define CSOLVER_H

# define maxn 400

# include <tuple>
# include <vector>
# include <iostream>

class CSolver {

    struct Edge {
        int u, v, id;
        double speed, c;
        Edge(int u0, int v0, int id0, double s0, double c0): u(u0), v(v0), id(id0), speed(s0), c(c0){}
    };

    int n, es, ind[maxn];
    std:: vector<Edge> in[maxn], out[maxn];
    double ret[maxn];

public:
    CSolver(int n0, std:: vector<std:: tuple<int, int, double> > &edges);

    void calcSingleNode(int x);
    std:: tuple<double, double, double> calc();
};

#endif // CSOLVER_H
