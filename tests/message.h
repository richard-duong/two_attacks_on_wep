#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <string>
#include <iostream>
using namespace std;

// only takes 24 characters
class Message{

	private:
		char msg[32];

	public:
		Message(char* msg){
			memset(this->msg, '\0', sizeof(msg));	
			strcpy(this->msg, msg);
		}

		Message(string msg){
			memset(this->msg, '\0', sizeof(msg));
			strcpy(this->msg, msg.c_str());
		}

		void print(){
			cout << this->msg;
		}

};

#endif
