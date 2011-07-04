#include "iobject.h"

IObject::IObject()
{
    mesh=0;
    position = D3DXVECTOR3(0.0f,0.0f,0.5f);
    rotation = D3DXVECTOR3(0.0f,0.0f,0.0f);
    dentisty = 1.0f;
    returation = 0.5f;
}

IObject::~IObject()
{
    if(mesh)mesh->Release();
}

void IObject::draw(){
    mesh->DrawSubset(0);
}

D3DXVECTOR3 IObject::getPosition(){
    return position;
}

D3DXVECTOR3 IObject::getRotation(){
    return rotation;
}

void IObject::setPosition(D3DXVECTOR3 value){
    position = value;
}

void IObject::setRotation(D3DXVECTOR3 value){
    rotation = value;
}

float IObject::getDentisty(){
    return dentisty;
}

void IObject::setDentisty(float dentisty){
   this->dentisty = dentisty;
}

float IObject::getReturation(){
    return returation;
}

void IObject::setReturation(float returation){
    this->returation = returation;
}
