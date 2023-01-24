
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

/*

Imagine we have an intersection with a stoplight.  Cars can drive
north south or east west, but we don't want north south cars in the
intersection at the same time as east west cars.  However, any number
of northsouth cars can be in the intersection at the same time, as
long as the stoplight is going that way.

The rules for the car threads are simple:

1.  Cars freely enter the intersection if it green in their direction.

2.  If it is yellow or green in the other direction, they do not enter
the intersection

3.  If they are in the intersection and the stoplight turns yellow,
they proceed as normal and exit the intersection at their normal speed

This stoplight is represented by a thread.  It changes from

1.  green northsouth
2.  yellow
3.  green eastwest
4.  yellow

...and then back to the top.

The stoplight changes from green to yellow after a specific time,
regardless of what the car situation is (i.e. it will change from ns
green to yellow, even if there are many ns cars and no ew cars
waiting).

However, the stoplight is smart about being yellow.  The stoplight is
yellow exactly long enough for all cars currently in the intersection
to leave.  If there are no cars in the intersection it will display
yellow and then display the new green right away.  If they are cars it
will change exactly as soon as all current cars leave the
intersection.  Because no car will enter the intersection when the
stoplight is yellow, there is no darger of the number of cars in the
intersection increasing when the stoplight is yellow.

The stoplight starts by being gree for northsouth cars and then transition the
yellow and to green for eastwest and so on.. (Note the print statement at line
135)


Add concurrency primitives to the given code to implement the behavior
described.  You may modify both car and stoplight threads.

Example output:

green in northsouth direction
eastwest car nearing intersection
northsouth car nearing intersection
northsouth car entering intersection
northsouth car nearing intersection
northsouth car entering intersection
yellow
northsouth car leaving intersection
northsouth car leaving intersection
green in eastwest direction
eastwest car entering intersection
eastwest car nearing intersection
eastwest car entering intersection
northsouth car nearing intersection
yellow
eastwest car leaving intersection
eastwest car leaving intersection
green in northsouth direction
northsouth car entering intersection
northsouth car nearing intersection
northsouth car entering intersection
eastwest car nearing intersection
yellow
northsouth car leaving intersection
northsouth car leaving intersection
green in eastwest direction
eastwest car entering intersection
yellow
eastwest car leaving intersection
green in northsouth direction

 */


void *north_south(void *arg)
{
    printf("northsouth car nearing intersection\n");
    printf("northsouth car entering intersection\n");
    sleep(1);
    printf("northsouth car leaving intersection\n");
}


void *east_west(void *arg)
{
    printf("eastwest car nearing intersection\n");
    printf("eastwest car entering intersection\n");
    sleep(1);
    printf("eastwest car leaving intersection\n");
}


void *stoplight(void *arg)
{

    while(1) {

        sleep(1);
        printf("yellow\n");

        // we need to wait for the intersection to clear
        printf("green in eastwest direction\n");
        sleep(1);
        printf("yellow");
        // we need to wait for the intersection to clear
        printf("green in northsouth direction\n");

    }

}

int main()
{

    pthread_t tid[100];
    int i = 0;

    // make sure we display the initial stoplight state before cars start
    printf("green in northsouth direction\n");
    pthread_create(&tid[i++],NULL,stoplight,NULL);


    pthread_create(&tid[i++],NULL,east_west,NULL);
    pthread_create(&tid[i++],NULL,north_south,NULL);
    pthread_create(&tid[i++],NULL,north_south,NULL);
    sleep(1);
    pthread_create(&tid[i++],NULL,east_west,NULL);
    pthread_create(&tid[i++],NULL,north_south,NULL);
    sleep(1);
    pthread_create(&tid[i++],NULL,east_west,NULL);
    pthread_create(&tid[i++],NULL,north_south,NULL);



    for(int j = 1; j < i; j++) {
        pthread_join(tid[j],NULL);
    }


    return 0;
}
