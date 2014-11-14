#ifndef TERRAIN_H
#define TERRAIN_H

class Terrain
{
public:
    Terrain(unsigned int width, unsigned int height);
    ~Terrain();

    unsigned int getWidth(){return mwidth;}
    unsigned int getHeight(){return mheight;}
    double * getData(){return mdata;}
    void reset();
    void reset(unsigned int width, unsigned int height);

private:
    unsigned int mwidth;
    unsigned int mheight;
    double *mdata;
};

#endif // TERRAIN_H
