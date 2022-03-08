#include <iostream>

#include "plant.h"
#include <vector>
#include <math.h>

using namespace std;

//Ввод исходных данных
void getInitialData(size_t channels[], size_t channelsCount, size_t& measureCount, double nominalValues[], double normalDeviation[])
{
    cerr << "Enter channels to measure:\n";
    for (size_t i = 0; i < channelsCount; i++)
    {
        cerr << " |L" << i + 1 << ": ";
        cin >> channels[i];
    }

    cerr << "Enter count of measured products: ";
    cin >> measureCount;

    cerr << "Enter nominal values for channels being measured:\n";
    for (size_t i = 0; i < channelsCount; i++)
    {
        cerr << " |Xn" << i + 1 << ": ";
        cin >> nominalValues[i];
    }

    cerr << "Enter normal deviation for channels being measured:\n";
    for (size_t i = 0; i < channelsCount; i++)
    {
        cerr << " |B" << i + 1 << ": ";
        cin >> normalDeviation[i];
    }
}

//Измерение массива каналов
void measureChannels(Plant plant, size_t channels[], size_t channelsCount, double measuredValues[])
{
    for (size_t i = 0; i < channelsCount; i++)
    {
        measuredValues[i] = plant_measure(channels[i], plant);
    }
}

//Формула проверки качества
bool qualityControl(double nominalValue, double actualValue, double normalDeviation)
{
    return abs(actualValue - nominalValue) < normalDeviation;
}

//Конъюнкция массива булевых значений
bool boolsConjunction(bool values[], size_t count)
{
    bool result = values[0];
    for (size_t i = 1; i < count; i++)
    {
        result = result && values[i];
    }
    return result;
}


int main()
{

    //Объявление переменных и получение их начальных значений
    Plant plant;
    plant_init(plant);

    const size_t CHANNELS_COUNT = 3;
    size_t channels[CHANNELS_COUNT];
    double nominalValues[CHANNELS_COUNT];
    double normalDeviation[CHANNELS_COUNT];
    bool channelPassedQualityCheck[CHANNELS_COUNT];
    size_t measureCount;

    getInitialData(channels, CHANNELS_COUNT, measureCount, nominalValues, normalDeviation);
    cerr << "\n-------------------------------------------\n";

    //Опрос каналов измерений и обработка полученных результатов
    double measuredValues[CHANNELS_COUNT];
    size_t defectiveCount = 0;
    for (size_t n = 0; n < measureCount; n++)
    {
        measureChannels(plant, channels, CHANNELS_COUNT, measuredValues);

        //Контроль отклонения от номанального значения по сравнению с номинальным отклонением
        for (size_t i = 0; i < CHANNELS_COUNT; i++)
        {
            channelPassedQualityCheck[i] = qualityControl(nominalValues[i], measuredValues[i], normalDeviation[i]);
        }

        //Вывод полученных измерений
        cout << "N" << n + 1 << " measurement result.\n";
        for (size_t i = 0; i < CHANNELS_COUNT; i++)
        {
            cout << " |channel " << channels[i] << ": ";
            cout << measuredValues[i] << " ";
            //Сигнал об отклонении
            if (!channelPassedQualityCheck[i])
            {
                cout << "!deviation detected";
            }
            cout << endl;
        }

        //Вывод списка каналов с отклонением и информации о нём
        if(!boolsConjunction(channelPassedQualityCheck, CHANNELS_COUNT))
        {
            defectiveCount++;

            cout << "   " << "Deviations: " << "\n";
            for (size_t i = 0; i < CHANNELS_COUNT; i++)
            {
                if(!channelPassedQualityCheck[i])
                {
                    cout << "    |channel " << channels[i] << ": ";
                    cout << "normal deviation: " << normalDeviation[i] << " ";
                    cout << "actual deviation: " << abs(measuredValues[i] - nominalValues[i]) << " ";
                    cout << endl;
                }
            }
        }

        cout << endl;
    }

    //Расчёт процента деффекта
    cout << "Defective: " << defectiveCount << " | ";
    cout << (static_cast<double>(defectiveCount) / measureCount) * 100 << "%\n";


    return 0;
}
