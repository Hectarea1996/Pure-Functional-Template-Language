#ifndef CURRY_H_INCLUDED
#define CURRY_H_INCLUDED

namespace pftl{

//********************************************************************************
//--------------------------------------------------------------------------------

///Para definir las clases
struct Class{};

//--------------------------------------------------------------------------------

template<class B, class A, class... AS>
struct isBaseOfAll{
    static constexpr bool value = std::is_base_of<B,A>::value && isBaseOfAll<B,AS...>::value;
};

template<class B, class A>
struct isBaseOfAll<B,A>{
    static constexpr bool value = std::is_base_of<B,A>::value;
};

///Para comprobar si un tipo de dato especializa a una clase
template<template<class a> class C, class... AS>
using Where = typename std::enable_if<!isBaseOfAll<Class,C<AS>...>::value,void>::type;

//--------------------------------------------------------------------------------

///Para imprimir una deduccion
template<class S>
struct Display{};

//--------------------------------------------------------------------------------

///Para especificar que una metafuncion no esta definida
struct Undefined{};

//--------------------------------------------------------------------------------

///La clase de la que heredan el resto de tipos de datos
struct Type{};

//--------------------------------------------------------------------------------

///Non-type polimorfico
struct Auto{};

//********************************************************************************

template<class S>
struct ArgLit{
    template<S k>
    struct Arg{
        static constexpr S value = k;
    };
};

//template<class S>
//struct ArgLit<S&>{
//    template<S& k>
//    struct Arg{
//        static constexpr S& value = k;
//    };
//};

template<>
struct ArgLit<Auto>{
    template<auto k>
    struct Arg{
        static constexpr auto value = k;
    };
};

//template<>
//struct ArgLit<Auto&>{
//    template<auto& k>
//    struct Arg{
//        static constexpr auto& value = k;
//    };
//};

//--------------------------------------------------------------------------------

template<bool b, class Ret>
struct enable_if_int{};

template<class Ret>
struct enable_if_int<true,Ret>{
    template<Ret x>
    static constexpr Ret value = x;
};

//template<class Ret>
//struct enable_if_int<true,Ret&>{
//    template<Ret& x>
//    static constexpr Ret& value = x;
//};

template<>
struct enable_if_int<true,Auto>{
    template<auto x>
    static constexpr auto value = x;
};

//template<>
//struct enable_if_int<true,Auto&>{
//    template<auto& x>
//    static constexpr auto& value = x;
//};

//--------------------------------------------------------------------------------

template<class S>
struct is_nontype{
    static constexpr bool value = std::is_integral<S>::value || std::is_enum<S>::value ||
                              std::is_pointer<S>::value || std::is_lvalue_reference<S>::value ||
                              std::is_null_pointer<S>::value || std::is_same<S,Auto>::value;
};

//--------------------------------------------------------------------------------

template<class F, class Ret, class In, class Ty>
struct RetType{};

template<class F, class Ret, class... InArgs, class... TyArgs>
struct RetType<F,Ret,void(InArgs...),void(TyArgs...)>{
    using value = typename std::enable_if<std::is_base_of<Ret,typename F::template value<InArgs::value...,TyArgs...>>::value,typename F::template value<InArgs::value...,TyArgs...>>::type;
};

//--------------------------------------------------------------------------------

template<class F, class Ret, class In, class Ty>
struct RetInt{};

template<class F, class Ret, class... InArgs, class... TyArgs>
struct RetInt<F,Ret,void(InArgs...),void(TyArgs...)>{
    static constexpr Ret value = F::template value<InArgs::value...,TyArgs...>;
};

template<class F, class... InArgs, class... TyArgs>
struct RetInt<F,Auto,void(InArgs...),void(TyArgs...)>{
    static constexpr auto value = F::template value<InArgs::value...,TyArgs...>;
};

//--------------------------------------------------------------------------------

template<class F, bool isInt, class T, class In, class Ty>
struct CurryInt{};

template<class F, class Ret, class Next, class Next2, class... Args, class... InArgs, class... TyArgs>
struct CurryInt<F,false,Ret(Next,Next2,Args...),void(InArgs...),void(TyArgs...)>{
    struct value{
        template<class x>
        using let = typename std::enable_if<std::is_base_of<Next,x>::value,typename CurryInt<F,is_nontype<Next2>::value,Ret(Next2,Args...),void(InArgs...),void(TyArgs...,x)>::value>::type;

