#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <random>

using namespace std;

int main(){
	int msgkey=1234;

	int qid = msgget(msgkey,IPC_CREAT | 0666); //Key Generator

	struct buf
	{
    	long mtype;// Required
    	char message[50]; //Message content
	};

	buf msg;
	int size = sizeof(msg)-sizeof(long);


	//Different mesages
	if(msgrcv(qid, (struct msgbuf *)&msg, size,997,0)!=-1){
    	printf("%s\n",msg.message);
    	msg.mtype = 41;
    	strcpy(msg.message, "Message Recieved.");
    	msgsnd(qid, (struct msgbuf *)&msg, size,0);
	}

	return 0;
}



