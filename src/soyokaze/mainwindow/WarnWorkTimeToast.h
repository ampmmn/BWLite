#pragma once

#include <memory>

namespace launcherapp {
namespace mainwindow {

// $BO"B32TF/;~4V$r7Y9p$9$k%H!<%9%H(B
class Toast
{
public:
	Toast();
	~Toast();

	void SetThreshold(int th);

	// $B%H!<%9%H$rI=<($9$k(B
	void Show();
	// $B$3$N%/%i%9$GI=<($7$?4{B8$N%H!<%9%H$r>C$9(B
	void Clear();

private:
	struct PImpl;
	std::unique_ptr<PImpl> in;

};

}
}

