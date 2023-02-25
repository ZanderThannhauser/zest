
#include <stdbool.h>

struct string;
struct strset;

bool strset_add(
	struct strset* this,
	struct string* element);
