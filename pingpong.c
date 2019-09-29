#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]){
   
   char buff[50];
   int pipe1[2];
   int pipe2[2];

   int pid ;

   pipe(pipe1);
   pipe(pipe2);

    pid = fork();

    if(pid == 0)
    {

        printf("decko\n");

        if(0 <= read(pipe1[0],buff,sizeof(buff))){
            printf("Child with pid %d received ping\n",getpid());

            write(pipe2[1],"p",1);
        }        

    }

    else if(pid> 0){

        printf("matka\n");
        write(pipe1[1],"p",1);

         if(0 <= read(pipe2[0],buff,sizeof(buff))){
            printf("Parent with pid %d received pong\n",getpid());            
        }        
    }

   else{
       printf("something wen wrong\n");
   }
   
    exit(0);

}