#include "BEDirectXDrawable.h"

BEDirectXDrawable::~BEDirectXDrawable()
{
	for (auto r : resources) delete r;
}

BEDirectXDrawableNested::~BEDirectXDrawableNested()
{
	for (auto d : drawables) delete d;
}
