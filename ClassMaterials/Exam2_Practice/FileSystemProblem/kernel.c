/*  Prints string chars to the console using Interrupt 0x10. */ 
void printString(char *chars);

/*  Reads a line from the console using Interrupt 0x16. */
void readString(char *line);

/* Advances the cursor to the next line on the console. */
void printNewLine();

/* Reads a sector from disk using Interrupt 0x13. */
void readSector(char *buffer, int sector);

/* Write a sector to disk using Interrupt 0x13. */
void writeSector(char *buffer, int sector);

/* Computes and returns the modulus of two integers. */
int mod(int a, int b);

/* Computes and returns the quotient from the integer division a/b. */
int div(int a, int b);

/* Allows you to create your own interrupt  */
void handleInterrupt21(int ax, int bx, int cx, int dx);

/* Reads the file with the given name into the buffer */
void readFile(char *name, char *buffer);

/* Write a file with the given name, buffer, and number of sectors to disk */
void writeFile(char *name, char *buffer, int numberOfSectors);

/* Finds and returns the length of a string */
int stringLength(char *string);

/* Executes the program with the given name in the given segment */
void executeProgram(char *name, int segment);

/* Terminates a program or reload the shell*/
void terminate();

void itoa(int, char*);

#define SECTOR_SIZE 512
/* #define MAX_FILE_SIZE 13312 */
#define MAX_FILE_SIZE 25600
#define MAX_FILE_NAME_LENGTH 6
#define DIRECTORY_ENTRY_SIZE 32

int main()
{
	char buffer[MAX_FILE_SIZE];

	makeInterrupt21();

	/* read the file into the buffer. */
	/* interrupt(0x21, 3, "messag\0", buffer, 0); */

	/* print the file that was read. */
	/* interrupt(0x21, 0, buffer, 0, 0); */

	interrupt(0x21, 4, "shell\0", 0x2000, 0);

	/* This infinite loop hangs up the kernel */
	while (1); 

	return 0;
}

/*  Prints string chars to the console using Interrupt 0x10. */
void printString(char *chars)
{
	char al;
	char ah;
	int ax, i, j;
	ah = 0xe;
	i = 0;
	while (chars[i] != 0x0) {
		al = chars[i];
		ax = ah*256+al;
		interrupt(0x10, ax, 0, 0, 0);
		if (al == 0xa) {
			interrupt(0x10, ah*256+0xd, 0, 0, 0);
		}
		i++;
	}
	return;
}

/*  Reads a line from the console using Interrupt 0x16. */
void readString(char *line)
{
	int i, lineLength, ax;
	char charRead, backSpace, enter;

	lineLength = 80;
	i = 0;
	ax = 0;
	backSpace = 0x8;
	enter = 0xd;
	charRead = interrupt(0x16, ax, 0, 0, 0);
	while (charRead != enter && i < lineLength - 2) {
		if (charRead != backSpace) {
			interrupt(0x10, 0xe*256+charRead, 0, 0, 0);
			line[i] = charRead;
			i++;
		} else {
			if (i > 0) {
				interrupt(0x10, 0xe*256+charRead, 0, 0, 0);
				i--;
				interrupt(0x10, 0xe*256+'\0', 0, 0, 0);
				interrupt(0x10, 0xe*256+backSpace, 0, 0, 0);
			}
		}
		charRead = interrupt(0x16, ax, 0, 0, 0);
	}
	line[i] = 0xa;
	line[i+1] = 0x0;
	printNewLine(); 
	return;
}

/* Advances the cursor to the next line on the console. */
void printNewLine()
{
	interrupt(0x10, 0xe*256+0xa, 0, 0, 0);
	interrupt(0x10, 0xe*256+0xd, 0, 0, 0);
	return;
}

/* Reads a sector from disk using Interrupt 0x13. */
void readSector(char *buffer, int sector)
{
	int relativeSector, head, track;
	int ah, al, ax, bx, ch, cl, cx, dh, dl, dx;
	relativeSector = mod(sector, 18)+1;
	head = mod(div(sector, 18), 2);
	track = div(sector, 36);
	ah = 2;
	al = 1;
	ax = ah*256+al;
	bx = buffer;
	ch = track;
	cl = relativeSector;
	cx = ch*256+cl;
	dh = head;
	dl = 0;
	dx = dh*256+dl;
	interrupt(0x13, ax, bx, cx, dx);
	return;
}

