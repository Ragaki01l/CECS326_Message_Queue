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
	msg.mtype = 251;
	int size = sizeof(msg)-sizeof(long);

	while(true){

		value = distribution(rng);
		if(value%msg.mtype==0){
			msg.mtype = 251;
			strcpy(msg.message, "ProbeC");
			msgsnd(qid, (struct msgbuf *)&msg, size,0);
		}
		
		
	
	}
	msg.mtype=3;
	strcpy(msg.message,"ProbeC Closed");
	msgsnd(qid,(struct msgbuf *) &msg, size,0);

	return 0;

}



