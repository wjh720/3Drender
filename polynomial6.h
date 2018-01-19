#ifndef Polynomial6_h
#define Polynomial6_h

#include <algorithm>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <bitset>
#include <cmath>
#include <ctime>
#include <queue>
#include <set>
#include <map>

struct Polynomial6 {
private:
    long double p6_a[7], p6_aa[7][7];
    std::vector<double> p6_roots;
    
    bool find_one_root(long double l, long double r, long double &x);
    void find_all_roots(long double l, long double r);
    
public:
    Polynomial6(const long double a[7]);
    
    void p6_init();
    
    long double F(long double x);
    long double dF(long double x);
    int count_root(long double l, long double r);
    std::vector<double> return_all_roots(double l, double r);
    
    void print() const {
        for (int i = 0; i < 7; i++)
            printf("%lf ", double(p6_a[i]));
        printf("\n");
    }
};


#endif // Polynomial6_h
