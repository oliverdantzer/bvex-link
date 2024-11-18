You must determine MTU manually, `ping [command-ip] -f -l 1492` and keep lowering num bytes until no fragmentation needed.

TODO: add this https://github.com/marketplace/actions/doxygen-github-pages-deploy-action


Required packages:
vcpkg
cmake
ninja-build

To use this, you must have vcpkg, have $VCPKG_ROOT defined then run `vcpkg install`

`cmake --preset=default`
`cmake --build build`
`./build/main`

Build nanopb headers:
`./generate-nanopb-headers.sh -p ../telemetry-uplink -o ./src/decode_payload/pb_ge
nerated sample.proto`