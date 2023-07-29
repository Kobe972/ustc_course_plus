#!/bin/bash

/headless/sponge/tap.sh start
service apache2 start
echo "QWx0NhMPkoM/bJr/ohvHXlviFhOyYrYb+qqdOnwLYo4"  > /var/www/html/xyzzy
WEB_HASH=`./apps/webget 169.254.10.1 /xyzzy | tee /dev/stderr | tail -n 1`
CORRECT_HASH="QWx0NhMPkoM/bJr/ohvHXlviFhOyYrYb+qqdOnwLYo4"

if [ "${WEB_HASH}" != "${CORRECT_HASH}" ]; then
    echo ERROR: webget returned output that did not match the test\'s expectations
    exit 1
fi
exit 0
