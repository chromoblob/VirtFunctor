
#include "VirtFunctor.hpp"
#include <cstdio>
#include <type_traits>
#include <utility>

#define Qual_No
#define Qual_L  &
#define Qual_R  &&

#define B0(c, v, r, o) \
    class B_##c##_##v##_##r##_##o { \
        const char *s; \
    protected: \
        B_##c##_##v##_##r##_##o(const char *s): s(s) {} \
    o: \
        virtual const char *operator () (int *i, const char *t) c v Qual_##r { \
            printf("B_" #c "_" #v "_" #r "_" #o "::operator () (%p, \"%s\")\n", (void *) i, t); \
            return s; \
        }; \
    };
#define B1(c, v, r) B0(c, v, r, public) B0(c, v, r, protected)
#define B2(c, v)    B1(c, v, No) B1(c, v, L) B1(c, v, R)
#define B3(c)       B2(c, ) B2(c, volatile)
#define B4          B3()    B3(const)

B4

#define Type_Ptr *
#define Type_Ref &

#define Comma ,

#define IfPublic_public(x)    x
#define IfPublic_protected(x)

#define Base_Ptr (*b)
#define Base_Ref b

#define Use_No(x) x
#define Use_L(x)  x
#define Use_R(x)  std::move(x)

#define Include(x) x
#define Omit(x)

#define Stringify(bc, bv, lc, lv, r, base, t) #bc "," #bv "," #lc "," #lv "," #r "," #base "," #t
#define Test0(o, same, bc, bv, lc, lv, r, base, t) \
    { \
    Define \
    (   o, bc, bv, r \
    ,   [] (base(lc lv B_##bc##_##bv##_##r##_##o Type_##t b Comma) int *i, const char *s) { \
            printf("lambda i=%p " Stringify(bc, bv, lc, lv, r, base, t) "\n", (void *) i); \
            same(IfPublic_##o(base(printf("base operator () returned string \"%s\"\n", Use_##r(Base_##t).B_##bc##_##bv##_##r##_public::operator () (i + 1, s));))) \
            return "lambda return value " Stringify(bc, bv, lc, lv, r, base, t); \
        } \
    , "ctor param: " Stringify(bc, bv, lc, lv, r, base, t) \
    ) \
    int i[2]; \
    IfPublic_##o(printf("virtual call " Stringify(bc, bv, lc, lv, r, base, t) " returned string \"%s\"\n\n" \
        , Use_##r(static_cast<B_##bc##_##bv##_##r##_##o &>(V))(i, "calling: " Stringify(bc, bv, lc, lv, r, base, t)) \
    );) \
    }
#define Test1(o, same, bc, bv, lc, lv, r) Test0(o, same, bc, bv, lc, lv, r, Include, Ptr) Test0(o, same, bc, bv, lc, lv, r, Include, Ref) Test0(o, same, bc, bv, lc, lv, r, Omit, )
#define Test2(o, same, bc, bv, lc, lv)    Test1(o, same, bc, bv, lc, lv, No)              Test1(o, same, bc, bv, lc, lv, L)               Test1(o, same, bc, bv, lc, lv, R)

// Lambda's base class pointer parameter can be more or samely cv-qualified as operator ().
#define Test3(o) \
    Test2(o, Include,      ,         ,      ,         ) \
    Test2(o,    Omit,      ,         ,      , volatile) \
    Test2(o,    Omit,      ,         , const,         ) \
    Test2(o,    Omit,      ,         , const, volatile) \
 /* Test2(o, Include,      , volatile,      , volatile) */ /* volatile lambdas cannot be called in C++ yet */ \
 /* Test2(o,    Omit,      , volatile, const, volatile) */ \
    Test2(o, Include, const,         , const,         ) \
    Test2(o,    Omit, const,         , const, volatile) \
 /* Test2(o, Include, const, volatile, const, volatile) */

#define Test \
    puts("** protected\n"); \
    { Test3(protected) } \
    puts("** public\n"); \
    { Test3(public) }

int main() {
    puts("\n*** Testing TypeFromFunctor ***\n");

#define Define(o, c, v, r, l, p) \
    auto L = l; \
    VirtFunctor<B_##c##_##v##_##r##_##o>::TypeFromFunctor<decltype(L)> V{L, p}; \
    static_assert(std::is_same<decltype(L), decltype(V.functor)>::value, "Lambda and wrapper's 'functor' field have different types");

    Test

#if __cplusplus >= 201703L
    puts("*** Testing Type ***\n");

#define Define(o, c, v, r, l, p) \
    VirtFunctor<B_##c##_##v##_##r##_##o>::Type V{l, p};

    Test
#endif

    puts("*** OK ***\n");

    return 0;
}

