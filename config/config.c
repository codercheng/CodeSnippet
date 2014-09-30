#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "config.h"

//////////////////////////////////////////////////
/*initialize the conf object*/
config_t conf = {
	DEF_INPUT_PATH,
	DEF_OUTPUT_PATH,
	1
};
/////////////////////////////////////////////////

/**
 * remove the space character at the left of the str
 */
static
char * trim_left(char *str) {
	char *p;
	if(str == NULL)
		return NULL;
	p = str;
	while(*p == ' ' && *p != '\0') {
		++p;
	}
	return strcpy(str, p);
}

/**
 * remove the space character at the right of the str
 */
static
char * trim_right(char *str) {
	int len;
	if(str == NULL)
		return NULL;

	len = strlen(str);
	while(len) {
		if(*(str+len-1) == ' ') {
			*(str+len-1) = '\0';
		} else {
			break;
		}
		len--;
	}
	return str;
}

/**
 * get the key and value of the line
 */
static
int get_kv(char *line, char **key, char **value) {
	char *p;
	char *t;
	trim_left(line);
	p = strchr(line, '=');
	if(p == NULL) {
		fprintf(stderr, "BAD FORMAT:%s\n", line);
		return -1;
	}
	*key = line;
	*value = p+1;
	*p = '\0';
	trim_right(*key);
	

	//remove the '\n' at the end of the line
	t = strrchr(*value, '\n');
	if(t != NULL)
		*t = '\0';
	t = strrchr(*value, '\r');
	if(t!= NULL)
		*t = '\0';
	trim_left(*value);
	trim_right(*value);
	return 0;
}

/**
 * read the configuration file, and export to conf object
 */
int read_config(config_t *conf) {
	char config_line[512];
	FILE *fp = fopen(CONFIG_FILE_PATH, "r");
	if(fp == NULL) {
		fprintf(stderr, "Can not open config file:%s\n", strerror(errno));
		return -1;
	}
	
	while(!feof(fp)) {
		int ret;
		char *key = NULL;
		char *value = NULL;
		memset(config_line, 0, sizeof(config_line));
		fgets(config_line, 512, fp);
		//printf("+++%s", config_line);
		trim_left(config_line);
		if(config_line[0] == '#' || config_line[0] == '\n'|| config_line[0] == '\r' || strcmp(config_line, "") == 0)
			continue;
		
		ret = get_kv(config_line, &key, &value);
		if(ret != -1) {
			if(IS_EQUAL(key, "input")) {
				strcpy(conf->input_path, value);
			} else if(IS_EQUAL(key, "output")) {
				strcpy(conf->output_path, value);
			} else if (IS_EQUAL(key, "show_mid_steps")) {
				conf->show_mid_steps = atoi(value);
			}
		}
		
	}
	fclose(fp);
	return 0;
}