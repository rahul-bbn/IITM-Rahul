#include"mq_common.h"
static int qid=-1;
static void cleanup(int sig_no){
    if (qid!=-1)
    {
        if (msgctl(qid,IPC_RMID,NULL)==-1)
        {
            perror("msgctl(IPC_RMID)");
        }
        else{
            fprintf(stderr,"\n[server] Queue removed (qid=%d)")
        }
    }
    
}