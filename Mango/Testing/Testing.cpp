#include "standard_library.hpp"
#include "Testing.hpp"

namespace test
{
	std::ostream& test::operator<<(std::ostream& out, const Column& col)
	{
		out << ccolor::dark_gray << "[";
		out << ccolor::light_blue << toUpper(std::string(col.name()));
		out << ccolor::dark_gray << "] {";
		out << ccolor::lime << col.dataType().toString() << ccolor::dark_gray << ", " << ccolor::lime << col.size() << " BYTES";
		out << ccolor::dark_gray << "}" << ccolor::reset;
		return out;
	}

	void columnSerializeDeserialize()
	{
		Column id("id", DataType::Value::INT, sizeof(int));
		Column salariu("salariu", DataType::Value::FLOAT, sizeof(float));
		Column nume("nume", DataType::Value::STRING, (strlen("nume") + 1) * sizeof(char));

		std::fstream file;
		file.open("TEST_FILE.dat", std::ios::out | std::ios::trunc | std::ios::binary);

		id.serialize(file);
		salariu.serialize(file);
		nume.serialize(file);

		file.close();

		file.open("TEST_FILE.dat", std::ios::in | std::ios::binary);

		Column col;
		col.deserialize(file);
		std::cout << col << '\n';

		col.deserialize(file);
		std::cout << col << '\n';

		col.deserialize(file);
		std::cout << col << '\n';
	}

}