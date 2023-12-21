Build Example: TwoCenterIntegral Section in ABACUS
==============


|      CI              | status |
|----------------------|--------|
| conda.recipe         | [![Conda Actions Status][actions-conda-badge]][actions-conda-link] |
| pip builds           | [![Pip Actions Status][actions-pip-badge]][actions-pip-link] |



An example project built with [pybind11](https://github.com/pybind/pybind11)
and scikit-build-core. Python 3.7+ (see older commits for older versions of
Python).

[actions-badge]:           https://github.com/jieli-matrix/two_center_integral/workflows/Tests/badge.svg
[actions-conda-link]:      https://github.com/jieli-matrix/two_center_integral/actions?query=workflow%3AConda
[actions-conda-badge]:     https://github.com/jieli-matrix/two_center_integral/workflows/Conda/badge.svg
[actions-pip-link]:        https://github.com/jieli-matrix/two_center_integral/actions?query=workflow%3APip
[actions-pip-badge]:       https://github.com/jieli-matrix/two_center_integral/workflows/Pip/badge.svg
[actions-wheels-link]:     https://github.com/jieli-matrix/two_center_integral/actions?query=workflow%3AWheels
[actions-wheels-badge]:    https://github.com/jieli-matrix/two_center_integral/workflows/Wheels/badge.svg


Installation
------------

- clone this repository
- `pip install ./two_center_integral`


CI Examples
-----------

There are examples for CI in `.github/workflows`. A simple way to produces
binary "wheels" for all platforms is illustrated in the "wheels.yml" file,
using [`cibuildwheel`][].

License
-------

pybind11 is provided under a BSD-style license that can be found in the LICENSE
file. By using, distributing, or contributing to this project, you agree to the
terms and conditions of this license.

Test call
---------

```python
import two_center_integral as m 
import numpy as np

# set parameters
n = 10
r = np.arange(n) / 10.0
q = 0.1
l = 1
jl = np.zeros(n).astype(np.float64)
# calculate the values of l-th order spherical Bessel function at q*r[ir]
m.sphbesj(n, r, q, l, jl)

```

[`cibuildwheel`]:          https://cibuildwheel.readthedocs.io
