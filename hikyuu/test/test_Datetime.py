import pytest
import pickle
from datetime import datetime, date
from hikyuu import Datetime


def test_Datetime_init():
    assert Datetime(None) == Datetime()
    assert Datetime().isNull()
    assert datetime(2001, 1, 2) == Datetime(20010102)
    assert datetime(2001, 1, 2) == Datetime(2001, 1, 2)
    assert datetime(2001, 1, 2) == Datetime("20010102")
    assert datetime(2001, 1, 2) == Datetime("2001-01-02")
    assert datetime(2001, 1, 2) == Datetime("2001-1-2")
    assert datetime(2001, 1, 2) == Datetime("2001-01-02 0:0:0")
    assert datetime(2001, 1, 2) == Datetime("20010102T000000")
    assert datetime(2001, 1, 2, 3, 4) == Datetime(200101020304)
    assert datetime(2001, 1, 2) == Datetime(datetime(2001, 1, 2))
    assert datetime(2001, 1, 2) == Datetime(date(2001, 1, 2))

    with pytest.raises(IndexError):
        Datetime(20010101010101)

    assert datetime(2001, 1, 2, 3, 4, 5, 6007) == Datetime(2001, 1, 2, 3, 4, 5, 6, 7)
    assert datetime(2001, 1, 2, 3, 4, 5) == Datetime("20010102T030405")
    assert datetime(2001, 1, 2, 3, 4, 5, 6007) == Datetime("20010102T030405.006007")
    assert datetime(2020, 5, 24, 10, 21, 24, 637352) == Datetime("2020-05-24 10:21:24.637352")


def test_Datetime_operator():
    assert Datetime(2001, 1, 2) >= datetime(2001, 1, 2)
    assert Datetime(2001, 1, 2) > datetime(2001, 1, 1)
    assert Datetime(2001, 1, 2, 1) >= datetime(2001, 1, 2)
    assert Datetime(2001, 1, 2, 1) > datetime(2001, 1, 2)
    assert Datetime(2001, 1, 2, 1) != datetime(2001, 1, 2)
    assert Datetime(2001, 1, 2, 1) <= datetime(2001, 1, 2, 1)
    assert Datetime(2001, 1, 2, 1) < datetime(2001, 1, 2, 2)


def test_Datetime_hash():
    date_map = {Datetime(2001, 1, 1): 1, Datetime(2001, 2, 1): 2}
    assert date_map[Datetime(2001, 1, 1)] == 1
    assert date_map[Datetime(2001, 2, 1)] == 2


def test_Datetime_pickle():
    d = Datetime.now()
    with open("temp", "wb") as f:
        pickle.dump(d, f)
    with open("temp", "rb") as f:
        x = pickle.load(f)
        assert d == x
