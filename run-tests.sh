set -ev
make buildtype=debug
make buildtype=test
make buildtype=release
zest ./tests/ || more /tmp/zest/output.txt
