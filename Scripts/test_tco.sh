#!/bin/bash


echo "set frame tco1 spn: 1615 value: 0"
echo "set frame tco1 spn: 1616 value: 0"

echo "set frame tco1 spn: 1612 value: 7"
echo "set frame tco1 spn: 1613 value: 7"

echo "send frame tco1 interface: $1"


echo "set frame tco1 spn: 1615 value: 1"	#Card 1 On

sleep 45

echo "set frame tco1 spn: 1612 value: 0"	#Driver 1 Rest

sleep 2

echo "set frame tco1 spn: 1612 value: 1"	#Driver 1 available

sleep 2

echo "set frame tco1 spn: 1612 value: 2"	#Driver 1 work

sleep 2

echo "set frame tco1 spn: 1612 value: 3"	#Driver 1 drive

sleep 2

echo "set frame tco1 spn: 1615 value: 0"	#Card 1 Off

sleep 2

echo "set frame tco1 spn: 1616 value: 1"	#Card 2 On

sleep 45

echo "set frame tco1 spn: 1613 value: 0"	#Driver 2 Rest

sleep 2

echo "set frame tco1 spn: 1613 value: 1"	#Driver 2 available

sleep 2

echo "set frame tco1 spn: 1613 value: 2"	#Driver 2 work

sleep 2

echo "set frame tco1 spn: 1616 value: 0"	#Card 2 Off

sleep 5
