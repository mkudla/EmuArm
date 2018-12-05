/*
Copyright (c) 2015-2018 Marek Kudla.

MIT License

ARM1-like cpu sandbox program
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXPROGRAMLENGTH 10000

const char* programdescription = "\n\nARM1-like cpu sandbox \n\
commands:\n\
mov - moves value to target register, ex: mov 3 2653\n\
shr - shifts register right by shiftvalue , ex: shr 3 2\n\
shl - shifts register left by shiftvalue , ex: shr 3 2\n\
add - adds two registers and puts output in third, ex: add 3 2 5\n\
sub - subs two registers and puts output in third, ex: sub 3 2 5\n\
mul - multiplies two registers and puts output in third, ex: mul 3 2 5\n\
xor - performs exclusive or operation on two regs and puts output in third, ex: xor 3 2 5\n\
and - performs and operation on two regs and puts output in third, ex: and 3 2 5\n\
quit - quits program\n\
edit* - write program line by line, write end and press enter \n\
load* - loads program from file\n\
*currently running loaded program is not implemented.\n\n";

	void fMabort(char *errstring);
	void fMprintbin(unsigned int binval);
	void fMzeroregisters(unsigned int *);
	void fMprintregisters(unsigned int *);
	void fMshrreg(unsigned int *registerarm, int registernumber, int movvalue);
	void fMshlreg(unsigned int *registerarm, int registernumber, int movvalue);
	void fMmovreg(unsigned int *registerarm, int registernumber, int movvalue);
	void fMaddreg(unsigned int *registerarm, int registernumber1,int registernumber2, int registernumber3);
	void fMsubreg(unsigned int *registerarm, int registernumber1,int registernumber2, int registernumber3);
	void fMmulreg(unsigned int *registerarm, int registernumber1,int registernumber2, int registernumber3);
	void fMxorreg(unsigned int *registerarm, int registernumber1,int registernumber2, int registernumber3);
	void fMandreg(unsigned int *registerarm, int registernumber1,int registernumber2, int registernumber3);
	void fMprintprogram(char * szProgram);
	void fMloadprogram(char * progfilename, char * szProgram, int programCounter, int programLength);
	void fMzeroprogram(char * szProgram, int progLength);
	void fMprintbinvl(unsigned int binval, unsigned int length);
	void fMprinthexvl(unsigned int binval, unsigned int length);
	void fMprinthexbyte(char binval);


main(int argc, char *argv[]){

	unsigned int registerarm[16]; // create register bank 16 x 32 bit
	char szProgram[MAXPROGRAMLENGTH]; //create program space list of sz, end is \0\0
	strcpy(szProgram, "\0\0"); // this is to stop the print program short if the list is un-initialized
	fMzeroregisters(&registerarm[0]); //reset the registers
	int interactivemode = 1;
	int programCounter = 0;
	unsigned int binProgram[MAXPROGRAMLENGTH];
	unsigned int binProgramLength = 0;
	
	printf(programdescription);

	while(1) {
		fMprintregisters(&registerarm[0]);
		fMprintprogram(&szProgram[0]);
		char str[100];
		char commandp[10];
		unsigned int val1,val2,val3;

		printf( "Enter command and press enter : ");

		if(interactivemode == 1) {
			gets(str); // gets the input
		} else {
			strcpy(str, &szProgram[programCounter]);
			programCounter += (strlen(str)+1);
			printf("prog line: %s \n", str);
			char dummy[10];
			gets(dummy);
		}

		sscanf(str, "%s%i%i%i", commandp, &val1, &val2, &val3); // processes input

		if(strstr(commandp,"mov")!=NULL){
			fMmovreg(&registerarm[0], val1, val2);
		}else if(strstr(commandp,"add")!=NULL){
			fMaddreg(&registerarm[0], val1, val2, val3);
		}else if(strstr(commandp,"sub")!=NULL){ //etc.
                        fMsubreg(&registerarm[0], val1, val2, val3);
		}else if(strstr(commandp,"mul")!=NULL){
                        fMmulreg(&registerarm[0], val1, val2, val3);
		}else if(strstr(commandp,"xor")!=NULL){ //etc.
                        fMxorreg(&registerarm[0], val1, val2, val3);
		}else if(strstr(commandp,"and")!=NULL){ //etc.
                        fMandreg(&registerarm[0], val1, val2, val3);
		}else if(strstr(commandp,"shr")!=NULL){ //etc.
                        fMshrreg(&registerarm[0], val1, val2);
		}else if(strstr(commandp,"shl")!=NULL){ //etc.
                        fMshlreg(&registerarm[0], val1, val2);
		}else if(strstr(commandp,"end")!=NULL){
			interactivemode = 1;
		}else if(strstr(commandp,"run")!=NULL){
			interactivemode = 0;
		}else if(strstr(commandp,"load")!=NULL){
			char progfilename[20];
			gets(progfilename);
			fMloadprogram(&progfilename[0], &szProgram[0], programCounter, MAXPROGRAMLENGTH);
		}else if(strstr(commandp,"quit")!=NULL){
			break;
		}else if(strstr(commandp,"edit")!=NULL){
			char commandline[20];
			int szProgramIndex = 0;
			fMzeroprogram(&szProgram[szProgramIndex], MAXPROGRAMLENGTH);
			do {
				gets(commandline);
				strcpy(&szProgram[szProgramIndex],commandline);
				szProgramIndex += (strlen(commandline)+1);
			} while (strstr(commandline,"end")==NULL);
			szProgram[szProgramIndex] = '\0';
		}else{
			printf("\n wrong command: %s \n", str);
		}


	}
	return 0;
}

void fMabort(char *errstring){
	fprintf(stderr, errstring);
	exit(1);
}

// prints 32 bit register passed as unsigned integer
void fMprintbin(unsigned int binval) {
	unsigned int index = 0x80000000u;
	char displaystring[] = "00000000000000000000000000000000";
	for (int ii=31;ii>=0;ii--) {
		if((index & binval) >0 ) {
			displaystring[31-ii] = '1';
		}
		index = index >> 1;
	}
	printf(displaystring);
	return;
}

// prints variable length register, also unsigned
void fMprintbinvl(unsigned int binval, unsigned int length) {
	unsigned int index = 0x00000001u;
	index = index << (length-1);
	char displaystring[length];
	for (unsigned int iter = 0; iter < length; iter++) {
		displaystring[iter] = '0';
	}
	for (unsigned int ii=length-1;ii>=0;ii--) {
		if((index & binval) >0 ) {
			displaystring[length-1-ii] = '1';
		}
		index = index >> 1;
	}
	printf(displaystring);
	return;
}

void fMprinthexvl(unsigned int binval, unsigned int length) {

	char displaystring[length/4+1];
	for (unsigned int iter = 0; iter < (length/4); iter+=4) {
		displaystring[iter] = '0';
	}

// code here

	unsigned int mask = 0x0000000Fu;
	for (int iter2 = 7; iter2 >= 0;iter2-- ) { // >=
		unsigned int result = binval & mask;
		switch (result) {
			case 0:
				displaystring[iter2] = '0';
				break;
			case 1:
				displaystring[iter2] = '1';
				break;
			case 2:
				displaystring[iter2] = '2';
				break;
			case 3:
				displaystring[iter2] = '3';
				break;
			case 4:
				displaystring[iter2] = '4';
				break;
			case 5:
				displaystring[iter2] = '5';
				break;
			case 6:
				displaystring[iter2] = '6';
				break;
			case 7:
				displaystring[iter2] = '7';
				break;
			case 8:
				displaystring[iter2] = '8';
				break;
			case 9:
				displaystring[iter2] = '9';
				break;
			case 10:
				displaystring[iter2] = 'A';
				break;
			case 11:
				displaystring[iter2] = 'B';
				break;
			case 12:
				displaystring[iter2] = 'C';
				break;
			case 13:
				displaystring[iter2] = 'D';
				break;
			case 14:
				displaystring[iter2] = 'E';
				break;
			case 15:
				displaystring[iter2] = 'F';
				break;
		}
		binval = binval >> 4;
	}
	displaystring[length/4] = 0;
	printf(displaystring);
	return;
}

void fMprinthexbyte(char binval) {

	char displaystring[2];


// code here

	char mask = 0x0Fu;
	for (int iter2 = 1; iter2 >= 0;iter2-- ) {
		unsigned int result = binval & mask;
		switch (result) {
			case 0:
				displaystring[iter2] = '0';
				break;
			case 1:
				displaystring[iter2] = '1';
				break;
			case 2:
				displaystring[iter2] = '2';
				break;
			case 3:
				displaystring[iter2] = '3';
				break;
			case 4:
				displaystring[iter2] = '4';
				break;
			case 5:
				displaystring[iter2] = '5';
				break;
			case 6:
				displaystring[iter2] = '6';
				break;
			case 7:
				displaystring[iter2] = '7';
				break;
			case 8:
				displaystring[iter2] = '8';
				break;
			case 9:
				displaystring[iter2] = '9';
				break;
			case 10:
				displaystring[iter2] = 'A';
				break;
			case 11:
				displaystring[iter2] = 'B';
				break;
			case 12:
				displaystring[iter2] = 'C';
				break;
			case 13:
				displaystring[iter2] = 'D';
				break;
			case 14:
				displaystring[iter2] = 'E';
				break;
			case 15:
				displaystring[iter2] = 'F';
				break;
		}
		binval = binval >> 4;
	}
	printf(displaystring);
	return;
}


void fMzeroregisters(unsigned int *registerarm) {
	for(int i=0; i<16; i++) {
	registerarm[i] = 0;
	}
}


void fMprintregisters(unsigned int *registerarm) {
	printf("\n");
	printf("\t-----------REGISTERS------------");
printf("\t");
printf("HEX");
printf("\t\t");
printf("DEC");
printf("\t");
	printf("\n");
	for(int i=0; i<16; i++) {
		printf("R%i\t",i);
		fMprintbin(registerarm[i]);
		printf("\t");
		fMprinthexvl(registerarm[i], 32);
		printf("\t");
		printf("%i",registerarm[i]);
		printf("\n");
	}
}

void fMmovreg(unsigned int *registerarm, int registernumber, int movvalue) {
	registerarm[registernumber] = movvalue;
}
void fMshrreg(unsigned int *registerarm, int registernumber, int movvalue) {
	registerarm[registernumber] = registerarm[registernumber] >> movvalue;
}
void fMshlreg(unsigned int *registerarm, int registernumber, int movvalue) {
	registerarm[registernumber] = registerarm[registernumber] << movvalue;
}
void fMaddreg(unsigned int *registerarm, int registernumber1,int registernumber2, int registernumber3) {
	registerarm[registernumber3] = registerarm[registernumber1] + registerarm[registernumber2];
}
void fMsubreg(unsigned int *registerarm, int registernumber1,int registernumber2, int registernumber3) {
	registerarm[registernumber3] = registerarm[registernumber1] - registerarm[registernumber2];
}
void fMmulreg(unsigned int *registerarm, int registernumber1,int registernumber2, int registernumber3) {
	registerarm[registernumber3] = registerarm[registernumber1] * registerarm[registernumber2];
}
void fMxorreg(unsigned int *registerarm, int registernumber1,int registernumber2, int registernumber3) {
	registerarm[registernumber3] = registerarm[registernumber1] ^ registerarm[registernumber2];
}
void fMandreg(unsigned int *registerarm, int registernumber1,int registernumber2, int registernumber3) {
	registerarm[registernumber3] = registerarm[registernumber1] & registerarm[registernumber2];
}
void fMprintprogram(char * szProgram) {
	int programindex = 0;
	do {
		char line[20];
		strcpy(line,&szProgram[programindex]);
		programindex += (strlen(line)+1);
		if(strstr(line,"\n")==NULL)
			printf("%s \n",	line);
		else
			printf("%s",	line);
	} while (szProgram[programindex] != '\0');
}
void fMzeroprogram(char * szProgram, int progLength) {
                for(int i = 0; i < progLength; i++) {
                        szProgram[i] = '\0';
                }
}

void fMloadprogram(char * progfilename, char * szProgram, int programCounter, int programLength) {

	fMzeroprogram(&szProgram[0], programLength);
	FILE *input_file;
	if ((input_file = fopen(progfilename, "r")) == NULL){
		printf("no file \n");
	}
	char programline[20];
	programCounter = 0;
	while(fgets(programline, 20, input_file) != NULL) {
		printf("%s", programline);
		strcpy(&szProgram[programCounter],programline);
		programCounter += (strlen(programline)+1);
	}
	szProgram[programCounter+1] = '\0';
	fclose(input_file);

}


