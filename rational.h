#ifndef RATIONAL_H_INCLUDED
#define RATIONAL_H_INCLUDED

#include "curry.h"
#include "ord.h"
#include "show.h"
#include "container.h"
#include <ratio>

namespace pftl{


struct Rational_t : public Ord_t{

    template<class R>
    using plusT = Undefined;//typename Ratio_plusT<Ratio<n,d>,R>::value;

    template<class R>
    using minusT = Undefined;

    template<class R>
    using multT = Undefined;

    template<class R>
    using divT = Undefined;

    using reduce = Undefined;

    constexpr static const float toFloat = 0.0f;

};


//-------------------
//-------------------
//-------------------


template<intmax_t n, intmax_t d = 1>
struct Ratio : public Rational_t{

    static const intmax_t num = n;
    static const intmax_t den = d;

    template<class R>
    static const bool equalT = n*R::den == d*R::num;//Ratio_equalT<Ratio<n,d>,R>::value;

    template<class R>
    using plusT = typename Ratio<n*R::den+d*R::num,d*R::den>::reduce;//typename Ratio_plusT<Ratio<n,d>,R>::value;

    template<class R>
    using minusT = typename Ratio<n*R::den-d*R::num,d*R::den>::reduce;

    template<class R>
    using multT = typename Ratio<n*R::num,d*R::den>::reduce;

    template<class R>
    using divT = typename Ratio<n*R::den,d*R::num>::reduce;

    using reduce = Ratio<std::ratio<n,d>::num,std::ratio<n,d>::den>;

    constexpr static const float toFloat = (float)n/(float)d;

    using show = Vector<'n','/','d'>;

};

//-------------------

struct reduce : public Currying<reduce,Type(Rational_t)>{
    template<class R>
    using value = typename R::reduce;
};


}

#endif // RATIONAL_H_INCLUDED
