#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/settings.h"
#include "../include/types.h"
#include "interface.h"

/*

#include<stdlib.h>
#include<time.h>

srand(time(NULL));
int random = rand() % 300;

*/

int main(){
	inf_init();


	printf("Writing Data Block\n");
	for(int i=0; i<300; i++){
#ifdef LEAKCHECK
		printf("set: %d\n",i);
#endif
		char *temp=(char*)malloc(PAGESIZE);
		memcpy(temp,&i,sizeof(i));
		inf_make_req(FS_SET_T,i,temp);
		free(temp);
	}

	printf("Writing Log Block\n");
	for(int i=0; i<300; i++){
#ifdef LEAKCHECK
		printf("set: %d\n",i);
#endif
		char *temp=(char*)malloc(PAGESIZE);
		memcpy(temp,&i,sizeof(i));
		inf_make_req(FS_SET_T,i,temp);
		free(temp);
	}


	int check;
	
	printf("Checking Data Block\n");
	for(int i=0; i<300; i++){
		char *temp=(char*)malloc(PAGESIZE);
		inf_make_req(FS_GET_T,i,temp);
		memcpy(&check,temp,sizeof(i));
		printf("get:%d\n",check);
		free(temp);
	}

	printf("Checking SW Log Block\n");
	for(int i=256000; i<256256; i++){
		char *temp=(char*)malloc(PAGESIZE);
		inf_make_req(FS_GET_T,i,temp);
		memcpy(&check,temp,sizeof(i));
		printf("get:%d\n",check);
		free(temp);
	}


	inf_free();
}
