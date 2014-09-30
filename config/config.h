#ifndef _CONFIG_H
#define _CONFIG_H

#ifdef __cplusplus

extern "C" 
{
#endif

#define CONFIG_FILE_PATH "config.conf"

/**
 * default configuration value
 */
#define DEF_INPUT_PATH   "/simon/input/test.c"
#define DEF_OUTPUT_PATH  "/simon/output/"
#define DEF_SHOW_MID_STEPS 1


#define IS_EQUAL(str1, str2) strncmp(str1, str2, strlen(str2))== 0

typedef struct {
	char input_path[512];
	char output_path[512];
	int show_mid_steps;
} config_t;

extern config_t conf;
extern int read_config(config_t *conf);

#ifdef __cplusplus
}
#endif


#endif
