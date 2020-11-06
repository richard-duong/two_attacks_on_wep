# Two Attacks On Wep
An extremely rigorous homework assignment for CS165 Computer Security
Written by Jacob Tan and Richard Duong

-----------------------------------------------------------------------------------------------------------------------

### Objective:
Demonstrate the vulnerabilities of CRC-32 and WEP protocol by designing a system 
<br><br>

### Struct Design Choices
struct packet<br>
 |- struct iv<br>
 |- struct ip_header<br>
 |- struct crc32<br>
 |- char[4] msg<br>
 |- char[16] raw<br>
 |- char[19] encryption<br>

struct iv<br>
 |- char arr[3]<br>

struct ip_header<br>
 |- char src[4]<br>
 |- char dest[4]<br>

struct crc<br>
 |- char [4] msg<br>
 |- char [4] result<br>
 
### Port Mapping
Alice IP:           169.235.16.75<br>
Alice Port:         50000<br>

CarolWEP IP:        100.100.100.100<br>
CarolWEP Port:      49500<br>

Access Point IP:    255.255.255.255<br>
Access Point Port:  49000<br>

Bob IP:             141.212.113.199<br>
Bob Port:           48500<br>

Carol IP:           128.2.42.95<br>
Carol Port:         48000<br>

### Client / Server Architecture
We ran into an issue with our design with determining how we would forward packets to ip addresses.<br>
So instead we came up with an alternative design:<br>
- We map an ip address to a specific port e.g. alice's ip -> port 50000<br>
- We will have alice, carolWEP, and AP as client to send and transmit packets; CarolWEP, AP, Carol, and Bob as servers to receive packets<br>
- We would have CarolWEP edit half the packets and send the received packets to AP, which would decrypt and send to the appropriate receivers.<br>

This mapping of IPV4 addresses to ports has its flaws, we can't emulate CarolWEP utilizing the Man in the Middle attack because it cannot spoof itself<br>
However it still demonstrates the attack on the encryption / packet validity over CRC32 and WEP Protocol very clearly.<br>


