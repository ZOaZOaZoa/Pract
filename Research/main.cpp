#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include "../Plant/plant.h"
#include "../Classes/channel.cpp"
#include "../Classes/table.cpp"

using namespace std;

void getInitialData(struct Channel& channel, size_t& count, double& lowestNom, double& highestNom, double& lowestDeviation, double& highestDeviation);
void fillWithValuesInRange(std::vector<double>& arr, double start, double end);
double computeDeffectivePercent(const std::vector<double>& values, double nominalValue, double nominalDeviation);
std::string convertToString(double num);
bool channelMeasureTest(double value, double nominalValue, double nominalDeviation);

int main()
{
    bool repeat;
    do
    {
        Plant plant;
        plant_init(plant);

        struct Channel channel;
        size_t measuresCount;
        double lowestNominalValue;
        double highestNominalValue;
        double lowestNominalDevation;
        double highestNominalDeviation;

        getInitialData(channel, measuresCount, lowestNominalValue, highestNominalValue, lowestNominalDevation, highestNominalDeviation);

        std::vector<double> measureResults(measuresCount);

        for(size_t i = 0; i < measureResults.size(); i++)
        {
            double result = plant_measure(channel.chanNum, plant);
            measureResults[i] = result;
        }

        const size_t COUNT_OF_NUMS_IN_RANGE = 12;

        std::vector<double> nominalValues(COUNT_OF_NUMS_IN_RANGE);
        std::vector<double> nominalDeviations(COUNT_OF_NUMS_IN_RANGE);

        fillWithValuesInRange(nominalValues, lowestNominalValue, highestNominalValue);
        fillWithValuesInRange(nominalDeviations, lowestNominalDevation, highestNominalDeviation);

        std::vector<std::string> headers = { "  N.", "Nom.Val.\\Nom.Dev" };
        for(double nomDev : nominalDeviations)
        {
            std::string deviationHeader = " ";
            deviationHeader += convertToString(nomDev);
            deviationHeader += " ";
            headers.push_back(deviationHeader);
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

        for(size_t i = 0; i < nominalValues.size(); i++)
        {
            std::vector<std::string> rowValues = { std::to_string(i + 1), convertToString(nominalValues[i]) };
            for(double nomDev : nominalDeviations)
            {
                //rowValues.push_back( (channelMeasureTest(measuredValues[i], nomVal, nominalDeviation)) ? "+" : "-" );
                double deffectivePercent = computeDeffectivePercent(measuredValues, nominalValues[i], nomDev);
                rowValues.push_back( convertToString(deffectivePercent) );
            }

            table.showRow(rowValues);
            table.showDelimiter();

        }

        repeat = false;
        bool correctInput = false;
        while(!correctInput)
        {
            std::cerr << "Repeat program? y/n: ";
            std::string answer;
            std::cin >> answer;
            if(answer == "y")
            {
                correctInput = true;
                repeat = true;
            }
            else if(answer == "n")
            {
                correctInput = true;
            }
            else
            {
                std::cerr << "Incorrect input\n";
                correctInput = false;
            }
        }

    } while(repeat);


    return 0;
}

double computeDeffectivePercent(const std::vector<double>& values, double nominalValue, double nominalDeviation)
{

    double percent = 0;
    for(double value : values)
    {
        if(!channelMeasureTest(value, nominalValue, nominalDeviation))
        {
            percent++;
        }
    }

    percent /= values.size();
    percent *= 100;

    return percent;
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

void getInitialData(struct Channel& channel, size_t& count, double& lowestNom, double& highestNom, double& lowestDeviation, double& highestDeviation)
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
    std::cerr << "Enter nominal deviation range for research <lowest value> <highest value>: ";
    while(!correctInput && counter < 100)
    {
        std::cin >> lowestDeviation;
        std::cin >> highestDeviation;
        if(lowestDeviation < highestDeviation)
        {
            correctInput = true;
        }
        else
        {
            std::cerr << "Lowest value should be lesser then highest\n";
            counter++;
        }
    }
}
