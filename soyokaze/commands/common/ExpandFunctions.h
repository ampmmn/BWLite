#pragma once

#include <vector>

namespace soyokaze {
namespace commands {
namespace common {

// $B<B9T;~0z?t(B($1...$n, $*)$B$rE83+$9$k(B
void ExpandArguments(CString& target, const std::vector<CString>& args);
// $B4D6-JQ?t(B($name)$B$rE83+$9$k(B
void ExpandEnv(CString& text);
//
bool ResolaveRelativeExePath(CString& text);


} // end of namespace common
} // end of namespace commands
} // end of namespace soyokaze


