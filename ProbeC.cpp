#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <random>
#include "kill_patch.h"

using namespace std;

int main()
{
	//gen random number using distribution(rng)
	default_random_engine rng(random_device{}());
	uniform_int_distribution<> distribution(0,100000);
	int msgkey=1234;
	int qid = msgget(msgkey,IPC_CREAT | 0666); //Key Generator 
	int value;
	
	struct buf
	{
    	long mtype;// Required
    	char message[50]; //Message content
	};

    buf msg,msgkill;

	int size = sizeof(msg)-sizeof(long);

	// First message sent from ProbeC is the terminating message as msg.message with msg.mtype as 3.
    msgkill.mtype=3;
    strcpy(msgkill.message,"C killed");

    kill_patch(qid,(struct msgbuf *) &msgkill, size,msgkill.mtype);

	msg.mtype = 251;										// magic seed set as 251

	while(true)												// ProbeC will continue sending messages until user sends the kill command
	{
		value = distribution(rng);							// Random value to be compared with the magic seed.
		if(value%msg.mtype==0)								// If random value is divisible by magic seed, message will be sent to Hub.
		{
			msg.mtype = 251;
			strcpy(msg.message, "ProbeC");
			msgsnd(qid, (struct msgbuf *)&msg, size,0);
		}
    }
	return 0;

}



