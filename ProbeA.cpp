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
	default_random_engine rng;
	normal_distribution<double> distribution(0,5000);

	bool received = 0;

	int msgkey=1234;
	int qid = msgget(msgkey,IPC_CREAT | 0666); //Key Generator

	struct buf
	{
    	long mtype;// Required
    	char message[50]; //Message content
	};

	buf msg;
	int size = sizeof(msg)-sizeof(long);


	msg.mtype = 997;
	strcpy(msg.message, "Hello there");
	msgsnd(qid, (struct msgbuf *)&msg, size,0);


	while(received!=1){
	//Different mesages
    	if(msgrcv(qid, (struct msgbuf *)&msg, size,41,0)!=-1){
        	msg.mtype = 997;
        	strcpy(msg.message, "Hello there2");
        	msgsnd(qid, (struct msgbuf *)&msg, size,0);
        	received=1;
    	}

	}


	return 0;

}



