#include "epsphere.h"

EpSphere::EpSphere(LPDIRECT3DDEVICE9 device)
{
    radius = 0.5f;
    D3DXCreateSphere(device,1.0f,20,20,&mesh,NULL);
}

D3DXMATRIX EpSphere::getWorld(){
    D3DXMATRIX pos,rot,sc,world;
    D3DXMatrixTranslation(&pos, position.x, position.y, position.z);
    //D3DXMatrixRotationYawPitchRoll(&rot, rotation.x, rotation.y, rotation.z);
    D3DXMatrixRotationYawPitchRoll(&rot, 0, 0, 0);
    D3DXMatrixScaling(&sc, radius, radius, radius);
    world = sc * rot * pos;
    return world;
}

D3DXMATRIX EpSphere::getSecWorld(int sec){
    D3DXMATRIX pos,rot,sc,world;
    D3DXVECTOR3 position = queue->getVecMomentPos(sec);
    D3DXQUATERNION orient = queue->getVecMomentOr(sec);
    D3DXMatrixTranslation(&pos, position.x, position.y, position.z);
    D3DXMatrixRotationQuaternion(&rot, &orient);
    D3DXMatrixScaling(&sc, radius, radius, radius);
    world = sc * rot * pos;
    return world;
}

int EpSphere::getType(){
    return 1;
}

D3DXVECTOR3 EpSphere::getSize(){
    return D3DXVECTOR3(radius,radius,radius);
}

void EpSphere::setSize(D3DXVECTOR3 val){
    radius = val.x;
}
