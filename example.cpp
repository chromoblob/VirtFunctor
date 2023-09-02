
#include <cstdio>

#include "VirtFunctor.hpp"

struct A {
    virtual void operator () (int, void *) const = 0;
};

struct B {
    const char *name;

    B(const char *name): name(name) {}
    virtual char operator () (void *p, int i) { printf("%s: B::operator () (%p, %d)\n", ++name, p, i); return 'A'; };
};

void callA(const A &a, int i, void *p) { a(i, p); }
char callB(B &b, void *p, int i) { return b(p, i); }

int main() {
    VirtFunctor<A>::Type a{[] (int i, void *p) { printf("%d %p\n", i, p); }};
    callA(a, 3, nullptr);

    char c = 'x';
    VirtFunctor<B>::Type b{[&c] (B *b, void *p, int i) { c = b->B::operator () (p, i) + i; return 'R'; }, ">b"};
    printf("%c\n", callB(b, nullptr, 5));
    printf("%c\n", c);
    return 0;
}

