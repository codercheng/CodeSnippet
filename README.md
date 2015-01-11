CodeSnippet
===========

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
### http_path_parser
   该程序用来处理http request中path。例如`test.php?user_name=simon&age=19`
   得到该path中所有的Key-value对。
   
   特别：对于`test.php?username=si&&mon&age=19` user_name中包含`&`的情况也做了处理。会得到`si&&mon`而不是`si`.
   
   定义了一个struct：
   ```c
   typedef struct {
	    char *key;
	    int key_len;
	    char *value;
	    int value_len;
	} KV;
   ```
   来记录key/value的起始位置和长度。
   接口：
   ```c
   int getKV(char *path, KV *kvs, int *kvs_num)
   ```
   传入一个KV数组和数组长度:
   ```c
   KV kvs[10];
   int  kvs_num = sizeof(kvs)/sizeof(kvs[0]);
   int ret = getKV(p, kvs, &kvs_num);
   int i;
   for (i=0; i<kvs_num; i++) {
        printf("%.*s=%.*s\n", kvs[i].key_len, kvs[i].key, kvs[i].value_len, kvs[i].value);
   }
   ```
   
   
