/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「SuperOJ 2018」游戏 30-10-2017
 * DP
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <typename T>
inline bool read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return false;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig ? x = -x : 0;
    return true;
}

inline void read(char &c) {
    while (c = read(), isspace(c) && c != -1)
        ;
}

inline int read(char *buf) {
    register int s = 0;
    register char c;
    while (c = read(), isspace(c) && c != -1)
        ;
    if (c == -1) {
        *buf = 0;
        return -1;
    }
    do
        buf[s++] = c;
    while (c = read(), !isspace(c) && c != -1);
    buf[s] = 0;
    return s;
}

const int OUT_LEN = 1000000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
    *oh++ = c;
}

template <typename T>
inline void print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        print('0');
    } else {
        x < 0 ? (print('-'), x = -x) : 0;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void print(const char *s) {
    for (; *s; s++) print(*s);
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

struct InputOutputStream {
    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        read(x);
        return *this;
    }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }

    ~InputOutputStream() { flush(); }
} io;
}

namespace {

const int MAXN = 20000;
const int INF = -1061109568;

using IO::io;

int a[MAXN + 1], sum[MAXN + 1], f[MAXN + 1][255], n;

inline int optMax(const register int a, const register int b) {
    return a > b ? a : b;
}

inline int dp(register int x, register int k) {
    register int &ret = f[x][k];
    if (ret != INF) return ret;
    return x + k > n
               ? 0
               : (x + k <= n
                      ? ret = optMax(ret, sum[x + k] - sum[x] - dp(x + k, k))
                      : 0,
                  x + k + 1 <= n
                      ? ret = optMax(
                            ret, sum[x + k + 1] - sum[x] - dp(x + k + 1, k + 1))
                      : 0,
                  ret);
}

inline void solve() {
    register int T;
    io >> T;
    while (T--) {
        io >> n;
        for (register int i = 1; i <= n; i++)
            io >> a[i], sum[i] = sum[i - 1] + a[i];
        memset(f, 0xc0, sizeof(f));
        io << dp(0, 1) << '\n';
    }
}
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}