#include <iostream>
#include <iomanip>
using namespace std;

void build_spline(double x[], double y[], int count, double m[]) {
    double a[100] = {}, b[100] = {}, c[100] = {}, r[100] = {};
    b[0] = b[count - 1] = 1;
    for (int i = 1; i < count - 1; i++) {
        double left = x[i] - x[i - 1];
        double right = x[i + 1] - x[i];
        a[i] = left;
        b[i] = 2 * (left + right);
        c[i] = right;
        r[i] = 6 * ((y[i + 1] - y[i]) / right
                  - (y[i] - y[i - 1]) / left);
    }
    for (int i = 1; i < count; i++) {
        double t = a[i] / b[i - 1];
        b[i] -= t * c[i - 1];
        r[i] -= t * r[i - 1];
    }
    m[count - 1] = r[count - 1] / b[count - 1];
    for (int i = count - 2; i >= 0; i--)
        m[i] = (r[i] - c[i] * m[i + 1]) / b[i];
}

double spline_value(double x[], double y[], double m[], int count, double z) {
    int i = 0;
    while (i < count - 2 && z > x[i + 1]) i++;
    double h = x[i + 1] - x[i];
    double left = x[i + 1] - z;
    double right = z - x[i];
    return m[i] * left * left * left / (6 * h)
         + m[i + 1] * right * right * right / (6 * h)
         + (y[i] - m[i] * h * h / 6) * left / h
         + (y[i + 1] - m[i + 1] * h * h / 6) * right / h;
}

int main() {
    int count;
    if (!(cin >> count) || count < 2 || count > 100) return 1;
    double x[100], y[100], m[100], z;
    for (int i = 0; i < count; i++) {
        if (!(cin >> x[i] >> y[i])) return 1;
        if (i > 0 && x[i] <= x[i - 1]) return 1;
    }
    if (!(cin >> z) || z < x[0] || z > x[count - 1]) return 1;
    build_spline(x, y, count, m);
    cout << fixed << setprecision(10);
    cout << spline_value(x, y, m, count, z) << '\n';
    return 0;
}
