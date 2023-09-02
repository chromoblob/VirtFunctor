
VirtFunctor
===========

This C++ library implements a small wrapper for functors (including lambdas) that makes them callable through virtual `operator ()` of a base class.

**Version 1** (2023-09-02) by **chromoblob**, using tricks by **Kevin Spinar** (**Alipha** on [libera.chat](https://libera.chat)).

Language
--------

**C++11 and newer** C++ versions. When using a C++ version without class template argument deduction (which are C++11, C++14), you must have a way to refer to the functor's type.

License
-------

0-clause-BSD-like; see `LICENSE`.

Example
-------

**C++17**:

```c++
#include <cstdio>

#include <VirtFunctor.hpp>

struct A {
    virtual void operator () (int, void *) const = 0;
};

struct B {
    const char *name;

    B(const char *name): name(name) {}
    virtual char operator () (void *p, int i) {
        printf("%s: B::operator () (%p, %d)\n", ++name, p, i);
        return 'A';
    };
};

void callA(const A &a, int i, void *p) { a(i, p); }
char callB(B &b, void *p, int i) { return b(p, i); }

int main() {
    VirtFunctor<A>::Type a{[] (int i, void *p) { printf("%d %p\n", i, p); }};
    callA(a, 3, nullptr);

    char c = 'x';
    VirtFunctor<B>::Type b{
        [&c] (B *b, void *p, int i) {
            c = b->B::operator () (p, i) + i;
            return 'R';
        },
        ">b"
    };
    printf("%c\n", callB(b, nullptr, 5));
    printf("%c\n", c);
    return 0;
}
```

Installation
------------

Create a directory with the file `VirtFunctor.hpp` and add it to your compiler's include path.

Usage
-----

```c++
#include <VirtFunctor.hpp>
```

Create a base class ***B*** whose `operator ()` is virtual, not private and has only one overload. Create an object, hereafter called *wrapper*, of the type `VirtFunctor<`***B***`>::Type` (with deduced template arguments; see below for C++11 and C++14 usage) with constructor parameters: a functor, and then any number of arguments. The wrapper's base object of class ***B*** will be created with these constructor arguments, and then the functor will be saved in the wrapper by copying or moving. Now, any reference or pointer to the wrapper can be converted to reference/pointer to ***B***; if you call through it `operator ()`, the functor will be called with these arguments. Functor's parameters must be: optionally a reference or pointer to not less cv-qualified ***B*** than `operator ()`, which will be the reference or pointer to the base subobject of the wrapper, and then exact sequence of parameters of `operator ()`, which will be the arguments of `operator ()` invocation. Functor's return value, if any, will be returned by `operator ()`.

Example with lambda (**C++17**):

```c++
void caller(B &b) {
    ... b(...) ...  // the lambda is called
}

void f(...) {
    ...
    VirtFunctor<B>::Type virt{
        [...] (/* B &b, // optionally */ ...) { ... },
        /* parameters to B's constructor */
    };
    ...
    caller(virt);
    ...
}
```

The functor object is available as the `functor` field of the wrapper.

If functor's first parameter is a reference or pointer to less cv-qualified ***B*** than `operator ()`, you will get an instantiation error. If you want to pass a pointer/reference to ***B*** as `operator ()`'s first argument, the functor must accept the pointer/reference to the base subobject as its first parameter in addition to the passed pointer/reference.

When using C++11 or C++14, use `VirtFunctor<`***B***`>::TypeFromFunctor<`***F***`>` instead of `Type`, where ***F*** is the functor type:

```c++
    auto lambda = [...] (...) { ... };
    VirtFunctor<B>::TypeFromFunctor<decltype(lambda)> virt{lambda, /* parameters to B's constructor */};
```

`TypeFromFunctor` is available in any C++ version.

In any version of C++ up to C++20, volatile lambdas cannot be called, and so VirtFunctor cannot be used with them. When they can, though, VirtFunctor will handle this automatically.

Compiler support
----------------

Works on latest clang and GCC; as of 2023-09-02, doesn't work in Cygwin's GCC because it is old and contains a bug.

`test.cpp`
----------

`test.cpp` tests `VirtFunctor.hpp` with every combination of cv- and ref-qualifiers, access specifiers and base subobject parameter types, it will be handy if you want to make and test changes to `VirtFunctor.hpp`.

Author
------

[**chromoblob**](https://github.com/chromoblob)

© 2023

