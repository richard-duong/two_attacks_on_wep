# Two Attacks On Wep
Written by Jacob Tan and Richard Duong

-----------------------------------------------------------------------------------------------------------------------
<br>
### Objective:
A coding implementation demonstrating the vulnerabilities of CRC-32 and WEP protocol 
<br><br>

### Design Choices
| struct packet
---- | struct iv
-------- | char arr[3]

---- | struct ip_header
-------- | char src[4]
-------- | char dest[4]

---- | char[4] msg

---- | struct crc
-------- | char [4] msg
-------- | char [4] result

### Client / Server Architecture
We ran into an issue with our design with determining how we would forward packets to ip addresses.
So instead we came up with an alternative design:
- We map an ip address to a specific port e.g. alice's ip -> port 50000
- We will have alice, carolWEP, and AP as client to send and transmit packets; CarolWEP, AP, Carol, and Bob as servers to receive packets
- We would have CarolWEP edit the half the packets and send the received packets to AP, which would decrypt and send to the appropriate receivers.

This mapping of IPV4 addresses to ports has its flaws, we can't emulate CarolWEP utilizing the Man in the Middle attack because it cannot spoof itself
However it still demonstrates the attack on the encryption / packet validity over CRC32 and WEP Protocol very clearly.
