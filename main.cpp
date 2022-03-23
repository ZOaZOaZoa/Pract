#include <iostream>
#include <iomanip>

#include "plant.h"
#include "channel.cpp"
#include "table.cpp"
#include <vector>
#include <math.h>
#include <string>

//Ввод исходных данных
void getInitialData(std::vector<channel>& channels, size_t& measureCount);
//Измерение всех каналов
void measureChannels(Plant plant, std::vector<channel>& channels);
//Подготовка вектора всех значений входящих в одну строку с результатами измерений в таблице
void fillMeasurementsResultsVectorForPrint(const std::vector<channel>& channels, size_t product, std::vector<std::string>& values);
//Преобразование double в нужный вид для вывода в таблице
std::string convertToString(double num);
//Конъюнкция значений проверки качества каждого канала
bool productPassedQualityCheck(std::vector<channel> channels);

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

    //Создание заголовка таблицы
    std::vector<std::string> headers = { "Prod. num.",
                                            "   L1   ", "Good.", "Got Dev. ", "Nom. Dev.",
                                            "   L2   ", "Good.", "Got Dev. ", "Nom. Dev.",
                                            "   L3   ", "Good.", "Got Dev. ", "Nom. Dev.",
                                                "Quality" };
    Table table = Table(headers);
    table.generateDelimiter();
    table.showDelimiter();
    table.showHeaders();
    table.showDelimiter();

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

        //Сборка вектора значений для печати их в таблице
        std::vector<std::string> values(1);
        fillMeasurementsResultsVectorForPrint(channels, product, values);
        table.showRow(values);
        table.showDelimiter();

        if(!productPassedQualityCheck(channels))
        {
            defectiveCount++;
        }
    }

    //Расчёт процента деффекта
    std::cout << "Defective: " << defectiveCount << " | ";
    std::cout << (static_cast<double>(defectiveCount) / measureCount) * 100 << "%\n";

    return 0;
}

void fillMeasurementsResultsVectorForPrint(const std::vector<channel>& channels, size_t product, std::vector<std::string>& values)
{
    values[0] = std::to_string(product + 1);
    for(size_t i = 0; i < channels.size(); i++)
    {
        std::string passedQualityCheck = (channels[i].passedQualityCheck ? "+  " : "-  ");
        std::string deviation = convertToString(abs(channels[i].lastMeasuredValue - channels[i].nominalValue));

        values.push_back(convertToString( channels[i].lastMeasuredValue ));
        values.push_back(passedQualityCheck);
        values.push_back(deviation);
        values.push_back(convertToString( channels[i].normalDeviation ));
    }
    std::string passedTesting = productPassedQualityCheck(channels) ? "+   " : "-   ";
    values.push_back(passedTesting);
}

std::string convertToString(double num)
{
    num = round(num * 100) / 100;
    std::string str = std::to_string(num);
    str.erase( str.find_last_not_of('0') + 1, std::string::npos );
    if(str.back() == '.')
    {
        str.pop_back();
    }

    return str;
}

bool productPassedQualityCheck(std::vector<channel> channels)
{
    bool result = 1;
    for (struct channel chan : channels)
    {
        result = result && chan.passedQualityCheck;
    }
    return result;
}

void measureChannels(Plant plant, std::vector<channel>& channels)
{
    for (size_t i = 0; i < channels.size(); i++)
    {
        channels[i].lastMeasuredValue = plant_measure(channels[i].chanNum, plant);
    }
}

void getInitialData(std::vector<channel>& channels, size_t& measureCount)
{
    //Переменная для проверкри правильности ввода
    bool correctInput;
    //Защита от бесконечного цикла при автоматическом вводе данных
    size_t counter;
    std::cerr << "Enter channels to measure:\n";
    for (size_t i = 0; i < channels.size(); i++)
    {
        correctInput = false;
        counter = 0;
        while(!correctInput && counter < 100)
        {
            std::cerr << " |L" << i + 1 << ": ";
            std::cin >> channels[i].chanNum;

            if(1 <= channels[i].chanNum && channels[i].chanNum <= 6 || 11 <= channels[i].chanNum && channels[i].chanNum <= 90)
            {
                correctInput = true;
            }
            else
            {
                std::cerr << "Only 1-6, 11-90 channels can be measured\n";
                counter++;
            }
        }
    }

    correctInput = false;
    counter = 0;
    while(!correctInput && counter < 100)
    {
        std::cerr << "Enter count of measured products: ";
        std::cin >> measureCount;
        if(measureCount > 0)
        {
            correctInput = true;
        }
        else
        {
            std::cerr << "Enter positive count of measured products\n";
            counter++;
        }
    }

    std::cerr << "Enter nominal values for channels being measured:\n";
    for (size_t i = 0; i < channels.size(); i++)
    {
        std::cerr << " |Xn" << i + 1 << ": ";
        std::cin >> channels[i].nominalValue;
    }

    std::cerr << "Enter normal deviation for channels being measured:\n";
    for (size_t i = 0; i < channels.size(); i++)
    {
        correctInput = false;
        counter = 0;
        while(!correctInput && counter < 100)
        {
            std::cerr << " |B" << i + 1 << ": ";
            std::cin >> channels[i].normalDeviation;

            if(channels[i].normalDeviation >= 0)
            {
                correctInput = true;
            }
            else
            {
                std::cerr << "Enter positive deviation\n";
                counter++;
            }
        }
    }
}
