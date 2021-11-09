Submitter name:Ashish Kaushik

Roll No.:2018MMB1279

Course:CS303

=================================


1. What does this program do

This program mimicks how the O.S. handles when there is a deadlock between different
processes; In case of deadlocks there are multiple ways by which the process (thread in our case)
can be terminated hence the average time between deadlocks is noted down for each case
which provides an insight about the heuristic which should be preferred.


2. A description of how this program works (i.e. its logic)

The program asks for the max resource cap of each resource type and the max threads
as well as deadlock check interval
Hence resource values are allocated and then as each thread summons resources as described in the question
(randomly) random function has been used to mimick all the behaviours with srand for rand seed.
there are 2d arrays rows->threads, col-> resource which contains resource allocated at present to various threads
and another 2d arrray for request from the thread as well as a 1 D array containing the quantity of resource available.
Hence using requested and present allocated resource for a thread the value of required resource is calculated.
If the resources are available the required amount is allocated else the thread waits for the resource.
meanwhile the deadlock checker makes repeated checks to see if the threads are in deadlock by looking
at the required and available for all threads to find if there is a way to allocate.

If there isnt a way it then terminates those threads using the heuristic
And a new thread is summoned the thread id is changed in the thread pool
Hence the pool remains consistent of max threads and deadlocks are removed by the deadlock checker.
If the deadlock is detected it will print  deadlock detected... after printing "detector starts"
other wise it wont print anything except "detector starts" to provide info that a check has been made
and will start again after time interval of d seconds as mentioned in 
the question also it will give the iteration of the detector (how much time it has been called)

One can also log about all info by turning on the logs.

3. How to compile and run this program

-----------------------------------------------------------------------------------------------
Compile
-----------------------------------------------------------------------------------------------
main file:
gcc -o client client.c -lpthread -ldl
./client


Example Output:
if No deadlocks it will show:
Detector Starts.......
Iteration 52
Else
Detector Starts.......
Iteration 53
Deadlock detected....
Thread 0 is in deadlock
Thread 1 is in deadlock
Thread 2 is in deadlock
Time interval between deadlocks 5.000000 second. Average time 10.600000Time interval between deadlocks 5.000000 second. Average time 10.600000
/* In some cases due to random generation it is likely there is no deadlock for some iteration
so please be patient*/

unit test:
gcc -o unit_test_heuristic unit_test_heuristic1.c -lpthread -ldl
./unit_test_heuristic
/*tests on heuristic 1 where max resources=0 [not sufficient] so it will always result in deadlocks
unless a thread doesnt ask for 0 resource*/

gcc -o unit_test_heuristic unit_test_heuristic2.c -lpthread -ldl
./unit_test_heuristic
/*tests on heuristic 2 where thread can ask at max 0 resource so it will never result in deadlocks*/

gcc -o unit_test_heuristic unit_test_heuristic3.c -lpthread -ldl
./unit_test_heuristic
/* A test on heuristic 3 with a sample test case*/
gcc -o unit_test_heuristic unit_test_heuristic4.c -lpthread -ldl
./unit_test_heuristic
/* A test on heuristic 4 with the same sample test case as heuristic 3 which can be used to compare time*/

client.c file will ask for the following inputs

Enter resource counts

Enter resource values press enter after each value
/* maximum amount of each resource which the O.S can have*/

Enter max threads

Deadlock detection check interval

Enter suitable heuristic [1,2,3,4]:
1) Thread which is in deadlock and acquiring max different type of resources will be terminated
2) All threads which are in deadlock will be terminated
3) Thread which is in deadlock and acquiring maximum total sum of resources will be terminated
4) Thread which is in deadlock and acquiring minimum total sum of resources will be terminated
/*By default case 1 will be selected*/

Enter 1 to turn on logging 0 to turn off [turned off by default]:
/* the logging wil show everything including when the resource has been required killing thread
creating any new thread and the requested vs allocated values in case of deadlocks,recommended to keep it off
while testing for first time because too much info might cause confusion*/

Enter the maximum amount of resource a thread can ask which is selected randomly from the range[0...MAX] [default 5]:
/*As the thread can ask for a resource of a certain type this
max represents how much quantity of resource at max a thread can ask for */


4. Provide a snapshot of a sample run

Heuristic results
I have run the code on 4 heuristic and noted the average time
between deadlocks after 100 deadlock checks with the following inputs:

Enter resource counts
5
Enter resource values press enter after each value
5
5
5
5
5
Enter max threads
3
Deadlock detection check interval
1
Enter suitable heuristic [1,2,3,4]:
1 
Enter 1 to turn on logging 0 to turn off [turned off by default]:
0
Enter the maximum amount of resource a thread can ask which is selected randomly from the range[0...MAX] [default 5]:
6

/*tested for all heuristic 1,2,3,4 case whose result is shown below*/
Results:

For case 1- Thread which is in deadlock and acquiring max different type of resources will be terminated
Average time between deadlocks: 5.000 seconds

For case 2- All threads which are in deadlock will be terminated
Average time between deadlocks: 16 seconds

For case 3- Thread which is in deadlock and acquiring maximum total sum of resources will be terminated
Average time between deadlocks: 6.4 seconds

For case 4- Thread which is in deadlock and acquiring minimum total sum of resources will be terminated
Average time between deadlocks: 1.136 seconds

My deduction:
As we know all the processes are random hence turning on the logging will allow us to get more info
The best is performed by case 2 followed by cases 3,4,1 (it actually depends on the random function
Because if by rand function somehow there are many deadlocks it will lead to less time between them. 
But still it provides a general idea...


