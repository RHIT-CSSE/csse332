#include "library.h"

int main(int argc, char** argv) {
    if (argc != 2 || argv[1][0] < '0' || argv[1][0] > '9') {
        perror("Error input arguments.\n");
        printf("Usage: %s <order_num>\n", argv[0]);
        return 0;
    }
    int order_num = atoi(argv[1]);
    printf("Start delivering %d\n", order_num);
    sleep(1);
    printf("Order %d delivered\n", order_num);
    return 0;
}