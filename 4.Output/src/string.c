#include "string.h"

int str_len(char* c, int max){
	int res = 0;
	// max is used as a fail safe so that the kernel doesn't end up reading the whole memory if the dev makes a mistake
	for(int i = 0; i < max; i++){
		if(*(c + i) != '\0')
			res++;
		else
			break;
	}
	return res;
}
