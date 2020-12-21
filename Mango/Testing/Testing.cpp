#include "standard_library.hpp"
#include "Testing.hpp"

namespace test
{
	std::string toUpper(std::string str)
	{
		std::transform(std::cbegin(str), std::cend(str), std::begin(str), ::toupper);
		return str;
	}

	void columnSerializeDeserialize()
	{
		/*Column id("id", DataType::Value::INT, sizeof(int));
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
		std::cout << col << '\n';*/
	}

	

	void tableSerializeDeserialize()
	{
		/*std::vector<Column> columns{ {"id", DataType::Value::INT, sizeof(int)},
			{"salariu", DataType::Value::FLOAT, sizeof(float) }, {"nume", DataType::Value::STRING, (strlen("nume") + 1) * sizeof(char)} };

		Table idk("idk", "TestDataBase", columns);
		idk.createFiles();
		idk.serializeConfig();

		std::cout << idk << '\n';
		
		std::vector<Column> col;
		Table mrg("idk", "TestDataBase", col);
		mrg.deserializeConfig();
		
		std::cout << mrg << '\n';*/
	}

	

	void mangoSerializeDeserialize()
	{
		/*MangoDB mango("TestDataBase");

		{
			std::vector<Column> columns{ {"id", DataType::Value::INT, sizeof(int)},
			{"salariu", DataType::Value::FLOAT, sizeof(float) }, {"nume", DataType::Value::STRING, 10 * sizeof(char)} };

			auto table = std::make_unique<Table>("idk", "TestDataBase", std::move(columns));

			mango.addTable(std::move(table));
		}

		{
			std::vector<Column> columns{ {"id", DataType::Value::INT, sizeof(int)},
			{"salariu", DataType::Value::FLOAT, sizeof(float) }, {"nume", DataType::Value::STRING, 25 * sizeof(char)} };

			auto table = std::make_unique<Table>("meh", "TestDataBase", std::move(columns));

			mango.addTable(std::move(table));
		}

		mango.storeTables();
		std::cout << mango << '\n';

		MangoDB dataBase("TestDataBase");

		dataBase.loadTables();

		std::cout << dataBase << '\n';
		*/

	}

	void testRow()
	{
		/*auto rowConfig = std::make_shared<RowConfiguration>();
		rowConfig->pushBack(sizeof(int), DataType::Value::INT);
		rowConfig->pushBack(sizeof(float), DataType::Value::FLOAT);
		rowConfig->pushBack(20 * sizeof(char), DataType::Value::STRING);
		rowConfig->pushBack(sizeof(int), DataType::Value::INT);

		size_t rowSize = rowConfig->totalSize();
		Row row(rowSize, rowConfig);

		int id = 5;
		row.setDataAt(0, &id, sizeof(int));

		float salariu = 20.5F;
		row.setDataAt(1, &salariu, sizeof(float));
		
		const char* nume = "Marcel";
		row.setDataAt(2, nume, (strlen(nume) + 1) * sizeof(char));

		int idk = 1;
		row.setDataAt(3, &idk, sizeof(int));

		std::cout << row.getInt(0) << '\n';
		std::cout << row.getFloat(1) << '\n';
		std::cout << row.getString(2) << '\n';
		std::cout << row.getInt(3) << '\n';*/
	}

}