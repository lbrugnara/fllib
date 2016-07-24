#include "Container.h"

/**
 * {@type} Type of container
 */
struct FlContainer {
    FlContainerType type;
};

FlContainerType
fl_container_get_type(FlContainer *c)
{
	return c->type;
}
