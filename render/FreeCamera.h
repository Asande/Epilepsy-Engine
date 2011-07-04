#ifndef CAMERA_H
#define CAMERA_H

#include "C:\Program Files\Microsoft DirectX SDK (August 2009)\Include\d3dx9.h"
#include "ICamera.h"


class FreeCamera : public ICamera{
private:

public:
        FreeCamera();
        ~FreeCamera();

        void rotateCamera(float mx,float my,DWORD timeElapsed);
        void update();
        void reset();
        void changeDistance(float);

	D3DMATRIX getViewMatrix();
	D3DXVECTOR3 getPosition();
};

#endif
