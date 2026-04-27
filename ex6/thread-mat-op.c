#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

int **A,**B,**sum,**prod;
int n;

void* mat_add(void* arg)
{
   int i,j;
   long thread_num=(long)arg;

   printf("\nThread %ld performing Matrix Addition\n", thread_num);

   for(i=0;i<n;i++)
   {
      for(j=0;j<n;j++)
      {
         sum[i][j]=A[i][j]+B[i][j];
      }
   }
   printf("Matrix Addition Result:\n");
   for(i=0;i<n;i++)
   {
      for(j=0;j<n;j++)
      {
         printf("%d\t",sum[i][j]);
      }
      printf("\n");
   }
   return NULL;
}

void* mat_mul(void* arg)
{
   int i,j,k;
   long thread_num=(long)arg;
   printf("\nThread %ld performs Matrix multiplication\n", thread_num);
   for(i=0;i<n;i++)
   {
      for(j=0;j<n;j++)
      {
         prod[i][j]=0;
         for(k=0;k<n;k++)
         {
            prod[i][j]+=A[i][j]*B[k][j];
         }
      }
   }
   printf("Matrix Multiplication Result:\n");
   for(i=0;i<n;i++)
   {
      for(j=0;j<n;j++)
      {
         printf("%d\t",prod[i][j]);
      }
      printf("\n");
   }
   return NULL;
}

int main()
{
   pthread_t t1,t2;
   int i,j,k;
   printf("Enter matrix size:\n");
   scanf("%d",&n);

   A=malloc(n*sizeof(int*));
   B=malloc(n*sizeof(int*));
   sum=malloc(n*sizeof(int*));
   prod=malloc(n*sizeof(int*));

   for(i=0;i<n;i++)
   {
      A[i]=malloc(n*sizeof(int));
      B[i]=malloc(n*sizeof(int));
      sum[i]=malloc(n*sizeof(int));
      prod[i]=malloc(n*sizeof(int));
   }
   for(i=0;i<n;i++)
   {
      for(j=0;j<n;j++)
      {
         A[i][j]=rand()%10;
      }
   }
   printf("Matrix A:\n");
   for(i=0;i<n;i++)
   {
      for(j=0;j<n;j++)
      {
         printf("%d\t",A[i][j]);
      }
      printf("\n");
   }


   for(i=0;i<n;i++)
   {
      for(j=0;j<n;j++)
      {
         B[i][j]=rand()%10;
      }
   }
   printf("Marix B:\n");
   for(i=0;i<n;i++)
   {
      for(j=0;j<n;j++)
      {
         printf("%d\t",B[i][j]);
      }
      printf("\n");
   }

   pthread_create(&t1,NULL,mat_add,(void*)1);
   pthread_join(t1,NULL);
   pthread_create(&t2,NULL,mat_mul,(void*)2);

   //pthread_join(t1,NULL);
   pthread_join(t2,NULL);

   for(i=0;i<n;i++)
   {
      free(A[i]);
      free(B[i]);
      free(sum[i]);
      free(prod[i]);
   }
   free(A);
   free(B);
   free(sum);
   free(prod);
}
