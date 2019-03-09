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

	buf msg;
	msg.mtype = 997;														// ProbeA's mtype, its magic seed, set as 997
	strcpy(msg.message, "ProbeA");
	int size = sizeof(msg)-sizeof(long);

	while(true)
	{
		value = distribution(rng);											// random value
		
		if(value < 100)														// If random value is less than 100, stop sending messages
			{
				break;
			}
		else if(value%msg.mtype==0){						// If the value is divisible by magic seed, send message.
			
			msgsnd(qid, (struct msgbuf *)&msg, size,0);						// Sends message with mtype 997.
			
			msgrcv(qid, (struct msgbuf *)&msg, size,41,0);			// Received acknowledgment from the Hub that message was sent.
			printf("%s\n",msg.message);
			
			msg.mtype = 997;
			strcpy(msg.message, "ProbeA");
			
		}		
	
	}
	msg.mtype=1;															// Sets mtype to 1 for Hub to set its ProbeA flag on
	strcpy(msg.message,"ProbeA Closed");
	msgsnd(qid,(struct msgbuf *) &msg, size,0);								// Sends message with mtype as 1.

	return 0;

}



