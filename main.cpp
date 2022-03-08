#include <iostream>

#include "plant.h"
#include <vector>
#include <math.h>

struct channel
{
    size_t chanNum;
    double nominalValue;
    double normalDeviation;
};

//���� �������� ������
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

//��������� ������� �������
void measureChannels(Plant plant, std::vector<channel> channels, std::vector<double>& measuredValues)
{
    for (size_t i = 0; i < channels.size(); i++)
    {
        measuredValues[i] = plant_measure(channels[i].chanNum, plant);
    }
}

//������� �������� ��������
bool qualityControl(struct channel chan, double actualValue)
{
    return abs(actualValue - chan.nominalValue) < chan.normalDeviation;
}

//���������� ������� ������� ��������
bool boolsConjunction(std::vector<bool> values)
{
    bool result = 1;
    for (bool value : values)
    {
        result = result && value;
    }
    return result;
}


int main()
{

    //���������� ���������� � ��������� �� ��������� ��������
    Plant plant;
    plant_init(plant);

    const size_t CHANNELS_COUNT = 3;
    std::vector<channel> channels(CHANNELS_COUNT);
    std::vector<bool> channelPassedQualityCheck(channels.size());
    size_t measureCount;

    getInitialData(channels, measureCount);
    std::cerr << "\n-------------------------------------------\n";

    //����� ������� ��������� � ��������� ���������� �����������
    std::vector<double> measuredValues(channels.size());
    size_t defectiveCount = 0;
    for (size_t n = 0; n < measureCount; n++)
    {
        measureChannels(plant, channels, measuredValues);

        //�������� ���������� �� ������������ �������� �� ��������� � ����������� �����������
        for (size_t i = 0; i < channels.size(); i++)
        {
            channelPassedQualityCheck[i] = qualityControl(channels[i], measuredValues[i]);
        }

        //����� ���������� ���������
        std::cout << "N" << n + 1 << " measurement result.\n";
        for (size_t i = 0; i < channels.size(); i++)
        {
            std::cout << " |channel " << channels[i].chanNum << ": ";
            std::cout << measuredValues[i] << " ";
            //������ �� ����������
            if (!channelPassedQualityCheck[i])
            {
                std::cout << "!deviation detected";
            }
            std::cout << std::endl;
        }

        //����� ������ ������� � ����������� � ���������� � ��
        //���� ������ ���� ���� ������������ �����...
        if(!boolsConjunction(channelPassedQualityCheck))
        {
            defectiveCount++;

            std::cout << "   " << "Deviations: " << "\n";
            for (size_t i = 0; i < channels.size(); i++)
            {
                if(!channelPassedQualityCheck[i])
                {
                    std::cout << "    |channel " << channels[i].chanNum << ": ";
                    std::cout << "normal deviation: " << channels[i].normalDeviation << " ";
                    std::cout << "actual deviation: " << abs(measuredValues[i] - channels[i].nominalValue) << " ";
                    std::cout << std::endl;
                }
            }
        }

        std::cout << std::endl;
    }

    //������ �������� ��������
    std::cout << "Defective: " << defectiveCount << " | ";
    std::cout << (static_cast<double>(defectiveCount) / measureCount) * 100 << "%\n";


    return 0;
}
