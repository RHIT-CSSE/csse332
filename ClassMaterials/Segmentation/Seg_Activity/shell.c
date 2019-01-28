
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
    interrupt(0x21,0,my_string,0,0);

    /* TODO: change data segment to kernel */
    /* addr = 0x06f3; */
    /* interrupt(0x21,0,"Shell printing: ",0,0); */
    
    /* interrupt(0x21,0,addr,0,0); */
    while(1)
        ;
}


