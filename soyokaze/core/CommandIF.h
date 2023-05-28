#pragma once

#include "core/CommandParameter.h"
#include "Pattern.h"

namespace soyokaze {
namespace core {

class CommandParameter;

class Command
{
protected:
	using Parameter = soyokaze::core::CommandParameter;
public:
	virtual ~Command() {}

	virtual CString GetName() = 0;
	virtual CString GetDescription() = 0;
	virtual BOOL Execute() = 0;
	virtual BOOL Execute(const Parameter& param) = 0;
	virtual CString GetErrorString() = 0;
	virtual HICON GetIcon() = 0;
	virtual int Match(Pattern* pattern) = 0;
	virtual Command* Clone() = 0;

	virtual uint32_t AddRef() = 0;
	virtual uint32_t Release() = 0;

};


} // end of namespace core
} // end of namespace soyokaze
