#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "blake2-impl.h"
#include <limits.h>
#include "blake2.h"
using namespace std;
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

int blake2b(void *out, size_t outlen, const void *in, size_t inlen, const void *key, size_t keylen) {
	blake2b_state S;
	int ret = -1;

	/* Verify parameters */
	if (NULL == in && inlen > 0) {
		goto fail;
	}

	if (NULL == out || outlen == 0 || outlen > BLAKE2B_OUTBYTES) {
		goto fail;
	}

	if ((NULL == key && keylen > 0) || keylen > BLAKE2B_KEYBYTES) {
		goto fail;
	}

	if (keylen > 0) {
		if (blake2b_init_key(&S, outlen, key, keylen) < 0) {
			goto fail;
		}
	}
	else {
		if (blake2b_init(&S, outlen) < 0) {
			goto fail;
		}
	}

	if (blake2b_update(&S, in, inlen) < 0) {
		goto fail;
	}
	ret = blake2b_final(&S, out, outlen);

fail:
	//clear_internal_memory(&S, sizeof(S));
	return ret;
}








int inblake(void * output, int dop){
    // basic init
    void* koutput = malloc(64);
    memcpy(koutput,output,sizeof(output));
    if(dop==0){
        FILE* dat ;
        dat = fopen("komhash.txt","rb");
        void* koml = malloc(64);
        fread(koml,64,1,dat);


        memcpy(koutput + 32,koml +32,32);
		fclose(dat);
    }

    //blake inverse function starts
    
    


}

void inblakefinal (blake2b_state * S,void *in, size_t inlen){
	//getting template
	FILE* dat ;
	if (inlen==1){
    	dat = fopen("256.txt","rb");
	}
	else{
		dat = fopen("196.txt","rb");
	}
    fread(S,sizeof(S),1,dat);
	fclose(dat);

	//replaceing template hash with hash whose origin has to be calculated
	uint8_t buffer = (uint8_t) in; 
	for (uint i = 0; i < 8; ++i) { /* Output full hash to temp buffer */
		store64((uint64_t*) S->h[i],buffer + sizeof(S->h[i]) * i);
	}

}

void inblakecompress(blake2b_state *S , uint8_t *block){
	// l is a ;m is b; n is c ; o is d 
#define inG( i, a, b, c, d, l, m, n, o)                                                    \
    do {       \
		uint64_t ba =0;\
		inxor(inrotr64(b, 63),b,c); \
		c = c - d;   \
		inxor(inrotr64(d, 16),d,ba);    \
		inxor(inrotr64(b, 24),b,c);		\
		c=c-d	;\
		inxor(inrotr64(d, 32),d,a);	\
		m[blake2b_sigma[r][2 * i + 0]]  = ((a - b -l)<0) ? ((a - b -l) + UINT64_MAX +1): (a - b -l) \                        
        m[blake2b_sigma[r][2 * i + 1]]  = ((ba - b -a)<0) ? ((ba - b -a) + UINT64_MAX +1): (ba - b -a) ;  \
    } while ((void)0, 0)
	
#define ikROUND()                                                               \
    do {                                                                   \
        inG( 0, v[0], v[4], v[8], v[12], v[0], v[4], v[8], v[12]);                                      \
        inG( 1, v[1], v[5], v[9], v[13], v[1], v[5], v[9], v[13]);                                      \
        inG( 2, v[2], v[6], v[10], v[14], v[2], v[6], v[10], v[14]);                                     \
        inG( 3, v[3], v[7], v[11], v[15], v[3], v[7], v[11], v[15]);                                     \
        inG( 4, v[0], v[5], v[10], v[15], v[0], v[5], v[10], v[15]);                                     \
        inG( 5, v[1], v[6], v[11], v[12], v[1], v[6], v[11], v[12]);                                     \
        inG( 6, v[2], v[7], v[8], v[13], v[2], v[7], v[8], v[13]);                                      \
        inG( 7, v[3], v[4], v[9], v[14], v[3], v[4], v[9], v[14]);                                      \
    } while((void)0, 0)

	uint64_t m[16];
	uint64_t v[16];
	unsigned int i, r =0;

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
	for (r = 0; r < 12; ++r) {
		ikROUND();
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

static FORCE_INLINE void inblake2b_increment_counter(blake2b_state *S,
	uint64_t inc) {
	S->t[0] -= inc;
	S->t[1] -= (S->t[0] > inc);
}