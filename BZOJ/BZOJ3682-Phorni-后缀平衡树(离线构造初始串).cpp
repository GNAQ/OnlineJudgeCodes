/*******************************************************************************
 * Copyright (c) 2016-2017, xehoth
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name xehoth, nor the names of its contributors may be used
 *       to endorse or promote products derived from this software without
 *       specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY XEHOTH AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL XEHOTH AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/
#include <bits/stdc++.h>
/**
 * 「BZOJ 3682」Phorni 02-06-2017
 * 后缀平衡树
 * @author xehoth
 */
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
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    iosig ? x = -x : 0;
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
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, oh - obuf, stdout), oh = obuf) : 0;
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

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
}

namespace SuffixArray {

const int MAXN = 100100;

inline bool islms(const int i, const bool *t) {
    return i > 0 && t[i] && !t[i - 1];
}

template <typename T>
inline void sort(T s, int *sa, const int len, const int sz, const int sigma,
                 bool *t, int *b, int *cb, int *p) {
    memset(b, 0, sizeof(int) * sigma);
    memset(sa, -1, sizeof(int) * len);
    for (register int i = 0; i < len; i++) b[s[i]]++;
    cb[0] = b[0];
    for (register int i = 1; i < sigma; i++) cb[i] = cb[i - 1] + b[i];
    for (register int i = sz - 1; i >= 0; i--) sa[--cb[s[p[i]]]] = p[i];
    for (register int i = 1; i < sigma; i++) cb[i] = cb[i - 1] + b[i - 1];
    for (register int i = 0; i < len; i++)
        if (sa[i] > 0 && !t[sa[i] - 1]) sa[cb[s[sa[i] - 1]]++] = sa[i] - 1;
    cb[0] = b[0];
    for (register int i = 1; i < sigma; i++) cb[i] = cb[i - 1] + b[i];
    for (register int i = len - 1; i >= 0; i--)
        if (sa[i] > 0 && t[sa[i] - 1]) sa[--cb[s[sa[i] - 1]]] = sa[i] - 1;
}

template <typename T>
inline void sais(T s, int *sa, const int len, bool *t, int *b, int *b1,
                 const int sigma) {
    register int i, j, x, p = -1, sz = 0, cnt = 0, *cb = b + sigma;
    for (t[len - 1] = 1, i = len - 2; i >= 0; i--)
        t[i] = s[i] < s[i + 1] || (s[i] == s[i + 1] && t[i + 1]);
    for (i = 1; i < len; i++)
        if (t[i] && !t[i - 1]) b1[sz++] = i;
    sort(s, sa, len, sz, sigma, t, b, cb, b1);
    for (i = sz = 0; i < len; i++)
        if (islms(sa[i], t)) sa[sz++] = sa[i];
    for (i = sz; i < len; i++) sa[i] = -1;
    for (i = 0; i < sz; i++) {
        for (x = sa[i], j = 0; j < len; j++) {
            if (p == -1 || s[x + j] != s[p + j] || t[x + j] != t[p + j]) {
                p = x, cnt++;
                break;
            } else if (j > 0 && (islms(x + j, t) || islms(p + j, t))) {
                break;
            }
        }
        sa[sz + (x >>= 1)] = cnt - 1;
    }
    for (i = j = len - 1; i >= sz; i--)
        if (sa[i] >= 0) sa[j--] = sa[i];
    register int *s1 = sa + len - sz, *b2 = b1 + sz;
    if (cnt < sz)
        sais(s1, sa, sz, t + len, b, b1 + sz, cnt);
    else
        for (i = 0; i < sz; i++) sa[s1[i]] = i;
    for (i = 0; i < sz; i++) b2[i] = b1[sa[i]];
    sort(s, sa, len, sz, sigma, t, b, cb, b2);
}

struct SuffixArray {
    int sa[MAXN], rk[MAXN], ht[MAXN], n;
    bool t[MAXN << 1];
    char s[MAXN];

    inline void build(const int sigma) {
        s[n] = 0, sais(s, sa, n + 1, t, ht, rk, sigma);
    }

    inline char &operator[](const int i) { return s[i]; }
} suffixArray;
}

namespace SuffixBalancedTree {

namespace ScapeGoatTree {

const int MAXN = 800005;
const double ALPHA = 0.7;

struct Node *null;

struct Node {
    Node *c[2];
    double v;
    int size, id;

    Node(double v = 0, int id = 0) : v(v), size(1), id(id) {
        c[0] = c[1] = null;
    }

    inline void maintain() { size = c[0]->size + c[1]->size + 1; }

    inline bool check() {
        return std::max(c[0]->size, c[1]->size) > size * ALPHA;
    }

