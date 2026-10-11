#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
using namespace std;

const int N = 100;

double vandermonde(double x[], double y[], int n, double z) {
    double a[N][N + 1], c[N] = {};
    for (int i = 0; i < n; i++) {
        a[i][0] = 1;
        for (int j = 1; j < n; j++)
            a[i][j] = a[i][j - 1] * x[i];
        a[i][n] = y[i];
    }
    for (int k = 0; k < n; k++) {
        int p = k;
        for (int i = k + 1; i < n; i++) {
            if (fabs(a[i][k]) > fabs(a[p][k]))
                p = i;
        }
        for (int j = 0; j <= n; j++) {
            double t = a[k][j];
            a[k][j] = a[p][j];
            a[p][j] = t;
        }
        for (int i = k + 1; i < n; i++) {
            double t = a[i][k] / a[k][k];
            a[i][k] = 0;
            for (int j = k + 1; j <= n; j++)
                a[i][j] -= t * a[k][j];
        }
    }
    for (int i = n - 1; i >= 0; i--) {
        double t = a[i][n];
        for (int j = i + 1; j < n; j++)
            t -= a[i][j] * c[j];
        c[i] = t / a[i][i];
    }
    double ans = c[n - 1];
    for (int i = n - 2; i >= 0; i--)
        ans = ans * z + c[i];
    return ans;
}

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

double forward(double x[], double y[], int n, double z) {
    double d[N][N] = {};
    for (int i = 0; i < n; i++)
        d[i][0] = y[i];
    for (int k = 1; k < n; k++) {
        for (int i = 0; i < n - k; i++)
            d[i][k] = d[i + 1][k - 1] - d[i][k - 1];
    }
    double t = (z - x[0]) / (x[1] - x[0]);
    double ans = y[0], p = 1;
    for (int k = 1; k < n; k++) {
        p *= (t - k + 1) / k;
        ans += p * d[0][k];
    }
    return ans;
}

double linear(double x[], double y[], int n, double z) {
    int i = 0;
    while (i < n - 2 && z > x[i + 1])
        i++;
    double t = (z - x[i]) / (x[i + 1] - x[i]);
    return (1 - t) * y[i] + t * y[i + 1];
}

double hermite(double x[], double y[], double s[], int n, double z) {
    int i = 0;
    while (i < n - 2 && z > x[i + 1])
        i++;
    double h = x[i + 1] - x[i];
    double t = (z - x[i]) / h;
    double h00 = 2 * t * t * t - 3 * t * t + 1;
    double h01 = -2 * t * t * t + 3 * t * t;
    double h10 = t * t * t - 2 * t * t + t;
    double h11 = t * t * t - t * t;
    return h00 * y[i] + h01 * y[i + 1]
         + h * h10 * s[i] + h * h11 * s[i + 1];
}

void calculate(double x[], double y[], double s[], int n, double z, double r[]) {
    r[0] = vandermonde(x, y, n, z);
    r[1] = lagrange(x, y, n, z);
    r[2] = newton(x, y, n, z);
    r[3] = forward(x, y, n, z);
    r[4] = linear(x, y, n, z);
    r[5] = hermite(x, y, s, n, z);
}

double F(double x, double c, double d, double e, double f) {
    return c * sin(d * x) + e * cos(f * x);
}

double DF(double x, double c, double d, double e, double f) {
    return c * d * cos(d * x) - e * f * sin(f * x);
}

const char *name[6] = {
    "Vandermonde", "Lagrange", "Newton", "NewtonForward",
    "PiecewiseLinear", "Hermite"
};

int main() {
    double a, b, c, d, e, f;
    int n, m;
    cout << "Enter a b c d e f node_count test_count:\n";
    cin >> a >> b >> c >> d >> e >> f >> n >> m;
    if (!cin || a >= b || n < 2 || n > N || m < 1)
        return 1;

    double p[7][6];
    for (int i = 0; i < 7; i++) {
        p[i][0] = a; p[i][1] = b; p[i][2] = c;
        p[i][3] = d; p[i][4] = e; p[i][5] = f;
    }
    p[1][0] -= (b - a) / 2;
    p[2][1] += (b - a) / 2;
    p[3][2] += 1;
    p[4][3] += 3;
    p[5][4] += 1;
    p[6][5] += 3;
    const char *label[7] = {"baseline", "a", "b", "c", "d", "e", "f"};

    ofstream summary("accuracy_summary.csv"), data("accuracy_points.csv");
    summary << "case,method,mean_absolute_error\n" << setprecision(17);
    data << "case,x,target";
    for (int k = 0; k < 6; k++)
        data << ',' << name[k];
    data << '\n' << setprecision(17);
    cout << scientific << setprecision(8);

    for (int i = 0; i < 7; i++) {
        a = p[i][0]; b = p[i][1]; c = p[i][2];
        d = p[i][3]; e = p[i][4]; f = p[i][5];
        double x[N], y[N], s[N], error[6] = {};
        for (int j = 0; j < n; j++) {
            x[j] = a + (b - a) * (double(j) / (n - 1));
            y[j] = F(x[j], c, d, e, f);
            s[j] = DF(x[j], c, d, e, f);
        }
        for (int j = 0; j < m; j++) {
            double z = a + (b - a) * ((j + 0.5) / m);
            double value = F(z, c, d, e, f), r[6];
            calculate(x, y, s, n, z, r);
            data << label[i] << ',' << z << ',' << value;
            for (int k = 0; k < 6; k++) {
                error[k] += fabs(r[k] - value);
                data << ',' << r[k];
            }
            data << '\n';
        }
        cout << "\nCase: " << label[i] << '\n';
        for (int k = 0; k < 6; k++) {
            cout << name[k] << ' ' << error[k] / m << '\n';
            summary << label[i] << ',' << name[k] << ',' << error[k] / m << '\n';
        }
    }
    return 0;
}
