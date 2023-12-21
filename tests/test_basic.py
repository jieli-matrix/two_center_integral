from __future__ import annotations

import two_center_integral as m

def test_sphbesj():
    assert m.sphbesj(0, 0) == 1.0
    assert m.sphbesj(1, 0) == 0.0

def test_dsphbesj():
    assert m.dsphbesj(0, 0) == -0.0
