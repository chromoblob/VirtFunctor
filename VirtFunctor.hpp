
#include <utility>

template <class B> class VirtFunctor {
    struct Expose: B { using B::operator (); };
    using OpType = decltype(&Expose::operator ());

    template <class F> struct TypeBase: B {
        F functor;

        template <class... A> TypeBase(const F  &f, A... a): B(std::forward<A>(a)...), functor(f) {}
        template <class... A> TypeBase(      F &&f, A... a): B(std::forward<A>(a)...), functor(std::move(f)) {}
    };

    enum Base { No, Ptr, Ref };
    template <Base, bool C = 0, bool V = 0> class Functor;

    template <class> struct Match;
    template <class F, class R, class... A> struct Match<R (F::*)                    (A...)>       { using Type = Functor<No>; };
    template <class F, class R, class... A> struct Match<R (F::*)                    (A...) const> { using Type = Functor<No>; };
    template <class F, class R, class... A> struct Match<R (F::*)               (B *, A...)>       { using Type = Functor<Ptr, false, false>; };
    template <class F, class R, class... A> struct Match<R (F::*)               (B *, A...) const> { using Type = Functor<Ptr, false, false>; };
    template <class F, class R, class... A> struct Match<R (F::*)(const          B *, A...)>       { using Type = Functor<Ptr, true,  false>; };
    template <class F, class R, class... A> struct Match<R (F::*)(const          B *, A...) const> { using Type = Functor<Ptr, true,  false>; };
    template <class F, class R, class... A> struct Match<R (F::*)      (volatile B *, A...)>       { using Type = Functor<Ptr, false, true>;  };
    template <class F, class R, class... A> struct Match<R (F::*)      (volatile B *, A...) const> { using Type = Functor<Ptr, false, true>;  };
    template <class F, class R, class... A> struct Match<R (F::*)(const volatile B *, A...)>       { using Type = Functor<Ptr, true,  true>;  };
    template <class F, class R, class... A> struct Match<R (F::*)(const volatile B *, A...) const> { using Type = Functor<Ptr, true,  true>;  };
    template <class F, class R, class... A> struct Match<R (F::*)               (B &, A...)>       { using Type = Functor<Ref, false, false>; };
    template <class F, class R, class... A> struct Match<R (F::*)               (B &, A...) const> { using Type = Functor<Ref, false, false>; };
    template <class F, class R, class... A> struct Match<R (F::*)(const          B &, A...)>       { using Type = Functor<Ref, true,  false>; };
    template <class F, class R, class... A> struct Match<R (F::*)(const          B &, A...) const> { using Type = Functor<Ref, true,  false>; };
    template <class F, class R, class... A> struct Match<R (F::*)      (volatile B &, A...)>       { using Type = Functor<Ref, false, true>;  };
    template <class F, class R, class... A> struct Match<R (F::*)      (volatile B &, A...) const> { using Type = Functor<Ref, false, true>;  };
    template <class F, class R, class... A> struct Match<R (F::*)(const volatile B &, A...)>       { using Type = Functor<Ref, true,  true>;  };
    template <class F, class R, class... A> struct Match<R (F::*)(const volatile B &, A...) const> { using Type = Functor<Ref, true,  true>;  };

    template <class F> using FuncType = typename Match<decltype(&F::operator ())>::Type;
public:
    template <class, class, class> class Type;

    template <class F, class R, class... A>                 struct Type<F, R (B::*)(A...),                   Functor<No>>:              TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a)                   override { return this->functor                                        (std::forward<A>(a)...); } };
    template <class F, class R, class... A>                 struct Type<F, R (B::*)(A...)                &,  Functor<No>>:              TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a)                &  override { return this->functor                                        (std::forward<A>(a)...); } };
    template <class F, class R, class... A>                 struct Type<F, R (B::*)(A...)                &&, Functor<No>>:              TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a)                && override { return this->functor                                        (std::forward<A>(a)...); } };
    template <class F, class R, class... A>                 struct Type<F, R (B::*)(A...) const,             Functor<No>>:              TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a) const             override { return this->functor                                        (std::forward<A>(a)...); } };
    template <class F, class R, class... A>                 struct Type<F, R (B::*)(A...) const          &,  Functor<No>>:              TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a) const          &  override { return this->functor                                        (std::forward<A>(a)...); } };
    template <class F, class R, class... A>                 struct Type<F, R (B::*)(A...) const          &&, Functor<No>>:              TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a) const          && override { return this->functor                                        (std::forward<A>(a)...); } };
    template <class F, class R, class... A>                 struct Type<F, R (B::*)(A...)       volatile,    Functor<No>>:              TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a)       volatile    override { return this->functor                                        (std::forward<A>(a)...); } };
    template <class F, class R, class... A>                 struct Type<F, R (B::*)(A...)       volatile &,  Functor<No>>:              TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a)       volatile &  override { return this->functor                                        (std::forward<A>(a)...); } };
    template <class F, class R, class... A>                 struct Type<F, R (B::*)(A...)       volatile &&, Functor<No>>:              TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a)       volatile && override { return this->functor                                        (std::forward<A>(a)...); } };
    template <class F, class R, class... A>                 struct Type<F, R (B::*)(A...) const volatile,    Functor<No>>:              TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a) const volatile    override { return this->functor                                        (std::forward<A>(a)...); } };
    template <class F, class R, class... A>                 struct Type<F, R (B::*)(A...) const volatile &,  Functor<No>>:              TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a) const volatile &  override { return this->functor                                        (std::forward<A>(a)...); } };
    template <class F, class R, class... A>                 struct Type<F, R (B::*)(A...) const volatile &&, Functor<No>>:              TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a) const volatile && override { return this->functor                                        (std::forward<A>(a)...); } };

    template <class F, class R, class... A, bool c, bool v> struct Type<F, R (B::*)(A...),                   Functor<Ptr, c,    v>>:    TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a)                   override { return this->functor(static_cast               <B *> (this), std::forward<A>(a)...); } };
    template <class F, class R, class... A, bool c, bool v> struct Type<F, R (B::*)(A...)                &,  Functor<Ptr, c,    v>>:    TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a)                &  override { return this->functor(static_cast               <B *> (this), std::forward<A>(a)...); } };
    template <class F, class R, class... A, bool c, bool v> struct Type<F, R (B::*)(A...)                &&, Functor<Ptr, c,    v>>:    TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a)                && override { return this->functor(static_cast               <B *> (this), std::forward<A>(a)...); } };
    template <class F, class R, class... A, bool v>         struct Type<F, R (B::*)(A...) const,             Functor<Ptr, true, v>>:    TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a) const             override { return this->functor(static_cast<const          B *> (this), std::forward<A>(a)...); } };
    template <class F, class R, class... A, bool v>         struct Type<F, R (B::*)(A...) const          &,  Functor<Ptr, true, v>>:    TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a) const          &  override { return this->functor(static_cast<const          B *> (this), std::forward<A>(a)...); } };
    template <class F, class R, class... A, bool v>         struct Type<F, R (B::*)(A...) const          &&, Functor<Ptr, true, v>>:    TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a) const          && override { return this->functor(static_cast<const          B *> (this), std::forward<A>(a)...); } };
    template <class F, class R, class... A, bool c>         struct Type<F, R (B::*)(A...)       volatile,    Functor<Ptr, c,    true>>: TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a)       volatile    override { return this->functor(static_cast      <volatile B *> (this), std::forward<A>(a)...); } };
    template <class F, class R, class... A, bool c>         struct Type<F, R (B::*)(A...)       volatile &,  Functor<Ptr, c,    true>>: TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a)       volatile &  override { return this->functor(static_cast      <volatile B *> (this), std::forward<A>(a)...); } };
    template <class F, class R, class... A, bool c>         struct Type<F, R (B::*)(A...)       volatile &&, Functor<Ptr, c,    true>>: TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a)       volatile && override { return this->functor(static_cast      <volatile B *> (this), std::forward<A>(a)...); } };
    template <class F, class R, class... A>                 struct Type<F, R (B::*)(A...) const volatile,    Functor<Ptr, true, true>>: TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a) const volatile    override { return this->functor(static_cast<const volatile B *> (this), std::forward<A>(a)...); } };
    template <class F, class R, class... A>                 struct Type<F, R (B::*)(A...) const volatile &,  Functor<Ptr, true, true>>: TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a) const volatile &  override { return this->functor(static_cast<const volatile B *> (this), std::forward<A>(a)...); } };
    template <class F, class R, class... A>                 struct Type<F, R (B::*)(A...) const volatile &&, Functor<Ptr, true, true>>: TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a) const volatile && override { return this->functor(static_cast<const volatile B *> (this), std::forward<A>(a)...); } };

    template <class F, class R, class... A, bool c, bool v> struct Type<F, R (B::*)(A...),                   Functor<Ref, c,    v>>:    TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a)                   override { return this->functor(static_cast               <B &>(*this), std::forward<A>(a)...); } };
    template <class F, class R, class... A, bool c, bool v> struct Type<F, R (B::*)(A...)                &,  Functor<Ref, c,    v>>:    TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a)                &  override { return this->functor(static_cast               <B &>(*this), std::forward<A>(a)...); } };
    template <class F, class R, class... A, bool c, bool v> struct Type<F, R (B::*)(A...)                &&, Functor<Ref, c,    v>>:    TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a)                && override { return this->functor(static_cast               <B &>(*this), std::forward<A>(a)...); } };
    template <class F, class R, class... A, bool v>         struct Type<F, R (B::*)(A...) const,             Functor<Ref, true, v>>:    TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a) const             override { return this->functor(static_cast<const          B &>(*this), std::forward<A>(a)...); } };
    template <class F, class R, class... A, bool v>         struct Type<F, R (B::*)(A...) const          &,  Functor<Ref, true, v>>:    TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a) const          &  override { return this->functor(static_cast<const          B &>(*this), std::forward<A>(a)...); } };
    template <class F, class R, class... A, bool v>         struct Type<F, R (B::*)(A...) const          &&, Functor<Ref, true, v>>:    TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a) const          && override { return this->functor(static_cast<const          B &>(*this), std::forward<A>(a)...); } };
    template <class F, class R, class... A, bool c>         struct Type<F, R (B::*)(A...)       volatile,    Functor<Ref, c,    true>>: TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a)       volatile    override { return this->functor(static_cast      <volatile B &>(*this), std::forward<A>(a)...); } };
    template <class F, class R, class... A, bool c>         struct Type<F, R (B::*)(A...)       volatile &,  Functor<Ref, c,    true>>: TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a)       volatile &  override { return this->functor(static_cast      <volatile B &>(*this), std::forward<A>(a)...); } };
    template <class F, class R, class... A, bool c>         struct Type<F, R (B::*)(A...)       volatile &&, Functor<Ref, c,    true>>: TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a)       volatile && override { return this->functor(static_cast      <volatile B &>(*this), std::forward<A>(a)...); } };
    template <class F, class R, class... A>                 struct Type<F, R (B::*)(A...) const volatile,    Functor<Ref, true, true>>: TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a) const volatile    override { return this->functor(static_cast<const volatile B &>(*this), std::forward<A>(a)...); } };
    template <class F, class R, class... A>                 struct Type<F, R (B::*)(A...) const volatile &,  Functor<Ref, true, true>>: TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a) const volatile &  override { return this->functor(static_cast<const volatile B &>(*this), std::forward<A>(a)...); } };
    template <class F, class R, class... A>                 struct Type<F, R (B::*)(A...) const volatile &&, Functor<Ref, true, true>>: TypeBase<F> { using TypeBase<F>::TypeBase; virtual R operator () (A... a) const volatile && override { return this->functor(static_cast<const volatile B &>(*this), std::forward<A>(a)...); } };

    template <class F> using TypeFromFunctor = Type<F, OpType, FuncType<F>>;

#if __cplusplus >= 201703L
    template <class F, class... A> Type(F, A...) -> Type<F, OpType, FuncType<F>>;
#endif
};

