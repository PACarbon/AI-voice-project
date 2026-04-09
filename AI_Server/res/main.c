#include "ifly.h"


int main(int argc, char const *argv[])
{
	// 调用自己封装的方法做语音听写以及文字转语音
    char rec_result[BUFFER_SIZE] = {NULL};
	myiat(rec_result);

	printf("=====================================================================\n");
	rtintf("外部拿到语音听写的结果！ %s \n", rec_result);
	printf("=====================================================================\n");
    
	const char* text = "调用自己封装的方法做语音听写以及文字转语音";
	mytts(text);


exit:
	
	log_out();
	
    return 0;
}