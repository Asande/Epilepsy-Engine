#ifndef IOBJECT_H
#define IOBJECT_H
#include "C:\Program Files\Microsoft DirectX SDK (August 2009)\Include\d3dx9.h"
#include <QList>
#include "EpilepsiyaObject.h"

class IObject
{
protected:
    LPDIRECT3DDEVICE9 device;

    D3DXVECTOR3 position;
    D3DXVECTOR3 rotation;

    ID3DXMesh   *mesh;

    float dentisty;
    float returation;

public:

    momentoQueue *queue;

    const static int EP_BOX = 0;
    const static int EP_SPHERE = 1;

    IObject();
    ~IObject();

    virtual D3DXMATRIX getWorld()=0;
    virtual D3DXMATRIX getSecWorld(int sec)=0;
    virtual int getType()=0;

    D3DXVECTOR3 getPosition();
    void setPosition(D3DXVECTOR3);

    D3DXVECTOR3 getRotation();
    void setRotation(D3DXVECTOR3);

    virtual D3DXVECTOR3 getSize() = 0;
    virtual void setSize(D3DXVECTOR3)=0;

    float getDentisty();
    void setDentisty(float);

    float getReturation();
    void setReturation(float);

    void draw();
};

#endif // IOBJECT_H
