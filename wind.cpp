#include "wind.h"

Wind::Wind(const Direct &direct, int power, double sediment)
    :mdirect(direct), mpower(power), msediment(sediment)
{
}

Wind &Wind::setPower(int power)
{
    if(power<0)power=0;
    if(power>MAX_POWER)power=MAX_POWER;
    mpower = power;
    return *this;
}

Wind &Wind::setDirect(const Direct &direct)
{
    mdirect = direct;
    return *this;
}

Wind &Wind::setSediment(double sediment)
{
    if(sediment<0)sediment=0;
    if(sediment>1)sediment=1;
    msediment = sediment;
    return *this;
}
