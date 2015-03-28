### config:
  这是一个简单的处理配置文件程序，简单易用，只需要`#include "config.h"`，在`config.h`中config_t 类型中添加好需要的配置信息就可以了就可以了。代码也很简单。
  
  bug fix:
  2015-01-11: `同前缀的key会重复匹配,strncmp-->strcmp`
  
  *demo*
  ``` c
  #include "config.h"
  #include <stdio.h>
  
  int main()
  {
  	read_config(&conf);
  	printf("input:%s\n", conf.input_path);
  	printf("output:%s\n", conf.output_path);
  	printf("show:%d\n", conf.show_mid_steps);
  	return 0;
  }
  ```
