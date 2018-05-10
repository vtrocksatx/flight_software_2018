#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

/* VT RockSat-X Frame Generator 
 * 	converts transmitted data recieved from an xbee into readable text.
 *	Created by Taylor Thackaberry
 *	4.27.2018
 */

int main()
{
FILE *input, *jenOutput, *leoOutput, *genOutput;

input = fopen("input.txt", "r"); //TODO: change name of file
if(input == NULL)
{
	fprintf(stderr, "Cannot open input file.\n");
	return 2;
}

//opening the output files
jenOutput = fopen("Jen.txt", "w");
leoOutput = fopen("Leo.txt", "w");
genOutput = fopen("General.txt", "w");

if(jenOutput == NULL)
{
	printf("Cannot open Jen's output file.\n");
	return 2;
}
if(leoOutput == NULL)
{
	printf("Cannot open Leo's output file.\n");
	return 2;
}
if(genOutput == NULL)
{
	printf("Cannot open general report file.\n");
	return 2;
}
uint8_t data[200] = {0}; 
uint8_t currHex = 0x00;
uint8_t x =0;
fscanf(input, "%"SCNu8"", &currHex);
while(currHex != EOF && x < 200)
{
	data[x] = currHex;
	x++;
	fscanf(input, "%"SCNu8"", &currHex);
}

//pointer to the data dump
uint8_t *p = data;
uint8_t start = 0;

//decoding and organizing the data...
uint64_t sourceAddr = *(uint64_t*)(p + start + 4);
uint16_t length = *(uint16_t*)(p + start + 2);
//Note: since length is stored in big endian order, this might have to be flipped?
char* message = calloc((length + 1),  sizeof(char));

for(int x = 0; x < length; x++)
{
	*(message + x) = *(p + x + start + 16);
}

//testing if the message works
fprintf(genOutput, "Message: %s\n", message);

/* finish later
if(f.sourceAddr == "FFFFFFFFFF") //Jen's address
{
}
*/


free(message);

//closing the files
fclose(input);
fclose(jenOutput);
fclose(leoOutput);
fclose(genOutput);
return 1;
}

