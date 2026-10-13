#include <iostream>
#include <iomanip>
using namespace std;

double hermite_global(double x[], double y[], double s[], int count, double w) {
    double z[200], d[200][200] = {};
    for (int i = 0; i < count; i++) {
        z[2 * i] = z[2 * i + 1] = x[i];
        d[2 * i][0] = d[2 * i + 1][0] = y[i];
        d[2 * i + 1][1] = s[i];
        if (i > 0)
            d[2 * i][1] = (y[i] - y[i - 1]) / (x[i] - x[i - 1]);
    }
    int total = 2 * count;
    for (int k = 2; k < total; k++)
        for (int i = k; i < total; i++)
            d[i][k] = (d[i][k - 1] - d[i - 1][k - 1]) / (z[i] - z[i - k]);
    double ans = d[total - 1][total - 1];
    for (int i = total - 2; i >= 0; i--)
        ans = ans * (w - z[i]) + d[i][i];
    return ans;
}

int main() {
    int count;
    if (!(cin >> count) || count < 1 || count > 100) return 1;
    double x[100], y[100], s[100], w;
    for (int i = 0; i < count; i++) {
        if (!(cin >> x[i] >> y[i] >> s[i])) return 1;
        if (i > 0 && x[i] <= x[i - 1]) return 1;
    }
    if (!(cin >> w)) return 1;
    cout << fixed << setprecision(10);
    cout << hermite_global(x, y, s, count, w) << '\n';
    return 0;
}
