set -ev
unset on_error
make buildtype=debug
make buildtype=test
make buildtype=release
zest ./tests -v
