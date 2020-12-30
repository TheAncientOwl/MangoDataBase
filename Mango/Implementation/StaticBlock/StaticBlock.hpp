#pragma once

///resource for static_block: https://stackoverflow.com/questions/19227664/whats-the-c-idiom-equivalent-to-the-java-static-block
#define CONCATE_(X,Y) X##Y
#define CONCATE(X,Y) CONCATE_(X,Y)
#define UNIQUE(NAME) CONCATE(NAME, __LINE__)

// `UNIQUE` macro required if we expect multiple `static` blocks in function
#define MANGO_STATIC_BLOCK static Mango::Implementation::Static_ UNIQUE(block) = [&]() -> void

namespace Mango::Implementation
{
	struct Static_
	{
		template<typename T>
		Static_(T only_once) { only_once(); }

		~Static_() {}  // to counter "warning: unused variable"
	};
}