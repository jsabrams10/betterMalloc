#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mymalloc.h"

int main(int argc, char* argv[]){


////////////////////


	//TEST1 - malloc() A POINTER AND FREE IT - PASS

	printf("\n------------------------------\n");
	printf("\nTEST 1\n\n");

	char* testCharPtr1 = malloc(1);

	printf("EXPECTED: *testCharPtr1 == J\n");

	if(testCharPtr1 == NULL){	printf("RECEIVED: testCharPtr1 == NULL\n");}

	else{
	
		*testCharPtr1 = 'J';
		printf("RECEIVED: *testCharPtr1 == %c\n\n", *testCharPtr1);
	}

	free(testCharPtr1);


////////////////////


	//TEST2 - malloc() A POINTER AND FREE IT - PASS

	printf("------------------------------\n");
	printf("\nTEST 2\n\n");

	int* testIntPtr1 = malloc(sizeof(int));

	printf("EXPECTED: *testIntPtr1 == 10\n");

	if(testIntPtr1 == NULL){ printf("RECEIVED: testIntPtr1 == NULL\n");}

	else{

		*testIntPtr1 = 10;
		printf("RECEIVED: *testIntPtr1 == %d\n\n", *testIntPtr1);
	}

	free(testIntPtr1);


////////////////////


	//TEST3 - malloc() A BYTE AND IMMEDIATELY FREE IT 150 TIMES - PASS

	printf("------------------------------\n");
	printf("\nTEST 3\n\n");

	char testCharArr1[150];
	int counter = 0;

	for(; counter < 149; counter++){

		testCharPtr1 = malloc(sizeof(char*));

		if(testCharPtr1 != NULL){

			*testCharPtr1 = 'A';
			testCharArr1[counter] = *testCharPtr1;
		}

		free(testCharPtr1);
	}

	printf("EXPECTED: testCharArr1[125] == A\n");
	printf("RECEIVED: testCharArr1[125] == %c\n\n", testCharArr1[125]);


////////////////////


	//TEST4 - malloc() A BYTE AND STORE THE POINTER IN AN ARRAY 150 TIMES. 
	//FREE EVERYTHING EVERY 50 - PASS
	
	printf("------------------------------\n");
	printf("\nTEST 4\n\n");

	char* testCharPtrArr1[150];
	counter = 0;

	while(counter < 150){

		testCharPtr1 = (char*)malloc(sizeof(char*));

		if(testCharPtr1 != NULL){

			testCharPtrArr1[counter] = testCharPtr1;
			counter++;
		}

		if(counter == 50){

			int counter2 = 0;

			for(; counter2 < 50; counter2++){

				//printf("counter2 == %d", counter2);
				free(testCharPtrArr1[counter2]); // ONE FAIL
			}
		}

		else if(counter == 100){

			int counter2 = 50;

			for(; counter2 < 100; counter2++){

				free(testCharPtrArr1[counter2]); // FAIL
			}
		}

		else if(counter == 150){

			int counter2 = 100;

			for(; counter2 < 150; counter2++){

				free(testCharPtrArr1[counter2]); // FAIL
			}
		}
	}


////////////////////


	//TEST5

	printf("------------------------------\n");
	printf("\nTEST 5\n\n");

	srand(time(0));
	int numMalls = 0;
	int numFrees = 0;
	char* testCharPtrArr2[50];
	counter = 0;

	while(numMalls < 50){

		int coinFlip = rand() % 2;

		if(coinFlip == 0){

			testCharPtr1 = (char*)malloc(1);
			testCharPtrArr2[numMalls] = testCharPtr1;
			numMalls++;
		}

		else{

			if(numMalls > numFrees){

				free(testCharPtrArr2[numFrees]); // FAIL
				numFrees++;
			}
		}
	}

	while(numFrees < 50){

		free(testCharPtrArr2[numFrees]); // FAIL
		numFrees++;
	}


////////////////////


	//TEST6


////////////////////


	//TEST7


////////////////////


	//TEST8 - GRACEFULLY HANDLE BEING ASKED FOR TOO MUCH MEMORY - PASS

	printf("------------------------------\n");
	printf("\nTEST 8\n\n");

	testCharPtr1 = (char*)malloc(4097);

	printf("EXPECTED: testCharPtr1 == NULL\n");

	if(testCharPtr1 == NULL){	printf("RECEIVED: testCharPtr1 == NULL\n\n");}

	else{				printf("RECEIVED: testCharPtr1 != NULL\n\n");}

	free(testCharPtr1);


////////////////////


	//TEST9 - MEMORY SATURATION - PASS

	printf("------------------------------\n");
	printf("\nTEST 9\n\n");

	testCharPtr1 = (char*)malloc(4096 - (sizeof(metaData)));
	char* testCharPtr2 = (char*)malloc(1);

	printf("EXPECTED: testCharPtr1 == NULL\n");

	if(testCharPtr1 == NULL){  	printf("RECEIVED: testCharPtr1 == NULL\n\n");}

	else if(testCharPtr2 == NULL){  printf("RECEIVED: testCharPtr2 == NULL\n\n");}

	free(testCharPtr1);
	free(testCharPtr2);


////////////////////


	//TEST10 - MEMORY SATURATION - PASS

	printf("------------------------------\n");
	printf("\nTEST 10\n\n");

	testCharPtr1 = (char*)malloc(4096 - (sizeof(metaData)) - 1);
	testCharPtr2 = (char*)malloc(1);

	printf("EXPECTED: testCharPtr2 == NULL\n");

	if(testCharPtr1 == NULL){  	printf("RECEIVED: testCharPtr1 == NULL\n\n");}

	else if(testCharPtr2 == NULL){  printf("RECEIVED: testCharPtr2 == NULL\n\n");}

	free(testCharPtr1);
	free(testCharPtr2);


////////////////////


	//TEST11 - FREEING ADDRESSES THAT AREN'T POINTERS - PASS
	
	printf("------------------------------\n");
	printf("\nTEST 7\n\n");

	int testInt;
	free((int*)testInt);

////////////////////


	//TEST12 - FREEING POINTERS THAT WEREN'T ALLOCATED BY MALLOC - PASS
	
	printf("------------------------------\n");
	printf("\nTEST 11\n\n");

	char* u = (char*)malloc(200);
	free(u + 10);
	free(u);

	int* y;
	free(y);


////////////////////


	//TEST13 - REDUNDANT FREEING OF THE SAME POINTER - PASS
	
	printf("------------------------------\n");
	printf("\nTEST 12\n\n");

	char* v = (char*)malloc(100);
	free(v);
	free(v);

	char* w = (char*)malloc(100);
	free(w);
	w = (char*)malloc(100);
	free(w);

printf("------------------------------\n\n");

}



