#include "numerical_radial.h"

#include <algorithm>
#include <cstring>
#include <limits>
#include <memory>
#include <numeric>

#include "constants.h"

using ModuleBase::PI;

NumericalRadial::NumericalRadial(const NumericalRadial& other) :
    symbol_(other.symbol_),
    itype_(other.itype_),
    l_(other.l_),
    izeta_(other.izeta_),
    nr_(other.nr_),
    nk_(other.nk_),
    is_fft_compliant_(other.is_fft_compliant_),
    pr_(other.pr_),
    pk_(other.pk_)
{
    // deep copy
    if (other.rgrid())
    {
        rgrid_ = new double[nr_];
        rvalue_ = new double[nr_];
        std::memcpy(rgrid_, other.rgrid_, nr_ * sizeof(double));
        std::memcpy(rvalue_, other.rvalue_, nr_ * sizeof(double));
    }

    if (other.kgrid())
    {
        kgrid_ = new double[nk_];
        kvalue_ = new double[nk_];
        std::memcpy(kgrid_, other.kgrid_, nk_ * sizeof(double));
        std::memcpy(kvalue_, other.kvalue_, nk_ * sizeof(double));
    }
}

NumericalRadial& NumericalRadial::operator=(const NumericalRadial& rhs)
{
    if (this == &rhs)
    {
        return *this;
    }

    // wipe off r & k space data
    wipe(true, true);

    symbol_ = rhs.symbol_;
    itype_ = rhs.itype_;
    izeta_ = rhs.izeta_;
    l_ = rhs.l_;

    nr_ = rhs.nr_;
    nk_ = rhs.nk_;

    is_fft_compliant_ = rhs.is_fft_compliant_;

    pr_ = rhs.pr_;
    pk_ = rhs.pk_;

    // deep copy
    if (rhs.rgrid())
    {
        rgrid_ = new double[nr_];
        rvalue_ = new double[nr_];
        std::memcpy(rgrid_, rhs.rgrid_, nr_ * sizeof(double));
        std::memcpy(rvalue_, rhs.rvalue_, nr_ * sizeof(double));
    }

    if (rhs.kgrid())
    {
        kgrid_ = new double[nk_];
        kvalue_ = new double[nk_];
        std::memcpy(kgrid_, rhs.kgrid_, nk_ * sizeof(double));
        std::memcpy(kvalue_, rhs.kvalue_, nk_ * sizeof(double));
    }

    return *this;
}

NumericalRadial::~NumericalRadial()
{
    delete[] rgrid_;
    delete[] kgrid_;
    delete[] rvalue_;
    delete[] kvalue_;
}

void NumericalRadial::build(const int l,
                            const bool for_r_space,
                            const int ngrid,
                            const double* const grid,
                            const double* const value,
                            const int p,
                            const int izeta,
                            const std::string symbol,
                            const int itype,
                            const bool init_sbt)
{
#ifdef __DEBUG
    assert(l >= 0);
    assert(ngrid > 1);
    assert(grid && value);

    // grid must be strictly increasing and every element must be non-negative
    assert(std::is_sorted(grid, grid + ngrid, std::less_equal<double>())); // using less_equal forbids equal values
    assert(grid[0] >= 0.0);
#endif

    // wipe off any existing r & k space data
    wipe(true, true);

    symbol_ = symbol;
    itype_ = itype;
    izeta_ = izeta;
    l_ = l;


    if (for_r_space)
    {
        nr_ = ngrid;
        pr_ = p;
        rgrid_ = new double[nr_];
        rvalue_ = new double[nr_];
        std::memcpy(rgrid_, grid, nr_ * sizeof(double));
        std::memcpy(rvalue_, value, nr_ * sizeof(double));
    }
    else
    {
        nk_ = ngrid;
        pk_ = p;
        kgrid_ = new double[nk_];
        kvalue_ = new double[nk_];
        std::memcpy(kgrid_, grid, nk_ * sizeof(double));
        std::memcpy(kvalue_, value, nk_ * sizeof(double));
    }

}


