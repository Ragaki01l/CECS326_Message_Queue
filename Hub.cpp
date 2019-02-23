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
	int Aoff,Boff,Coff=0;
	int received=0;
	
	struct buf
	{
    	long mtype;// Required
    	char message[50]; //Message content
	};

	buf msg;
	int size = sizeof(msg)-sizeof(long);


	//Different mesages
	while(true){
		
		if(msgrcv(qid, (struct msgbuf *)&msg, size,0,0)!=-1){
			printf("%s\n",msg.message);
			if(msg.mtype==997){
				msg.mtype = 41;
				strcpy(msg.message, "Message Received.");
				msgsnd(qid, (struct msgbuf *)&msg, size,0);
			}
			if(msg.mtype==1){
			Aoff=1;
			}
		}		
		
		if(Aoff,Boff,Coff==1)
		{
			break;
		}
	}
	return 0;
}



