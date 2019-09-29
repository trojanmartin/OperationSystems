#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


void searchPrimes(int left_pipe[]){
    int actualNumber;
    int number;
    int wpid;
    int status =0; 
    int right_pipe[2];

    //zatvorim write stranu pipy lebo viem ze uz nikdy do nej nebudem pisat
    close(left_pipe[1]);

    int pid = fork();

    //ak som searchPrimes
    if(pid ==0){

        //vytvorim pipe do ktorej budem vkladat veci pre dalsi proces
        pipe(right_pipe);

        //ak sa nieco v pipe nachadza
        if(read(left_pipe[0],&actualNumber,sizeof(actualNumber)) > 0){
            //vypisem prve cislo lebo viem ze to je prvocislo
            printf("prime %d\n",actualNumber);        

            //chceckujem vsetky nasledujuce cisla v pipe
            while(read(left_pipe[0],&number,sizeof(number)) > 0){
                 
                // ak nieje delitelne aktualnym prvocislom, je to kandidat na prvocislo a poslem ho dalej
                if(number % actualNumber != 0){                    
                    write(right_pipe[1],&number,sizeof(number));                   
                }
            }       
            // opakujem to iste v dalsom procese
            searchPrimes(right_pipe);                
        }         
    }

    //ak som rodic cakam na svoje dieta kym skonci
    else if (pid > 0) {
        while ((wpid = wait(&status)) > 0);
    }

    else{
        printf("Error while creating new procces");
    }   
}

int main(){

    int pipe1[2];    
    pipe(pipe1);    

    for(int i=2; i<= 35;i++){
        
        write(pipe1[1],&i,sizeof(i));       
    }

    searchPrimes(pipe1);
    exit(0);
}


