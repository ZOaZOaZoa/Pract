#include <iostream>

#include "plant.h"
#include <vector>
#include <math.h>

using namespace std;

struct channel
{
    size_t chanNum;
    double nominalValue;
    double normalDeviation;
};

//���� �������� ������
void getInitialData(vector<channel>& channels, size_t& measureCount)
{
    cerr << "Enter channels to measure:\n";
    for (size_t i = 0; i < channels.size(); i++)
    {
        cerr << " |L" << i + 1 << ": ";
        cin >> channels[i].chanNum;
    }

    cerr << "Enter count of measured products: ";
    cin >> measureCount;

    cerr << "Enter nominal values for channels being measured:\n";
    for (size_t i = 0; i < channels.size(); i++)
    {
        cerr << " |Xn" << i + 1 << ": ";
        cin >> channels[i].nominalValue;
    }

    cerr << "Enter normal deviation for channels being measured:\n";
    for (size_t i = 0; i < channels.size(); i++)
    {
        cerr << " |B" << i + 1 << ": ";
        cin >> channels[i].normalDeviation;
    }
}

//��������� ������� �������
void measureChannels(Plant plant, vector<channel> channels, vector<double>& measuredValues)
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
bool boolsConjunction(vector<bool> values)
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
    vector<channel> channels(CHANNELS_COUNT);
    vector<bool> channelPassedQualityCheck(channels.size());
    size_t measureCount;

    getInitialData(channels, measureCount);
    cerr << "\n-------------------------------------------\n";

    //����� ������� ��������� � ��������� ���������� �����������
    vector<double> measuredValues(channels.size());
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
        cout << "N" << n + 1 << " measurement result.\n";
        for (size_t i = 0; i < channels.size(); i++)
        {
            cout << " |channel " << channels[i].chanNum << ": ";
            cout << measuredValues[i] << " ";
            //������ �� ����������
            if (!channelPassedQualityCheck[i])
            {
                cout << "!deviation detected";
            }
            cout << endl;
        }

        //����� ������ ������� � ����������� � ���������� � ��
        //���� ������ ���� ���� ������������ �����...
        if(!boolsConjunction(channelPassedQualityCheck))
        {
            defectiveCount++;

            cout << "   " << "Deviations: " << "\n";
            for (size_t i = 0; i < channels.size(); i++)
            {
                if(!channelPassedQualityCheck[i])
                {
                    cout << "    |channel " << channels[i].chanNum << ": ";
                    cout << "normal deviation: " << channels[i].normalDeviation << " ";
                    cout << "actual deviation: " << abs(measuredValues[i] - channels[i].nominalValue) << " ";
                    cout << endl;
                }
            }
        }

        cout << endl;
    }

    //������ �������� ��������
    cout << "Defective: " << defectiveCount << " | ";
    cout << (static_cast<double>(defectiveCount) / measureCount) * 100 << "%\n";


    return 0;
}
