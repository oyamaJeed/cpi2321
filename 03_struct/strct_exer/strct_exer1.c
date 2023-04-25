#include <stdio.h>

#include "exer_com.h"

int main(){

	ST_EXER st_val[] = {	{'1', '2', "Good morning!"},
				{'3', '4', "Good afternoon!"}};
	ST_EXER *st_val_p = &st_val[0];

	char str_A[] = "This is a pen!";

	printf("*** Init ****\n");

	printf("struct from = %c\n", st_val_p->from);
	printf("struct to = %c\n",st_val_p->to);
	printf("struct msg = %s\n",st_val_p->msg);

	st_val_p++;
	printf("struct from = %c\n", st_val_p->from);
	printf("struct to = %c\n",st_val_p->to);
	printf("struct msg = %s\n",st_val_p->msg);

	st_val_p->from = 'A';
	st_val_p->to = 'B';
	for(int i = 0; i < sizeof(str_A); i++){
		*(st_val_p->msg + i) = *(str_A + i);		
	}
	st_val_p--;
	printf("*** Modify ****\n");

	printf("struct from = %c\n", st_val[0].from);
	printf("struct to = %c\n",st_val[0].to);
	printf("struct msg = %s\n",st_val[0].msg);
	
	st_val_p++;
	printf("struct from = %c\n", st_val[1].from);
	printf("struct to = %c\n",st_val[1].to);
	printf("struct msg = %s\n",st_val[1].msg);
	

	return 0;
}


