# BBB
### Troubleshooting

#### Ubuntu

##### Can't SSH and can't ping 192.168.7.1 nor 192.168.7.2
Check the `dmesg` output. If you get something like: 
```
IPv6: ADDRCONF(NETDEV_UP): enxc8a030b54d37: link is not ready
```
Disabling IPv6 might solve the problem: 
- `vim /etc/sysctl.conf`
- Add `net.ipv6.conf.<Target Interface>.disable_ipv6 = 1` and save
- Verify changes `cat /proc/sys/net/ipv6/conf/<Target Interface>/disable_ipv6` must output 1

Reference: http://askubuntu.com/questions/440649/how-to-disable-ipv6-in-ubuntu-14-04
