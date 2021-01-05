#include "standard_library.hpp"

#include "App/Menu/Menu.hpp"
#include "App/Utils/GeneratorUUID/GeneratorUUID.hpp"

int main()
{
	std::set<int> mySet;

	for (int i = 0; i < 10000; ++i)
	{
		int uuid = App::Utils::generateUUID();
		std::cout << uuid << '\n';
		if (!mySet.insert(uuid).second)
		{
			std::cout << "Nope, found same value.\n";
			system("pause");
			return 0;
		}
	}


	std::cout << "\n\n\n";
	system("pause");
	return 0;
}