#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

union byte{ // two 4 bit values representing instructions. In bytes
  struct{
    unsigned int higher : 4;
    unsigned int lower : 4;
  } bits;
  char byteVal;
} byte;

char * registers[8]={"\%eax","\%ecx","\%edx","\%ebx","\%esp","\%ebp","\%esi","\%edi"};

unsigned int pc=0;

char memory[4096];

int getHexVal(char c){
	if(c >= '0' && c <= '9'){
		return c-'0';
	}
	if(c >= 'A' && c <= 'F'){
		return 10+c-'A';

	}
	if(c >= 'a' && c <= 'f'){
		return 10+c-'a';

	}
	else{
	printf("");
	exit(1);
	}
}

void donoop(){
	pc+=2;
}

void dohalt(){
	pc+=2;
}
void rrmovl(){
	printf("rrmovl\t");
	pc+=2;
	printf("%s, %s\n",registers[getHexVal(memory[pc])],registers[getHexVal(memory[pc+1])] );
	pc+=2;
}
void irmovl(){
	printf("irmovl\t");
	pc+=2;
	printf("$%c%c%c%c,",memory[pc+2],memory[pc+3],memory[pc],memory[pc+1]);
	pc+=8;
	printf("%s\n",registers[getHexVal(memory[pc+1])]);
	pc+=2;
	
}
void rmmovl(){
	printf("rmmovl\t");
	pc+=2;
	printf("%s,",registers[getHexVal(memory[pc])]);
	printf("%c%c%c%c",memory[pc+4],memory[pc+5],memory[pc+2],memory[pc+3]);
	printf("(%s)\n", registers[getHexVal(memory[pc+1])]);
	pc+=10;

}
void mrmovl(){
	printf("mrmovl\t");
	pc+=2;
	printf("%c%c%c%c",memory[pc+4],memory[pc+5],memory[pc+2],memory[pc+3]);	
	printf("(%s),",registers[getHexVal(memory[pc+1])]);
	printf("%s\n",registers[getHexVal(memory[pc])]);
	pc+=10;

}

void addl(){
	printf("addl\t");
	pc+=2;
	printf("%s, %s\n",registers[getHexVal(memory[pc])],registers[getHexVal(memory[pc+1])] );
	pc+=2;

}
void subl(){
	printf("subl\t");
	pc+=2;
	printf("%s, %s\n",registers[getHexVal(memory[pc])],registers[getHexVal(memory[pc+1])] );
	pc+=2;
}
void andl(){
	printf("andl\t");
	pc+=2;
	printf("%s, %s\n",registers[getHexVal(memory[pc])],registers[getHexVal(memory[pc+1])] );
	pc+=2;
}
void xorl(){
	printf("xorl\t");
	pc+=2;
	printf("%s, %s\n",registers[getHexVal(memory[pc])],registers[getHexVal(memory[pc+1])] );
	pc+=2;
}
void mull(){
	printf("mull\t");
	pc+=2;
	printf("%s, %s\n",registers[getHexVal(memory[pc])],registers[getHexVal(memory[pc+1])] );
	pc+=2;
}
void cmpl(){
	printf("cmpl\t");
	pc+=2;
	printf("%s, %s\n",registers[getHexVal(memory[pc])],registers[getHexVal(memory[pc+1])] );
	pc+=2;
}

void jmp(){
	printf("jmp\t");
	printf("%c%c%c%c\n",memory[pc+4],memory[pc+5],memory[pc+2],memory[pc+3]);
	pc+=10;
}
void jle(){
	printf("jle\t");
	printf("%c%c%c%c\n",memory[pc+4],memory[pc+5],memory[pc+2],memory[pc+3]);
	pc+=10;
}
void jl(){
	printf("jl\t");
	printf("%c%c%c%c\n",memory[pc+4],memory[pc+5],memory[pc+2],memory[pc+3]);
	pc+=10;
}
void je(){
	printf("je\t");
	printf("%c%c%c%c\n",memory[pc+4],memory[pc+5],memory[pc+2],memory[pc+3]);
	pc+=10;
}
void jne(){
	printf("jne\t");
	printf("%c%c%c%c\n",memory[pc+4],memory[pc+5],memory[pc+2],memory[pc+3]);
	pc+=10;
}
void jge(){
	printf("jge\t");
	printf("%c%c%c%c\n",memory[pc+4],memory[pc+5],memory[pc+2],memory[pc+3]);
	pc+=10;
}
void jg(){
	printf("jg\t");
	printf("%c%c%c%c\n",memory[pc+4],memory[pc+5],memory[pc+2],memory[pc+3]);
	pc+=10;
}
void call(){
	printf("call\t $");
	printf("%c%c%c%c\n",memory[pc+4],memory[pc+5],memory[pc+2],memory[pc+3]);
	pc+=10;

}
void ret(){
	printf("ret\n");
	pc+=2;
}