        using type = Ret(Next,Next2,Args...);
        using type_ret = Ret;
        using type_arg = Next;
    };
};

template<class F, class Ret, class Next, class Next2, class... Args, class... InArgs, class... TyArgs>
struct CurryInt<F,true,Ret(Next,Next2,Args...),void(InArgs...),void(TyArgs...)>{
    struct value{
        template<Next x>
        using let = typename CurryInt<F,is_nontype<Next2>::value,Ret(Next2,Args...),void(InArgs...,typename ArgLit<Next>::template Arg<x>),void(TyArgs...)>::value;

        using type = Ret(Next,Next2,Args...);
        using type_ret = Ret;
        using type_arg = Next;
    };
};

//template<class F, class Ret, class Next, class Next2, class... Args, class... InArgs, class... TyArgs>
//struct CurryInt<F,true,Ret(Next&,Next2,Args...),void(InArgs...),void(TyArgs...)>{
//    struct value{
//        template<Next& x>
//        using let = typename CurryInt<F,is_nontype<Next2>::value,Ret(Next2,Args...),void(InArgs...,typename ArgLit<Next&>::template Arg<x>),void(TyArgs...)>::value;
//
//        using type = Ret(Next&,Next2,Args...);
//        using type_ret = Ret;
//        using type_arg = Next&;
//    };
//};

template<class F, class Ret, class Next2, class... Args, class... InArgs, class... TyArgs>
struct CurryInt<F,true,Ret(Auto,Next2,Args...),void(InArgs...),void(TyArgs...)>{
    struct value{
        template<auto x>
        using let = typename CurryInt<F,is_nontype<Next2>::value,Ret(Next2,Args...),void(InArgs...,typename ArgLit<Auto>::template Arg<x>),void(TyArgs...)>::value;

        using type = Ret(Auto,Next2,Args...);
        using type_ret = Ret;
        using type_arg = Auto;
    };
};

//template<class F, class Ret, class Next2, class... Args, class... InArgs, class... TyArgs>
//struct CurryInt<F,true,Ret(Auto&,Next2,Args...),void(InArgs...),void(TyArgs...)>{
//    struct value{
//        template<auto& x>
//        using let = typename CurryInt<F,is_nontype<Next2>::value,Ret(Next2,Args...),void(InArgs...,typename ArgLit<Auto&>::template Arg<x>),void(TyArgs...)>::value;
//
//        using type = Ret(Auto&,Next2,Args...);
//        using type_ret = Ret;
//        using type_arg = Auto&;
//    };
//};

template<class F, class Ret, class Next, class... InArgs, class... TyArgs>
struct CurryInt<F,false,Ret(Next),void(InArgs...),void(TyArgs...)>{
    struct value{
        template<class x>
        static constexpr Ret let = enable_if_int<std::is_base_of<Next,x>::value,Ret>::template value<RetInt<F,Ret,void(InArgs...),void(TyArgs...,x)>::value>; //enable_if_int<std::is_base_of<Next,x>::value,Ret,F::template value<InArgs::value...,TyArgs...,x>>::value;

        using type = Ret(Next);
        using type_ret = Ret;
        using type_arg = Next;
    };
};

template<class F, class Ret, class Next, class... InArgs, class... TyArgs>
struct CurryInt<F,true,Ret(Next),void(InArgs...),void(TyArgs...)>{
    struct value{
        template<Next x>
        static constexpr Ret let = RetInt<F,Ret,void(InArgs...,typename ArgLit<Next>::template Arg<x>),void(TyArgs...)>::value; //F::template value<InArgs::value...,x,TyArgs...>;

