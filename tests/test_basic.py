from __future__ import annotations
import pytest
import two_center_integral as m
np = pytest.importorskip("numpy")


def test_version():
    assert m.__version__ == "0.0.1"


def test_setData():
    a = m.MyClassWrapper()
    a.setData(np.arange(10).astype(np.int32), 10)
    assert a.displayData(9) == 9

