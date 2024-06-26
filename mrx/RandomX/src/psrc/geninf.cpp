#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../blake2/blake2-impl.h"
#include <limits.h>
#include "../blake2/blake2.h"
#include "../blake2/blake2b.c"
using namespace std;

void inxor3(uint64_t a,uint64_t b,uint64_t c,uint64_t d);
static FORCE_INLINE uint64_t inrotr64(const uint64_t w, const unsigned c);
void inxor3(uint64_t a,uint64_t b,uint64_t c,uint64_t d);
static const uint64_t blake2b_IV[8] = {
	UINT64_C(0x6a09e667f3bcc908), UINT64_C(0xbb67ae8584caa73b),
	UINT64_C(0x3c6ef372fe94f82b), UINT64_C(0xa54ff53a5f1d36f1),
	UINT64_C(0x510e527fade682d1), UINT64_C(0x9b05688c2b3e6c1f),
	UINT64_C(0x1f83d9abfb41bd6b), UINT64_C(0x5be0cd19137e2179) };

static const unsigned int blake2b_sigma[12][16] = {
	{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
	{14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3},
	{11, 8, 12, 0, 5, 2, 15, 13, 10, 14, 3, 6, 7, 1, 9, 4},
	{7, 9, 3, 1, 13, 12, 11, 14, 2, 6, 5, 10, 4, 0, 15, 8},
	{9, 0, 5, 7, 2, 4, 10, 15, 14, 1, 11, 12, 6, 8, 3, 13},
	{2, 12, 6, 10, 0, 11, 8, 3, 4, 13, 7, 5, 15, 14, 1, 9},
	{12, 5, 1, 15, 14, 13, 4, 10, 0, 7, 6, 3, 9, 2, 8, 11},
	{13, 11, 7, 14, 12, 1, 3, 9, 5, 0, 15, 4, 8, 6, 2, 10},
	{6, 15, 14, 9, 11, 3, 0, 8, 12, 2, 13, 7, 1, 4, 10, 5},
	{10, 2, 8, 4, 7, 6, 1, 5, 15, 11, 9, 14, 3, 12, 13, 0},
	{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
	{14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3},
};


int inblake(void * output, int dop,void* insout){
    // basic init
    void* koutput = malloc(64);
    memcpy(koutput,output,sizeof(output));
    if(dop == 1){
    FILE* dat ;
    dat = fopen("komhash.txt","rb");
    void* koml = malloc(64);
    fread(koml,64,1,dat);


    memcpy(koutput + 32,koml +32,32);
	fclose(dat);
	}

    //blake inverse function starts
	blake2b_state  S;
    inblakefinal(&S,koutput,dop);
    inblake2b_update(&S,insout,dop);


}

int inblake2b_update(blake2b_state *S, void *in, size_t inlen) {
	uint8_t *pin = (uint8_t *)in;
	uint8_t valstro=0; 
	if(inlen >= 1){
		inlen =128;
		S->buflen -= (unsigned int)inlen;
		valstro=256;
	}
	else{
		inlen =68;
		S->buflen -= (unsigned int)inlen;
		valstro=196;
	}
	
	memcpy((uint8_t*) pin,&S->buf[S->buflen], inlen);
	
	while (inlen <( valstro - BLAKE2B_BLOCKBYTES)) {
		pin -= (size_t) BLAKE2B_BLOCKBYTES;
		inlen += BLAKE2B_BLOCKBYTES;
		inblake2b_increment_counter(S, BLAKE2B_BLOCKBYTES);
		inblakecompress(S, pin);
	}
	pin -=(size_t) BLAKE2B_BLOCKBYTES;
	inlen += BLAKE2B_BLOCKBYTES;
	inblakecompress(S, S->buf);
	inblake2b_increment_counter(S, BLAKE2B_BLOCKBYTES);
	memcpy(pin,&S->buf[0], BLAKE2B_BLOCKBYTES);
	return 0;
}

void inblakefinal (blake2b_state * S,void *in, size_t inlen){
	//getting template
	FILE* dat ;
	if (inlen>=1){
    	dat = fopen("256.txt","rb");
	}
	else{
		dat = fopen("196.txt","rb");
	}
    fread(S,sizeof(S),1,dat);
	fclose(dat);

	//replaceing template hash with hash whose origin has to be calculated
	uint64_t* buffer = (uint64_t*) in; 
	memcpy(&S->h[0],buffer,64);
	inblakecompress(S ,S->buf);
	if (inlen>=1){
		FILE* datk;
		datk = fopen("dre.txt","rb");
        blake2b_state* koml ;
        fread(koml,sizeof(blake2b_state),1,datk);
		S->f[0]=koml->f[0];
		fclose(datk);
	}
	else{
		FILE* datk;
		datk = fopen("dre1.txt","rb");
        blake2b_state* koml ;
        fread(koml,sizeof(blake2b_state),1,datk);
		S->f[0]=koml->f[0];
		fclose(datk);
	}


	inblake2b_increment_counter(S,S->buflen);

}

void inblakecompress(blake2b_state *S , uint8_t *block){
	// l is a ;m is b; n is c ; o is d 
	uint64_t m[16];
	uint64_t v[16];

#define inG( r,  i,  a,  b,  c,  d,  l,  z,  n,  o)                                          \
    do { 									\
		uint64_t ba =0;\
		b = inrotr64(b, 63) ^ c ; \
		c = c - d;   \
		d= inrotr64(d, 16)^ l;    \
		m[(blake2b_sigma[r][2 * (i) + 1])] = l-b;  \
		b = inrotr64(b, 24) ^ c;		\
		c=c-d	;\
		ba = inrotr64(d, 32)^ o;	\
		m[(blake2b_sigma[(r)][2 * (i) + 1])] -=ba ;								\
		a=ba-b;												\
		m[(blake2b_sigma[(r)][2 * (i) + 0])] = (((a & 0x1) ==0 )? a/2: ((a-1)/2));	     \
        a = (((a & 0x1) ==0 )? a/2: ((a-1)/2)+1);  \
		d=o;\
    } while ((void)0, 0)

#define ikROUND(r)                                                               											\
    do {                                                                   													\
        inG((r), 0, v[0], v[4], v[8], v[12], v[0], v[4], v[8],  blake2b_IV[4] ^ S->t[0]);                                     \
        inG((r), 1, v[1], v[5], v[9], v[13], v[1], v[5], v[9], blake2b_IV[5] ^ S->t[1]);                                      \
        inG((r), 2, v[2], v[6], v[10], v[14], v[2], v[6], v[10], blake2b_IV[6] ^ S->f[0]);                                    \
        inG((r), 3, v[3], v[7], v[11], v[15], v[3], v[7], v[11], blake2b_IV[7] ^ S->f[1]);                                    \
        inG((r), 4, v[0], v[5], v[10], v[15], v[0], v[5], v[10], blake2b_IV[7] ^ S->f[1]);                                    \
        inG((r), 5, v[1], v[6], v[11], v[12], v[1], v[6], v[11],  blake2b_IV[4] ^ S->t[0]);                                   \
        inG((r), 6, v[2], v[7], v[8], v[13], v[2], v[7], v[8], blake2b_IV[5] ^ S->t[1]);                                      \
    	inG((r), 7, v[3], v[4], v[9], v[14], v[3], v[4], v[9], blake2b_IV[6] ^ S->f[0]);                                      \
    }	while ((void)0, 0)



	
	unsigned int i, r;

	v[8] = blake2b_IV[0];
	v[9] = blake2b_IV[1];
	v[10] = blake2b_IV[2];
	v[11] = blake2b_IV[3];
	v[12] = blake2b_IV[4] ^ S->t[0];
	v[13] = blake2b_IV[5] ^ S->t[1];
	v[14] = blake2b_IV[6] ^ S->f[0];
	v[15] = blake2b_IV[7] ^ S->f[1];

	for (i = 0; i < 8; ++i) {
		inxor3(S->h[i] ,S->h[i] , v[i],v[i + 8]);
	}
	for (r = 11; r > -1; --r) {
		
		ikROUND(r);
	}
	 

	for (i = 0; i < 16; ++i) {
		block[i] = load64( m + i * sizeof(m[i]));
	}

	for (i = 0; i < 8; ++i) {
		S->h[i] = v[i]  ;
	}

#undef ikROUND 
#undef inG
}


void inxor3(uint64_t a,uint64_t b,uint64_t c,uint64_t d){
	int k= 0;

	while(k<sizeof(a)*8){
		if (a & (0x1<<k)){
			b |= 0x1<<k;
			c &= 0x0<<k;
			d &= 0x0<<k;
		}
		else{
			b &= 0x0<<k;
			c |= 0x1<<k;
			d |= 0x1<<k;

		}

		k+=1;
	}




}

void inxor(int64_t l,int64_t a,int64_t b){
	int k= 0;

	while(k<sizeof(l)*8){
		if (l & (0x1 <<k)){
			a |= 0x1 <<k;
			b &= 0x0 <<k;
			
		}
		else{
			b |= 0x1 <<k;
			a |= 0x1 <<k;

		}

		k+=1;
	}
}

static FORCE_INLINE uint64_t inrotr64(const uint64_t w, const unsigned c) {
	return (w << c) | (w >> (64 - c));
}

static FORCE_INLINE void inblake2b_increment_counter(blake2b_state *S, uint64_t inc) {
	S->t[1] -= (S->t[0] < inc);
	S->t[0] -= inc;
}