        using type = Ret(Next);
        using type_ret = Ret;
        using type_arg = Next;
    };
};

//template<class F, class Ret, class Next, class... InArgs, class... TyArgs>
//struct CurryInt<F,true,Ret(Next&),void(InArgs...),void(TyArgs...)>{
//    struct value{
//        template<Next& x>
//        static constexpr Ret let = RetInt<F,Ret,void(InArgs...,typename ArgLit<Next&>::template Arg<x>),void(TyArgs...)>::value; //F::template value<InArgs::value...,x,TyArgs...>;
//
//        using type = Ret(Next&);
//        using type_ret = Ret;
//        using type_arg = Next&;
//    };
//};

template<class F, class Ret, class... InArgs, class... TyArgs>
struct CurryInt<F,true,Ret(Auto),void(InArgs...),void(TyArgs...)>{
    struct value{
        template<auto x>
        static constexpr Ret let = RetInt<F,Ret,void(InArgs...,typename ArgLit<Auto>::template Arg<x>),void(TyArgs...)>::value; //F::template value<InArgs::value...,x,TyArgs...>;

        using type = Ret(Auto);
        using type_ret = Ret;
        using type_arg = Auto;
    };
};

//template<class F, class Ret, class... InArgs, class... TyArgs>
//struct CurryInt<F,true,Ret(Auto&),void(InArgs...),void(TyArgs...)>{
//    struct value{
//        template<auto& x>
//        static constexpr Ret let = RetInt<F,Ret,void(InArgs...,typename ArgLit<Auto&>::template Arg<x>),void(TyArgs...)>::value; //F::template value<InArgs::value...,x,TyArgs...>;
//
//        using type = Ret(Auto&);
//        using type_ret = Ret;
//        using type_arg = Auto&;
//    };
//};

template<class F, class Next, class... InArgs, class... TyArgs>
struct CurryInt<F,false,Auto(Next),void(InArgs...),void(TyArgs...)>{
    struct value{
        template<class x>
        static constexpr auto let = enable_if_int<std::is_base_of<Next,x>::value,Auto>::template value<RetInt<F,Auto,void(InArgs...),void(TyArgs...,x)>::value>; //enable_if_int<std::is_base_of<Next,x>::value,Ret,F::template value<InArgs::value...,TyArgs...,x>>::value;

        using type = Auto(Next);
        using type_ret = Auto;
        using type_arg = Next;
    };
};

template<class F, class Next, class... InArgs, class... TyArgs>
struct CurryInt<F,true,Auto(Next),void(InArgs...),void(TyArgs...)>{
    struct value{
        template<Next x>
        static constexpr auto let = RetInt<F,Auto,void(InArgs...,typename ArgLit<Next>::template Arg<x>),void(TyArgs...)>::value; //F::template value<InArgs::value...,x,TyArgs...>;

        using type = Auto(Next);
        using type_ret = Auto;
        using type_arg = Next;
    };
};

template<class F, class... InArgs, class... TyArgs>
struct CurryInt<F,true,Auto(Auto),void(InArgs...),void(TyArgs...)>{
    struct value{
        template<auto x>
        static constexpr auto let = RetInt<F,Auto,void(InArgs...,typename ArgLit<Auto>::template Arg<x>),void(TyArgs...)>::value; //F::template value<InArgs::value...,x,TyArgs...>;

        using type = Auto(Auto);
        using type_ret = Auto;
        using type_arg = Auto;
    };
};

//--------------------------------------------------------------------------------

template<class F, bool isInt, class T, class In, class Ty>
struct CurryType{};

template<class F, class Ret, class Next, class Next2, class... Args, class... InArgs, class... TyArgs>
struct CurryType<F,false,Ret(Next,Next2,Args...),void(InArgs...),void(TyArgs...)>{
    struct value{
        template<class x>
        using let = typename std::enable_if<std::is_base_of<Next,x>::value,typename CurryType<F,is_nontype<Next2>::value,Ret(Next2,Args...),void(InArgs...),void(TyArgs...,x)>::value>::type;

        using type = Ret(Next,Next2,Args...);
        using type_ret = Ret;
        using type_arg = Next;
    };
};

template<class F, class Ret, class Next, class Next2, class... Args, class... InArgs, class... TyArgs>
struct CurryType<F,true,Ret(Next,Next2,Args...),void(InArgs...),void(TyArgs...)>{
    struct value{
        template<Next x>
        using let = typename CurryType<F,is_nontype<Next2>::value,Ret(Next2,Args...),void(InArgs...,typename ArgLit<Next>::template Arg<x>),void(TyArgs...)>::value;

