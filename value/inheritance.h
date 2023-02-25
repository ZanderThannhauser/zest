
struct stringtree;

struct value;

struct value_inheritance
{
	struct stringtree* (*print)(const struct value*);
	
	int (*compare)(const struct value*, const struct value*);
	
	void (*free)(struct value*);
};

