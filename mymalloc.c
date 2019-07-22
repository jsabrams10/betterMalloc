#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"

unsigned short loadShort(unsigned char* metaDataPtr){

	unsigned short mostSigByte = (unsigned short)*metaDataPtr;
	unsigned short leastSigByte = (unsigned short)*(metaDataPtr + 1);
	mostSigByte = mostSigByte << 8;
	return (unsigned short)(mostSigByte | leastSigByte);
}

void storeShort(unsigned short toStore, unsigned char* metaDataPtr){

	unsigned short mostSigByte = toStore;
	unsigned short leastSigByte = toStore;
	mostSigByte = mostSigByte >> 8;
	*metaDataPtr = mostSigByte;
	*(metaDataPtr + 1) = leastSigByte;
}

metaData constructMeta(size_t requestedSize){

	metaData newMeta;
	newMeta.used = (unsigned short)(5341);
	newMeta.size = (unsigned short)(requestedSize);
	newMeta.prev = (unsigned short)(0);
	newMeta.next = (unsigned short)(0);
	return newMeta;
}

metaData loadMeta(unsigned short loadBegex){

	metaData newMeta;
	newMeta.used = loadShort(&(myBlock[loadBegex]));
	newMeta.size = loadShort(&(myBlock[loadBegex + 2]));
	newMeta.prev = loadShort(&(myBlock[loadBegex + 4]));
	newMeta.next = loadShort(&(myBlock[loadBegex + 6]));
	return newMeta;
}

void storeMeta(size_t requestedSize, unsigned short storeBegex){

	metaData newMeta = constructMeta(requestedSize);
	storeShort(newMeta.used, &(myBlock[storeBegex]));
	storeShort(newMeta.size, &(myBlock[storeBegex + 2]));
	storeShort(newMeta.prev, &(myBlock[storeBegex + 4]));
	storeShort(newMeta.next, &(myBlock[storeBegex + 6]));
}

void printMeta(unsigned short printBegex){

	metaData toPrint = loadMeta(printBegex);
	printf("\nUsed = %d\n", toPrint.used);
	printf("Size = %d\n", toPrint.size);
	printf("Prev = %d\n", toPrint.prev);
	printf("Next = %d\n\n", toPrint.next);
}

unsigned short findOpenSpace(size_t requestedSize){

	//EMPTY LIST - RETURN 1
	if(loadShort(&(myBlock[1])) != (unsigned short)(5341) && 
		loadShort(&(myBlock[5])) == (unsigned short)(0)){

		storeShort((unsigned short)(5341), &(myBlock[1]));
		storeShort((unsigned short)(requestedSize), &(myBlock[3]));
		storeShort((unsigned short)(0), &(myBlock[5]));
		storeShort((unsigned short)(0), &(myBlock[7]));
		return 1;
	}

	//INITIALIZE METADATA REFERENCE INDECES
	unsigned short prevMetaBegex = 1;
	metaData prevMeta = loadMeta(1);

	unsigned short newMetaBegex = prevMetaBegex + 8 + prevMeta.size;
	unsigned short nextMetaBegex = prevMeta.next;

	//ITERATE THROUGH LIST CHECKING FOR OPEN SPACE LARGE ENOUGH
	while(prevMeta.next != 0 && (prevMeta.used == (unsigned short)(5341) || 
		prevMetaBegex == (unsigned short)(1))){

		newMetaBegex = prevMetaBegex + 8 + prevMeta.size;
		nextMetaBegex = prevMeta.next;

		//OPEN SPACE LARGE ENOUGH FOUND - PROCEED WITH SETUP
		if(nextMetaBegex - (prevMetaBegex + 8 + prevMeta.size) >= requestedSize + 8){

			storeShort((unsigned short)(5341), &(myBlock[newMetaBegex]));

			storeShort((unsigned short)(requestedSize), 
				&(myBlock[newMetaBegex + 2]));

			//SET newMeta.prev to prevMetaBegex IN ARRAY
			storeShort((unsigned short)(prevMetaBegex),
				&(myBlock[newMetaBegex + 4]));

			//SET newMeta.next TO nextMetaBegex IN ARRAY
			storeShort((unsigned short)(nextMetaBegex), 
				&(myBlock[newMetaBegex + 6]));

			//SET prevMeta.next TO newMetaBegex IN ARRAY
			if(prevMetaBegex != 0){

				storeShort((unsigned short)(newMetaBegex),
					&(myBlock[prevMetaBegex + 6]));
			}

			//SET nextMeta.prev TO newMeta IN ARRAY
			if(nextMetaBegex != 0){

				storeShort((unsigned char)(newMetaBegex), 
					&(myBlock[nextMetaBegex + 4]));
			}

			return newMetaBegex;
		}

		//INCREMENT AND CONTINUE
		prevMetaBegex = prevMeta.next;
		prevMeta = loadMeta(prevMeta.next);
	}

	//OPEN SPACE LARGE ENOUGH FOUND AT END OF LIST - PROCEED WITH SETUP
	if(4095 - newMetaBegex > 1 && 4095 - newMetaBegex >= requestedSize + 8){

		storeShort((unsigned short)(5341),
			&(myBlock[newMetaBegex]));

		storeShort((unsigned short)(requestedSize),
			&(myBlock[newMetaBegex + 2]));

		//SET newMeta.prev TO prevMetaBegex
		storeShort((unsigned short)(prevMetaBegex), &(myBlock[newMetaBegex + 4]));

		//SET newMeta.next TO nextMetaBegex
		storeShort((unsigned short)(nextMetaBegex), &(myBlock[newMetaBegex + 6]));

		//SET prevMeta.next TO newMeta Begex
		if(prevMetaBegex != 0){

			storeShort((unsigned short)(newMetaBegex), 
				&(myBlock[prevMetaBegex + 6]));
		}

		//SET nextMeta.prev TO newMetaBegex
		if(nextMetaBegex != 0){

			storeShort((unsigned char)(newMetaBegex), 
				&(myBlock[nextMetaBegex + 4]));
		}

		return newMetaBegex;
	}

	//NO OPEN SPACE LARGE ENOUGH
	return 0; 
}

