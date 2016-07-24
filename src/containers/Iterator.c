#include "../Std.h"
#include "../Mem.h"
#include "Iterator.h"
#include "Container.h"
#include "Vector.h"
#include "Dictionary.h"

struct FlIterator {
    FlContainerType type;
};

struct FlVectorIterator {
    FlContainerType type;
    FlByte* base;
    unsigned int current;
    size_t dtsize;  
};

struct FlListIterator {
    FlContainerType type;
    struct FlListNode *current;
    struct FlListNode *next;
    struct FlListNode *prev;
};

/* Iteration functions */
FlIterator*
fl_iterator_start(void *c)
{
    FlIterator *it = NULL;
	FlContainer *container = c;
    FlContainerType type = fl_container_get_type(container);
    switch(type)
    {
        case FL_CONTAINER_TYPE_VECTOR:
        {
            FlVector *vector = (FlVector*)c;
            size_t dtsize = fl_vector_dtsize(vector);
            FlVectorIterator *vit = fl_malloc(sizeof(FlVectorIterator));
            vit->type = type;
            vit->base = (FlByte*)fl_vector_get(vector, 0) - dtsize;
            vit->current = 0;
            vit->dtsize = dtsize;
            it = (FlIterator*)vit;
            break;
        }
        case FL_CONTAINER_TYPE_LIST:
        case FL_CONTAINER_TYPE_DICTIONARY:
        {
            FlList *list = NULL;
            if (type == FL_CONTAINER_TYPE_LIST)
                list = (FlList*)c;
            else
                list = fl_dictionary_to_list(c);
            FlListIterator *lit = fl_malloc(sizeof(FlListIterator));
            lit->type = type;
            lit->prev = NULL;
            lit->current = NULL;
            lit->next = fl_list_get_node(list, 0);
            it = (FlIterator*)lit;
            break;
        }
        default:
            flm_exit(ERR_FATAL, "Invalid FlContainer type");
    }
    return it;
}

FlIterator*
fl_iterator_end(void *c)
{
    FlIterator *it = NULL;
    FlContainer *container = c;
    FlContainerType type = fl_container_get_type(container);
    switch(type)
    {
        case FL_CONTAINER_TYPE_VECTOR:
        {
            FlVector *vector = (FlVector*)c;
            size_t dtsize = fl_vector_dtsize(vector);
            FlVectorIterator *vit = fl_malloc(sizeof(FlVectorIterator));
            vit->type = type;
            vit->base = (FlByte*)fl_vector_get(vector, 0) + (dtsize * fl_vector_length(vector));
            vit->current = 0;
            vit->dtsize = dtsize;
            it = (FlIterator*)vit;
            break;
        }
        case FL_CONTAINER_TYPE_LIST:
        case FL_CONTAINER_TYPE_DICTIONARY:
        {
            FlList *list = NULL;
            if (type == FL_CONTAINER_TYPE_LIST)
                list = (FlList*)c;
            else
                list = fl_dictionary_to_list(c);
            FlListIterator *lit = fl_malloc(sizeof(FlListIterator));
            lit->type = type;
            lit->prev = fl_list_get_node(list, fl_list_length(list)-1);
            lit->current = NULL;
            lit->next = NULL;
            it = (FlIterator*)lit;
            break;
        }
        default:
            flm_exit(ERR_FATAL, "Invalid FlContainer type");
    }
    return it;
}

bool 
fl_iterator_is_end(FlIterator *it, void* container)
{
    switch(it->type)
    {
        case FL_CONTAINER_TYPE_VECTOR:
        {
        	FlIterator *end = fl_iterator_end(container);
        	bool res = fl_iterator_equals(it, end);
            fl_free(end);
            return res;
        }
        case FL_CONTAINER_TYPE_LIST:
        case FL_CONTAINER_TYPE_DICTIONARY:
        {
            FlListIterator *lit = (FlListIterator*)it;
            return lit->current == NULL;
        }
        default:
            flm_exit(ERR_FATAL, "Invalid FlIterator type");
    }
    return true;
}

