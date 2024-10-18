
## Create TUN device named tun0 in my user space
`sudo ip tuntap add dev tun0 mode tun user $(whoami)`
`sudo ip link set tun0 up`

## Give the interface in IP address
`sudo ip addr add 10.0.0.1/24 dev tun0`

## Forward traffic from tun0 to eth0
`sudo iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE`
`sudo iptables -A FORWARD -i tun0 -o eth0 -j ACCEPT`
`sudo iptables -A FORWARD -i eth0 -o tun0 -m state --state RELATED,ESTABLISHED -j ACCEPT`

