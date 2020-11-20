# Two Attacks On Wep
Written by Jacob Tan and Richard Duong<br>

### Objective:
Demonstrate the vulnerabilities of CRC-32 and WEP protocol with the use of 2 different attacks:<br>
1. **The packet-redirect attack** which takes advantage of CRC32's linearity property to modify the destination of the ip header,
and tricks the Access Point into decrypting the packet before sending it off to the attacker.<br>
2. **The "chop chop" attack** which also takes advantage of CRC32's linearity property and as long as the Access Point
returns an acknowledgement for whether the message was accepted or not, one could recover the plaintext by guessing the
plaintext byte-by-byte.

### Standard WEP Protocol
Let's say Alice wants to send packets to Bob. Alice will follow these steps to attempt to securely send her packet to Bob.
1. Alice generates a packet with an ip header and message
2. Alice runs CRC on her current packet, and appends CRC to the end of her packet
3. Alice generates a random IV, and generates a key with (WEP Password || IV)
4. Alice uses this key to encrypt her packet, and then prefix the encrypted packet with the IV she used
5. Alice sends this off to the Access Point
6. Access Point receives and decrypts the packet using (WEP Password || IV) from the packet prefix
7. Access Point calculates the CRC on the original packet and compares to the attached CRC to make sure nothing was modified
8. Access Point sends the decrypted packet over to Bob
9. Bob receives the packets
<br><br>

-----------------------------------------------------------------------------------------------------------------------
## The Packet-Redirect Attack

### The Attack
#### From an overview perspective, this is what Carol does if she wants to read Alice's packets
1. Carol sets up an intermediate Access Point called CarolWEP
2. CarolWEP stands in between Alice and the Access Point, which picks up Alice's packets
3. CarolWEP modifies the destination in the packet, and sends it on to the Access Point
4. Access Point receives and decrypts the packet using (WEP Password || IV) from the packet prefix
5. Access Point calculates CRC and compares to attached CRC
6. Access Point sends the decrypted packet over to Carol
7. Carol receives the packet<br><br>

#### How does this happen?
CRC has a flaw because it modifies bytes in place to calculate the CRC checksum, which can be used to Carol's advantage.<br>
Therefore CRC has a property where CRC(A) ⊕ CRC(B) = CRC(A ⊕ B) which can be used with XOR's self-inverse property.<br>

**Self-Inverse Property**<br>
0xFA ⊕ 0xFA = 0<br>

Therefore, if I wanted to edit a message, and I know the specific location of the message, then I can overwrite<br>
it with this property while still retaining the CRC checksum and maintaining packet validity.<br>

**Example Packet Attack**<br>
Assume all values are hexadecimal.<br>
To execute this attack, we need to know 2 things:<br>
1. The location of the bytes we want to edit, in the example below, it's index 4 - 7<br>
2. The original byte values (we can assume the attacker knows where the packet should be delivered)<br>

Let's assume for this example, the packet only has SRC and DEST, which are 4 bytes and in order respectively.
Carol can construct 2 packets that she can XOR the encryption and cancel out the underlying DEST bytes, and replace<br>
it with her own destination. Cancelled values are prefixed with a **-** sign for demonstration purposes.<br>

|        	|     	|     	|     	|     	|     	|     	|     	|     	|
|--------	|-----	|-----	|-----	|-----	|-----	|-----	|-----	|-----	|
|        	|  AA 	|  BB 	|  CC 	|  DD 	| -11 	| -22 	| -33 	| -44 	|
|    ⊕   	| -00 	| -00 	| -00 	| -00 	|  FF 	|  FF 	|  FF 	|  FF 	|
|    ⊕   	| -00 	| -00 	| -00 	| -00 	| -11 	| -22 	| -33 	| -44 	|
|    =   	|  AA 	|  BB 	|  CC 	|  DD 	|  FF 	|  FF 	|  FF 	|  FF 	|

This demonstration is done with a plaintext, but the same idea applies even with the WEP encryption as long<br>
as the two conditions above are met. There is another intermediate step not mentioned here, but the CRC is<br>
calculated **for each buffer she uses**, and XOR'ed on top of the CRC that is attached to the end of the packet.<br>

### Client / Server Architecture
We ran into an issue with our design with determining how we would forward packets to ip addresses.<br>
So instead we came up with an alternative design:<br>
- We map an ip address to a specific port e.g. alice's ip -> port 50000<br>
- We will have Alice, CarolWEP, and AP as clients to send and transmit packets; CarolWEP, AP, Carol, and Bob as servers to receive packets<br>
- We would have CarolWEP edit half the packets and send the received packets to AP, which would decrypt and send to the appropriate receivers.<br>

This mapping of IPV4 addresses to ports has its flaws, we can't emulate CarolWEP utilizing the Man in the Middle attack because it cannot spoof itself<br>
However it still demonstrates the attack on the encryption / packet validity over CRC32 and WEP Protocol very clearly.<br>

### Port Mapping

| Entity           	| IPv4            	| Port  	|
|------------------	|-----------------	|-------	|
| **Alice**        	| 169.235.16.75   	| 50000 	|
| **CarolWEP**     	| 100.100.100.100 	| 49500 	|
| **Access Point** 	| 255.255.255.255 	| 49000 	|
| **Bob**          	| 141.212.113.199 	| 48500 	|
| **Carol**        	| 128.2.42.95     	| 48000 	|
<br><br>

-----------------------------------------------------------------------------------------------------------------------
## The Chop Chop Attack

### The Attack
#### This attack works if Carol is able to receive a response from the Access Point about dropped packets.
1. Carol has to have access to the message + CRC(message)<br>
2. Carol starts by reverse engineering the CRC(message), and stepping backwards from it<br>
3. Let message' be when Carol chops off the last byte of the message<br>
4. From now on, we'll call Carol's updates crc<br>
5. Carol sets crc = crc ⊕ 0xFFFFFFFF<br>
6. Carol searches for a top byte table index match with the top byte of crc and stores that index as crc_index<br>
7. Carol sets crc = crc ⊕ table\[crc_index\]<br>
8. Carol shifts crc left 8 bits<br>
9. Carol is now looking for the last byte that matches CRC(message') ⊕ 0xFFFFFFFF<br>
10. Carol can find the last byte value from the message with: crc_index ⊕ last byte<br>

#### How does this happen?
The way CRC is calculated, is that it modifies byte by byte. As I add more bytes, the CRC expands to fit that extra byte.<br>
So essentially each of the CRC calculations in each step before are all valid for those substrings.<br>
So this attack works by working backwards for each CRC calculation on the substrings. When we reverse engineer the CRC<br>
calculation, we look for the table index that was used before to calculate this current CRC. However the drawback is that<br>
because the original CRC calculation shifts it over a byte, we essentially need to guess the last byte in the CRC substring<br>
Luckily this is at most 256 guesses, and once we have that, we use ⊕'s commutative property and use the CRC last byte ⊕ table index<br>
for the value of the last byte of the original message.
