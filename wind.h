#ifndef WIND_H
#define WIND_H

#include "direct.h"

class Wind
{
public:
    static const int MAX_POWER =15;
    Wind(const Direct &direct, int power, double sediment);

    Wind &setPower(int power);
    Wind &setDirect(const Direct &direct);
    Wind &setSediment(double sediment);

    int getPower(){return mpower;}
    double getPowerwithHeight(int height){return withheight[height];}
    const Direct &getDirect(){return mdirect;}
    double getSediment(){return msediment;}

    double getErosionRate(){return (double)mpower / MAX_POWER;}
    double getDepositRate(){return getErosionRate() * msediment;}
private:
    void calcwindwithheight();
    int mpower;
    Direct mdirect;
    double msediment;
    double withheight[256];
};

#endif // WIND_H
