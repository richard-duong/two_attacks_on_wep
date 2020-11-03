#include "crc32.h"

int main(){

	char* msg = "hello";
	struct Crc32 crc32;
	generate(crc32, msg);
	printf("%zu", crc32.result);
	
}