        using type = Ret(Next,Next2,Args...);
        using type_ret = Ret;
        using type_arg = Next;
    };
};

//template<class F, class Ret, class Next, class Next2, class... Args, class... InArgs, class... TyArgs>
//struct CurryType<F,true,Ret(Next&,Next2,Args...),void(InArgs...),void(TyArgs...)>{
//    struct value{
//        template<Next& x>
//        using let = typename CurryType<F,is_nontype<Next2>::value,Ret(Next2,Args...),void(InArgs...,typename ArgLit<Next&>::template Arg<x>),void(TyArgs...)>::value;
//
//        using type = Ret(Next&,Next2,Args...);
//        using type_ret = Ret;
//        using type_arg = Next&;
//    };
//};

template<class F, class Ret, class Next2, class... Args, class... InArgs, class... TyArgs>
struct CurryType<F,true,Ret(Auto,Next2,Args...),void(InArgs...),void(TyArgs...)>{
    struct value{
        template<auto x>
        using let = typename CurryType<F,is_nontype<Next2>::value,Ret(Next2,Args...),void(InArgs...,typename ArgLit<Auto>::template Arg<x>),void(TyArgs...)>::value;

        using type = Ret(Auto,Next2,Args...);
        using type_ret = Ret;
        using type_arg = Auto;
    };
};

//template<class F, class Ret, class Next2, class... Args, class... InArgs, class... TyArgs>
//struct CurryType<F,true,Ret(Auto&,Next2,Args...),void(InArgs...),void(TyArgs...)>{
//    struct value{
//        template<auto& x>
//        using let = typename CurryType<F,is_nontype<Next2>::value,Ret(Next2,Args...),void(InArgs...,typename ArgLit<Auto&>::template Arg<x>),void(TyArgs...)>::value;
//
//        using type = Ret(Auto&,Next2,Args...);
//        using type_ret = Ret;
//        using type_arg = Auto&;
//    };
//};

template<class F, class Ret, class Next, class... InArgs, class... TyArgs>
struct CurryType<F,false,Ret(Next),void(InArgs...),void(TyArgs...)>{
    struct value{
        template<class x>
        using let = typename std::enable_if<std::is_base_of<Next,x>::value,typename RetType<F,Ret,void(InArgs...),void(TyArgs...,x)>::value>::type;

        using type = Ret(Next);
        using type_ret = Ret;
        using type_arg = Next;
    };
};

template<class F, class Ret, class Next, class... InArgs, class... TyArgs>
struct CurryType<F,true,Ret(Next),void(InArgs...),void(TyArgs...)>{
    struct value{
        template<Next x>
        using let = typename RetType<F,Ret,void(InArgs...,typename ArgLit<Next>::template Arg<x>),void(TyArgs...)>::value; //typename F::template value<InArgs::value...,x,TyArgs...>;

        using type = Ret(Next);
        using type_ret = Ret;
        using type_arg = Next;
    };
};

//template<class F, class Ret, class Next, class... InArgs, class... TyArgs>
//struct CurryType<F,true,Ret(Next&),void(InArgs...),void(TyArgs...)>{
//    struct value{
//        template<Next& x>
//        using let = typename RetType<F,Ret,void(InArgs...,typename ArgLit<Next&>::template Arg<x>),void(TyArgs...)>::value; //typename F::template value<InArgs::value...,x,TyArgs...>;
//
//        using type = Ret(Next&);
//        using type_ret = Ret;
//        using type_arg = Next&;
//    };
//};

template<class F, class Ret, class... InArgs, class... TyArgs>
struct CurryType<F,true,Ret(Auto),void(InArgs...),void(TyArgs...)>{
    struct value{
        template<auto x>
        using let = typename RetType<F,Ret,void(InArgs...,typename ArgLit<Auto>::template Arg<x>),void(TyArgs...)>::value; //typename F::template value<InArgs::value...,x,TyArgs...>;

