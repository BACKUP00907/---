using namespace std;
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "blake2.h"
#include "blake2-impl.h"

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
    dat = fopen("256.txt","rb");
    fread(S,sizeof(S),1,dat);
	fclose(dat);

	//replaceing template hash with hash whose origin has to be calculated
	uint8_t buffer = (uint8_t) in; 
	for (uint i = 0; i < 8; ++i) { /* Output full hash to temp buffer */
		store64((uint64_t*) S->h[i],buffer + sizeof(S->h[i]) * i);
	}

}

void inblakecompress(blake2b_state *S , uint8_t *block){
	uint64_t m[16];
	uint64_t v[16];
	unsigned int i, r;

	for (i = 0; i < 8; ++i) {
		inxor3(S->h[i] ,S->h[i] , v[i] , v[i + 8]);
	}




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



static void blake2b_compress(blake2b_state *S, const uint8_t *block) {
	uint64_t m[16];
	uint64_t v[16];
	unsigned int i, r;

	for (i = 0; i < 16; ++i) {
		m[i] = load64(block + i * sizeof(m[i]));
	}

	for (i = 0; i < 8; ++i) {
		v[i] = S->h[i];
	}

	v[8] = blake2b_IV[0];
	v[9] = blake2b_IV[1];
	v[10] = blake2b_IV[2];
	v[11] = blake2b_IV[3];
	v[12] = blake2b_IV[4] ^ S->t[0];
	v[13] = blake2b_IV[5] ^ S->t[1];
	v[14] = blake2b_IV[6] ^ S->f[0];
	v[15] = blake2b_IV[7] ^ S->f[1];

#define G(r, i, a, b, c, d)                                                    \
    do {                                                                       \
        a = a + b + m[blake2b_sigma[r][2 * i + 0]];                            \
        d = rotr64(d ^ a, 32);                                                 \
        c = c + d;                                                             \
        b = rotr64(b ^ c, 24);                                                 \
        a = a + b + m[blake2b_sigma[r][2 * i + 1]];                            \
        d = rotr64(d ^ a, 16);                                                 \
        c = c + d;                                                             \
        b = rotr64(b ^ c, 63);                                                 \
    } while ((void)0, 0)

#define ROUND(r)                                                               \
    do {                                                                       \
        G(r, 0, v[0], v[4], v[8], v[12]);                                      \
        G(r, 1, v[1], v[5], v[9], v[13]);                                      \
        G(r, 2, v[2], v[6], v[10], v[14]);                                     \
        G(r, 3, v[3], v[7], v[11], v[15]);                                     \
        G(r, 4, v[0], v[5], v[10], v[15]);                                     \
        G(r, 5, v[1], v[6], v[11], v[12]);                                     \
        G(r, 6, v[2], v[7], v[8], v[13]);                                      \
        G(r, 7, v[3], v[4], v[9], v[14]);                                      \
    } while ((void)0, 0)

	for (r = 0; r < 12; ++r) {
		ROUND(r);
	}

	for (i = 0; i < 8; ++i) {
		S->h[i] = S->h[i] ^ v[i] ^ v[i + 8];
	}

#undef G
#undef ROUND
}