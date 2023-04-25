#include <stdio.h>

#include "exer_com.h"

int main(){
	UNI_EXER uni_val = {"CDGood evening!"};
	UNI_EXER *uni_val_p = &uni_val;
	ST_ARY str_A = {"EFThis is an apple!"};

	printf("*** Init ****\n");
	printf("uni st from=%c\n",uni_val_p->exer.from);
	printf("uni st to=%c\n",uni_val_p->exer.to);
	printf("uni st msg=%s\n",uni_val_p->exer.msg);

	uni_val_p->sary = str_A;

	printf("*** Modify ****\n");
	printf("uni st from=%c\n",uni_val.exer.from);
	printf("uni st to=%c\n",uni_val.exer.to);
	printf("uni st msg=%s\n",uni_val.exer.msg);

	return 0;
}
