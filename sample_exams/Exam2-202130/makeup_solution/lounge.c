#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

/*
 In this problem, your job is to coordinate access to the CSSE department
 lounge. Due to funding issues, there can only be one person in the lounge at
 a time.

 There are three types of people that can go into the lounge;
 - senior faculty
 - junior faculty
 - students.

 Here are the rules for entering the lounge:
 - When the lounge is empty, whoever arrives first can enter (i.e., first come
   first serve).

 - If the lounge is occupied, then access to the lounge occurs by priority as
   follows:
	- senior faculty have priority over both students and junior faculty
	- junior faculty are the cool people and they are close with their
	  students, thus junior faculty and students have the same priority. In
	  other words, if no seniors are waiting, then junior faculty and
	  students compete on a first-come-first-serve basis.

 As an example, consider the case where a senior faculty is in the lounge. And
 at the same time, another senior faculty and a student arrive. Moments later,
 a junior faculty arrives. All three new arriving people are waiting.

 When the senior faculty exits the lounge, since there is another senior
 faculty waiting, they get priority over students and junior faculty.
 Therefore that second senior faculty goes into the lounge.

 Once the second senior faculty exits the lounge, then it is
 first-come-first-serve between the student and the junior faculty. Since the
 student arrives earlier, then the student goes into the lounge first.
 Finally, the junior faculty goes into the lounge when the student comes out.


 Hint: I used a total of 3 semaphores to implement my solution.

 Testing:
 ========

 We provide you with 5 test cases for you to test your code. Running the
 program without any arguments (i.e., just `./lounge`) will run all 5 test
 cases. If you'd like to run a particular test case, you can pass the number
 of that test case as an argument to the program. For example to run test 2,
 run `./lounge 2`.

 Here is my example outcome for each test case;

 $ ./lounge 0 | grep lounge
        A student has entered the lounge....
        A student has exited the lounge....
        A student has entered the lounge....
        A student has exited the lounge....
        A student has entered the lounge....
        A student has exited the lounge....
        A student has entered the lounge....
        A student has exited the lounge....
        A student has entered the lounge....
        A student has exited the lounge....


 $ ./lounge 1 | grep lounge
FIRST lounge BATCH STARTING:
	 Senior faculty entered the lounge....
	 Senior faculty exited the lounge....
	 Junior faculty entered the lounge....
	 Junior faculty exited the lounge....
SECOND lounge BATCH STARTING:
	 Senior faculty entered the lounge....
	 Senior faculty exited the lounge....
	 Senior faculty entered the lounge....
	 Senior faculty exited the lounge....
	 Senior faculty entered the lounge....
	 Senior faculty exited the lounge....
	 Senior faculty entered the lounge....
	 Senior faculty exited the lounge....
	 Junior faculty entered the lounge....
	 Junior faculty exited the lounge....
	 Junior faculty entered the lounge....
	 Junior faculty exited the lounge....
	 Junior faculty entered the lounge....
	 Junior faculty exited the lounge....
	 Junior faculty entered the lounge....
	 Junior faculty exited the lounge....

 $ ./lounge 2 | grep lounge
FIRST lounge BATCH STARTING:
	 Senior faculty entered the lounge....
	 Senior faculty exited the lounge....
	 A student has entered the lounge....
	 A student has exited the lounge....
SECOND lounge BATCH STARTING:
	 Senior faculty entered the lounge....
	 Senior faculty exited the lounge....
	 Senior faculty entered the lounge....
	 Senior faculty exited the lounge....
	 Senior faculty entered the lounge....
	 Senior faculty exited the lounge....
	 Senior faculty entered the lounge....
	 Senior faculty exited the lounge....
	 A student has entered the lounge....
	 A student has exited the lounge....
	 A student has entered the lounge....
	 A student has exited the lounge....
	 A student has entered the lounge....
	 A student has exited the lounge....
	 A student has entered the lounge....
	 A student has exited the lounge....

 $ ./lounge 3 | grep lounge
FIRST lounge BATCH STARTING:
	 Junior faculty entered the lounge....
	 Junior faculty exited the lounge....
	 A student has entered the lounge....
	 A student has exited the lounge....
SECOND lounge BATCH STARTING:
	 Junior faculty entered the lounge....
	 Junior faculty exited the lounge....
	 A student has entered the lounge....
	 A student has exited the lounge....
	 Junior faculty entered the lounge....
	 Junior faculty exited the lounge....
	 A student has entered the lounge....
	 A student has exited the lounge....
	 A student has entered the lounge....
	 A student has exited the lounge....
	 A student has entered the lounge....
	 A student has exited the lounge....
	 Junior faculty entered the lounge....
	 Junior faculty exited the lounge....
	 Junior faculty entered the lounge....
	 Junior faculty exited the lounge....

 $ ./lounge 4 | grep lounge
FIRST lounge BATCH STARTING:
	 Senior faculty entered the lounge....
	 Senior faculty exited the lounge....
	 Junior faculty entered the lounge....
	 Junior faculty exited the lounge....
	 A student has entered the lounge....
	 A student has exited the lounge....

*/


