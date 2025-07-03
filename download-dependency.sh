#!/bin/bash
set -e

BOOST_URL="https://archives.boost.io/release/1.82.0/source/boost_1_82_0.tar.gz"

mkdir -p external

curl -L -o external/boost_1_82_0.tar.gz "$BOOST_URL"
tar -xzf external/boost_1_82_0.tar.gz -C external
rm external/boost_1_82_0.tar.gz

echo "Boost 1.82.0 downloaded and extracted to external/boost_1_82_0"
