#ifndef __IFLY_H__
#define __IFLY_H__

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "qisr.h"
#include "qtts.h"
#include "msp_cmn.h"
#include "msp_errors.h"

/*iat main以前的所有代码，建议删除上传用户词表*/
#define	BUFFER_SIZE	4096
#define FRAME_LEN	640 
#define HINTS_SIZE  100

extern char g_rec_result[];

int login();

void my_iat();

void my_tts(const char *text);


#endif
