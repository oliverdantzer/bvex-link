#!/bin/bash

# Check if the network interface is provided as an argument
if [ -z "$1" ]; then
  echo "Usage: $0 <network-interface>"
  exit 1
fi

# Define the network interface from the first argument
INTERFACE="$1"

# Remove all qdisc rules from the interface
sudo tc qdisc del dev $INTERFACE root

echo "Removed all qdisc rules from $INTERFACE"