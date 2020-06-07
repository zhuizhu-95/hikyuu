import pytest
import pickle
from hikyuu import Parameter


def test_Parameter():
    x = Parameter()
    assert x.have("1") == False

    x["1"] = True
    assert x.have("1") == True
    assert x['1'] == True
    x["1"] = False
    assert x['1'] == False
    with pytest.raises(RuntimeError):
        x['1'] = 1

    x['2'] = 10
    assert x['2'] == 10
    x['2'] = 30
    assert x['2'] == 30
    with pytest.raises(RuntimeError):
        x['2'] = True

    x['3'] = 3.1
    assert x['3'] == 3.1
    x['3'] = 5.3
    assert x['3'] == 5.3
    with pytest.raises(RuntimeError):
        x['3'] = 3

    x['4'] = '中国'
    assert x['4'] == '中国'
    x['4'] = 'test'
    assert x['4'] == 'test'
    with pytest.raises(RuntimeError):
        x['4'] = 1

    assert x.have('unsupport') == False
    with pytest.raises(RuntimeError):
        x['unsupport'] = []


def test_Parameter_other_func():
    x = Parameter()
    x.set('1', 2)
    x.set('2', 3)
    assert x['1'] == 2
    assert x['2'] == 3
    assert x.get('1') == 2
    assert x.get('2') == 3

    assert x.get_name_list() == ['1', '2']

    x = Parameter()
    x['int'] = 10
    assert x.type('int') == 'int'
    x['bool'] = True
    assert x.type('bool') == 'bool'
    x['double'] = 0.1
    assert x.type('double') == 'double'
    x['string'] = 'string'
    assert x.type('string') == 'string'


"""
TODO Parameter序列化报异常，暂时无法解决
def test_Parameter_pickle():
    x = Parameter()
    x['bool'] = False
    x['int'] = 3
    x['double'] = 4.5
    x['string'] = 'test string'
    with open("temp", "wb") as f:
        pickle.dump(x, f)

    with open("temp", "rb") as f:
        d = pickle.load(f)

    assert d['bool'] == False
    assert d['int'] == 3
    assert d['double'] == 4.5
    assert d['string'] == 'test string'
"""