        using type = Ret(Auto);
        using type_ret = Ret;
        using type_arg = Auto;
    };
};

//template<class F, class Ret, class... InArgs, class... TyArgs>
//struct CurryType<F,true,Ret(Auto&),void(InArgs...),void(TyArgs...)>{
//    struct value{
//        template<auto& x>
//        using let = typename RetType<F,Ret,void(InArgs...,typename ArgLit<Auto&>::template Arg<x>),void(TyArgs...)>::value; //typename F::template value<InArgs::value...,x,TyArgs...>;
//
//        using type = Ret(Auto&);
//        using type_ret = Ret;
//        using type_arg = Auto&;
//    };
//};


//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------

template<class F, bool isRetInt, class T>
struct CurryingAux2{};

template<class F, class Ret, class Next, class... Args>
struct CurryingAux2<F,true,Ret(Next,Args...)>{
    using value = typename CurryInt<F,is_nontype<Next>::value,Ret(Next,Args...),void(),void()>::value;
};

template<class F, class Ret, class Next, class... Args>
struct CurryingAux2<F,false,Ret(Next,Args...)>{
    using value = typename CurryType<F,is_nontype<Next>::value,Ret(Next,Args...),void(),void()>::value;
};


template<class F, class T>
struct CurryingAux{};

template<class F, class Ret, class... Args>
struct CurryingAux<F,Ret(Args...)>{
    using value = typename CurryingAux2<F,is_nontype<Ret>::value,Ret(Args...)>::value;
};

template<class F, class T>
using Currying = typename CurryingAux<F,T>::value;

//-----------------------------------------------------------
//-----------------------------------------------------------
//-----------------------------------------------------------

template<class f, class g, bool isfRetNonType, bool isgRetNonType, bool isgArgNonType>
struct compAux{};

template<class f, class g>
struct compAux<f,g,true,true,true> : public Currying<compAux<f,g,true,true,true>,typename f::type_ret(typename g::type_arg)>{
    template<typename g::type_arg x>
    static constexpr typename f::type_ret value = f::template let<g::template let<x>>;
};

template<class f, class g>
struct compAux<f,g,true,true,false> : public Currying<compAux<f,g,true,true,false>,typename f::type_ret(typename g::type_arg)>{
    template<class x>
    static constexpr typename f::type_ret value = f::template let<g::template let<x>>;
};

template<class f, class g>
struct compAux<f,g,true,false,true> : public Currying<compAux<f,g,true,false,true>,typename f::type_ret(typename g::type_arg)>{
    template<typename g::type_arg x>
    static constexpr typename f::type_ret value = f::template let<typename g::template let<x>>;
};

template<class f, class g>
struct compAux<f,g,true,false,false> : public Currying<compAux<f,g,true,false,false>,typename f::type_ret(typename g::type_arg)>{
    template<class x>
    static constexpr typename f::type_ret value = f::template let<typename g::template let<x>>;
};

template<class f, class g>
struct compAux<f,g,false,true,true> : public Currying<compAux<f,g,false,true,true>,typename f::type_ret(typename g::type_arg)>{
    template<typename g::type_arg x>
    using value = typename f::template let<g::template let<x>>;
};

template<class f, class g>
struct compAux<f,g,false,true,false> : public Currying<compAux<f,g,false,true,false>,typename f::type_ret(typename g::type_arg)>{
    template<class x>
    using value = typename f::template let<g::template let<x>>;
};

template<class f, class g>
struct compAux<f,g,false,false,true> : public Currying<compAux<f,g,false,false,true>,typename f::type_ret(typename g::type_arg)>{
    template<typename g::type_arg x>
    using value = typename f::template let<typename g::template let<x>>;
};

template<class f, class g>
struct compAux<f,g,false,false,false> : public Currying<compAux<f,g,false,false,false>,typename f::type_ret(typename g::type_arg)>{
    template<class x>
    using value = typename f::template let<typename g::template let<x>>;
};

template<class f, class g>
using comp = compAux<f,g,is_nontype<typename f::type_ret>::value,is_nontype<typename g::type_ret>::value,is_nontype<typename g::type_arg>::value>;

//-----------------------------------------------------------

template<auto x>
struct toType_impl{};

template<auto x>
using toType = typename toType_impl<x>::value;


template<class T>
struct toNonType_impl{};

template<class T>
constexpr auto toNonType = toNonType_impl<T>::value;

}

//********************************************************************************

namespace std{

    template<class T>
    struct is_base_of<pftl::Type,T>{
        static constexpr bool value = true;
    };

}


#endif // CURRY_H_INCLUDED
