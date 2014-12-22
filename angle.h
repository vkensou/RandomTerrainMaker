#ifndef ANGLE_H
#define ANGLE_H

class Radian;

class Degree
{
public:
    Degree();
    Degree(double d);
    Degree(const Radian &r);

    operator double() const
    {
        return degree;
    }

    double get() const{return degree;}
private:
    double degree;
};

class Radian
{
public:
    Radian();
    Radian(double r);
    Radian(const Degree &d);

    operator double() const
    {
        return radian;
    }

    double get() const{return radian;}
private:
    double radian;
};

#endif // ANGLE_H
