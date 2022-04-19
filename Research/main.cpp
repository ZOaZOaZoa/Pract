#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <discpp.h>
#include "../Plant/plant.h"
#include "../Classes/channel.cpp"
#include "../Classes/table.cpp"

using namespace std;

void getInitialData(struct Channel& channel, size_t& count, double& lowestNom, double& highestNom, size_t& nomSteps, double& lowestDeviation, double& highestDeviation, size_t& devSteps);
void drawGraph(const std::vector<double>& values, const std::vector<double>& xs, std::string xAxisName, double startValue, double endValue, std::string plotNameRow1, std::string plotNameRow2);
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
        size_t nominalValSteps;
        double lowestNominalDevation;
        double highestNominalDeviation;
        size_t nominalDevSteps;

        getInitialData(channel, measuresCount, lowestNominalValue, highestNominalValue, nominalValSteps, lowestNominalDevation, highestNominalDeviation, nominalDevSteps);

        std::vector<double> nominalValues(nominalValSteps);
        std::vector<double> nominalDeviations(nominalDevSteps);

        fillWithValuesInRange(nominalValues, lowestNominalValue, highestNominalValue);
        fillWithValuesInRange(nominalDeviations, lowestNominalDevation, highestNominalDeviation);

        //Формирование заголовка таблицы и горизонтального разделителя
        std::vector<std::string> headers = { "  N1", "Nom.Val.\\Nom.Dev" };
        for(double nomDev : nominalDeviations)
        {
            std::string deviationHeader = "  ";
            deviationHeader += convertToString(nomDev);
            deviationHeader += "  ";
            headers.push_back(deviationHeader);
        }

        Table table = Table(headers);
        table.generateDelimiter();
        table.showDelimiter();

        std::vector<std::string> rowValues = { " ", "N2"};
        size_t rowsToFill = table.columns() - rowValues.size();
        for(size_t i = 0; i < rowsToFill; i++)
        {
            rowValues.push_back( std::to_string(i + 1));
        }
        table.showRow(rowValues);

        table.showDelimiter();
        table.showHeaders();
        table.showDelimiter();

        //Измерение данных
        std::vector<double> measuredValues(measuresCount);
        for(size_t i = 0; i < measuredValues.size(); i++)
        {
            channel.measure(plant);
            measuredValues[i] = channel.lastMeasuredValue;
        }

        //Формирование матрицы процентов деффективности и вывод данных в таблицу
        std::vector< std::vector<double> > deffectiveResults(nominalValSteps, std::vector<double>(nominalDevSteps));
        for(size_t i = 0; i < nominalValues.size(); i++)
        {
            rowValues = vector<std::string>{ std::to_string(i + 1), convertToString(nominalValues[i]) };
            for(size_t j = 0; j < nominalDeviations.size(); j++)
            {
                double deffectivePercent = computeDeffectivePercent(measuredValues, nominalValues[i], nominalDeviations[j]);
                deffectiveResults[i][j] = deffectivePercent;
                std::string value = convertToString(deffectivePercent);
                value += '%';
                rowValues.push_back( value );
            }

            table.showRow(rowValues);
            table.showDelimiter();
        }


        repeat = false;
        bool endIteration = false;
        while(!endIteration)
        {
            std::cerr << "1 - Make graph, 2 - Enter new data, 3 - End program: ";
            std::string answer;
            std::cin >> answer;

            switch(std::atoi(answer.c_str()))
            {
            //Создание графика из полученных данных
            case 1:
                {
                    std::vector<double> yValues;
                    std::vector<double> xValues;
                    double lowestX;
                    double highestX;
                    std::string xAxisName;
                    std::string plotNameRow1;

                    bool correctAnswer = false;
                    while(!correctAnswer)
                    {
                        std::cerr << "1 - Graph nominal deviations, 2 - Graph nominal values: ";
                        std::cin >> answer;
                        switch(std::atoi(answer.c_str()))
                        {
                        //Заполнение данных для графика зависимости деффективности от номинального отклонения
                        case 1:
                            int N;
                            std::cerr << "Enter N1: ";
                            std::cin >> N;
                            N--;

                            xValues = std::vector<double>(nominalDeviations.size());
                            for(size_t i = 0; i < nominalDeviations.size(); i++)
                            {
                                xValues[i] = nominalDeviations[i];
                            }
                            lowestX = lowestNominalDevation;
                            highestX = highestNominalDeviation;
                            yValues = std::vector<double>(deffectiveResults[N].size());
                            for(size_t i = 0; i < deffectiveResults[N].size(); i++)
                            {
                                yValues[i] = deffectiveResults[N][i];
                            }
                            xAxisName = "Nominal Deviation";

                            plotNameRow1 = "Deffective percent and nominal deviations dependence.\n";

                            correctAnswer = true;
                            break;
                        //Заполнение данных для графика зависимости деффективности от номинального значения
                        case 2:
                            std::cerr << "Enter N2: ";
                            std::cin >> N;
                            N--;

                            xValues = std::vector<double>(nominalValues.size());
                            for(size_t i = 0; i < nominalValues.size(); i++)
                            {
                                xValues[i] = nominalValues[i];
                            }
                            lowestX = lowestNominalValue;
                            highestX = highestNominalValue;
                            yValues = std::vector<double>(nominalValues.size());
                            for(size_t i = 0; i < nominalValues.size(); i++)
                            {
                                yValues[i] = deffectiveResults[i][N];
                            }
                            xAxisName = "Nominal Value";

                            plotNameRow1 = "Deffective percent and nominal values dependence.";

                            correctAnswer = true;
                            break;
                        default:
                            std::cerr << "Incorrect  input. Enter number from the list";
                            correctAnswer = false;
                            break;
                        }
                    }
                    std::string plotNameRow2 = "Channel ";
                    plotNameRow2 += std::to_string(channel.chanNum);
                    plotNameRow2 += ", Measures count ";
                    plotNameRow2 += std::to_string(measuresCount);

                    drawGraph(yValues, xValues, xAxisName, lowestX, highestX, plotNameRow1, plotNameRow2);
                    break;
                }
            //Ввод новых данных для исследования
            case 2:
                endIteration = true;
                repeat = true;
                break;
            //Завершение программы
            case 3:
                endIteration = true;
                break;
            case 0:
            default:
                std::cerr << "Incorrect input. Enter number from the list\n";
                endIteration = false;
                break;
            }
        }
    } while(repeat);


    return 0;
}

