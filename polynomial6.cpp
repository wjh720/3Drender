#include "config.h"
#include "const.h"
#include "polynomial6.h"

#include <iostream>

const long double eps1 = 1e-10;
const long double eps2 = 1e-14;
const long double eps3 = 1e-20;

Polynomial6::Polynomial6(const long double a[7]) {
    for (int i = 0; i < 7; i++)
        p6_a[i] = a[i];
    p6_init();
}

void Polynomial6::p6_init() {
    memset(p6_aa, 0, sizeof(p6_aa));
    
    for (int i = 0; i < 7; i++) {
        p6_aa[0][i] = p6_a[i];
        if (i < 6) p6_aa[1][i] = (i + 1) * p6_a[i + 1];
    }
    
    for (int i = 2; i < 7; i++) {
        long double a = p6_aa[i - 2][8 - i] / p6_aa[i - 1][7 - i];
        long double b = (p6_aa[i - 2][7 - i] - a * p6_aa[i - 1][6 - i]) / p6_aa[i - 1][7 - i];
        
        for (int j = 0; j < 7 - i; j++)
            p6_aa[i][j] = (j ? a * p6_aa[i - 1][j - 1] : 0) + b * p6_aa[i - 1][j] - p6_aa[i - 2][j];
        
        if (std::abs(p6_aa[i][6 - i]) < eps3) {
            for (int j = 0; j < 7 - i; j++)
                p6_aa[i][j] = 0;
            break;
        }
    }
}

bool Polynomial6::find_one_root(long double l, long double r, long double &x) {
    x = l;
    for (int i = 0; i < Config::newton_iteration_depth; i++) {
        long double f = F(x), d = dF(x), dx;
        
        if (std::abs(f) < eps2)
            return l < x - eps1 && x + eps1 < r;
        if (std::abs(d) < eps1)
            break;
        
        dx = f / d, x -= dx;
        if (std::abs(dx) < eps1)
            break;
    }
    return std::abs(x) < eps2 && l < x - eps1 && x + eps1 < r;
}

void Polynomial6::find_all_roots(long double l, long double r) {
    if (r - l < eps1) {
        if (std::abs(F(l)) < eps2)
            p6_roots.push_back(l);
        return;
    }
    
    int c = count_root(l, r);
    if (c == 0)
        return;
    else if (c == 1) {
        long double x;
        if (find_one_root(l, r, x)) {
            p6_roots.push_back(x);
            return;
        }
    }
    
    long double mid = (l + r) / 2;
    find_all_roots(l, mid);
    find_all_roots(mid, r);
}

long double Polynomial6::F(long double x) {
    long double s = 0;
    for (int i = 6; i >= 0; i--)
        s = s * x + p6_a[i];
    return s;
}

long double Polynomial6::dF(long double x) {
    long double s = 0;
    for (int i = 5; i >= 0; i--)
        s = s * x + p6_a[i + 1] * (i + 1);
    return s;
}

//theorem
int Polynomial6::count_root(long double l, long double r) {
    int s = 0;
    long double fl, fr, gl = 0, gr = 0;
    if (r - l < eps1)
        return 0;
    
    for (int i = 0; i < 7; i++) {
        if (p6_aa[i][6 - i] == 0)
            break;
        
        fl = fr = 0;
        for (int j = 6 - i; j >= 0; j--) {
            fl = fl * l + p6_aa[i][j];
            fr = fr * r + p6_aa[i][j];
        }
        if (std::abs(fl) > eps3) {
            if (fl * gl < 0) s++;
            gl = fl;
        }
        if (std::abs(fr) > eps3) {
            if (fr * gr < 0) s--;
            gr = fr;
        }
    }
    return s;
}

std::vector<double> Polynomial6::return_all_roots(double l, double r) {
    p6_roots.clear();
    find_all_roots(l, r);
    if (std::abs(F(l)) < eps2)
        p6_roots.push_back(l);
    if (std::abs(F(r)) < eps2)
        p6_roots.push_back(r);
    return p6_roots;
}