sem_t lounge;
sem_t junior_block;
sem_t lock;

int num_seniors = 0;
int num_juniors = 0;

void *senior_faculty(void *arg)
{
	printf("Senior faculty waiting\t");
	sem_wait(&lock);
	num_seniors++;
	if (num_seniors == 1) {
		sem_post(&lock);
		sem_wait(&junior_block);
	} else {
		sem_post(&lock);
	}

	/* try to go into the lounge */
	sem_wait(&lounge);
	printf("\n\t Senior faculty entered the lounge....\n");
	sleep(3);
	printf("\n\t Senior faculty exited the lounge....\n");
	sem_post(&lounge);

	sem_wait(&lock);
	num_seniors--;
	if (num_seniors == 0) {
		sem_post(&junior_block);
	}
	sem_post(&lock);

}


void *junior_faculty(void *arg)
{
	printf("Junior faculty waiting\t");
	sem_wait(&junior_block);
	sem_post(&junior_block);

	sem_wait(&lounge);
	printf("\n\t Junior faculty entered the lounge....\n");
	sleep(2);
	printf("\n\t Junior faculty exited the lounge....\n");
	sem_post(&lounge);
}

void *student(void *arg)
{
	printf("A student is waiting\t");
	sem_wait(&junior_block);
	sem_post(&junior_block);

	sem_wait(&lounge);
	printf("\n\t A student has entered the lounge....\n");
	sleep(1);
	printf("\n\t A student has exited the lounge....\n");
	sem_post(&lounge);
}

void test0(void)
{
	const int total_students = 5;
	pthread_t students[total_students];

	printf("\n########################## TEST 0 ########################################\n");
	for(int i = 0; i < total_students; i++) {
		pthread_create(&students[i], 0, student, NULL);
	}

	for(int i = 0; i < total_students; i++) {
		pthread_join(students[i], NULL);
	}
}

void test1(void)
{
	const int total_seniors = 5;
	const int total_juniors = 5;
	pthread_t seniors[total_seniors];
	pthread_t juniors[total_juniors];

	printf("\n########################## TEST 1 ########################################\n");
	printf("FIRST lounge BATCH STARTING:\n");
	pthread_create(&seniors[0], 0, senior_faculty, NULL);
	pthread_create(&juniors[0], 0, junior_faculty, NULL);

	sleep(6);
	printf("SECOND lounge BATCH STARTING:\n");

	pthread_create(&seniors[1], 0, senior_faculty, NULL);
	pthread_create(&seniors[2], 0, senior_faculty, NULL);
	pthread_create(&juniors[1], 0, junior_faculty, NULL);
	pthread_create(&juniors[2], 0, junior_faculty, NULL);

	pthread_create(&seniors[3], 0, senior_faculty, NULL);
	pthread_create(&seniors[4], 0, senior_faculty, NULL);
	pthread_create(&juniors[3], 0, junior_faculty, NULL);
	pthread_create(&juniors[4], 0, junior_faculty, NULL);

	for(int i=0; i < total_seniors; i++) {
		pthread_join(seniors[i], NULL);
	}

	for(int i=0; i < total_juniors; i++) {
		pthread_join(juniors[i], NULL);
	}

	printf("All done...\n");
}

