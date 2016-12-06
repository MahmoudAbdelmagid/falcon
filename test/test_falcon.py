import pytest

import io
import os
from contextlib import redirect_stderr
from contextlib import redirect_stdout

from falcon.flyer import Flyer

@pytest.fixture
def testFlyer():
    return Flyer(mode='test')

@pytest.fixture
def submitFlyer():
    return Flyer(mode='submit')

@pytest.fixture
def debugFlyer():
    return Flyer(debug=True)

@pytest.fixture
def localFlyer():
    return Flyer(local=True)

def capture_stderr(exceptionType, func):
    f = io.StringIO()
    with redirect_stderr(f), pytest.raises(exceptionType):
        func()
    return f.getvalue()

def capture_stdout(func):
    f = io.StringIO()
    with redirect_stdout(f):
        func()
    return f.getvalue()

def test_constructs(testFlyer):
    assert testFlyer.mode == 'test'

def test_err_stored(testFlyer):
    step = 'testing'
    msg = 'testing!'
    testFlyer.generate_err(step, Exception(msg))
    assert isinstance(testFlyer.errs[step], Exception)
    assert str(testFlyer.errs[step]) == msg

def test_err_displayed(debugFlyer):
    step = 'testing'
    msg = 'testing!'

    def make_an_err():
        debugFlyer.generate_err(step, Exception(msg))

    err = capture_stderr(Exception, make_an_err)
    assert msg in err

def test_out_stored(submitFlyer):
    step = 'submitting'
    msg = '{"sample": "json"}'
    submitFlyer.generate_output(step, msg)
    assert submitFlyer.outs[step] == msg

def test_out_displayed(debugFlyer):
    step = 'submitting'
    msg = '{"sample": "json"}'

    def make_output():
        debugFlyer.generate_output(step, msg)

    out = capture_stdout(make_output)
    assert msg in out

def test_set_env_var(debugFlyer):
    key = 'TEST'
    value = 'VALUE'
    debugFlyer.set_env_var(key, value)
    assert os.getenv(key) == value

def test_set_env_vars(debugFlyer):
    evars = [{'key': 'value'}, {'key2': 'value2'}]
    debugFlyer.set_env_vars(evars)
    assert os.getenv('key') == 'value'
    assert os.getenv('key2') == 'value2'