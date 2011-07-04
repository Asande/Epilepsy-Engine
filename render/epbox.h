#ifndef EPBOX_H
#define EPBOX_H
#include "iobject.h"

class EpBox : public IObject
{
protected:

    float width, height, depth;

public:
    int getType();
    EpBox(LPDIRECT3DDEVICE9 device);
    D3DXMATRIX getWorld();
    D3DXMATRIX getSecWorld(int sec);
    D3DXVECTOR3 getSize();
    void setSize(D3DXVECTOR3);
};

#endif // EPBOX_H
