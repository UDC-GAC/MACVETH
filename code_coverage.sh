#!/bin/bash

cd build

{ read x y z version ; } < <(lcov --version)
if (( $version < 1.14 )) then
    mkdir tmp && cd tmp
    wget http://mirrors.kernel.org/ubuntu/pool/universe/l/lcov/lcov_1.14-2_all.deb
    sudo dpkg -i lcov_1.14-2_all.deb
    if (( $? != 0 )) then
        sudo apt --fix-broken install -y
        sudo dpkg -i lcov_1.14-2_all.deb
    fi
    cd ..
    rm -Rf tmp
fi
# Create lcov report: capture coverage info
lcov --directory . --capture --output-file coverage.info --gcov-tool gcov-9
# filter out system and extra files.
# To also not include test code in coverage add them with full path to the patterns: '*/tests/*'
lcov --remove coverage.info '/usr/*' "${HOME}"'/.cache/*' --output-file coverage.info
# output coverage data for debugging (optional)
lcov --list coverage.info
# Uploading to CodeCov
# '-f' specifies file(s) to use and disables manual coverage gathering and
# file search which has already been done above
# URL: https://codecov.io/gh/markoshorro/MACVETH/branch/develop
bash <(curl -s https://codecov.io/bash) -t 43267e87-5aeb-41fd-b0c9-d7c3d2e2237a -f coverage.info || echo "Codecov didnot collect coverage reports"