void pushl(){
	printf("pushl\t");
	pc+=2;
	printf("%s\n",registers[getHexVal(memory[pc])]);
	pc+=2;
}
void popl(){
	printf("popl\t");
	pc+=2;
	printf("%s\n",registers[getHexVal(memory[pc])]);
	pc+=2;
}

void readB(){
	printf("readB\t");
	pc+=2;
	printf("%s\n",registers[getHexVal(memory[pc])]);
	pc+=10;
}
void readL(){
	printf("readL\t");
	pc+=2;
	printf("%s\n",registers[getHexVal(memory[pc])]);
	pc+=10;
}
void writeB(){
	printf("writeB\t");
	pc+=2;
	printf("%s\n",registers[getHexVal(memory[pc])]);
	pc+=10;
}
void writeL(){
	printf("writeL\t");
	pc+=2;
	printf("%s\n",registers[getHexVal(memory[pc])]);
	pc+=10;
}
void movsbl(){
	printf("movsbl\t");
	pc+=2;
	printf("%s,",registers[getHexVal(memory[pc])]);
	printf("(%s)\n",registers[getHexVal(memory[pc])]);
	pc+=10;
}
int main(int argc, char** argv){

	if(strcmp(argv[1],"-h")==0){
		printf("This program gives out y86 assembly instructions contained in the y86 program file.\n");
		printf("Usage: y86dis <y86 input file>\n");
		exit(0);
	}

	FILE * fyle = fopen(argv[1],"r");
	if(fyle==NULL){
		printf("File not found in directory\n");
		return 1;
	}

	int items=fscanf(fyle,"%s",memory);

	while(items!=EOF){
		if(strcmp(memory,".size")==0){
			printf("Size of program is\t");
			fscanf(fyle,"%s",memory);
			printf("%x\n",(int) strtol(memory,NULL,16));
		}
		else if(strcmp(memory,".text")==0){
			printf("Text directive, time to assemble program.\n");
			fscanf(fyle,"%s",memory);
			fscanf(fyle,"%s",memory);

			pc=0;

			while(memory[pc]!="\0"){
				byte.bits.higher=getHexVal(memory[pc+1]);
				byte.bits.lower=getHexVal(memory[pc]);
				unsigned char op=byte.byteVal;

				switch(op){

					case 0x00:
					donoop();
					break;

					case 0x10:
					dohalt();
					break;

					case 0x20:
					rrmovl();
					break;

					case 0x30:
					irmovl();
					break;

					case 0x40:
					rmmovl();
					break;

					case 0x50:
					mrmovl();
					break;

					case 0x60:
					addl();
					break;

					case 0x61:
					subl();
					break;

					case 0x62:
					andl();
					break;

					case 0x63:
					xorl();
					break;

					case 0x64:
					mull();
					break;

					case 0x65:
					cmpl();
					break;

					case 0x70:
					jmp();
					break;

					case 0x71:
					jle();
					break;

					case 0x72:
					jl();
					break;

					case 0x73:
					je();
					break;

					case 0x74:
					jne();
					break;

					case 0x75:
					jge();
					break;

					case 0x76:
					jg();
					break;

					case 0x80:
					call();
					break;

					case 0x90:
					ret();
					break;

					case 0xa0:
					pushl();
					break;

					case 0xb0:
					popl();
					break;

					case 0xc0:
					readB();
					break;

					case 0xc1:
					readL();
					break;

					case 0xd0:
					writeB();
					break;

					case 0xd1:
					writeL();
					break;

					case 0xe0:
					movsbl();
					break;

					default:

					printf("Invalid operation given in .text.\n");

				}
			}


		}
		items=fscanf(fyle,"%s",memory);
	}
	return 0;
}

