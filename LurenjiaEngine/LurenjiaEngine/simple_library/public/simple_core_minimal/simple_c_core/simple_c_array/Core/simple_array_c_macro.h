#pragma once
#include "../../simple_core_minimal.h"

#define SIMPLE_ARRAY_C_STRUCT(array_c_name,data_type) \
typedef struct array_c_name##_type \
{ \
	int size; \
	data_type *data; \
}array_c_name;

#define SIMPLE_ARRAY_C(array_c_name,data_type) \
typedef struct array_c_name##_type \
{ \
	int size; \
	int max_size; \
	data_type *data; \
}array_c_name; \
void init_##array_c_name(array_c_name *array_c) \
{\
	assert(array_c); \
	array_c->size = 0; \
	array_c->max_size = 1; \
	array_c->data = malloc(array_c->max_size * sizeof(data_type)); \
	memset(array_c->data, 0, 1); \
}\
void destroy_##array_c_name(array_c_name *array_c) \
{ \
	assert(array_c); \
	free(array_c->data); \
} \
void add_##array_c_name(data_type in_data, array_c_name *array_c) \
{ \
	assert(array_c && in_data); \
	if (array_c->size >= array_c->max_size) \
	{ \
		array_c->max_size++; \
		array_c->data = realloc(array_c->data, array_c->max_size * sizeof(data_type));\
	} \
	array_c->data[array_c->size] = in_data; \
	array_c->size++; \
} 