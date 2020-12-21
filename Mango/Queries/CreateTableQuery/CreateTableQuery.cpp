#include "standard_library.hpp"
#include "CreateTableQuery.hpp"

namespace Mango::Queries
{
	using namespace Mango::Exceptions;

    void CreateTableQuery::parseTableName(std::string_view firstPart)
    {
        auto args = splitAtChar(firstPart, ' ');

        if (args.size() != 3)
            throw InvalidSyntaxException("Invalid create table query syntax");

        if (args[0] != "CREATE" || args[1] != "TABLE")
            throw InvalidArgumentException("Check missing \"CREATE\" nor \"TABLE\" keywords");

        if (!isValidIdentifier(args[2]))
            throw InvalidArgumentException({ "Invalid table name \"", args[2], "\"" });

        if (m_TableName.length() >= MANGO_MAX_TABLE_NAME_LENGTH)
            throw InvalidArgumentException({ "Length limit for table name name is 50 (", args[2], ")" });

        m_TableName = args[2];
    }

    void CreateTableQuery::parseColumns(std::string_view columnsPart)
    {
        auto tuples = splitAtChar(columnsPart, ',');

        if (tuples.empty())
            throw InvalidSyntaxException("Table must have at least one column");

        for (std::set<std::string_view> columnNames; const auto& tuple : tuples)
        {
            auto args = splitAtChar(tuple, ' ');

            if (args.size() < 2)
                throw InvalidSyntaxException({ "Missing something at \"", tuple, "\"" });

            auto& columnName = args[0];
            {
                if (!isValidIdentifier(columnName))
                    throw InvalidArgumentException({ "Invalid column name \"", columnName, "\"" });

                if (columnName.length() >= MANGO_MAX_COLUMN_NAME_LENGTH)
                    throw InvalidArgumentException({ "Length limit for column name is 50 (", columnName, ")" });

                if (!columnNames.insert(columnName).second)
                    throw InvalidSyntaxException("Column names must be unique in table");
            }

            auto& dataType = args[1];
            {
                if (dataType == MANGO_DATA_TYPE_INT_STRING)
                {
                    if (args.size() != 2)
                        throw InvalidSyntaxException({ "Unknown sequence \"", tuple, "\"" });
                    m_Columns.emplace_back(columnName, DataType::Value::INT, sizeof(int));
                }
                else if (dataType == MANGO_DATA_TYPE_FLOAT_STRING)
                {
                    if (args.size() != 2)
                        throw InvalidSyntaxException({ "Unknown sequence \"", tuple, "\"" });
                    m_Columns.emplace_back(columnName, DataType::Value::FLOAT, sizeof(float));
                }
                else if (dataType == MANGO_DATA_TYPE_STRING_STRING)
                {
                    if (args.size() != 3)
                        throw InvalidSyntaxException({ "Unknown sequence \"", tuple, "\"" });

                    int stringLength = 0;
                    try
                    {
                        stringLength = std::stoi((std::string)args[2]);
                    }
                    catch (...)
                    {
                        throw NanException({ "\"", args[2], "\" should be size for \"", columnName, "\"" });
                    }

                    if (stringLength < 1)
                        throw InvalidArgumentException("Text size cannot be less than 1");

                    m_Columns.emplace_back(columnName, DataType::Value::STRING, stringLength * sizeof(char));
                }
                else throw InvalidArgumentException({ "Unknown type \"", dataType, "\"" });
            }
        }

    }

    void CreateTableQuery::checkResidualParts(Statement columns, Statement::iterator stringEnd)
    {
        if (std::string_view end(std::next(columns.closed), stringEnd); end.size() > 1)
        {
            while (end.front() == ' ')
                end.remove_prefix(1);
            end.remove_suffix(1);
            if (!end.empty())
                throw InvalidSyntaxException({ "Unknown sequence \"", end, "\"" });
        }
    }

    bool QUERY_API CreateTableQuery::match(std::string_view sql) const
    {
        return sql.starts_with("CREATE");
    }

    void QUERY_API CreateTableQuery::parse(std::string_view sql)
    {
        m_TableName.clear();
        m_Columns.clear();

        if (sql.back() != ';')
            throw InvalidSyntaxException("Missing ';'");

        Statement::iterator DEFAULT = std::cbegin(sql);
        Statement columns(DEFAULT, DEFAULT, "(", ")");

        for (auto it = std::cbegin(sql), end = std::cend(sql); it != end; ++it)
            switch (*it)
            {
                case ')': columns.closed = it;  break;
                case '(': if (columns.open == DEFAULT)  columns.open = it;  break;
            }

        columns.checkValidOrder(DEFAULT);

        checkResidualParts(columns, std::cend(sql));

        parseTableName({ std::cbegin(sql), columns.open });

        parseColumns({ std::next(columns.open), columns.closed });
    }

    void QUERY_API CreateTableQuery::validate(const_ref<MangoDB> dataBase)
    {
        if (dataBase.getTable(m_TableName))
            throw TableAlreadyExistsException("Table already exists", std::move(m_TableName));
    }

    void QUERY_API CreateTableQuery::execute(ref<MangoDB> dataBase)
    {
        std::unique_ptr<Table> tablePtr = std::make_unique<Table>(std::move(m_TableName), dataBase.m_DirectoryPath,
                                                                  std::move(m_Columns));
        dataBase.addTable(std::move(tablePtr));
    }
}