set -ev
unset on_error
[ ./tests/-wc.c -nt /tmp/wc ] && gcc -Wall -Werror ./tests/-wc.c -static -o /tmp/wc
[ ./tests/-hexdump.c -nt /tmp/hexdump ] && gcc -Wall -Werror ./tests/-hexdump.c -static -o /tmp/hexdump
make buildtype=debug
make buildtype=test
make buildtype=release
sudo /home/zander/bin/zest ./tests -v
