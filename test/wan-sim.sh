#!/bin/bash

# Check if the required arguments are provided
if [ $# -lt 3 ]; then
  echo "Usage: $0 <network-interface> <latency> <packet-loss>"
  exit 1
fi

# Define the network interface, latency, and packet loss from the arguments
INTERFACE="$1"
LATENCY="$2"
PACKET_LOSS="$3"

# Function to remove all qdisc rules
cleanup() {
  echo -e "\nExit: Removing all qdisc rules from $INTERFACE"
  sudo tc qdisc del dev $INTERFACE root
  exit 0
}

# Trap CTRL+C (SIGINT) and call the cleanup function
trap cleanup SIGINT

# Add specified latency and packet loss
sudo tc qdisc add dev $INTERFACE root netem delay ${LATENCY}ms loss ${PACKET_LOSS}%
echo "Added ${LATENCY}ms latency and ${PACKET_LOSS}% packet loss to $INTERFACE"

# Run an infinite loop to keep the script running
while true; do
  sleep 1
done