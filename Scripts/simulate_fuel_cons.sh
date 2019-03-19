#!/bin/bash


cat <<EOF


set frame vin vin: abcde012345678901
set frame ccvs spn: 84 value: 40
set frame eec1 spn: 190 value: 400
set frame lfe spn: 183 value: 0

send frame ccvs interface: $1
send frame eec1 interface: $1
send frame vin interface: $1
send frame lfe interface: $1

EOF

sleep 15

echo "set frame lfe spn: 183 value: 5"

sleep 10

echo "set frame lfe spn: 183 value: 7"

sleep 10
