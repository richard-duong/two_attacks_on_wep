#include <iostream>
#include <string>
#include "crc32.h"
#include "ip_header.h"
#include "message.h"

using namespace std;

int main(){



	char* str1 = "Hello";
	Crc crc(str1);
	crc.print();

	Message msg(str1); 
	//msg.print();

	char ip1[20] = "192.168.1.1";
	char ip2[20] = "100.100.5.5";
	Ip_Header header(ip1, ip2);
	//header.print();


}
