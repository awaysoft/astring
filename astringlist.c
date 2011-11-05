#include "astring.h"
#include "astringlist.h"

struct _AStringList {
	auchar             flag;
	volatile aboolean  lock;

	AString **         data;
	auint              length;
};
