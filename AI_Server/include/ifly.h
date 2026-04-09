#ifndef __IFLY_H__
#define __IFLY_H__

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include "qisr.h"
#include "qtts.h"
#include "msp_cmn.h"
#include "msp_errors.h"

/*iat main以前的所有代码，建议删除上传用户词表*/
#define	BUFFER_SIZE	4096
#define FRAME_LEN	640 
#define HINTS_SIZE  100

int login_ifly();

void myiat(char * rec_result);

void mytts(const char *text);

void log_out();

#endif
