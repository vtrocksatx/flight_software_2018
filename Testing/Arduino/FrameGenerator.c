#include <stdio.h>


/* VT RockSat-X Frame Generator 
 * 	converts transmitted data recieved from an xbee into readable text.
 *	Created by Taylor Thackaberry
 *	4.27.2018
 */

void readFrame()
{
FILE *input, *jenOutput, *leoOutput, *genOutput;

input = fopen("input.txt", "r"); //TODO: change name of file
if(input == NULL)
{
	fprintf(stderr, "Cannot open input file.\n");
	exit(1);
}

//opening the output files
jenOutput = fopen("Jen.txt", "w");
leoOutput = fopen("Leo.txt", "w");
genOutput = fopen("General.txt", "w");

if(jenOutput == NULL)
{
	printf("Cannot open Jen's output file.\n");
	exit(1);
}
if(leoOutput == NULL)
{
	printf("Cannot open Leo's output file.\n");
	exit(1);
}
if(genOutput == NULL)
{
	printf("Cannot open general report file.\n");
	exit(1);
}

int length;
int frameType;
char* sourceAddr;
int recieveOp;
char* data;
int checkSum;
int start;
//right now the way this info is parsed is bad: the computer can't make sense of the spaces. I'm going to have them read in as chars and have an extra layer of conversion to make them legible. 
fscanf(input, "%2d ", &start);
while(start == 0x7E)
{
	int length = 0;
	fscanf(input, "%2d ", &length);
	fscanf(input, "%2d ", &frameType);
	fscanf(input, "%2d %2d %2d %2d %2d %2d %2d %2d ", &sourceAddr);
	fscanf(input, "%2d %2d ", &sourceAddr16bit);
	fscanf(input, "%2d ", &recieveOp);
	data = malloc((length + (length / 2 - 1) * sizeof(char))):
	if(data == NULL)
	{
		printf(genOutput, "Malloc error.\n");
		exit(2);
	}
	for (int x = 0; x < length + (length / 2 - 1); x++)
	{
		fscanf(input, "%d", *(data + x));
	}
	if(*(sourceAddr) == "FF FF FF FF FF FF FF FF") //Jen's address
	{
		for(int x = 0; x < length + (length / 2 - 1); x++)
		{
			fprintf(jenOutput, "%d", *(data + x);
		}
		fprintf(jenOutput, "\n");
	}
	else if(*(sourceAddr) == "FF FF FF FF FF FF FF FE") //Leo's address
	{
		for(int x = 0; x < length + (length / 2 - 1); x++)
		{
			fprintf(leoOutput, "%d", *(data + x));
		}
		fprintf(leoOutput, "\n");
	}
	else
	{
		fprintf(genOutput, "Data for neither file found.\n");
	}
fscanf(input, "%2d ", &start);
}


//closing the files
fclose(input);
fclose(jenOutput);
fclose(leoOutput);
fclose(genOutput);

}




