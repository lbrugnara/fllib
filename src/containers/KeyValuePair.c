#include <string.h>
#include "../Mem.h"
#include "KeyValuePair.h"

struct FlKeyValuePair
{
	FlByte *key;
	FlByte *val;
	size_t kdtsize;
	size_t vdtsize;
};

FlKeyValuePair
fl_kvp_new(size_t kdtsize, size_t vdtsize)
{
	FlKeyValuePair kvp = fl_malloc(sizeof(struct FlKeyValuePair));
	kvp->kdtsize = kdtsize;
	kvp->vdtsize = vdtsize;
	kvp->key = fl_malloc(kdtsize);
	kvp->val = fl_malloc(vdtsize);
	return kvp;
}

void
fl_kvp_set(FlKeyValuePair kvp, const FlByte *key, const FlByte *val)
{
	memcpy(kvp->key, key, kvp->kdtsize);
	memcpy(kvp->val, val, kvp->vdtsize);
}

FlKeyValuePair
fl_kvp_new_set(const FlByte *key, size_t kdtsize, const FlByte *val, size_t vdtsize)
{
	FlKeyValuePair kvp = fl_kvp_new(kdtsize, vdtsize);
	fl_kvp_set(kvp, key, val);
	return kvp;
}

size_t
fl_kvp_key_dtsize(FlKeyValuePair kvp)
{
	return kvp->kdtsize;
}

size_t
fl_kvp_val_dtsize(FlKeyValuePair kvp)
{
	return kvp->vdtsize;
}

void* fl_kvp_get_key(FlKeyValuePair kvp)
{
	return kvp->key;
}

void* fl_kvp_get_val(FlKeyValuePair kvp)
{
	return kvp->val;
}

void
fl_kvp_delete(FlKeyValuePair kvp)
{
	fl_free(kvp->key);
	fl_free(kvp->val);
	fl_free(kvp);
}
