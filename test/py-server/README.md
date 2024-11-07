## Compile protocol buffers
```bash
protoc \
    --python_out=./generated/ \
    --pyi_out=./generated/ \
    sample.proto
```
The pyi is necessary for IDEs to understand the classes.
For installing protoc, I had to install pre-built binaries instead of using package manager for it to work

# create and activate venv
```bash
python3 -m venv venv && source ./venv/bin/activate && pip install -r requirements.txt
```