    inline void *operator new(size_t);
} pool[MAXN], *cur = pool, *pos[MAXN], *root;

inline void *Node::operator new(size_t) { return cur++; }

inline void init() {
    null = new Node(), null->size = 0;
    pos[0] = root = new Node(0.5);
}

double badL, badR;
char *s;

inline Node **insert(Node *&p, double l, double r, int id) {
    if (p == null) {
        pos[id] = p = new Node((l + r) / 2, id);
        return &null;
    } else {
        p->size++;
        Node **res;
        if (s[id] < s[p->id] ||
            (s[id] == s[p->id] && pos[id - 1]->v < pos[p->id - 1]->v))
            res = insert(p->c[0], l, (l + r) / 2, id);
        else
            res = insert(p->c[1], (l + r) / 2, r, id);
        if (p->check()) res = &p, badL = l, badR = r;
        return res;
    }
}

inline void travel(Node *p, std::vector<Node *> &v) {
    if (p == null) return;
    travel(p->c[0], v);
    v.push_back(p);
    travel(p->c[1], v);
}

inline Node *divide(std::vector<Node *> &v, int l, int r, double lv,
                    double rv) {
    if (l >= r) return null;
    register int mid = l + r >> 1;
    Node *p = v[mid];
    p->v = (lv + rv) / 2;
    p->c[0] = divide(v, l, mid, lv, (lv + rv) / 2);
    p->c[1] = divide(v, mid + 1, r, (lv + rv) / 2, rv);
    p->maintain();
    return p;
}

inline void rebuild(Node *&p) {
    static std::vector<Node *> v;
    v.clear(), travel(p, v), p = divide(v, 0, v.size(), badL, badR);
}
}

inline void insert(int id) {
    using namespace ScapeGoatTree;
    Node **p = insert(root, 0, 1, id);
    if (*p != null) rebuild(*p);
}

inline void init(char *s) { ScapeGoatTree::s = s, ScapeGoatTree::init(); }

inline void build(int *sa, const int n) {
    using namespace ScapeGoatTree;
    static std::vector<Node *> v;
    v.resize(n + 1);
    v[0] = root;
    for (register int i = 1; i <= n; i++)
        pos[n - sa[i]] = v[i] = new Node(0, n - sa[i]);
    root = divide(v, 0, v.size(), 0, 1);
}
}

namespace SegmentTree {

using SuffixBalancedTree::ScapeGoatTree::pos;

const int MAXN = 800005;

int in[MAXN], d[MAXN * 8], M;

inline int min(int a, int b) {
    return pos[in[a]]->v != pos[in[b]]->v
               ? (pos[in[a]]->v > pos[in[b]]->v ? b : a)
               : std::min(a, b);
}

inline void maintain(int x) { d[x] = min(d[x << 1], d[x << 1 | 1]); }

inline void build(const int n) {
    for (M = 1; M < n + 4; M <<= 1)
        ;
    for (register int i = 1; i <= n; i++) d[i + M] = i;
    for (register int i = M - 1; i; i--) maintain(i);
}

inline void modify(int x) {
    for (x = x + M >> 1; x; x >>= 1) maintain(x);
}

inline int query(int s, int t) {
    register int ans = d[M + s];
    for (s = s + M - 1, t = t + M + 1; s ^ t ^ 1; s >>= 1, t >>= 1) {
        if (~s & 1) ans = min(ans, d[s ^ 1]);
        if (t & 1) ans = min(ans, d[t ^ 1]);
    }
    return ans;
}

inline void solve() {
    using namespace IO;
    register int n, m, len, code;
    static char s[MAXN];
    read(n), read(m), read(len), read(code), read(s + 1);
    std::reverse(s + 1, s + len + 1);
    SuffixBalancedTree::init(s);
    std::reverse_copy(s + 1, s + len + 1, SuffixArray::suffixArray.s);
    SuffixArray::suffixArray.n = len;
    SuffixArray::suffixArray.build(128);
    SuffixBalancedTree::build(SuffixArray::suffixArray.sa, len);
    for (register int i = 1; i <= n; i++) read(in[i]);
    build(n);
    static char opt[5];
    register int ans = 0;
    for (register int i = 1; i <= m; i++) {
        read(opt);
        if (opt[0] == 'I') {
            int c;
            read(c);
            code ? (c ^= ans) : 0;
            s[++len] = c + 'a';
            SuffixBalancedTree::insert(len);
        } else if (opt[0] == 'C') {
            register int x, to;
            read(x), read(to), in[x] = to;
            modify(x);
        } else {
            register int l, r;
            read(l), read(r);
            ans = query(l, r);
            print(ans), print('\n');
        }
    }
}
}

int main() {
#ifdef DBG
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
#endif
    SegmentTree::solve();
    IO::flush();
    return 0;
}