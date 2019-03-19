#!/bin/bash

echo "create frame name: dm1 title: DM1"

echo "set frame dm1 period: 1000 source: 10"

echo "add dtc dm1 oc: 3 fmi: 5 spn: 52"
echo "add dtc dm1 oc: 4 fmi: 6 spn: 78"
echo "add dtc dm1 oc: 0 fmi: 7 spn: 145"

echo "send frame dm1 interface: $1"

sleep 5

echo "unsend frame dm1"

echo "set dtc dm1 2 oc: 3 fmi: 7 spn: 145"

echo "send frame dm1 interface: $1"

sleep 5

echo "unsend frame dm1"

echo "delete dtc dm1 2"

echo "send frame dm1 interface: $1"

sleep 10
