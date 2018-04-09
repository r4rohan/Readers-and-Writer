#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

//sem_t readCountAccess;
//sem_t databaseAccess;
int readcount = 0,writer_count = 0,flag = 0,no_of_readers = 0;

pthread_mutex_t lock_ReadCount;
pthread_mutex_t lock_Database;

pthread_t thread1[100],thread2[100];

void *Reader(void *arg);
void *Writer(void *arg);

int main()
{

pthread_mutex_init(&lock_ReadCount,NULL);
pthread_mutex_init(&lock_Database,NULL);
int i=0,nreader,nwriter;
//sem_init(&readCountAccess,0,1);
//sem_init(&databaseAccess,0,1);

//pthread_t Readers_thr[100],Writer_thr[100];
printf("\nEnter number of Readers :");
scanf("%d",&nreader);
printf("\nEnter number of Writers :");
scanf("%d",&nwriter);

for(i=0;i<nreader;i++)
{
pthread_create(&thread1[i],NULL,Reader,(void*)i);
}
for(i=0;i<nwriter;i++)
{
pthread_create(&thread2[i],NULL,Writer,(void*)i);
}
for(i=0;i<nreader;i++)
{
pthread_join(thread1[i],NULL);
}

for(i=0;i<nwriter;i++)
{
pthread_join(thread2[i],NULL);
}
printf("\n");
//sem_destroy(&databaseAccess);
//sem_destroy(&readCountAccess);
return 0;
}

void *Writer(void *arg)
{

	sleep(3);
	int *temp=(int*)arg;
	printf("\nWriter %d is comming into database for writting",temp);
//sem_wait(&databaseAccess);
	writer_count++;
	printf("\n no.of writers are waiting for writting the data in database : %d",writer_count);
	pthread_mutex_lock(&lock_Database);
	


	printf("\nwriter %d is writting into database",temp);
	if(flag == 1){
		flag = 0;
	}
	printf("\n no. of readers present in database for writting the data = %d\n",flag);
	writer_count--;
	printf("\nWriter %d is leaving the database",temp);

	
	pthread_mutex_unlock(&lock_Database);
//sem_post(&databaseAccess);
}

void *Reader(void *arg)
{
	//printf("\n reader before going to sleep ");
	sleep(3);
	//printf("\n reader after sleep");
	int *temp=(int*)arg;
	printf("\nReader %d is trying to reading the data in database",temp);
//sem_wait(&readCountAccess);
	no_of_readers++;
	printf("\nno. of readers are waiting to enter in database for reading : %d",no_of_readers);
	pthread_mutex_lock(&lock_ReadCount);

	flag = 1;
	readcount++;
	printf("\nreader %d about to reading ",temp);
	if(readcount==1)
	{
//sem_wait(&databaseAccess);
		no_of_readers--;
		pthread_mutex_lock(&lock_Database);

		printf("\nReader %d is reading the database",temp);

	}
//sem_post(&readCountAccess);
//sem_wait(&readCountAccess);

	pthread_mutex_unlock(&lock_ReadCount);

	pthread_mutex_lock(&lock_ReadCount);

	readcount--;

	if(readcount==0)
	{
		printf("\nReader %d is leaving the database",temp);
//sem_post(&databaseAccess);
		//no_of_readers--;
		pthread_mutex_unlock(&lock_Database);

	}
//sem_post(&readCountAccess);

	pthread_mutex_unlock(&lock_ReadCount);
}
