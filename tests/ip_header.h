#ifndef __IP_HEADER_H__
#define __IP_HEADER_H__
#include <string>
#include <iostream>
using namespace std;

class Ip_Header{

	private:
		char src[20];
		char dest[20]; 

	public:
		Ip_Header(char* src, char* dest){
			strcpy(this->src, src);
			strcpy(this->dest, dest);
		}

		Ip_Header(string src, string dest){
			strcpy(this->src, src.c_str());
			strcpy(this->dest, dest.c_str());
		}

		~Ip_Header(){}

		void print(){
			cout << src << dest;
		}
};




#endif
