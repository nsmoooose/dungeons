#ifndef __D_TREE_HPP__
#define __D_TREE_HPP__

#include "object.hpp"

struct pos {
	double x,y,z;
};

enum living {
	alive,
	dead
};

struct tree_type;
struct tree_object : public world_object {
	struct tree_type *type;
	struct pos *pos;
	enum living state;
	double age;
	double height;

	virtual world_object_type *get_type ();
	virtual void update (double d);
};

struct tree_type : public world_object_type {
	unsigned short growth_rate;
	short max_height;
	short min_height;

	virtual world_object *create_instance ();
};

#endif
