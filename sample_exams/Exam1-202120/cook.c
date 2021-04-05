#include "library.h"

int main(int argc, char** argv) {
    if (argc != 2 || argv[1][0] < '0' || argv[1][0] > '9') {
        perror("Error input arguments.\n");
        printf("Usage: %s <order_num>\n", argv[0]);
        return 0;
    }
    int order_num = atoi(argv[1]);
    printf("Start COOKING order %d\n", order_num);
    sleep(3);
    if (order_num == 2) {
        printf("FAILED COOKING order %d\n", order_num);
        exit(1);
    } else if (order_num == 3) {
        sleep(3);
    }
    printf("Finish COOKING order %d\n", order_num);
    // return 0 is exactly same as exit(0) here.
    return 0;
}