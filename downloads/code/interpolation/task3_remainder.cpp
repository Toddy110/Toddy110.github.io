#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
using namespace std;

const int N = 100;

double lagrange(double x[], double y[], int n, double z) {
    double ans = 0;
    for (int i = 0; i < n; i++) {
        double t = 1;
        for (int j = 0; j < n; j++) {
            if (j != i)
                t *= (z - x[j]) / (x[i] - x[j]);
        }
        ans += y[i] * t;
    }
    return ans;
}

double newton(double x[], double y[], int n, double z) {
    double d[N][N] = {};
    for (int i = 0; i < n; i++)
        d[i][0] = y[i];
    for (int k = 1; k < n; k++) {
        for (int i = 0; i < n - k; i++)
            d[i][k] = (d[i + 1][k - 1] - d[i][k - 1])
                    / (x[i + k] - x[i]);
    }
    double ans = d[0][n - 1];
    for (int i = n - 2; i >= 0; i--)
        ans = ans * (z - x[i]) + d[0][i];
    return ans;
}

int main() {
    double a, b;
    int n, m;
    cout << "Enter a b power node_count:\n";
    cin >> a >> b >> n >> m;
    if (!cin || a >= b || n < 0 || m <= n + 1 || m > N)
        return 1;

    double x[N], y[N];
    for (int i = 0; i < m; i++) {
        x[i] = a + (b - a) * (double(i) / (m - 1));
        y[i] = pow(x[i], n);
    }
    const int test_count = 301;
    double error1 = 0, error2 = 0;
    ofstream data("polynomial_points.csv");
    data << "x,target,Lagrange,Newton,theoretical_remainder\n" << setprecision(17);
    for (int j = 0; j < test_count; j++) {
        double z = a + (b - a) * ((j + 0.5) / test_count);
        double value = pow(z, n);
        double v1 = lagrange(x, y, m, z);
        double v2 = newton(x, y, m, z);
        error1 += fabs(v1 - value);
        error2 += fabs(v2 - value);
        data << z << ',' << value << ',' << v1 << ',' << v2 << ",0\n";
    }
    error1 /= test_count;
    error2 /= test_count;
    ofstream summary("polynomial_summary.csv");
    summary << "method,mean_absolute_error,theoretical_remainder\n" << setprecision(17);
    summary << "Lagrange," << error1 << ",0\n";
    summary << "Newton," << error2 << ",0\n";
    cout << scientific << setprecision(8);
    cout << "Lagrange " << error1 << '\n';
    cout << "Newton " << error2 << '\n';
    cout << "Theoretical remainder: 0\n";
    return 0;
}
