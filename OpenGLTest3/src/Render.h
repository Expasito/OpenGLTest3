
#pragma once
#include "Headers.h"

namespace Render {
	 GLFWwindow* init();
	void callBacks(GLFWwindow*, GLFWkeyfun, GLFWframebuffersizefun, GLFWmousebuttonfun, GLFWscrollfun, GLFWcursorposfun);


}