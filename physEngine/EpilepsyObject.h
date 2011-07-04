#ifndef EPILEPSYOBJECT_H
#define EPILEPSYOBJECT_H

#include "C:\Program Files\Microsoft DirectX SDK (August 2009)\Include\d3dx9.h"
#include <vector>
#include "MomentoQueue.h"

#define M_PI       3.14159265358979323846

class EpilepsyObject
{
protected:

        EpObject *Parent,**Joint;

        D3DXVECTOR3 vecMomentum, vecAngularMomentum;

        D3DXVECTOR3 vecVelocity,vecAngularVelocity;
        D3DXVECTOR3 vecLinearImpulse;
        D3DXVECTOR3 vecPosition;
        D3DXVECTOR3 vecOldDirection,vecDirection;
        D3DXVECTOR3 vecLocalPointJointOffset, *vecLocalPointParentOffset,
                    vecWorldPointJointOffset, *vecWorldPointParentOffset;

        D3DXQUATERNION quatOrientation;

        bool isParent, isJoint;

        float Mass, Coefficient, Time, sphereRadius, coeffOfSpring, MU;

        int jointCount,numberOfJoint,numberForParent,angularImpulseCount,linearImpulseCount;

        float dotProduct(D3DXVECTOR3 v1,D3DXVECTOR3 v2)
        {
                return D3DXVec3Dot(&v1,&v2);
        }

        D3DXVECTOR3 crossProduct(D3DXVECTOR3 v1,D3DXVECTOR3 v2)
        {
                D3DXVECTOR3 result;
                D3DXVec3Cross(&result,&v1,&v2);
                return result;
        }

        D3DXVECTOR3 matVec3Multiply(D3DXMATRIX mat,D3DXVECTOR3 vec)
        {
                D3DXMATRIX matPtopCollision,matMat;
                D3DXMatrixIdentity(&matPtopCollision);
                D3DXVECTOR3 vecMat;
                matPtopCollision._11=vec.x;
                matPtopCollision._21=vec.y;
                matPtopCollision._31=vec.z;
                matPtopCollision._22=0.f;
                matPtopCollision._33=0.f;
                matPtopCollision._44=0.f;
                matMat=mat*matPtopCollision;
                vecMat.x=matMat._11;
                vecMat.y=matMat._21;
                vecMat.z=matMat._31;
                return vecMat;
        }

public:
        EpilepsyObject(){}
        virtual void setWorldPoints()=0;
        virtual void setPlanes()=0;
        virtual bool getType()=0;
        virtual D3DXPLANE getPlanes(int)=0;
        virtual D3DXVECTOR3 getDirection()=0;
        virtual D3DXVECTOR3 getOldDirection()=0;
        virtual D3DXVECTOR3 getPosition()=0;
        virtual D3DXVECTOR3 getAngularVelocity()=0;
        virtual D3DXVECTOR3 getAngularMomentum()=0;
        virtual D3DXVECTOR3 getWorldPoints(int)=0;
        virtual float getMass()=0;
        virtual float getSphereRadius()=0;
        virtual float getCoefficient()=0;
        virtual D3DXVECTOR3 getVecParentOffset(int)=0;
        virtual void setTime(float)=0;
        virtual void setSpring(EpObject*,D3DXVECTOR3,D3DXVECTOR3,float)=0;
        virtual int setVecParentOffset(D3DXVECTOR3,EpObject*)=0;
        virtual void setNumberOfJoint(int)=0;
        virtual void setMoment()=0;
        virtual void addGravityForce(D3DXVECTOR3)=0;
        virtual void addForceToPoint(D3DXVECTOR3,D3DXVECTOR3)=0;
        virtual void addForceToObject(D3DXVECTOR3,D3DXVECTOR3)=0;
        virtual void addLinearImpulse(D3DXVECTOR3,D3DXVECTOR3,bool)=0;
        virtual void addAngularImpulse(D3DXVECTOR3,D3DXVECTOR3)=0;
        virtual D3DXVECTOR3 computeImpulse(D3DXVECTOR3,D3DXVECTOR3,float)=0;
        virtual D3DXVECTOR3 computeImpulse(D3DXVECTOR3,D3DXVECTOR3,D3DXVECTOR3,EpObject*)=0;
        virtual void ProcessCollisionWithQuadrate(EpObject*)=0;
        virtual void ProcessCollisionWithSphere(EpObject*)=0;
        virtual void ProcessCollisionWithPlane(D3DXPLANE,float)=0;
        virtual void moveObject()=0;
        MomentoQueue *moment;
};

#endif // EPILEPSYOBJECT_H
