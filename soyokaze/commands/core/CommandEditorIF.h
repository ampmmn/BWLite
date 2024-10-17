#pragma once

#include "commands/core/UnknownIF.h"

namespace launcherapp {
namespace core {

class CommandEditor : virtual public UnknownIF
{
public:
	// $B%3%^%s%I$OJT=82DG=$+(B?
	virtual void SetOriginalName(LPCTSTR name) = 0;
	// $B%3%^%s%I$rJT=8$9$k$?$a$N%@%$%"%m%0$r:n@.(B/$B<hF@$9$k(B
	virtual bool DoModal() = 0;
};

}  // end of namespace core
}  // end of namespace launcherapp



