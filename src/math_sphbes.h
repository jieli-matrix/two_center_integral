//! spherical Bessel function of the first kind
/*!
 *  This function computes j_l(x) by series expansion for x < l
 *  and by ascending recurrence for x >= l.
 *                                                              */
double sphbesj(const int l,   //!< [in] order
               const double x //!< [in] argument
);

//! derivative of spherical Bessel function
double dsphbesj(const int l,   //!< [in] order
                const double x //!< [in] argument
);

//! computes the values of l-th order spherical Bessel function at q*r[ir]
void sphbesj(const int n,           //!< [in] number of r grid points
             const double *const r, //!< [in] r grid
             const double q,        //!< [in] wave vector
             const int l,     //!< [in] order of the spherical Bessel function
             double *const jl //!< [out] results
);

//! computes the derivative of l-th order spherical Bessel function at q*r[ir]
void dsphbesj(const int n,           //!< [in] number of r grid points
              const double *const r, //!< [in] r grid
              const double q,        //!< [in] wave vector
              const int l,      //!< [in] order of the spherical Bessel function
              double *const djl //!< [out] results
);

// utility functions for sphbesj
double _sphbesj_ascending_recurrence(int l, double x);
double _sphbesj_series(int l, double x);