void test2(void)
{
	printf("\n########################## TEST 2 ########################################\n");
	const int total_seniors = 5;
	const int total_students = 5;
	pthread_t seniors[total_seniors];
	pthread_t students[total_students];

	printf("FIRST lounge BATCH STARTING:\n");
	pthread_create(&seniors[0], 0, senior_faculty, NULL);
	pthread_create(&students[0], 0, student, NULL);

	pthread_join(seniors[0], NULL);
	pthread_join(students[0], NULL);

	printf("SECOND lounge BATCH STARTING:\n");
	pthread_create(&seniors[1], 0, senior_faculty, NULL);
	pthread_create(&students[1], 0, student, NULL);

	pthread_create(&students[2], 0, student, NULL);
	pthread_create(&seniors[2], 0, senior_faculty, NULL);

	pthread_create(&students[3], 0, student, NULL);
	pthread_create(&students[4], 0, student, NULL);
	pthread_create(&seniors[3], 0, senior_faculty, NULL);
	pthread_create(&seniors[4], 0, senior_faculty, NULL);

	for(int i = 1; i < total_seniors; i++)
		pthread_join(seniors[i], NULL);

	for(int i = 1; i < total_students; i++)
		pthread_join(students[i], NULL);
}

void test3(void)
{
	printf("\n########################## TEST 3 ########################################\n");
	const int total_juniors = 5;
	const int total_students = 5;
	pthread_t juniors[total_juniors];
	pthread_t students[total_students];

	printf("FIRST lounge BATCH STARTING:\n");
	pthread_create(&juniors[0], 0, junior_faculty, NULL);
	pthread_create(&students[0], 0, student, NULL);

	pthread_join(juniors[0], NULL);
	pthread_join(students[0], NULL);

	printf("SECOND lounge BATCH STARTING:\n");
	pthread_create(&juniors[1], 0, junior_faculty, NULL);
	pthread_create(&students[1], 0, student, NULL);

	pthread_create(&juniors[2], 0, junior_faculty, NULL);
	pthread_create(&students[2], 0, student, NULL);

	pthread_create(&students[3], 0, student, NULL);
	pthread_create(&students[4], 0, student, NULL);
	pthread_create(&juniors[3], 0, junior_faculty, NULL);
	pthread_create(&juniors[4], 0, junior_faculty, NULL);

	for(int i = 1; i < total_juniors; i++)
		pthread_join(juniors[i], NULL);

	for(int i = 1; i < total_students; i++)
		pthread_join(students[i], NULL);
}

void test4(void)
{
	printf("\n########################## TEST 4 ########################################\n");
	const int total_seniors = 1;
	const int total_juniors = 1;
	const int total_students = 1;

	pthread_t students[total_students];
	pthread_t seniors[total_seniors];
	pthread_t juniors[total_juniors];

	printf("FIRST lounge BATCH STARTING:\n");
	pthread_create(&seniors[0], 0, senior_faculty, NULL);
	pthread_create(&juniors[0], 0, junior_faculty, NULL);
	pthread_create(&students[0], 0, student, NULL);

	pthread_join(seniors[0], NULL);
	pthread_join(juniors[0], NULL);
	pthread_join(students[0], NULL);
}

int
main(int argc, char **argv)
{
	setbuf(stdout, NULL);
	sem_init(&lounge, 0, 1);
	sem_init(&junior_block, 0, 1);
	sem_init(&lock, 0, 1);

	int test = -1;
	if (argc > 1) {
		test = atoi(argv[1]);
	}

	switch(test) {
	case 0:
		test0();
		break;
	case 1:
		test1();
		break;
	case 2:
		test2();
		break;
	case 3:
		test3();
		break;
	case 4:
		test4();
		break;
	default:
		test0();
		test1();
		test2();
		test3();
		test4();
	}

	sem_destroy(&lounge);
	sem_destroy(&junior_block);
	sem_destroy(&lock);
}
