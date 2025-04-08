# Python Wrapper for native `bcp-fetch` C Library

This library offers [Python 3](https://www.python.org) bindings for the bcp-fetch C library.


## Example

```python
import foo

# This will also instantiate the native struct
instance = foo.Foo()

# This will wrap to the native library functions
instance.foob()
print(f"The foo's baz is {instance.baz}")
```


## Build and Installation

The build uses [cython](https://cython.org) along with [setuptools](https://setuptools.readthedocs.io/en/latest) and requires `Python >= 3.6`.

It expects the bcp-fetch library to be installed and findable by the `Python` interpreter.

```
python3 setup.py build_ext
python3 setup.py install
```

To simplify the build process the library is included in the `cmake` targets for the [foo project](https://github.com/kmhsonnenkind/cmake-cython-example).

Follow the build steps in the mentioned repository for an easy way to get the library and all its dependencies to build and run.


## Tests

The Python bindings are tested using [pytest](https://docs.pytest.org/). They can be found in the *tests* directory and triggered using *setup.py*'s `pytest` target.

```sh
python3 setup.py pytest
```
