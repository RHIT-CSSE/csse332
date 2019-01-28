
int main(){
    void* addr;
    char my_string[8];
    my_string[0] = 'M';
    my_string[1] = 'i';
    my_string[2] = 'n';
    my_string[3] = 'e';
    my_string[4] = '\r';
    my_string[6] = '\n';
    my_string[7] = '\0';

    /* change stack segment */
    interrupt(0x21,0,my_string,0,0);
    change_stack_seg(0x3000);
    /*After change the stack segment, the string is disappeared.*/
    interrupt(0x21,0,my_string,0,0);

    /* TODO: change data segment to kernel and print out the string "haha..."
    in the kernel.
    STEP 1:
        In the kernel.c, it prints out the addr of that particular string in hex.
        You will need to hardcode the addr variable to that addr.
    SETP 2:
        Take a look at the change_stack_seg in lib.asm. Implement a func called
        change_data_seg(x), which change the data segment register to x. The data
        segment register is named "ds".
    STEP 3:
        Use the function above to change ds to kernel's data segment, which is 0x1000.
        Then, you can use the addr in the first step to print out that string.
        */
    /* addr = 0xffff; */
    /* interrupt(0x21,0,"Shell printing: ",0,0); */
    
    /* interrupt(0x21,0,addr,0,0); */
    while(1)
        ;
}


