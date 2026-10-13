#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

double lagrange(double x[], double y[], int count, double z) {
    double ans = 0;
    for (int i = 0; i < count; i++) {
        double t = 1;
        for (int j = 0; j < count; j++)
            if (j != i) t *= (z - x[j]) / (x[i] - x[j]);
        ans += y[i] * t;
    }
    return ans;
}

int main() {
    int count;
    if (!(cin >> count) || count < 2 || count > 100) return 1;
    double x[100], y[100];
    for (int i = 0; i < count; i++) {
        x[i] = -1.0 + 2.0 * i / (count - 1);
        y[i] = 1 / (1 + 25 * x[i] * x[i]);
    }
    double max_error = 0;
    for (int j = 0; j <= 10000; j++) {
        double z = -1.0 + 2.0 * j / 10000;
        double exact = 1 / (1 + 25 * z * z);
        double error = fabs(lagrange(x, y, count, z) - exact);
        if (error > max_error) max_error = error;
    }
    cout << fixed << setprecision(10) << max_error << '\n';
    return 0;
}
