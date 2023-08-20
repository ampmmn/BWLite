#pragma once

#include "commands/specialfolderfiles/SpecialFolderFile.h"

#include <memory>
#include <vector>

namespace soyokaze {
namespace commands {
namespace specialfolderfiles {

class SpecialFolderFiles
{
public:
	SpecialFolderFiles();
	~SpecialFolderFiles();

public:
	bool GetShortcutFiles(std::vector<ITEM>& items);

private:
	void GetFiles(std::vector<ITEM>& items, int csidl);

private:
	struct PImpl;
	std::unique_ptr<PImpl> in;


};


} // end of namespace specialfolderfiles
} // end of namespace commands
} // end of namespace soyokaze
