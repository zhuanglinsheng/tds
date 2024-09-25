/*
 * Copyright (C) 2022 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#include <tds/databox.h>


struct databox
{
	size_t __capacity;
	size_t __elesize;
	void *__data;  /* created by `malloc` */
};

