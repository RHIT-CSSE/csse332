#include "library.h"

int main(int argc, char** argv) {
    if (argc != 2 || argv[1][0] < '0' || argv[1][0] > '9') {
        printf("Error input arguments: %s\n", argv[1]);
        printf("Usage: %s <order_num>\n", argv[0]);
        return 0;
    }
    int order_num = atoi(argv[1]);
    printf("Order %d received\n", order_num);
    sleep(2);
    printf("Order %d processed\n", order_num);
    return 0;
}