void drawGraph(const std::vector<double>& values, const std::vector<double>& xs, std::string xAxisName, double startValue, double endValue, std::string plotNameRow1, std::string plotNameRow2)
{
    Dislin g;
    g.metafl("cons");
    g.scrmod("revers");
    g.disini();

    // Задает подписи осей.
    g.name(xAxisName.c_str(), "x");
    g.name("Deffective, %", "y");

    // Количество цифр после запятой по оси абсцисс: нет, только целая часть.
    g.labdig(1, "x");

    // Количество промежуточных меток (между числами) на осях.
    g.ticks((endValue - startValue) / 9, "x");
    g.ticks(10, "y");

    // Задает заголовок графика.
    g.titlin(plotNameRow1.c_str(), 1);
    g.titlin(plotNameRow2.c_str(), 2);

    const int background_color = g.intrgb(0.95, 0.95, 0.95);
    g.axsbgd(background_color);

    // Задает координатную сетку.
    g.graf(
        // Область значений по оси абсцисс (X).
        startValue, endValue,
        // Наименьшее значение на оси абсцисс и следующее за ним.
        startValue, (endValue - startValue) / 9,
        // Область значений по оси ординат (Y).
        0.0, 100.0,
        // Наименьшее значение на оси ординат и следующее за ним.
        0.0, 10.0);

    g.setrgb(0.7, 0.7, 0.7);
    g.grid(1, 1);

    g.color("fore");
    g.title();

    // Рисует данные.
    g.color("red");
    g.curve(&xs[0], &values[0], values.size());

    g.disfin();
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

//Заполнение вектора промежуточными значениями при заданных начальном и конечном значениях
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

//Преобразование double в строку в определенном формате
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

void getInitialData(struct Channel& channel, size_t& count, double& lowestNom, double& highestNom, size_t& nomSteps, double& lowestDeviation, double& highestDeviation, size_t& devSteps)
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

    std::cerr << "Enter count of nominal value steps: ";
    std::cin >> nomSteps;

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

    std::cerr << "Enter count of nominal deviation steps: ";
    std::cin >> devSteps;
}
