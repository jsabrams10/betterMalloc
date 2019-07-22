#ifndef mymalloc_h
#define mymalloc_h

#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)

static unsigned char myBlock[4096];

//8-BYTE META DATA
typedef struct _metaData{
	unsigned short used;
	unsigned short size;
	unsigned short prev;
	unsigned short next;
} metaData;

void* mymalloc(size_t, const char*, int);
void myfree(void*, const char*, int);

#endif
