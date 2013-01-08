#ifndef __D_OBJECT_HPP__
#define __D_OBJECT_HPP__

struct world_object_type;
struct world_object {
	virtual world_object_type *get_type ()=0;
	virtual void update (double d)=0;
};

struct world_object_type {
	virtual world_object *create_instance () = 0;
};

#endif
