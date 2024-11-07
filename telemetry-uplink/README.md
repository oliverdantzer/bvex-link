

## Requirements

- `vcpkg`, `$VCPKG_ROOT` defined

## Install dependencies

- `vcpkg install`

## Building and running
Requires ninja
`apt install ninja-build`

- `cmake --preset=vcpkg`
- `cmake --build build`
- `./build/main`



## Generating new nanopb headers
- [Download nanopb binaries](https://jpa.kapsi.fi/nanopb/download/)
- Extract to appropriate folder
- I put mine in `~/devtools/`
```bash
$ ./generate_nanopb_headers ~/devtools/nanopb
```
