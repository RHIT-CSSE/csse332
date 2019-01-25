void printString(char *);
void printAddr(void *);
void readString(char *);
void readSector(char *, int);
int mod(int,int);
int div(int,int);
void handleInterrupt21(int,int,int,int);
int strlen(char *);
int strncmp(char*, char*, int);
void readFile(char* );
void executeProgram(char*, int);
void terminate();

#define MAX_FILESIZE 13312
int main()
{
    char *c;
    c = "haha. I belong to kernel. You can't print me from your shell. Haha, again.\r\n\0";
	makeInterrupt21();
    

    printAddr(c);


	interrupt(0x21, 4, "shell\0", 0x2000, 0);  

	while(1)
		;
}

void printString(char * s)
{
	int i;
    i = 0;
	while(s[i] != '\0'){
		interrupt(0x10, 0xE*256 + s[i],0,0,0);
		i++;
	}
}

void printAddr(void* addr)
{
	int i;
    int cur;
    interrupt(0x10, 0xE*256 + '0',0,0,0);
    interrupt(0x10, 0xE*256 + 'x',0,0,0);
    for(i = 0; i<4;i++){
       cur = (((int) addr >> (4*(3-i))) & 0xf);
       if(cur < 10){
           interrupt(0x10, 0xE*256 + (cur + '0'),0,0,0);
       }
       else{
           interrupt(0x10, 0xE*256 + (cur - 10 + 'a'),0,0,0);
       }
    }
    interrupt(0x10, 0xE*256 + 0xa,0,0,0);
    interrupt(0x10, 0xE*256 + 0xd,0,0,0);
}

void readString(char * s)
{
	char t;
	int i = 0;
	while(1){
		t = interrupt(0x16, 0x0,0,0,0);
		if(t == 0xD){
			interrupt(0x10, 0xE *256 + 0xA,0,0,0);
			interrupt(0x10, 0xE *256 + 0xD,0,0,0);
			s[i] = 0xA;
			s[i+1] = 0x0;
			break;
		}
		else if (t == 0x8){
			interrupt(0x10, 0xE *256 + t,0,0,0);
			interrupt(0x10, 0xE *256 + ' ',0,0,0);
			interrupt(0x10, 0xE *256 + t,0,0,0);
			if(i > 0)
				i--;
		}
		else{
			interrupt(0x10, 0xE *256 + t,0,0,0);
			s[i] = t;
			i++;
		}
	}
}

void readSector(char* buff, int sector)
{
	interrupt(0x13, 2 * 256 + 1, buff,
			div(sector,36)*256 + mod(sector,18) + 1,
			mod(div(sector,18),2)*256 + 0);
}

int mod(int a, int b)
{
	while(a >= b)
		a = a - b;
	return a;
}
int div(int a, int b)
{
	int quo = 0;
	while (( quo + 1 ) * b <= a){
		quo++;
	}
	return quo;
}

void handleInterrupt21(int ax, int bx, int cx, int dx)
{
    char msg[6];
    msg[0] = 'E';
    msg[1] = 'r';
    msg[2] = 'r';
    msg[3] = 'o';
    msg[4] = 'r';
    msg[5] = '\0';
	if(ax == 0){
		printString(bx);
	}
	else if(ax == 1){
		readString(bx);
	}
	else if(ax == 2){
		readSector(bx,cx);
	}
	else if (ax == 3){
		readFile(bx,cx);
	}
	else if (ax == 4){
		executeProgram(bx,cx);
	}
	else if (ax == 5){
		terminate();
	}
	else{
		printString(msg);
	}

}
int strlen(char * s)
{
	int len;

	len = 0;
	while(s[len] != '\0')
		len++;

	return len;
}

int strncmp(char* s1, char* s2, int n)
{
	int i;
	int ret;

	ret = 1;
	for(i = 0;i < n;i++){
		if(s1[i] != s2[i]){
			ret = -1;
			break;
		}
	}
	return ret;
}

void readFile(char* filename, char* buff)
{
	/* char map[512]; */
	char dir[512];
    char msg[10];
	int i;
	int match_len;
	int fn_len;
	int found;

    msg[0] = 'N';
    msg[1] = 'o';
    msg[2] = ' ';
    msg[3] = 'F';
    msg[4] = 'i';
    msg[5] = 'l';
    msg[6] = 'e';
    msg[7] = '\r';
    msg[8] = '\n';
    msg[9] = '\0';
	fn_len = strlen(filename);

	/* readSector(map,1); */
	readSector(dir,2);

	found = -1;
	for(i = 0;i < 512; i = i + 32){
		match_len = 6;
		if(match_len > fn_len )
			match_len = fn_len;
		/* printString("Search File..\r\n"); */
		if(strncmp(filename,dir+i,match_len) > 0){
			found = i + 6;
			/* printString("File Found!\r\n"); */
			break;
		}
	}
    if(found < 0){
        printString(msg);
		return;
    }

	i = 0;
	while(i < 26 && dir[found + i] != 0){
		readSector(buff, dir[found + i]);
		buff = buff + 512;
		i++;
	}
}

void executeProgram(char* name, int segment)
{
	char buff[MAX_FILESIZE];
	int i;

	readFile(name, buff);

	for(i = 0;i<MAX_FILESIZE;i++){
		putInMemory(segment,i,buff[i]);
		/* if(buff[i] == 0x3) */
		/* 	break; */
	}
	launchProgram(segment);
}

void terminate()
{
    char Shell[6];
    Shell[0] = 's';
    Shell[1] = 'h';
    Shell[2] = 'e';
    Shell[3] = 'l';
    Shell[4] = 'l';
    Shell[5] = '\0';
	interrupt(0x21, 4, Shell, 0x2000, 0);  
}
