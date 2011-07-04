#include "epbox.h"

EpBox::EpBox(LPDIRECT3DDEVICE9 device)
{
    width = 1.0f;
    height = 1.0f;
    depth = 1.0f;
    D3DXCreateBox(device, 1.0f, 1.0f, 1.0f, &mesh, NULL);
}

D3DXMATRIX EpBox::getWorld(){
    D3DXMATRIX pos,rot,sc,world;
    D3DXMatrixTranslation(&pos, position.x, position.y, position.z);
    //D3DXMatrixRotationYawPitchRoll(&rot, rotation.x, rotation.y, rotation.z);
    D3DXMatrixRotationYawPitchRoll(&rot, 0, 0, 0);
    D3DXMatrixScaling(&sc, width, height, depth);
    world = sc * rot * pos;
    return world;
}

D3DXMATRIX EpBox::getSecWorld(int sec){
    D3DXMATRIX pos,rot,sc,world;
    D3DXVECTOR3 position = queue->getVecMomentPos(sec);
    D3DXQUATERNION orient = queue->getVecMomentOr(sec);
    D3DXMatrixTranslation(&pos, position.x, position.y, position.z);
    D3DXMatrixRotationQuaternion(&rot, &orient);
    D3DXMatrixScaling(&sc, width, height, depth);
    world = sc * rot * pos;
    return world;
}

int EpBox::getType(){
    return 0;
}

D3DXVECTOR3 EpBox::getSize(){
    return D3DXVECTOR3(width, height, depth);
}

void EpBox::setSize(D3DXVECTOR3 size){
    width = size.x;
    height = size.y;
    depth = size.z;    
}
