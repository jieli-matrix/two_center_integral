#include "math_sphbes.h"
#include <cassert>
#include <cmath>

double _sphbesj_ascending_recurrence(int l, double x) {

    // should be used when x > l && l > 0
    
    double invx = 1.0 / x;
    double j0 = std::sin(x) * invx;
    double j1 = ( j0 - std::cos(x) ) * invx;

    double jl;
    for (int i = 2; i <= l; ++i) {
        jl = (2*i-1) * invx * j1 - j0;
        j0 = j1;
        j1 = jl;
    }

    return j1; // at the end of the loop j1 == jl
}

double _sphbesj_series(int l, double x) {

    // should be used when x < l

    // the absolute ratio between the k-th and (k-1)-th terms is x^2 / k / (4l+4k+2) (k >= 1)
    // terms are guaranteed to be monotonically decreasing from the beginning for x < sqrt(4l+6).
    // terms are guaranteed to be monotonically decreasing from the (l/4)-th term for x < l

    double jl = 0.0;
    constexpr double eps = 1e-17; // series terminate when the k-th term is less than eps * jl

    // zeroth order term: x^l / (2l+1)!!
    int k = 0;
    double kth_term = 1.0;
    for (int i = 1; i <= l; ++i) {
        kth_term *= x / (2 * i + 1);
    }

    double x_sqr_half = 0.5 * x * x;
    do {
        jl += kth_term;
        k += 1;
        kth_term *= -x_sqr_half / ( k * (2*(l+k)+1) );
    } while ( std::abs(kth_term) > std::abs(eps * jl) );

    return jl;
}

double sphbesj(const int l, const double x)
{
    assert( l >= 0 );
    assert( x >= 0 );

    // j_l(0)
    if ( x == 0 )
    {
        return l ? 0.0 : 1.0;
    }

    if ( x < l )
    {
        return _sphbesj_series(l, x);
    }
    else
    {
        double invx = 1.0 / x;
        switch (l)
        {
          case 0:
            return std::sin(x) * invx;
          case 1:
            return ( std::sin(x) * invx - std::cos(x) ) * invx; 
            // NOTE: the following explicit expressions are not necessarily faster than ascending recurrence,
            // but we keep them just in case we need them in the future.
          //case 2:
          //  return ( (3.0 * invx  - x) * std::sin(x) - 3.0 * std::cos(x) ) * (invx * invx);
          //case 3:
          //  return ( std::sin(x) * (15.0 * invx - 6.0 * x) + std::cos(x) * (x * x - 15.0) ) * std::pow(invx, 3); 
          //case 4:
          //  return ( std::sin(x) * (std::pow(x,3) - 45.0 * x + 105.0 * invx) 
          //          + std::cos(x) * (10.0 * x * x - 105.0) ) * std::pow(invx, 4);
          //case 5:
          //  return ( std::sin(x) * (15.0 * std::pow(x,3) - 420.0 * x + 945.0 * invx) 
          //          + std::cos(x) * (-std::pow(x, 4) + 105.0 * x * x - 945.0) ) * std::pow(invx, 5);
          //case 6:
          //  return ( std::sin(x) * (-std::pow(x, 5) + 210.0 * std::pow(x, 3) - 4725.0 * x + 10395.0 * invx) 
          //          + std::cos(x) * (-21.0 * std::pow(x, 4) + 1260.0 * x * x - 10395.0) ) * std::pow(invx, 6);
          default:
            return _sphbesj_ascending_recurrence(l, x);
        }
    }
}

double dsphbesj(const int l, const double x)
{
    assert( l >= 0 );
    assert( x >= 0 );
    return l == 0 ? -sphbesj(1, x) : ( l * sphbesj(l - 1, x) - (l + 1) * sphbesj(l + 1, x) ) / (2 * l + 1);
}