FlPointer
fl_iterator_next(FlIterator *it)
{
    switch(it->type)
    {
        case FL_CONTAINER_TYPE_VECTOR:
        {
            FlVectorIterator *vit = (FlVectorIterator*)it;
            vit->current += vit->dtsize;
            break;
        }
        case FL_CONTAINER_TYPE_LIST:
        case FL_CONTAINER_TYPE_DICTIONARY:
        {
            FlListIterator *lit = (FlListIterator*)it;
            lit->prev = lit->current;
            lit->current = lit->next;
            if (lit->current != NULL )
                lit->next = fl_list_node_next(lit->current);
            else
                lit->next = NULL;
            break;
        }
        default:
            flm_exit(ERR_FATAL, "Invalid FlIterator type");
    }
    return it;
}

FlPointer
fl_iterator_prev(FlIterator *it)
{
    switch(it->type)
    {
        case FL_CONTAINER_TYPE_VECTOR:
        {
            FlVectorIterator *vit = (FlVectorIterator*)it;
            vit->current -= vit->dtsize;
            break;
        }
        case FL_CONTAINER_TYPE_LIST:
        case FL_CONTAINER_TYPE_DICTIONARY:
        {
            FlListIterator *lit = (FlListIterator*)it;
            lit->next = lit->current;
            lit->current = lit->prev;
            if (lit->current != NULL )
                lit->prev = fl_list_node_prev(lit->current);
            else
                lit->prev = NULL;
            break;
        }
        default:
            flm_exit(ERR_FATAL, "Invalid FlIterator type");
    }
    return it;
}

FlPointer 
fl_iterator_value(FlIterator *it)
{
    switch(it->type)
    {
        case FL_CONTAINER_TYPE_VECTOR:
        {
            FlVectorIterator *vit = (FlVectorIterator*)it;
            return vit->base + vit->current;
        }
        case FL_CONTAINER_TYPE_LIST:
        case FL_CONTAINER_TYPE_DICTIONARY:
        {
            FlListIterator *lit = (FlListIterator*)it;
            return fl_list_node_data(lit->current);
        }
        default:
            flm_exit(ERR_FATAL, "Invalid FlIterator type");
    }
    return NULL;
}

bool
fl_iterator_equals(FlIterator *i1, FlIterator *i2)
{
    if (i1->type != i2->type)
        return false;

    switch(i1->type)
    {
        case FL_CONTAINER_TYPE_VECTOR:
        {
            FlVectorIterator *vi1 = (FlVectorIterator*)i1;
            FlVectorIterator *vi2 = (FlVectorIterator*)i2;
            return vi1->base+vi1->current == vi2->base + vi2->current;
        }
        case FL_CONTAINER_TYPE_LIST:
        case FL_CONTAINER_TYPE_DICTIONARY:
        {
            FlListIterator *lit1 = (FlListIterator*)i1;
            FlListIterator *lit2 = (FlListIterator*)i2;
            return lit1->current == lit2->current;
        }
        default:
            flm_exit(ERR_FATAL, "Invalid FlIterator type");
    }
    return false;
}

bool
fl_iterator_has_value(FlIterator *it)
{
    switch(it->type)
    {
        case FL_CONTAINER_TYPE_VECTOR:
        {
            FlVectorIterator *vit = (FlVectorIterator*)it;
            return (vit->base + vit->current) != NULL;
        }
        case FL_CONTAINER_TYPE_LIST:
        case FL_CONTAINER_TYPE_DICTIONARY:
        {
            FlListIterator *lit = (FlListIterator*)it;
            return fl_list_node_data(lit->current) != NULL;
        }
        default:
            flm_exit(ERR_FATAL, "Invalid FlIterator type");
    }
    return false;
}

size_t
fl_iterator_current_index(FlIterator *it)
{
    switch(it->type)
    {
        case FL_CONTAINER_TYPE_VECTOR:
        {
            FlVectorIterator *vit = (FlVectorIterator*)it;
            if (vit->dtsize == 0)
                return -1;
            return vit->current / vit->dtsize;
        }
        case FL_CONTAINER_TYPE_LIST:
        case FL_CONTAINER_TYPE_DICTIONARY:
        {
            flm_exit(ERR_UNIMPLEMENTED, "current_index is not implemented for container of type FlList");
            return 0;
        }
        default:
            flm_exit(ERR_FATAL, "Invalid FlIterator type");
    }
    return 0;
}
