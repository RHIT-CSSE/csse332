//loadFile.c
//Michael Black, 2007
//
//Loads a file into the file system
//This should be compiled with gcc and run outside of the OS

#include <stdio.h>

main(int argc, char* argv[])
{
	int i;

	if (argc<2)
	{
		printf("Specify file name to load\n");
		return;
	}

	//open the source file
	FILE* loadFil;
	loadFil=fopen(argv[1],"r");
	if (loadFil==0)
	{
		printf("File not found\n");
		return;
	}

	//open the floppy image
	FILE* floppy;
	floppy=fopen("floppya.img","r+");
	if (floppy==0)
	{
		printf("floppya.img not found\n");
		return;
	}

	//load the disk map
	char map[512];
	fseek(floppy,512,SEEK_SET);
	for(i=0; i<512; i++)
		map[i]=fgetc(floppy);

	//load the directory
	char dir[512];
	fseek(floppy,512*2,SEEK_SET);
	for (i=0; i<512; i++)
		dir[i]=fgetc(floppy);

	//find a free entry in the directory
	for (i=0; i<512; i=i+0x20)
		if (dir[i]==0)
			break;
	if (i==512)
	{
		printf("Not enough room in directory\n");
		return;
	}
	int dirindex=i;

	//fill the name field with 00s first
	for (i=0; i<6; i++)
		dir[dirindex+i]=0x00;
	//copy the name over
	for (i=0; i<6; i++)
	{
		if(argv[1][i]==0)
			break;
		dir[dirindex+i]=argv[1][i];
	}

	dirindex=dirindex+6;

	//find free sectors and add them to the file
	int sectcount=0;
	while(!feof(loadFil))
	{
		if (sectcount==26)
		{
			printf("Not enough space in directory entry for file\n");
			return;
		}

		//find a free map entry
		for (i=0; i<256; i++)
			if (map[i]==0)
				break;
		if (i==256)
		{
			printf("Not enough room for file\n");
			return;
		}

		//mark the map entry as taken
		map[i]=0xFF;

		//mark the sector in the directory entry
		dir[dirindex]=i;
		dirindex++;
		sectcount++;

		//move to the sector and write to it
		fseek(floppy,i*512,SEEK_SET);
		for (i=0; i<512; i++)
		{
			if (feof(loadFil))
			{
				fputc(0x0, floppy);
				break;
			}
			else
			{
				char c = fgetc(loadFil);
				fputc(c, floppy);
			}	
		}
	}

	//write the map and directory back to the floppy image
        fseek(floppy,512,SEEK_SET);
        for(i=0; i<512; i++)
		fputc(map[i],floppy);
        
        fseek(floppy,512*2,SEEK_SET);
        for (i=0; i<512; i++)
		fputc(dir[i],floppy);

	fclose(floppy);
	fclose(loadFil);
}
