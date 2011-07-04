#include "FreeCamera.h"
#include <math.h>

FreeCamera::FreeCamera(){
        position = D3DXVECTOR3(5.0f, 5.0f, 5.0f);
        lookAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        whereUp = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
        sensetivity = 200;
        u = 0.75;
        v = 0.5;
        speed = 0.5f;
        distance = 5.0f;
}

FreeCamera::~FreeCamera(){}


void FreeCamera::rotateCamera(float mx, float my, DWORD timeElapsed){
        u += mx / sensetivity;
        v += my / sensetivity;
        if( u >= 2 * D3DX_PI )
                u = 0.005f;
        if( u <= 0 )
                u = 2 * (D3DX_PI - 0.01f);
        if( v >= D3DX_PI/2 )
                v = D3DX_PI/2;
        if( v <= (-D3DX_PI/2) )
                v = -D3DX_PI/2;
        update();
}

D3DMATRIX FreeCamera::getViewMatrix(){
        D3DXMatrixLookAtLH(&matView, &position, &lookAt, &whereUp);
	return matView;
}

void FreeCamera::update(){
   position = D3DXVECTOR3( (lookAt.x + cos(u) * cos(v)) * distance,
                           (lookAt.y + sin(u) * cos(v)) * distance,
                           (lookAt.z + sin(v)) * distance);
}

void FreeCamera::reset(){
    position = D3DXVECTOR3(5.0f, 5.0f, 5.0f);
    lookAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    whereUp = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
    u = 0.75;
    v = 0.5;
    distance = 5;
}

D3DXVECTOR3 FreeCamera::getPosition(){
	return position;
}

void FreeCamera::changeDistance(float deltadist){

    distance += deltadist;
    update();
}
