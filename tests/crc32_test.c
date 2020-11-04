#include "inttypes.h"
#include "../headers/crc32.h"

int main(){

  char* msg = "hello";
  int size = strlen(msg);
  crc32 crc;
  generate_crc(&crc, msg, size);

  for(int i = 0; i < 4; ++i){
    printf("message %d: %d\n", i, msg[i]);
    printf("crc %d: %d\n", i, crc.result[i]);
  }

}