void myfree(void* memToFreePtr, const char* fileName, int lineNumber){

	if(memToFreePtr == NULL){

		printf("Failed myfree on line %d of %s; provided NULL pointer.\n\n",
			lineNumber, fileName);

		return;
	}

	unsigned char* metaDataPtr = (unsigned char*)(memToFreePtr) - 8;
	unsigned short currSealCheck = loadShort(metaDataPtr);

	//VALID FREE REQUEST
	if(currSealCheck == (unsigned short)(5341)){

		unsigned char prevMetaBegex = loadShort(metaDataPtr + 4);
		unsigned char nextMetaBegex = loadShort(metaDataPtr + 6);

		//NO prevMeta AND nextMeta IN USE - currMeta IS FIRST IN LIST OF SIZE > 1
		if(prevMetaBegex == 0 && nextMetaBegex != 0){

			//RESET nextMeta.prev TO 0
			storeShort((unsigned short)(0), &(myBlock[nextMetaBegex + 4]));
		}

		//prevMeta IN USE
		else{

			//nextMeta IN USE - currMeta IS NEITHER FIRST NOR LAST IN LIST
			if(nextMetaBegex != 0){

				//RESET prevMeta.next TO nextMetaBegex
				storeShort((unsigned short)(nextMetaBegex), 
					&(myBlock[prevMetaBegex + 6]));

				//RESET nextMeta.prev TO prevMetaBegex
				storeShort((unsigned short)(prevMetaBegex), 
					&(myBlock[nextMetaBegex + 4]));
			}

			//NO nextMeta - currMeta IS LAST IN LIST
			else{

				//RESET prevMeta.next TO 0
				storeShort((unsigned short)(0), 
					&(myBlock[prevMetaBegex + 6]));
			}
		}

		storeShort((unsigned short)(1057), metaDataPtr);
		storeShort((unsigned short)(0), metaDataPtr + 2);
		return;
	}

	//INVALID FREE REQUEST - PRINT AND RETURN

	//printf("@1\n");
	//printMeta(1);
	//printMeta(33);

	printf("Failed myfree on line %d of %s; invalid request.\n\n", 
		lineNumber, fileName);
}

void* mymalloc(size_t requestedSize, const char* fileName, int lineNumber){

	//INVALID MALLOC REQUEST - PRINT AND RETURN
	if(requestedSize < 1 || requestedSize > 4087){

		printf("Failed mymalloc on line %d of %s; invalid request.\n\n",
			lineNumber, fileName);

		return NULL;
	}

	//FIRST MYMALLOC CALL
	if(myBlock[0] != 'Y'){

		myBlock[0] = 'Y';
		findOpenSpace(requestedSize);
		return (void*)(&myBlock[9]);
	}

	//NOT FIRST MYMALLOC CALL
	else{

		unsigned short newMetaBegex = findOpenSpace(requestedSize);

		//NOT ENOUGH SPACE - PRINT AND RETURN
		if(newMetaBegex == 0){

			printf("Failed mymalloc on line %d of %s; not enough space.\n\n",
				lineNumber, fileName);

			return NULL;
		}

		//printf("@1");
		//printMeta(1);

		//printf("@17");
		//printMeta(17);

		//printf("@33");
		//printMeta(33);

		return (void*)(&myBlock[newMetaBegex + 8]);
	}
}



