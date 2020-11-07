#include "../headers/packet.h"
#include "../headers/crc32.h"

/* My SID = 861281974, we are using the last 2 bytes of char "71" as the message */
int main(){

  unsigned char index = 0;
  unsigned char match = 0;
  char msg[5] = "71";
  char check[5] = "7";

  __uint32_t crc = generate_crc(msg, 2);
  __uint32_t crc_check = generate_crc(check, 1) ^ 0xFFFFFFFF;
  
  printf("Initial CRC value of %c: %X\n", check[0], crc_check);
  printf("CRC initial hex: %X\n", crc);

  crc ^= 0xFFFFFFFF;
  printf("CRC reverse hex: %X\n", crc);

  for(int i = 0; i < 256; ++i){
    if((crc & 0xFF000000) == (crc_tbl[i] & 0xFF000000)){
      index = i;
      crc ^= crc_tbl[i];
      crc <<= 8;
      break;
    }
  }
  printf("CRC XOR with table[index] + shift: %X\n", crc);
  printf("Index found: %X\n", index);

  for(int i = 0; i < 256; ++i){
    if(crc + i == crc_check){
      match = i;
      printf("Matches CRC value of %c\n", check[0]);
    } 
  }

  printf("Final value is: %c\n", index ^ match);

}
