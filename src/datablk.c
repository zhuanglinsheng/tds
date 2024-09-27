/*
 * Copyright (C) 2022 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#include <tds/datablk.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct datablk {
	size_t __head_loc;
	size_t __len;
	/* `elesize * capacity` more spaces for data storage */
};

#define datablk_basic_size(elesize, capacity) ((elesize) * (capacity))

datablk *tds_datablk_create(size_t elesize, size_t capacity)
{
	size_t data_size = elesize * capacity;
	size_t basic_size = sizeof(datablk);
	size_t total_size = basic_size + data_size;
	datablk *blk = NULL;

	assert(elesize > 0);
	assert(capacity > 0);

	if (NULL == (blk = (datablk *) malloc(total_size))) {
		printf("Error ... tds_datablk_create\n");
		return NULL;
	}
	blk->__head_loc = 0;
	blk->__len = 0;
	return blk;
}

void *tds_datablk_get_data(const datablk *blk, size_t elesize, size_t capacity)
{
	char *p = (char *)blk;

	assert(NULL != blk);
	assert(elesize > 0);
	assert(capacity > 0);

	return p + datablk_basic_size(elesize, capacity);
}

