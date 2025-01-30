



## Building

### Requirements

- `vcpkg`, `$VCPKG_ROOT` defined
<!-- - Ninja : `apt install ninja-build` -->

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
Fails for some reason:
```bash
../shared/generate-nanopb-headers.sh --output-dir=./src/generated/nanopb sample.proto request.proto response.proto
```
Works:
```bash
~/devtools/nanopb/generator/nanopb_generator.py --output-dir=./src/generated/nanopb sample.proto request.proto response.proto
```