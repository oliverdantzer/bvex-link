You must determine MTU manually, `ping [command-ip] -f -l 1492` and keep lowering num bytes until no fragmentation needed.

To use this, you must have vcpkg, have $VCPKG_ROOT defined then run vcpkg install
`cmake --preset=default`
`cmake --build build`
`./build/onboard-link`