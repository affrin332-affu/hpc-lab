#include<stdio.h>
#include<string.h>
#include<mpi.h>
#include<ctype.h>
int main()
{
        char str[100];
        char in_str[100];
        int comm_size;
        int my_rank;
        MPI_Init(NULL,NULL);
        MPI_Comm_size(MPI_COMM_WORLD,&comm_size);
        MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

        MPI_Status status;
        if(my_rank==1)
        {
                sprintf(str,"hello");
                MPI_Send(str,strlen(str)+1,MPI_CHAR,0,1,MPI_COMM_WORLD);
        }
        else if(my_rank==2)
        {
                sprintf(str,"malayalams");
                MPI_Send(str,strlen(str)+1,MPI_CHAR,0,2,MPI_COMM_WORLD);
        }
        else if(my_rank==3)
        {
                sprintf(str,"island");
                MPI_Send(str,strlen(str)+1,MPI_CHAR,0,1,MPI_COMM_WORLD);
        }
        else if(my_rank==4)
        {
                sprintf(str,"level");
                MPI_Send(str,strlen(str)+1,MPI_CHAR,0,2,MPI_COMM_WORLD);
        }
        else
        {
                int i,j;
                printf("\nMaster process :%d\n",my_rank);
                for(i=1;i<comm_size;i++)
                {
                        MPI_Recv(str,100,MPI_CHAR,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
                        if(status.MPI_TAG==1)
                        {
                                for(j=0;str[j]!='\0';j++)
                                {
                                        str[j]=toupper(str[j]);
                                }
                                printf("\nResult:%s from process %d\n",str,status.MPI_SOURCE);
                        }
                        else if(status.MPI_TAG==2)
                        {
                                for(j=strlen(str)-1;j>=0;j--)
                                {
                                        in_str[j]=str[j];
                                }
                                if(strcmp(str,in_str)==0)
                                {
                                        printf("\nResult:%s Palindrome from process %d\n",str,status.MPI_SOURCE);
                                }
                                else
                                {
                                        printf("\nResult:%s Not palindrome from process %d\n",str,status.MPI_SOURCE);
                                }
                        }
                }
        }
        MPI_Finalize();

}


