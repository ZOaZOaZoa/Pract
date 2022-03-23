#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include "../Plant/plant.h"
#include "../Classes/channel.cpp"
#include "../Classes/table.cpp"

using namespace std;

void getInitialData(struct Channel& channel, size_t& count, double& lowestNom, double& highestNom, double& nominalDeviation);
void fillWithValuesInRange(std::vector<double>& arr, double start, double end);
std::string convertToString(double num);
bool channelMeasureTest(double value, double nominalValue, double nominalDeviation);

int main()
{
    Plant plant;
    plant_init(plant);

    struct Channel channel;
    size_t measuresCount;
    double lowestNominalValue;
    double highestNominalValue;
    double nominalDeviation;

    getInitialData(channel, measuresCount, lowestNominalValue, highestNominalValue, nominalDeviation);

    std::vector<double> measureResults(measuresCount);

    for(size_t i = 0; i < measureResults.size(); i++)
    {
        double result = plant_measure(channel.chanNum, plant);
        measureResults[i] = result;
    }

    const size_t COUNT_OF_NUMS_IN_RANGE = 15;

    std::vector<double> nominalValues(COUNT_OF_NUMS_IN_RANGE);

    fillWithValuesInRange(nominalValues, lowestNominalValue, highestNominalValue);

    std::vector<std::string> headers = { "N\\Nom.Val.", "  Value  " };
    for(double nomVal : nominalValues)
    {
        headers.push_back(convertToString(nomVal));
    }

    Table table = Table(headers);
    table.generateDelimiter();
    table.showDelimiter();
    table.showHeaders();
    table.showDelimiter();

    std::vector<double> measuredValues(measuresCount);
    for(size_t i = 0; i < measuredValues.size(); i++)
    {
        channel.measure(plant);
        measuredValues[i] = channel.lastMeasuredValue;
    }

    for(size_t i = 0; i < measuredValues.size(); i++)
    {
        std::vector<std::string> rowValues = { std::to_string(i + 1), convertToString(measuredValues[i]) };
        for(double nomVal : nominalValues)
        {
            rowValues.push_back( (channelMeasureTest(measuredValues[i], nomVal, nominalDeviation)) ? "+" : "-" );
        }

        table.showRow(rowValues);
        table.showDelimiter();

    }

    return 0;
}

bool channelMeasureTest(double value, double nominalValue, double nominalDeviation)
{
    return abs(value - nominalValue) < nominalDeviation;
}

void fillWithValuesInRange(std::vector<double>& arr, double start, double end)
{
    double step = (end - start) / (arr.size() - 1);
    double currentValue = start;
    for(size_t i = 0; i < arr.size(); i++)
    {
        arr[i] = currentValue;
        currentValue += step;
    }
}

std::string convertToString(double num)
{
    num = round(num * 100) / 100;
    std::string str = std::to_string(num);
    str.erase( str.find_last_not_of('0') + 1, std::string::npos );
    if(str.back() == '.')
    {
        str += "0";
    }

    return str;
}

void getInitialData(struct Channel& channel, size_t& count, double& lowestNom, double& highestNom, double& nominalDeviation)
{
    std::cerr << "Enter channel to research: ";
    bool correctInput = false;
    size_t counter = 0;
    while(!correctInput && counter < 100)
    {
        std::cin >> channel.chanNum;
        if(1 <= channel.chanNum && channel.chanNum <= 6 || 11 <= channel.chanNum && channel.chanNum <= 90)
        {
            correctInput = true;
        }
        else
        {
            std::cerr << "Only 1-6, 11-90 channels can be measured\n";
            counter++;
        }
    }

    std::cerr << "Enter measure count for research: ";
    correctInput = false;
    counter = 0;
    while(!correctInput && counter < 100)
    {
        std::cin >> count;
        if(count >= 0)
        {
            correctInput = true;
        }
        else
        {
            std::cerr << "Enter positive number\n";
            counter++;
        }
    }

    correctInput = false;
    counter = 0;
    std::cerr << "Enter nominal value range for research <lowest value> <highest value>: ";
    while(!correctInput && counter < 100)
    {
        std::cin >> lowestNom;
        std::cin >> highestNom;
        if(lowestNom < highestNom)
        {
            correctInput = true;
        }
        else
        {
            std::cerr << "Lowest value should be lesser then highest\n";
            counter++;
        }
    }

    correctInput = false;
    counter = 0;
    std::cerr << "Enter nominal deviation: ";
    while(!correctInput && counter < 100)
    {
        std::cin >> nominalDeviation;
        if(nominalDeviation >= 0)
        {
            correctInput = true;
        }
        else
        {
            std::cerr << "Enter positive value\n";
            counter++;
        }
    }
}
