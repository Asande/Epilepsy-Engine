#ifndef ICAMERA_H
#define ICAMERA_H

#include "C:\Program Files\Microsoft DirectX SDK (August 2009)\Include\d3dx9.h"

class ICamera{

public:

protected:
	D3DXMATRIX	matView;
	D3DXVECTOR3 position;
	D3DXVECTOR3 lookAt;
	D3DXVECTOR3 whereUp;

	int sensetivity;
        float distance;
        float speed;

	float u,v;
public:
        virtual void rotateCamera(float mx,float my,DWORD timeElapsed)=0;
        virtual void update()=0;
        virtual void reset()=0;


        virtual void changeDistance(float)=0;

	virtual D3DMATRIX getViewMatrix()=0;
        virtual D3DXVECTOR3 getPosition()=0;

};


#endif
