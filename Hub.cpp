#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <random>
#include "force_patch.h"

using namespace std;

int main(){
	int msgkey=1234;

	int qid = msgget(msgkey,IPC_CREAT | 0666); //Key Generator
	int Aoff,Boff,Coff=0;
	int received=0;
	int pid=1;
	
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
						
			if(received>10000&& pid!=1 ){
				force_patch(pid);
				Boff=1;
			}
			if(msg.mtype==997){
				msg.mtype = 41;
				strcpy(msg.message, "Message Received.");
				msgsnd(qid, (struct msgbuf *)&msg, size,0);
			}

			if(msg.mtype==1){
				Aoff=1;
			}
			if(msg.mtype==3){
				Coff=1;
			}
			else if(msg.mtype==100){
				pid=stoi(msg.message);
			}
		}		
		
		if(Aoff==1&&Boff==1&&Coff==1)
		{
			break;
		}

		received++;
	}
	msgctl (qid, IPC_RMID, NULL);

	return 0;
}



