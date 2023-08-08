#pragma once

#include "Pattern.h"


/**
 * 部分一致比較を行うためのクラス
 */
class PartialMatchPattern : public Pattern
{
public:
	PartialMatchPattern();
	virtual ~PartialMatchPattern();

	void SetParam(const soyokaze::core::CommandParameter& param) override;
	int Match(const CString& str) override;
	CString GetFirstWord() override;
	CString GetWholeString() override;

protected:
	struct PImpl;
	PImpl* in;
};

