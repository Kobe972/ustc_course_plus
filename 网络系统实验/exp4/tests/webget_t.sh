#!/bin/bash

service apache2 restart
echo "QWx0NhMPkoM/bJr/ohvHXlviFhOyYrYb+qqdOnwLYo4"  > /var/www/html/xyzzy
WEB_HASH=`./apps/webget 127.0.0.1 /xyzzy | tee /dev/stderr | tail -n 1`
CORRECT_HASH="QWx0NhMPkoM/bJr/ohvHXlviFhOyYrYb+qqdOnwLYo4"

if [ "${WEB_HASH}" != "${CORRECT_HASH}" ]; then
    echo ERROR: webget returned output that did not match the test\'s expectations
    exit 1
fi
exit 0
