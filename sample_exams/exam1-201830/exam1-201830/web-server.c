/* web-server.c -
 *	 
 * Program written by Delvin Defoe, March 30, 2018.
 *
 * This program simulates a web server that responds to 
 * requests from various clients. Each time a client accesses
 * the server, they make the same initial requests. Thus, the
 * same responses are returned to every client.  
 * 
 * Note that the web server is flakey and may terminate with 
 * errors at random times. If that happens a few times, the 
 * sever admin application will restart the web server. You 
 * will simulate this behavior by actualling killing this 
 * process with the kill pid command from a terminal window.
 * 
 * You should not edit this file, except to adjust the SECS_TO_SLEEP
 * macro if you wish to test out different delays. 
 * 
 * All work for this problem should be done in the server-admin.c
 * file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAX_RESPONSES 13
#define SECS_TO_SLEEP 2
#define TRUE 1
#define FALSE 0

void serveResources();

char *responses[MAX_RESPONSES] = {
    "GET / 200 5.505 ms - 898",
    "GET /styles.bundle.js 200 2.443 ms - 63101",
    "GET /scripts.bundle.js 200 2.382 ms - 20810",
    "GET /main.bundle.js 200 2.474 ms - 65308",
    "GET /inline.bundle.js 200 2.499 ms - 5830",
    "GET /polyfills.bundle.js 200 2.484 ms - 200843",
    "GET /vendor.bundle.js 200 2.490 ms - 5463350",
    "GET /general/csse332_main.html 200 0.391 ms - 3143",
    "GET /general/pandoc.css 200 0.578 ms - 4234",
    "GET /general/csse332_info.html 200 0.539 ms - 9873",
    "GET /general/pandoc.css 304 0.496 ms - -",
    "GET /sessions 200 5.192 ms - -",
    "GET /headers 304 1.851 ms - -"
};

int main(int argc, char *argv[]) {
    int pid = getpid();
    while(TRUE) {
        printf("server (pid %d) is listening on port 8080 (ok, not really but lets pretend)\n", pid);  
        serveResources();
    }
    return 0;
}

void serveResources() {
    int i;
    for (i = 0; i < MAX_RESPONSES; i++){
        printf("%s\n", responses[i]);
        sleep(SECS_TO_SLEEP);
    }
}