void NumericalRadial::set_grid(const bool for_r_space, const int ngrid, const double* const grid, const char mode)
{
#ifdef __DEBUG
    assert(mode == 'i' || mode == 't');
    assert(ngrid > 1);

    // grid must be strictly increasing and every element must be non-negative
    assert(std::is_sorted(grid, grid + ngrid, std::less_equal<double>())); // using less_equal forbids equal values
    assert(grid[0] >= 0.0);
#endif

    // tbu stands for "to be updated"
    double*& grid_tbu = (for_r_space ? rgrid_ : kgrid_);
    double*& value_tbu = (for_r_space ? rvalue_ : kvalue_);
    int& ngrid_tbu = (for_r_space ? nr_ : nk_);

    if (mode == 't')
    { // obtain new values by a transform from the other space
        // make sure a transform from the other space is available
#ifdef __DEBUG
        assert(for_r_space ? (kgrid_ && kvalue_) : (rgrid_ && rvalue_));
#endif

        delete[] grid_tbu;
        delete[] value_tbu;
        grid_tbu = new double[ngrid];
        value_tbu = new double[ngrid];
        ngrid_tbu = ngrid;
        std::memcpy(grid_tbu, grid, ngrid * sizeof(double));

        is_fft_compliant_ = is_fft_compliant(nr_, rgrid_, nk_, kgrid_);
    }
    else
    { // obtain new values by interpolation in the current space
        // make sure an interpolation in the current space is available
#ifdef __DEBUG
        assert(grid_tbu && value_tbu);
#endif

        

        double* grid_new = new double[ngrid];
        double* value_new = new double[ngrid];

        std::memcpy(grid_new, grid, ngrid * sizeof(double));
        std::fill_n(value_new, ngrid, 0.0);

        // do interpolation for grid points within the range of the origional grid
        // for grid points outside the original range, simply set the values to zero

        // grid_start is the first grid point that is greater than or equal to grid_tbu[0]
        double* grid_start = std::lower_bound(grid_new, grid_new + ngrid, grid_tbu[0]);

        // grid_end is the first grid point that is strictly greater than grid_tbu[ngrid_tbu-1]
        double* grid_end = std::upper_bound(grid_new, grid_new + ngrid, grid_tbu[ngrid_tbu - 1]);


        delete[] grid_tbu;
        delete[] value_tbu;

        grid_tbu = grid_new;
        value_tbu = value_new;
        ngrid_tbu = ngrid;

        is_fft_compliant_ = is_fft_compliant(nr_, rgrid_, nk_, kgrid_);
    }
}

void NumericalRadial::set_uniform_grid(const bool for_r_space,
                                       const int ngrid,
                                       const double cutoff,
                                       const char mode,
                                       const bool enable_fft)
{
    double* grid = new double[ngrid];
    double dx = cutoff / (ngrid - 1);
    for (int i = 0; i != ngrid; ++i)
    {
        grid[i] = i * dx;
    }

    set_grid(for_r_space, ngrid, grid, mode);
    delete[] grid;

    if (enable_fft)
    {
        set_uniform_grid(!for_r_space, ngrid, PI / dx, 't', false);
    }
}

void NumericalRadial::set_value(const bool for_r_space, const double* const value, const int p)
{
#ifdef __DEBUG
    assert(for_r_space ? rvalue_ : kvalue_);
#endif
    if (for_r_space)
    {
        std::memcpy(rvalue_, value, nr_ * sizeof(double));
        pr_ = p;
    }
    else
    {
        std::memcpy(kvalue_, value, nk_ * sizeof(double));
        pk_ = p;
    }
}

void NumericalRadial::wipe(const bool r_space, const bool k_space)
{
#ifdef __DEBUG
    assert(r_space || k_space);
#endif

    // wipe the grid and value in r/k space
    if (r_space)
    {
        delete[] rgrid_;
        delete[] rvalue_;
        rgrid_ = nullptr;
        rvalue_ = nullptr;
        nr_ = 0;
        pr_ = 0;
    }
    
    if (k_space)
    {
        delete[] kgrid_;
        delete[] kvalue_;
        kgrid_ = nullptr;
        kvalue_ = nullptr;
        nk_ = 0;
        pk_ = 0;
    }
    is_fft_compliant_ = false;
}




bool NumericalRadial::is_fft_compliant(const int nr,
                                       const double* const rgrid,
                                       const int nk,
                                       const double* const kgrid) const
{
    if (!rgrid || !kgrid || nr != nk || nr < 2)
    {
        return false;
    }

    double dr = rgrid[nr - 1] / (nr - 1);
    double dk = kgrid[nk - 1] / (nk - 1);
    double tol = 4.0 * std::numeric_limits<double>::epsilon();

    return std::abs(dr * dk - PI / (nr - 1)) < tol
           && std::all_of(rgrid, rgrid + nr, [&](const double& r) { return std::abs(r - (&r - rgrid) * dr) < tol; })
           && std::all_of(kgrid, kgrid + nk, [&](const double& k) { return std::abs(k - (&k - kgrid) * dk) < tol; });
}
