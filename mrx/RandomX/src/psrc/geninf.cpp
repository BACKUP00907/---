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
    }

    //blake inverse function starts
    
    







}