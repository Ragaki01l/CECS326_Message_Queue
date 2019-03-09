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
	
        cout<<qid<<endl;

	struct buf
	{
    	long mtype;// Required
    	char message[50]; //Message content
	};

	buf msg;
	
	// First message sent from ProbeB is the pid as msg.message with msg.mtype as 100.
	msg.mtype = 100;
	int size = sizeof(msg)-sizeof(long);
	strncpy(msg.message,to_string(getpid()).c_str());

	msgsnd(qid, (struct msgbuf *)&msg, size,0);

	while(true)													// ProbeB will continue to send messages until the Hub
	{															// receives over 10,000 messages and then forces ProbeB to exit via force_patch.

		value = distribution(rng);								// random value to compare with magic seed.
		msg.mtype = 257;										// magic seed set as 257
                
		if(value%msg.mtype==0)									// If random value is divisible by magic seed, message will be sent to Hub.
		{
			strncpy(msg.message, "ProbeB");
			msgsnd(qid, (struct msgbuf *)&msg, size,0);
			
		}
    }

	return 0;

}



