import pytest
import pickle
from hikyuu import *


class AddIndicator(IndicatorImp):
    def __init__(self, indicator):
        super(AddIndicator, self).__init__("AddIndicator")
        self._ready_buffer(indicator.size(), 1)
        for i in range(len(indicator)):
            self._set(indicator[i] + 1, i)
        self.value = 1
        self.ind = indicator

    def check(self):
        print("check")
        return True

    def _calculate(self, ind):
        print("******")
        for i in range(len(ind)):
            self._set(ind[i] + 1, i)

    def _clone(self):
        print("=======")
        p = AddIndicator(self.ind)
        p.value = self.value + 1
        return p


def test_PythonIndicator():
    a = [0, 1, 2, 3]
    x = PRICELIST(a)
    m = Indicator(AddIndicator(x))
    assert m.name == "AddIndicator"
    assert len(m) == 4
    assert m.empty() == False
    assert abs(m[0] - 1) < 0.0001
    assert abs(m[1] - 2) < 0.0001
    assert abs(m[2] - 3) < 0.0001
    assert abs(m[3] - 4) < 0.0001

    b = [1, 2, 3, 4]
    x = PRICELIST(b)
    m = m(x)
    assert m.size() == 4
    assert m.empty() == False
    assert abs(m[0] - 2) < 0.0001
    assert abs(m[1] - 3) < 0.0001
    assert abs(m[2] - 4) < 0.0001
    assert abs(m[3] - 5) < 0.0001
