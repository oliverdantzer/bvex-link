# Method 1, failed
## Create TUN device named tun0 in my user space
`sudo ip tuntap add dev tun0 mode tun user $(whoami)`
`sudo ip link set tun0 up`

## Give the interface in IP address
`sudo ip addr add 10.0.0.1/24 dev tun0`

## Forward traffic from tun0 to eth0
`sudo iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE`
`sudo iptables -A FORWARD -i tun0 -o eth0 -j ACCEPT`
`sudo iptables -A FORWARD -i eth0 -o tun0 -m state --state RELATED,ESTABLISHED -j ACCEPT`

# Method 2
## Add interface eth10
`sudo ip link add eth10 type dummy`
## Set state to up
`ip link set eth1 up`
## Create a new bridge and change its state to up:
`ip link add name bridge_name type bridge`
## Adding the interface into the bridge is done by setting its master to bridge_name
`ip link set dev bridge_name up`
`ip link set eth1 master bridge_name`
## Remove
`sudo ip link delete eth10 type dummy`
