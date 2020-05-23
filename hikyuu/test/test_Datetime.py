import pytest
from datetime import datetime, date
from hikyuu import Datetime


def test_Datetime_init():
    assert datetime(2001, 1, 2) == Datetime(20010102).datetime()
    assert datetime(2001, 1, 2) == Datetime(2001, 1, 2).datetime()
    assert datetime(2001, 1, 2) == Datetime("20010102").datetime()
    assert datetime(2001, 1, 2) == Datetime("2001-01-02").datetime()
    assert datetime(2001, 1, 2) == Datetime("2001-1-2").datetime()
    assert datetime(2001, 1, 2) == Datetime("2001-01-02 0:0:0").datetime()
    assert datetime(2001, 1, 2) == Datetime("20010102T000000").datetime()
    assert datetime(2001, 1, 2, 3, 4) == Datetime(200101020304).datetime()
