#define COMMAND_LENGTH 64
#define LINE_FEED 0xa
#define FALSE 0
#define TRUE 1
#define MAX_FILESIZE 25600
#define MAX_INPUT 80
#define SECTOR_SIZE 512
#define MAX_FILE_NAME_LENGTH 6
#define DIRECTORY_ENTRY_SIZE 32


/* Check to see if this is an expected command. */
int isExpectedCommand(char *command, char *expectedCommand);

/* Finds and returns the length of a string. */
int stringLength(char *string);

/* Runs the type command on the given file. */
void runTypeCommand(char *file);

/* Runs the copy command on the given files. */
void runCopyCommand(char *file1, char *file2);



int main()
{
    char prompt[7];
    char input[MAX_INPUT];
    char command[COMMAND_LENGTH];
    char fileName[COMMAND_LENGTH];
    char fileName2[COMMAND_LENGTH];
    char newLine[2];
    int i, j, k, lastIndex;

    prompt[0] = 'S';
    prompt[1] = 'H';
    prompt[2] = 'E';
    prompt[3] = 'L';
    prompt[4] = 'L';
    prompt[5] = '>';
    prompt[6] = '\0';

    newLine[0] = LINE_FEED;
    newLine[1] = '\0';

    while (1) {
        interrupt(0x21, 0, prompt, 0, 0);
        interrupt(0x21, 1, input, 0, 0);

        for (i=0; i<COMMAND_LENGTH; i++) {
            if(input[i] == ' ' || input[i] == '\n') {
                command[i] = '\0';
                break;
            } else {
                command[i] = input[i];
            }
        }
        lastIndex = i;
        command[COMMAND_LENGTH-1] = '\0';
        j = 0;
        for (i=lastIndex+1; i<COMMAND_LENGTH-1; i++) { 
            if (input[i] == ' ' || input[i] == '\n' || input[i] == 0) {
                fileName[j] = '\0';
                break;
            } else {
                fileName[j] = input[i];   
                j++;
            }
        }
        fileName[j] = '\0';
        fileName[COMMAND_LENGTH-1] = '\0';
        lastIndex = i;
        k = 0;
        for (i=lastIndex+1; i<COMMAND_LENGTH-1; i++) { 
            if (input[i] == ' ' || input[i] == '\n' || input[i] == 0) {
                fileName2[k] = '\0';
                break;
            } else {
                fileName2[k] = input[i];   
                k++;
            }
        }
        fileName2[k] = '\0';
        fileName2[COMMAND_LENGTH-1] = '\0';
        

        if (isExpectedCommand(command, "type\0")) {
            if (j == 0) {
                interrupt(0x21, 0, "Argument missing.\n\0", 0, 0);
            } else {
                runTypeCommand(fileName);
            }
        } else if (isExpectedCommand(command, "execute\0")) {  
            if (j == 0) {
                interrupt(0x21, 0, "argument missing.\n\0", 0, 0);
            } else {
                interrupt(0x21, 4, fileName, 0x2000, 0);
            }
        } else if (isExpectedCommand(command, "delete\0")) {  
            if (j == 0) {
                interrupt(0x21, 0, "argument missing.\n\0", 0, 0);
            } else {
                interrupt(0x21, 7, fileName, 0x2000, 0);
            }
        } else if (isExpectedCommand(command, "copy\0")) {  
            if (j == 0 || k == 0) {
                interrupt(0x21, 0, "Argument missing.\n\0", 0, 0);
            } else {
                runCopyCommand(fileName, fileName2);
            }
        } else{
            interrupt(0x21, 0, "Bad command! Enter a correct command.\n\0", 0, 0);
        }
    }
    return 0;
}

/* Check to see if this is an expected command. */
int isExpectedCommand(char *command, char *expectedCommand)
{
    int expected, lenC, lenExC, i;
    expected = TRUE;
    lenC = stringLength(command);
    lenExC = stringLength(expectedCommand);
    if (lenC != lenExC) {
        return FALSE;
    }
    for (i=0; i<lenC; i++) {
        if (command[i] != expectedCommand[i]) {
            expected = FALSE;
            break;
        }
    }
    return expected;
}

/* Finds and returns the length of a string */
int stringLength(char *string)
{
    int len;
    len = 0;
    while (string[len] != '\0') {
        len++;
    }
    return len;
}

/* Runs the type command on the given file. */
void runTypeCommand(char *file)
{
    char fileData[MAX_FILESIZE];
    interrupt(0x21, 3, file, fileData, 0);
    /* Uncomment this after finishing STEP 3 */
    /* interrupt(0x21, 0, fileData, 0, 0); */    
}

/* compare the first n characters of two strings */
int compareString(char* one, char* two, int size)
{
    int i;
    for (i=0; i<size; ++i) {
        if (one[i] != two[i]) {
            return 0;
        }
    }
    return 1;
}

/* Runs the copy command on the given files. */
void runCopyCommand(char *file1, char *file2)
{
    char buffer[MAX_FILESIZE];
    int i,j;

    for(i = 0;i<MAX_FILESIZE;i++)
        buffer[i] = 0x00;

    interrupt(0x21, 3, file1, buffer, 0);

    /* reuse i to count number of sectors */
    for(i = 0;i < MAX_FILESIZE/SECTOR_SIZE;i++){
        /* for each sector check if all entries are zero */
        /*     if yes, end sector counting. */
        for(j = 0;j<SECTOR_SIZE;j++){
            if(buffer[i*SECTOR_SIZE+j] != 0){ 
                break;
            }   
        }   
        /* if all entries in this sector are zero */                                    
        if(j == SECTOR_SIZE){
            break;
        }
    }

    interrupt(0x21, 8, file2, buffer, i);    
}


