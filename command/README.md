## Ubuntu installation

[Install Python version 3.13](../python-instructions.md)

Create virtual env, install requirements
`python3.13 -m venv venv
source venv/bin/activate
python -m pip install -r requirements.txt
`

[Start redis server](../redis-instructions.md)

Running
```bash
python run.py
```

src/generated/nano_pb2 can be built by running make in https://github.com/nanopb/nanopb/tree/master/generator/proto
OR downloading prebuilt nanopb binaries

Build nanopb headers:
```bash
protoc -I/home/oliver/devtools/nanopb/generator -I/home/oliver/devtools/nanopb/generator/proto -I/home/oliver/devtools/nanopb/generator/proto/google/protobuf -I../shared --python_out=./src/generated --pyi_out=./src/generated sample.proto nanopb.proto
```