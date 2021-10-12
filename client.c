#include"headers.h"

struct data{
	int **allocation;
	int **request;
	int **need;
	int *finish;
	int *available;
	int *max_threads;
	int *r_count;
	int *d;
};
struct data * create_data(int *max_threads,int *r_count,int *d,int *r_arr){
	struct data *temp=(struct data *)malloc(sizeof(struct data));

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
	printf("Enter resource counts\n");
	scanf("%d",r_count);

	r_arr=(int*) malloc(sizeof(int)*(*r_count));
	printf("Enter resource values press enter after each value\n");
	
	for(int i=0;i<*r_count;i++){
		scanf("%d",&r_arr[i]);
	}
	printf("Enter max threads\n");

	scanf("%d",max_threads);
	
	printf("Deadlock detection check interval\n");

	scanf("%d",d);

	return r_arr;
}

void *function( void *arg){

}
void summon_pthreads(pthread_t *thread,int *max_threads,int *d,int *r_arr){
	/*summons process max_threads amount of threads*/	
	for(int i=0;i<max_threads;i++){
		pthread_create(&thread[i],NULL,function,NULL);
	}
}

void deadlock_check_thread(){
	/*summon thread that checks for deadlocks and prints*/

}

int main(){

	int *max_threads,*r_count, *r_arr,*d;

	max_threads=(int *)malloc(sizeof(int));
	r_count=(int *)malloc(sizeof(int));
	d=(int *)malloc(sizeof(int));

	r_arr=take_input(max_threads,r_count,r_arr,d);

	//check_inputs(max_threads,r_count,r_arr);
	pthread_t thread[*max_threads];

	summon_pthreads(thread,max_threads,d,r_arr);

	deadlock_check_thread();

	return 0;


}