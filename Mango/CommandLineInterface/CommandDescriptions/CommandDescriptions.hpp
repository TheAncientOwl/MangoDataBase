#pragma once

namespace Mango::CommandDescriptions
{
	namespace Create
	{
		void syntax(int number);
		void desc();
	}

	namespace Delete
	{
		void syntax(int number);
		void desc();
	}

	namespace Display
	{
		void syntax(int number);
		void desc();
	}

	namespace Drop
	{
		void syntax(int number);
		void desc();
	}

	namespace Insert
	{
		void syntax(int number);
		void desc();
	}

	namespace Truncate
	{
		void syntax(int number);
		void desc();
	}

	namespace Save
	{
		void syntax(int number);
		void desc();
	}

	namespace Import
	{
		void syntax(int number);
		void desc();
	}

	namespace Select
	{
		void syntax(int number);
		void desc();
	}

	namespace Update
	{
		void syntax(int number);
		void desc();
	}

	namespace Exit
	{
		void syntax(int number);
	}
}