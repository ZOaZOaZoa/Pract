#include <iostream>

#include "plant.h"
#include <vector>
#include <math.h>

struct channel
{
    size_t chanNum;
    double nominalValue;
    double normalDeviation;
    double lastMeasuredValue;
    bool passedQualityCheck;

    void qualityControl()
    {
        passedQualityCheck = abs(lastMeasuredValue - nominalValue) < normalDeviation;
    }

};

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
    std::cerr << "\n-------------------------------------------\n";

    //Опрос каналов измерений и обработка полученных результатов
    size_t defectiveCount = 0;
    for (size_t n = 0; n < measureCount; n++)
    {
        measureChannels(plant, channels);

        //Контроль отклонения от номинального значения по сравнению с номинальным отклонением
        for (size_t i = 0; i < channels.size(); i++)
        {
            channels[i].qualityControl();
        }

        //Вывод полученных измерений
        std::cout << "N" << n + 1 << " measurement result.\n";
        for (struct channel chan : channels)
        {
            std::cout << " |channel " << chan.chanNum << ": ";
            std::cout << chan.lastMeasuredValue << " ";
            //Сигнал об отклонении
            if (!chan.passedQualityCheck)
            {
                std::cout << "!deviation detected";
            }
            std::cout << std::endl;
        }

        //Вывод списка каналов с отклонением и информации о нём
        //Если найден хоть один деффективный канал...
        if(!productPassedQualityCheck(channels))
        {
            defectiveCount++;

            std::cout << "   " << "Deviations: " << "\n";
            for (struct channel chan : channels)
            {
                if(!chan.passedQualityCheck)
                {
                    std::cout << "    |channel " << chan.chanNum << ": ";
                    std::cout << "normal deviation: " << chan.normalDeviation << " ";
                    std::cout << "actual deviation: " << abs(chan.lastMeasuredValue - chan.nominalValue) << " ";
                    std::cout << std::endl;
                }
            }
        }

        std::cout << std::endl;
    }

    //Расчёт процента деффекта
    std::cout << "Defective: " << defectiveCount << " | ";
    std::cout << (static_cast<double>(defectiveCount) / measureCount) * 100 << "%\n";


    return 0;
}
