#include "../crc32.h"
#include "inttypes.h"

int main(){

	char* msg = "hello";
	int size = strlen(msg);
	struct Crc32 crc32;
	generate(&crc32, msg);

	printf("%" PRIu32 "\n", crc32.result);	

}
