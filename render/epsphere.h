#ifndef EPSPHERE_H
#define EPSPHERE_H
#include "iobject.h"

class EpSphere : public IObject
{
protected:

    float radius;

public:

    int getType();
    EpSphere(LPDIRECT3DDEVICE9 device);
    D3DXMATRIX getWorld();
    D3DXMATRIX getSecWorld(int sec);

    D3DXVECTOR3 getSize();
    void setSize(D3DXVECTOR3);
};

#endif // EPSPHERE_H