/* Write a sector to disk using Interrupt 0x13. */
void writeSector(char *buffer, int sector)
{
	int relativeSector, head, track;
	int ah, al, ax, bx, ch, cl, cx, dh, dl, dx;
	relativeSector = mod(sector, 18)+1;
	head = mod(div(sector, 18), 2);
	track = div(sector, 36);
	ah = 3;
	al = 1;
	ax = ah*256+al;
	bx = buffer;
	ch = track;
	cl = relativeSector;
	cx = ch*256+cl;
	dh = head;
	dl = 0;
	dx = dh*256+dl;
	interrupt(0x13, ax, bx, cx, dx);
	return;
}
/* Computes and returns the modulus of two integers. */
int mod(int a, int b)
{
	int temp;
	temp = a;
	while (temp >= b) {
		temp = temp - b;
	}
	return temp;
}

/* Computes and returns the quotient from the integer division a/b. */
int div(int a, int b)
{
	int quotient;
	quotient = 0;
	while ((quotient+1)*b <= a) {
		quotient++;
	}
	return quotient;
}

/* Allows you to create your own interrupt  */
void handleInterrupt21(int ax, int bx, int cx, int dx)
{
	char error[26];

	error[0] = 'E';
	error[1] = 'R';
	error[2] = 'R';
	error[3] = 'O';
	error[4] = 'R';
	error[5] = ':';
	error[6] = ' ';
	error[7] = 'U';
	error[8] = 'n';
	error[9] = 'k';
	error[10] = 'n';
	error[11] = 'o';
	error[12] = 'w';
	error[13] = 'n';
	error[14] = ' ';
	error[15] = 'i';
	error[16] = 'n';
	error[17] = 't';
	error[18] = 'e';
	error[19] = 'r';
	error[20] = 'r';
	error[21] = 'u';
	error[22] = 'p';
	error[23] = 't';
	error[24] = '.';
	error[25] = 0;

	if (ax == 0) {
		printString(bx);
	} else if (ax == 1) {
		readString(bx);
	} else if (ax == 2) {
		readSector(bx, cx);
	} else if (ax == 3) {
		readFile(bx, cx);
	} else if (ax == 4) {
		executeProgram(bx, cx);
	} else if (ax == 5) {
		terminate();
	} else if (ax == 8) {
		writeFile(bx, cx, dx);
	} else {
		printString(error);
		printNewLine(); 
	}
	return;
}

/* Reads the file with the given name into the buffer */
void readFile(char *name, char *buffer)
{
	int entryCount, entrySize, i, j, k, nameLength, entryPos;
	int len, bufferPos;
	char directory[SECTOR_SIZE];
	char fileChunk[SECTOR_SIZE];
	char fnf[16];

	fnf[0] = 'F';
	fnf[1] = 'i';
	fnf[2] = 'l';
	fnf[3] = 'e';
	fnf[4] = ' ';
	fnf[5] = 'n';
	fnf[6] = 'o';
	fnf[7] = 't';
	fnf[8] = ' ';
	fnf[9] = 'f';
	fnf[10] = 'o';
	fnf[11] = 'u';
	fnf[12] = 'n';
	fnf[13] = 'd';
	fnf[14] = '.';
	fnf[15] = 0;

	readSector(directory, 2);
	nameLength = MAX_FILE_NAME_LENGTH;
	len = stringLength(name);
	entrySize = DIRECTORY_ENTRY_SIZE;
	entryCount = div(SECTOR_SIZE, entrySize);
	bufferPos = 0;

	for (i=0; i<MAX_FILE_SIZE; i++) {
		buffer[i] = 0;
	}

	/* Search file name in the directory */
	for (i=0; i<entryCount; i++) {
		entryPos = i*entrySize;
		k = 0;
		for (j=entryPos; j<entryPos+len; j++) {
			if (directory[j] != name[k]) {
				k = 0;
				break;
			}
			k++;
		}
		if (k == len) {
			break;
		}
	}
	/* return if no filename is found */
	if (k != len) {
		printString(fnf);
		printNewLine();
		return;
	}
	j = entryPos+nameLength;
	for (i=0; i<entrySize-nameLength; i++) {
		if (directory[j+i] == 0x00) {
			break;
		}
		readSector(fileChunk, directory[j+i]);
		for (k=0; k<SECTOR_SIZE; k++) {
			buffer[bufferPos] = fileChunk[k];
			bufferPos++;
		}
	}
	return;
}



