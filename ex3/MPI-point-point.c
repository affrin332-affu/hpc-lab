#include<stdio.h>
#include<string.h>
#include<mpi.h>

const int MAX_STRINGS=100;

int main()
{
        char msg[100];
        int comm_size;
        int my_rank;
        int i;
        MPI_Init(NULL,NULL);
        MPI_Comm_size(MPI_COMM_WORLD,&comm_size);
        MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

        if(my_rank!=0)
        {
                sprintf(msg,"\nHello from process %d\n",my_rank);
                MPI_Send(msg,strlen(msg)+1,MPI_CHAR,0,0,MPI_COMM_WORLD);
        }
        else
        {
                printf("Hello from process %d",my_rank);
                for(i=1;i<comm_size;i++)
                {
                        MPI_Recv(msg,100,MPI_CHAR,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                        printf("%s",msg);
                }
        }
        MPI_Finalize();

}
