



## Building

### Requirements

- `vcpkg`, `$VCPKG_ROOT` defined
- Ninja : `apt install ninja-build`

### Steps

- `vcpkg install`
- `cmake --preset=vcpkg`
- `cmake --build build`

## Consuming `telemetry-uplink` library

- Requires transitive dependency `nanopb`
- Easiest way to do this is with `vcpkg` and `cmake`
    - [Integrate consumer project with vcpkg](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-bash) and add `nanopb` to your manifest dependencies
    - Add this project's directory with `add_subdirectory`
    - You can now link `telemetry-uplink` to your targets

## Generating new nanopb headers
- [Download nanopb binaries](https://jpa.kapsi.fi/nanopb/download/)
- Extract to appropriate folder
- I put mine in `~/devtools/`
```bash
$ ./generate_nanopb_headers ~/devtools/nanopb
```