/* Write a file with the given name, buffer, and number of sectors to disk */
void writeFile(char *name, char *buffer, int numberOfSectors)
{
	int entryCount, entrySize, i, j, k, nameLength, entryPos;
	int len, bufferPos;
	char directory[SECTOR_SIZE];
	char map[SECTOR_SIZE];
	char fileChunk[SECTOR_SIZE];
	char error[20];

	error[0] = 'E';
	error[1] = 'r';
	error[2] = 'r';
	error[3] = 'o';
	error[4] = 'r';
	error[5] = ' ';
	error[6] = 'w';
	error[7] = 'r';
	error[8] = 'i';
	error[9] = 't';
	error[10] = 'i';
	error[11] = 'n';
	error[12] = 'g';
	error[13] = ' ';
	error[14] = 'f';
	error[15] = 'i';
	error[16] = 'l';
	error[17] = 'e';
	error[18] = '.';
	error[19] = 0;

	readSector(map, 1);
	readSector(directory, 2);
	nameLength = MAX_FILE_NAME_LENGTH;
	len = stringLength(name);
	entrySize = DIRECTORY_ENTRY_SIZE;
	entryCount = div(SECTOR_SIZE, entrySize);
	bufferPos = 0;

	for (i=0; i<entryCount; i++) {
		entryPos = i*entrySize;
		if (directory[entryPos] == 0) {
			break;
		}
	}
	if (directory[entryPos] != 0) {
		printString(error);
		printNewLine();
		return;
	}

	for (i=0; i<len; i++) {
		directory[entryPos+i] = name[i];
	}
	for ( ; i<nameLength; i++) {
		directory[entryPos+i] = 0;
	}

	if (numberOfSectors > entrySize-nameLength+SECTOR_SIZE-1) {
		printString(error);
		printNewLine();
		return;
	}

	for (j=0, k=3; j<numberOfSectors; i++, j++) {
		while (map[k] == 0xFF && k<SECTOR_SIZE) {
			k++;
		}
		if (k == SECTOR_SIZE) {
			break;
		}
		map[k] = 0xFF;
		directory[entryPos+i] = k;
	}

	if (j != numberOfSectors) {
		printString(error);
		printNewLine();
		return;
	}

	for (i=6, j=0; j<numberOfSectors; i++, j++) {
		for (k=0; k<SECTOR_SIZE; k++, bufferPos++) {
			fileChunk[k] = buffer[bufferPos];
		}
		writeSector(fileChunk, directory[entryPos+i]);
	}

	writeSector(map, 1);
	writeSector(directory, 2);
}  




/* Finds and returns the length of a string */
int stringLength(char *string)
{
	int len;
	len = 0;
	while (string[len] != '\0') {
		len++;
		if (len == MAX_FILE_NAME_LENGTH) {
			break;
		}
	}
	return len;
}

/* Executes the program with the given name in the given segment */
void executeProgram(char *name, int segment)
{
	int i;
	char program[MAX_FILE_SIZE];

	readFile(name, program);
	for (i = 0; i < MAX_FILE_SIZE; i++) {
		putInMemory(segment, i, program[i]);
	}
	launchProgram(segment);
	return;
}

/* Terminates a program or reload the shell*/
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

void itoa(int n, char* buff)
{
	int p;
	int start;
	start = 0;
	p = 10000;
	buff[0] = '\0';
	buff[1] = '\0';
	buff[2] = '\0';
	while(p >= 1){ 
		if(((n/p) > 0) || (start > 0) ){
			*(buff++) = (n/p) + '0';
			n = n - (n/p)*p;
			start = 1;
		}   
		p = p/10;
	}   
	*(buff++) = ' ';
	*(buff) = '\0';

}

