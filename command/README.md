Create virtual env
`python3 -m venv venv`

Activate venv Linux
`source ./venv`

Activating virtual env on Windows
`source ./win-venv.sh`

src/generated/nano_pb2 can be built by running make in https://github.com/nanopb/nanopb/tree/master/generator/proto
OR downloading prebuilt nanopb binaries

Build nanopb headers:
```bash
protoc -I/home/oliver/devtools/nanopb/generator -I/home/oliver/devtools/nanopb/generator/proto -I/home/oliver/devtools/nanopb/generator/proto/google/protobuf -I../shared --python_out=./src/generated --pyi_out=./src/generated sample.proto nanopb.proto
```