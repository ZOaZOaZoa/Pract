#include <math.h>
#include "../Plant/plant.h"

struct Channel
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

    void measure(Plant plant)
    {
        lastMeasuredValue = plant_measure(chanNum, plant);
    }

};
