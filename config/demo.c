#include "config.h"
#include <stdio.h>

int main()
{
	int ret;
	ret = read_config(&conf);
	if(ret == -1) {
		return -1;
	}
	printf("input:%s\n", conf.input_path);
	printf("output:%s\n", conf.output_path);
	printf("show:%d\n", conf.show_mid_steps);

	return 0;
}
