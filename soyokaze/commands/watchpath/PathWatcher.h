#pragma once

namespace soyokaze {
namespace commands {
namespace watchpath {

class PathWatcher
{
public:
	struct ITEM {
		CString mPath;
		CString mMessage;
	};
private:
	PathWatcher();
	~PathWatcher();

public:
	static PathWatcher* Get();

	void RegisterPath(const CString& cmdName, const ITEM& item);
	void UnregisterPath(const CString& cmdName);
	
protected:
	struct PImpl;
	std::unique_ptr<PImpl> in;
};

}
}
}

