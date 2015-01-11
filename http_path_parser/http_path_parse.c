#include <stdio.h>
#include <string.h>

typedef struct {
    char *key;
    int key_len;
    char *value;
    int value_len;
} KV;

static
char *checkValid(char *p) {
	if(*p == '\0')
		return NULL;
	while(*(++p) != '\0') {
		if(*p == '=') {
			return NULL;
		} else if(*p == '&') {
			return p;
		}
	}

	return p;
}

int getKV(char *path, KV *kvs, int *kvs_num) {
	char *b = strchr(path, '?');
	int cnt = 0;
	while(b != NULL && *b != '\0' && *(b+1) != '\0') {
        if(*kvs_num < cnt) {/*tha array kvs' len is less than b*/
            break;
		}

		char *p1 = b+1; //start
		char *p2 = strchr(p1, '=');//'='
        if(p2 == NULL) { //error
            return -1;
        }
		char *p3 = strchr(p2, '&');
		if(p3 == NULL) {/*end of the string*/
			p3 = strchr(p2, '\0');
		}
		//check end
		while(checkValid(p3) != NULL) {
			p3 = checkValid(p3);
		}
		//printf("%.*s=%.*s\n", p2-p1, p1, p3-p2-1, p2+1);

		kvs[cnt].key = p1;
		kvs[cnt].key_len = p2-p1;
		kvs[cnt].value = p2+1;
		kvs[cnt].value_len = p3-p2-1;
		cnt++;

		if(p3 != '\0') {
			b = p3;
		} else {
			break;
		}
	}
	*kvs_num = cnt;
	return 0;
}

int main(void) {
	char p[] = "test.html?key1=value1&key2=value3&key4=value&4&&key5=value5";
	
	KV kvs[10];
    int  kvs_num = sizeof(kvs)/sizeof(kvs[0]);
	int ret = getKV(p, kvs, &kvs_num);

	if(ret == -1) {
        printf("invalid path\n");
	}
	int i;
	for (i=0; i<kvs_num; i++) {
        printf("%.*s=%.*s\n", kvs[i].key_len, kvs[i].key, kvs[i].value_len, kvs[i].value);
	}
	return 0;
}

