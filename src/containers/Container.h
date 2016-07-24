#ifndef FL_CONTAINER_H
#define FL_CONTAINER_H

#include <stdio.h>
#include <stdlib.h>
#include "../Types.h"

/**
 * Type of containers defained in fllib.
 */
typedef enum {
	FL_CONTAINER_TYPE_VECTOR,
	FL_CONTAINER_TYPE_LIST,
	FL_CONTAINER_TYPE_DICTIONARY
} FlContainerType;

typedef struct FlContainer FlContainer;

/**
 * Returns the {FlContainerType} of container {c}
 */
FlContainerType fl_container_get_type(FlContainer *c);

#endif /* FL_CONTAINER_H */
