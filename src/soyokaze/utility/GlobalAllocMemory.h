#pragma once


class GlobalAllocMemory
{
public:
	GlobalAllocMemory(size_t len) : mMemHandle(nullptr)
	{
		mMemHandle = GlobalAlloc(GHND | GMEM_SHARE , len);
	}
	~GlobalAllocMemory() {
		if (mMemHandle) {
			GlobalFree(mMemHandle);
		}
	}

	operator HGLOBAL() { return mMemHandle; }

	void* Lock() {
		if (mMemHandle) {
			return GlobalLock(mMemHandle);
		}
		return nullptr;
	}
	void Unlock() {
		if (mMemHandle) {
			GlobalUnlock(mMemHandle);
		}
	}

	// $B=jM-8"$rJ|4~$9$k(B
	HGLOBAL Release() {
		HGLOBAL h = mMemHandle;
		mMemHandle = nullptr;
		return h;
	}

private:
	HGLOBAL mMemHandle;

};


