#include <iostream>
#include <iomanip>

#include "plant.h"
#include "channel.cpp"
#include <vector>
#include <math.h>
#include <string>

//Ввод исходных данных
void getInitialData(std::vector<channel>& channels, size_t& measureCount)
{
    std::cerr << "Enter channels to measure:\n";
    for (size_t i = 0; i < channels.size(); i++)
    {
        std::cerr << " |L" << i + 1 << ": ";
        std::cin >> channels[i].chanNum;
    }

    std::cerr << "Enter count of measured products: ";
    std::cin >> measureCount;

    std::cerr << "Enter nominal values for channels being measured:\n";
    for (size_t i = 0; i < channels.size(); i++)
    {
        std::cerr << " |Xn" << i + 1 << ": ";
        std::cin >> channels[i].nominalValue;
    }

    std::cerr << "Enter normal deviation for channels being measured:\n";
    for (size_t i = 0; i < channels.size(); i++)
    {
        std::cerr << " |B" << i + 1 << ": ";
        std::cin >> channels[i].normalDeviation;
    }
}

//Измерение всех каналов
void measureChannels(Plant plant, std::vector<channel>& channels)
{
    for (size_t i = 0; i < channels.size(); i++)
    {
        channels[i].lastMeasuredValue = plant_measure(channels[i].chanNum, plant);
    }
}

//Конъюнкция значений проверки качества каждого канала
bool productPassedQualityCheck(std::vector<channel> channels)
{
    bool result = 1;
    for (struct channel chan : channels)
    {
        result = result && chan.passedQualityCheck;
    }
    return result;
}


int main()
{

    //Объявление переменных и получение их начальных значений
    Plant plant;
    plant_init(plant);

    const size_t CHANNELS_COUNT = 3;
    std::vector<channel> channels(CHANNELS_COUNT);
    std::vector<bool> channelPassedQualityCheck(channels.size());
    size_t measureCount;

    getInitialData(channels, measureCount);

    //Составление горизонтального разделителя для вывода таблицы
    std::string tableDelimiter = "+----------+";
    for(size_t i = 0; i < channels.size(); i++)
    {
        tableDelimiter += "--------+-----+---------+---------+";
    }
    tableDelimiter += "-------+";
    //Получаем вид "+----------+--------+-----+---------+---------+--------+-----+---------+---------+--------+-----+---------+---------+-------+"
    std::cout << "\n" << tableDelimiter << "\n";

    //Заголовок таблицы
    std::cout << "|Prod. num.|";
    for (size_t i = 0; i < channels.size(); i++)
    {
        std::cout << "   L" << i + 1 << "   |Good.|Nom. Dev.|Got Dev. |";
    }
    std::cout << "Quality|";
    //Получаем вид "|Prod. num.|   L1   |Good.|Nom. Dev.|Got Dev. |   L2   |Good.|Nom. Dev.|Got Dev. |   L3   |Good.|Nom. Dev.|Got Dev. |Quality|"
    std::cout << "\n" << tableDelimiter << "\n";

    //Опрос каналов измерений и обработка полученных результатов
    size_t defectiveCount = 0;
    for (size_t product = 0; product < measureCount; product++)
    {
        measureChannels(plant, channels);

        //Контроль отклонения от номинального значения по сравнению с номинальным отклонением
        for (size_t i = 0; i < channels.size(); i++)
        {
            channels[i].qualityControl();
        }

        //Вывод полученных измерений
        std::cout << "|" << std::setw(10) << product + 1;
        for (size_t i = 0; i < channels.size(); i++)
        {
            std::string passedQualityCheck = (channels[i].passedQualityCheck ? "+" : "-");
            std::cout << std::setprecision(3)
            << "|" << std::setw(8) << channels[i].lastMeasuredValue
            << "|" << std::setw(3) << passedQualityCheck << "  "
            << "|" << std::setw(9) << channels[i].normalDeviation
            << "|" << std::setw(9) << abs(channels[i].lastMeasuredValue - channels[i].nominalValue);
        }
        bool boolPassedTesting = productPassedQualityCheck(channels);
        std::string passedTesting = (boolPassedTesting ? "+" : "-");
        std::cout << "|" << std::setw(4) << passedTesting << "   ";
        std::cout << "|\n" << tableDelimiter << "\n";

        if(!boolPassedTesting)
        {
            defectiveCount++;
        }
    }

    //Расчёт процента деффекта
    std::cout << "Defective: " << defectiveCount << " | ";
    std::cout << (static_cast<double>(defectiveCount) / measureCount) * 100 << "%\n";

    return 0;
}
