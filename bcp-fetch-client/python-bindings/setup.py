from setuptools import setup, Extension
from Cython.Build import cythonize

# Reuse README as long description
with open('README.md', 'r') as f:
    readme = f.read()


# Actual foo wrapper written in Cython
bcp_fetch_extension = Extension(
    'bcp_fetch.bcp_fetch_ext',
    language='c++',
    sources=['src/bcp_fetch/bcp_fetch_ext.pyx'],
    libraries=['bcp_fetch']
)


# Python installable package
setup(
    name='bcp-fetch',
    version='0.0.1',
    description='Wrapper to native `bcp-fetch` library',
    long_description=readme,
    long_description_content_type='text/markdown',
    author='Martin Kloesch',
    author_email='martin.kloesch@gmail.com',
    url='https://github.com/oliverdantzer/bvex-link/tree/main/bcp-fetch-client/python-bindings',
    packages=['bcp_fetch'],
    ext_modules=cythonize(
        bcp_fetch_extension,
        language_level=3.6,
        compiler_directives={"linetrace": True} # Opt-in via CYTHON_TRACE macro
    ),
    package_dir={'': 'src'},
    package_data={
        'bcp_fetch': ['bcp_fetch_ext.pxd', 'c_bcp_fetch.pxd']
    },
    include_package_data=True,
    setup_requires=[
        'cython >= 0.22.1',
        'pytest-runner',
    ],
    tests_require=['pytest'],
    zip_safe=False,
    # cmdclass=_cmdclass,
    keywords=[
        'bcp-fetch',
        'cmake',
        'cython'
    ],
    classifiers=[
        'Development Status :: 3 - Alpha',
        'Environment :: Console',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: MIT License',
        'Natural Language :: English',
        'Operating System :: OS Independent',
        'Programming Language :: Cython',
        'Programming Language :: C',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: Implementation :: CPython',
        'Topic :: Software Development :: Libraries',
        'Topic :: Utilities',
        'Typing :: Typed'
    ],
    license='MIT'
)
