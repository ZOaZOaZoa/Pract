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
