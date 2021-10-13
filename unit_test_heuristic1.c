#include"headers.h"

int h=1;
int logs=0;
int itr=0;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
struct data{
	int **allocation;
	int **request;
	int **need;
	int *finish;
	int *available;
	int *max_threads;
	int *r_count;
	int *d;
	pthread_t *thread;
};
struct data * create_data(int *max_threads,int *r_count,int *d,int *r_arr,pthread_t *p){
	struct data *temp=(struct data *)malloc(sizeof(struct data));
	temp->max_threads=max_threads;
	temp->d=d;
	temp->r_count=r_count;
	temp->allocation=(int **)malloc(sizeof(int *)*(*max_threads));
	for(int i=0;i<(*max_threads);i++){
		temp->allocation[i]=(int*)malloc(sizeof(int)*(*r_count));
	}
	temp->request=(int **)malloc(sizeof(int *)*(*max_threads));
	for(int i=0;i<(*max_threads);i++){
		temp->request[i]=(int*)malloc(sizeof(int)*(*r_count));
	}
	temp->need=(int **)malloc(sizeof(int *)*(*max_threads));
	for(int i=0;i<(*max_threads);i++){
		temp->need[i]=(int*)malloc(sizeof(int)*(*r_count));
	}

	temp->finish=(int *)malloc(sizeof(int)*(*r_count));

	temp->available=(int *)malloc(sizeof(int)*(*r_count));

	temp->thread=p;
	
	for(int i=0;i<(*max_threads);i++){
		for(int j=0;j<(*r_count);j++){
			temp->allocation[i][j]=temp->need[i][j]=temp->request[i][j]=0;
		}
	}
	
	for(int i=0;i<(*r_count);i++){
		temp->finish[i]=0;
		temp->available[i]=r_arr[i];
	}
	return temp;

}

void check_inputs(int *max_threads,int *r_count,int *r_arr){
	/*checks resource parameters*/
	printf("MaxThreads: %d\nResource Count: %d\n",*max_threads,*r_count);
	for(int i=0;i<*r_count;i++){
		printf("resource %d cap is: %d\n",i+1,r_arr[i]);
	}

	return ;
}



int * take_input(int *max_threads,int *r_count,int *r_arr,int *d){
	/* 
		function: takes input;
		input: max threads((int *)max_threads), resource count((int *) r_count),resource_array (int*)r_arr
		output: ((int *)R)max resource cap array;
	*/

	*r_count=5;

	r_arr=(int*) malloc(sizeof(int)*(*r_count));
	
	for(int i=0;i<*r_count;i++){
		r_arr[i]=0;
	}


	*max_threads=3;


	*d=1;

	return r_arr;
}
int MAX_RESOURCE=5;
void *function( void *arg){
	pthread_mutex_lock(&mutex);
	struct data *info=arg;
	int r_count=*(info->r_count);
	int d=*(info->d);
	int *t_resources=(int *) malloc(sizeof(int)*r_count);
	int thread_number;
	int *finish=(int *) malloc(sizeof(int)*r_count);
	for(int i=0;i<*(info->max_threads);i++){
		if(pthread_self()==(info->thread[i])){
			thread_number=i;
			break;
		}
	}
	pthread_mutex_unlock(&mutex);
	while(1){
		for(int i=0;i<r_count;i++){
			t_resources[i]=rand()%MAX_RESOURCE;
			finish[i]=0;
			pthread_mutex_lock(&mutex);
			info->request[thread_number][i]=t_resources[i];
			pthread_mutex_unlock(&mutex);
		}
		int j=0;
		while(j<r_count){
			srand(time(0));
			int i=rand()%r_count;
			
			if(finish[i]==0){
				sleep(d);
				pthread_mutex_lock(&mutex);
				int need=info->request[thread_number][i]-info->allocation[thread_number][i];
				int available=info->available[i];
				pthread_mutex_unlock(&mutex);
				if(need>available){
					continue;
				}
				pthread_mutex_lock(&mutex);
				info->available[i]-=need;
				info->allocation[thread_number][i]+=need;
				pthread_mutex_unlock(&mutex);
				if(logs ==1)
				printf("Got resource %d of process %d with need %d and available %d\n",i+1,thread_number,need,available);
				j++;
				finish[i]=1;
			}
			
		}	
		double d2=7/(float)10;
		d2=d2*d;
		srand(time(0));
		d2+=(rand()%8)/(float)10;
		sleep(d2);
		pthread_mutex_lock(&mutex);
		for(int i=0;i<r_count;i++){
			info->available[i]+=info->allocation[thread_number][i];
			info->allocation[thread_number][i]=0;
		}
		pthread_mutex_unlock(&mutex);

	}
	
	
}
void summon_pthreads(struct data *info){
	/*summons process max_threads amount of threads*/	
	pthread_mutex_lock(&mutex);
	for(int i=0;i<*(info->max_threads);i++){
		int n=pthread_create(&info->thread[i],NULL,function,(void *) info);
		if(n<0) printf("Error Creating thread\n");
	}
	pthread_mutex_unlock(&mutex);
}
float total_time_gap=0,avg=0;
int q=0;

