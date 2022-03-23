#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

class Table
{
private:
    std::vector<std::string> columnsHeaders;

public:
    std::string delimiter;

    Table(std::vector<std::string> _columnsHeaders)
    {
        columnsHeaders = _columnsHeaders;
    }

    void generateDelimiter()
    {
        delimiter = "+";
        for (std::string header : columnsHeaders)
        {
            delimiter += std::string(header.length(), '-');
            delimiter += "+";
        }
    }

    void showDelimiter()
    {
        std::cout << delimiter << '\n';
    }

    void showHeaders()
    {
        std::cout << '|';
        for(std::string header : columnsHeaders)
        {
            std::cout << header << '|';
        }
        std::cout << '\n';
    }

    void showRow(std::vector<std::string> values)
    {
        if(values.size() < columnsHeaders.size())
        {
            std::cerr << "Not enought argumants in values vector for Table.showRow()\n";
            return;
        }

        std::cout << '|';
        for(size_t i = 0; i < columnsHeaders.size(); i++)
        {
            std::cout << std::setprecision(3) << std::setw(columnsHeaders[i].length()) << values[i] << '|';
        }
        std::cout << '\n';
    }

};
