#include <stdio.h>

#define ARY_MAX 3

char **get_modify_str_ary(int,char *);

int main(int argc, char *argv[])
{
	char inp_ary1[][8] = {"Appl", "Orang", "Banan"};
	for(int i = 0; i < ARY_MAX; i++){
		printf("Before str ary[%d]=%s\n", i, inp_ary1[i]);
	}
	**get_modify_str_ary(3,*inp_ary1);
	printf("-------------------------------------\n");
	for(int i = 0; i < ARY_MAX; i++){
		printf("After str ary[%d]=%s\n", i, inp_ary1[i]);
	}
	
	return 0;
}

char **get_modify_str_ary(int n_mem, char *inps){
	int len_str = sizeof(inps);
	char add_str[][2] = {"e", "e", "a"};
	for(int x = 0; x < n_mem; x++){
		for(int y = 7; y > 0; y--){
			if(*(inps+(x*len_str)+y) != '\0'){
				*(inps+(x*len_str)+y+1) = add_str[x][0];
				*(inps+(x*len_str)+y+2) = add_str[x][1];
				break;
			}
		}
	}
}