void checktime(time_t *seconds,int *max_threads){
	q++;
	float gap=time(NULL)-*seconds;
	total_time_gap+=gap;
	avg=total_time_gap/q;
	if(*max_threads>1){
		printf("Time interval between deadlocks %f second. Average time %f\n",gap,avg);
	}else{
		printf("Time interval between insufficient resources %f second. Average time %f\n",gap,avg);
	}
	*seconds=time(NULL);
}
void *d_check(void *arg){
	
	time_t seconds=time(NULL);
	pthread_mutex_lock(&mutex);
	struct data *info=arg;
	int d=*(info->d);
	pthread_mutex_unlock(&mutex);
	while(1){
		sleep(d);
		itr++;
		printf("Detector Starts.......\n");
		printf("Iteration %d\n",itr);
		pthread_mutex_lock(&mutex);
		int max_threads=*(info->max_threads);
		int r_counts=*(info->r_count);
		int alloc[max_threads][r_counts],need[max_threads][r_counts],request[max_threads][r_counts];
		int *finish=(int *) malloc(sizeof(int ) *((r_counts)));
		int *work=(int *) malloc(sizeof(int ) *((r_counts)));
		for(int i=0;i<r_counts;i++){
			finish[i]=0;
			work[i]=info->available[i];
		}
		for(int i=0;i<max_threads;i++){
			for(int j=0;j<r_counts;j++){
				alloc[i][j]=info->allocation[i][j];
				request[i][j]=info->request[i][j];
				need[i][j]=request[i][j]-alloc[i][j];
			}
		}
		int c=0;
		while(c<max_threads){
			int f=0;
			for(int i=0;i<max_threads;i++){
				if(finish[i]==0){
					int j;
					for(j=0;j<r_counts;j++){
						if(need[i][j]>work[j]) break;
					}
					if(j==r_counts){
						for(int k=0;k<r_counts;k++) work[k]+=alloc[i][k];
						c++;
						finish[i]=1;
						f=1;
					}
					
				}
			}
			if(f==0){
				if(max_threads>1)
				printf("Deadlock detected....\n");
				else{
					printf("Resource is not available in sufficient amount for the single thread.\n");
				}
				//sleep(5);
				if(h==2){
					for(int i=0;i<max_threads;i++){
						if(finish[i]==0){
							if(max_threads>1)
							printf("Thread %d is in deadlock\n",i);
							else{
							printf("Thread %d has insufficient resources\n",i);
							}
							if(logs ==1)
							printf("Logging thread %d info...\n",i);
							for(int j=0;j<r_counts;j++){
								if(logs ==1)
								printf("%d th resource requires: %d available: %d allocated:%d \n",j,request[i][j],work[j],alloc[i][j]);

							}
							for(int j=0;j<r_counts;j++){
								info->available[j]+=info->allocation[i][j];
								info->allocation[i][i]=alloc[i][j]=0;
								info->request[i][j]=request[i][j]=need[i][j]=info->need[i][j]=0;
							}
							int n=pthread_kill(info->thread[i],0);
							if(n<0) {
								if(logs ==1)
								printf("Unable to kill thread %d\n",i);
								abort();
							}
							if(logs ==1)
							printf("Thread killed successfully\n");
							n=pthread_create(&info->thread[i],NULL,function,(void *)info);
							if(n<0) {
								if(logs ==1)
								printf("Unable to create thread %d\n",i);
								abort();
							}
							if(logs ==1)
							printf("Thread created\n");
						}
						
					}
					checktime(&seconds,&max_threads);
					break;
				}
				else if(h==3){
					int size[max_threads];
					for(int i=0;i<max_threads;i++){
						int s=0;
						size[i]=0;
						if(finish[i]==0){
							if(max_threads>1)
							printf("Thread %d is in deadlock\n",i);
							else{
							printf("Thread %d has insufficient resources\n",i);
							}
							if(logs ==1)
							printf("Logging thread %d info...\n",i);
							for(int j=0;j<r_counts;j++){
								if(logs ==1)
								printf("%d th resource requires: %d available: %d allocated:%d \n",j,request[i][j],work[j],alloc[i][j]);
								if(alloc[i][j]>0){
									s+=alloc[i][j];
								}
							}
							if(logs ==1)
							printf("Therefore thread %d is hogging up %d resources\n",i,s);
							size[i]=s;
						}
					}
					int mx_el=0;
					for(int i=0;i<max_threads;i++){
						if(size[mx_el]<size[i]){
							mx_el=i;
						}
					}
					if(logs==1)
					printf("Thread %d has the max resources\nInitiating kill thread....\n",mx_el);
					for(int i=0;i<r_counts;i++){
						info->available[i]+=info->allocation[mx_el][i];
						info->allocation[mx_el][i]=alloc[mx_el][i]=0;
						info->request[mx_el][i]=request[mx_el][i]=need[mx_el][i]=info->need[mx_el][i]=0;
					}
					int n=pthread_kill(info->thread[mx_el],0);
					if(n<0) {
						if(logs ==1)
						printf("Unable to kill thread %d\n",mx_el);
						abort();
					}
					if(logs ==1)
					printf("Thread killed successfully\n");
					n=pthread_create(&info->thread[mx_el],NULL,function,(void *)info);
					if(n<0) {
						if(logs ==1)
						printf("Unable to create thread %d\n",mx_el);
						abort();
					}
					if(logs ==1)
					printf("Thread created\n");
					checktime(&seconds,&max_threads);
					break;

				}
				else if(h==4){
					int size[max_threads];
					for(int i=0;i<max_threads;i++){
						int s=0;
						size[i]=0;
						if(finish[i]==0){

							if(max_threads>1)
							printf("Thread %d is in deadlock\n",i);
							else{
							printf("Thread %d has insufficient resources\n",i);
							}
							if(logs ==1)
							printf("Logging thread %d info...\n",i);
							for(int j=0;j<r_counts;j++){
								if(logs ==1)
								printf("%d th resource requires: %d available: %d allocated:%d \n",j,request[i][j],work[j],alloc[i][j]);
								if(alloc[i][j]>0){
									s+=alloc[i][j];
								}
							}
							if(logs ==1)
							printf("Therefore thread %d is hogging up %d resources\n",i,s);
							size[i]=s;
						}
					}
					int mx_el=0;
					for(int i=0;i<max_threads;i++){
						if(size[mx_el]>size[i]){
							mx_el=i;
						}
					}
					if(logs==1)
					printf("Thread %d has the max resources\nInitiating kill thread....\n",mx_el);
					for(int i=0;i<r_counts;i++){
						info->available[i]+=info->allocation[mx_el][i];
						info->allocation[mx_el][i]=alloc[mx_el][i]=0;
						info->request[mx_el][i]=request[mx_el][i]=need[mx_el][i]=info->need[mx_el][i]=0;
					}
					int n=pthread_kill(info->thread[mx_el],0);
					if(n<0) {
						if(logs ==1)
						printf("Unable to kill thread %d\n",mx_el);
						abort();
					}
					if(logs ==1)
					printf("Process killed successfully\n");
					n=pthread_create(&info->thread[mx_el],NULL,function,(void *)info);
					if(n<0) {
						if(logs ==1)
						printf("Unable to create thread %d\n",mx_el);
						abort();
					}
					if(logs ==1)
					printf("Thread created\n");
					checktime(&seconds,&max_threads);
					break;
				}
				else{
					int size[max_threads];
					for(int i=0;i<max_threads;i++){
						int s=0;
						size[i]=0;
						if(finish[i]==0){
							if(max_threads>1)
							printf("Thread %d is in deadlock\n",i);
							else{
							printf("Thread %d has insufficient resources\n",i);
							}
							if(logs ==1)
							printf("Logging thread %d info...\n",i);
							for(int j=0;j<r_counts;j++){
								if(logs ==1)
								printf("%d th resource requires: %d available: %d allocated:%d \n",j,request[i][j],work[j],alloc[i][j]);
								if(alloc[i][j]>0){
									s++;
								}
							}
							if(logs ==1)
							printf("Therefore thread %d is hogging up %d resources\n",i,s);
							size[i]=s;
						}
					}
					int mx_el=0;
					for(int i=0;i<max_threads;i++){
						if(size[mx_el]<size[i]){
							mx_el=i;
						}
					}
					if(logs==1)
					printf("Thread %d has the max resources\nInitiating kill thread....\n",mx_el);
					for(int i=0;i<r_counts;i++){
						info->available[i]+=info->allocation[mx_el][i];
						info->allocation[mx_el][i]=alloc[mx_el][i]=0;
						info->request[mx_el][i]=request[mx_el][i]=need[mx_el][i]=info->need[mx_el][i]=0;
					}
					int n=pthread_kill(info->thread[mx_el],0);
					if(n<0) {
						if(logs ==1)
						printf("Unable to kill thread %d\n",mx_el);
						abort();
					}
					if(logs ==1)
					printf("Thread killed successfully\n");
					n=pthread_create(&info->thread[mx_el],NULL,function,(void *)info);
					if(n<0) {
						if(logs ==1)
						printf("Unable to create thread %d\n",mx_el);
						abort();
					}
					if(logs ==1)
					printf("Thread created\n");
					checktime(&seconds,&max_threads);
					break;

				}
				
				
			}
		}
		if(c==max_threads){
			if(logs ==1)
			printf("No deadlocks encountered\n");
			fflush(stdout);
		}
		pthread_mutex_unlock(&mutex);
	}
	
}

void deadlock_check_thread(struct data *info){
	/*summon thread that checks for deadlocks and prints*/
	pthread_t thread;
	pthread_create(&thread,NULL,d_check,(void *) info);
}

int main(){

	int *max_threads,*r_count, *r_arr,*d;
	struct data *info;
	max_threads=(int *)malloc(sizeof(int));
	r_count=(int *)malloc(sizeof(int));
	d=(int *)malloc(sizeof(int));

	r_arr=take_input(max_threads,r_count,r_arr,d);
	h=1;
    logs=0;
    MAX_RESOURCE=6;
	pthread_t thread[*max_threads];
	info=create_data(max_threads,r_count,d,r_arr,thread);
	
	summon_pthreads(info);
	deadlock_check_thread(info);
	while(1){
		int gg=1;
	}

	return 0;


}