/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1010」玩具装箱 11-09-2017
 * 斜率优化
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
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig ? x = -x : 0;
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

using IO::io;
const int MAXN = 50000;
#define long long long

struct Task {
    int n, l, a[MAXN];
    long s[MAXN + 1], f[MAXN + 1];

    template <typename T>
    inline T square(const T &x) {
        return x * x;
    }

    inline long g(const int i) { return s[i] + i - l - 1; }

    inline long h(const int j) { return s[j] + j; }

    inline double slope(const int a, const int b) {
        return double((f[a] + square(h(a))) - (f[b] + square(h(b)))) /
               double(h(a) - h(b));
    }

    inline void solve() {
        io >> n >> l;
        for (register int i = 0; i < n; i++) io >> a[i];
        for (register int i = 1; i <= n; i++) s[i] = s[i - 1] + a[i - 1];
        static long q[MAXN];
        register long *l = q, *r = q;
        for (register int i = 1; i <= n; i++) {
            while (l < r && slope(*(l + 1), *l) <= 2 * g(i)) l++;
            f[i] = f[*l] + square(g(i) - h(*l));
            while (l < r && slope(*r, *(r - 1)) > slope(i, *r)) r--;
            *++r = i;
        }
        io << f[n];
    }

} task;

#undef long
}

int main() {
    task.solve();
    return 0;
}