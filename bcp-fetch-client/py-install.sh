cmake --preset debug
cmake --build build
cd python-bindings
python -m pip install .