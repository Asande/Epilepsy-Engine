#ifndef EPILEPSYSPHERE_H
#define EPILEPSYSPHERE_H

#include "EpilepsyObject.h"

class EpilepsySphere : public EpilepsyObject
{
private:
        D3DXMATRIX matInvInertiaTensor;
        int momentoCount;
        bool isPlaneContact,isNewPlaneContact;
public:
        EpBall(){}
        EpBall(D3DXVECTOR3 vecPosition,float sphereRadius,D3DXVECTOR3 vecDirection,float Coefficient,float Time,float Dentisty,float MU)
        {
                isPlaneContact = isNewPlaneContact = 0;
                this->vecPosition = vecPosition;
                this->sphereRadius = sphereRadius;
                this->MU = MU;
                //this->Mass=Mass;
                Mass=8*sphereRadius*sphereRadius*sphereRadius*Dentisty;
                this->vecDirection=vecDirection;
                vecOldDirection=vecDirection;
                D3DXMatrixIdentity(&matInvInertiaTensor);
                float xs=4*sphereRadius*sphereRadius,ys=4*sphereRadius*sphereRadius,zs=4*sphereRadius*sphereRadius;
                float Ixx=Mass*(ys+zs),Iyy=Mass*(xs+zs),Izz=Mass*(xs+ys);
                matInvInertiaTensor._11=1.f/Ixx;
                matInvInertiaTensor._22=1.f/Iyy;
                matInvInertiaTensor._33=1.f/Izz;
                vecLinearImpulse=vecVelocity=vecAngularMomentum=vecAngularVelocity=vecMomentum=D3DXVECTOR3(0.f,0.f,0.f);
                vecVelocity=D3DXVECTOR3(0.f,(float)M_PI/3.f,0.f);
                D3DXQuaternionIdentity(&quatOrientation);
                //float lengthOfVector=(float)sqrt(pow(vecVelocity.x,2)+pow(vecVelocity.y,2)+pow(vecVelocity.z,2)),sinOfAngular=(float)sin(lengthOfVector/2.f),cosOfAngular=(float)cos(lengthOfVector/2.f);
                //quatOrientation=D3DXQUATERNION(vecVelocity.x*sinOfAngular/lengthOfVector,vecVelocity.y*sinOfAngular/lengthOfVector,vecVelocity.z*sinOfAngular/lengthOfVector,cosOfAngular);
                D3DXQuaternionNormalize(&quatOrientation,&quatOrientation);
                this->Coefficient=Coefficient;
                this->Time=Time;
                isParent=isJoint=false;
                angularImpulseCount=linearImpulseCount=numberForParent=numberOfJoint=jointCount=0;
                moment=new momentoQueue(vecPosition,quatOrientation);
        }
        bool getType(){return 0;}
        void setWorldPoints(){}
        void setPlanes(){}
        D3DXPLANE getPlanes(int i){return D3DXPLANE(0.f,0.f,0.f,0.f);}
        D3DXVECTOR3 getDirection(){return vecDirection;}
        D3DXVECTOR3 getOldDirection(){return vecOldDirection;}
        D3DXVECTOR3 getPosition(){return vecPosition;}
        D3DXVECTOR3 getAngularVelocity(){return vecAngularVelocity;}
        D3DXVECTOR3 getAngularMomentum(){return vecAngularMomentum;}
        D3DXVECTOR3 getWorldPoints(int i){return vecPosition;}
        float getMass(){return Mass;}
        float getSphereRadius(){return sphereRadius;}
        float getCoefficient(){return Coefficient;}
        D3DXVECTOR3 getVecParentOffset(int numberOfJoint)
        {
                return vecWorldPointParentOffset[numberOfJoint];
        }
        void setTime(float Time){this->Time=Time;}
        void setSpring(EpObject *Parent,D3DXVECTOR3 vecParentOffset,D3DXVECTOR3 vecJointOffset,float coeffOfSpring)
        {
                isJoint=true;
                numberForParent=Parent->setVecParentOffset(vecParentOffset,this);
                this->Parent=Parent;
                vecLocalPointJointOffset=vecJointOffset;
                this->coeffOfSpring=coeffOfSpring;
                D3DXMATRIX matOrientation;
                D3DXVECTOR4 m;
                D3DXMatrixRotationQuaternion(&matOrientation,&quatOrientation);
                D3DXVec3Transform(&m,&vecLocalPointJointOffset,&matOrientation);
                vecWorldPointJointOffset.x=m.x;
                vecWorldPointJointOffset.y=m.y;
                vecWorldPointJointOffset.z=m.z;
                vecWorldPointJointOffset+=vecPosition;
        }
        int setVecParentOffset(D3DXVECTOR3 vecParentOffset,EpObject *Joint)
        {
                isParent=true;
                if(jointCount<=numberOfJoint){
                        this->Joint[jointCount]=Joint;
                }
                vecLocalPointParentOffset[jointCount++]=vecParentOffset;
                D3DXMATRIX matOrientation;
                D3DXMatrixRotationQuaternion(&matOrientation,&quatOrientation);
                D3DXVECTOR4 m;
                for(int i=0;i<numberOfJoint;i++){
                    D3DXVec3Transform(&m,&vecLocalPointParentOffset[i],&matOrientation);
                    vecWorldPointParentOffset[i].x=m.x;
                    vecWorldPointParentOffset[i].y=m.y;
                    vecWorldPointParentOffset[i].z=m.z;
                    vecWorldPointParentOffset[i]+=vecPosition;
                }
                return jointCount-1;
        }
        void setNumberOfJoint(int numberOfJoint)
        {
                this->numberOfJoint=numberOfJoint;
                Joint=new EpObject*[numberOfJoint];
                vecLocalPointParentOffset=new D3DXVECTOR3[numberOfJoint];
                vecWorldPointParentOffset=new D3DXVECTOR3[numberOfJoint];
        }
        void setMoment(){moment->setMoment(vecPosition,quatOrientation);}
        void addGravityForce(D3DXVECTOR3 vecGravity)
        {
                vecDirection+=vecGravity*Time;
        }
        void addForceToObject(D3DXVECTOR3 vecLinearForce,D3DXVECTOR3 vecAngularForce)
        {
                vecDirection+=vecLinearForce/Mass*Time;
                vecAngularMomentum+=vecAngularForce*Time;
        }
        void addForceToPoint(D3DXVECTOR3 vecForce,D3DXVECTOR3 vecPtoP)
        {
                vecDirection+=vecForce/Mass*Time;
                D3DXVECTOR3 vecCross=crossProduct(vecPtoP,vecForce*Time);
                vecAngularMomentum+=vecCross;
        }
        void addLinearImpulse(D3DXVECTOR3 vecLinearImpulse,D3DXVECTOR3 vecPtoP,bool isImpulseCount)
        {
                if(isImpulseCount)linearImpulseCount++;
                this->vecLinearImpulse+=vecLinearImpulse/Mass;
        }
        void addAngularImpulse(D3DXVECTOR3 vecAngularImpulse,D3DXVECTOR3 vecPtoP)
        {
                D3DXVECTOR3 vecCross=crossProduct(vecPtoP,vecAngularImpulse),vecMU=-vecCross*MU*1.5f;
                vecAngularMomentum+=vecCross+vecMU;
        }
        D3DXVECTOR3 computeImpulse(D3DXVECTOR3 vecPtoP1,D3DXVECTOR3 vecPtoP2,D3DXVECTOR3 vecCollisionNormal,EpObject *Quadrate)
        {
                D3DXVECTOR3 vecCross1,vecCross2,vecMU;
                D3DXVec3Cross(&vecCross1,&vecAngularVelocity,&vecPtoP1);
                D3DXVec3Cross(&vecCross2,&Quadrate->getAngularVelocity(),&vecPtoP2);
                D3DXVECTOR3 vecPointVelocity1=vecOldDirection+vecCross1,vecPointVelocity2=Quadrate->getOldDirection()+vecCross2;
                vecMU=((vecPointVelocity1-vecPointVelocity2)+vecCollisionNormal*D3DXVec3Dot(&(vecPointVelocity1-vecPointVelocity2),&(-vecCollisionNormal)))*MU*Mass;
                float ImpulseNumenator,ImpulseDenominator,V1,V2;
                V1=-D3DXVec3Dot(&vecCollisionNormal,&vecPointVelocity1);
                V2=-D3DXVec3Dot(&vecCollisionNormal,&vecPointVelocity2);
                ImpulseNumenator=V1-((Mass-Quadrate->getMass())*V1+2*V2*Quadrate->getMass())/(Mass+Quadrate->getMass())*(this->Coefficient*Quadrate->getCoefficient());
                ImpulseDenominator=(1.f/Mass)+dotProduct(crossProduct(matVec3Multiply(matInvInertiaTensor,crossProduct(vecPtoP1,vecCollisionNormal)),vecPtoP1),vecCollisionNormal);
                return vecCollisionNormal*(ImpulseNumenator/ImpulseDenominator)-vecMU;
        }
        D3DXVECTOR3 computeImpulse(D3DXVECTOR3 vecPtoP,D3DXVECTOR3 vecCollisionNormal,float Coefficient)
        {
                D3DXVECTOR3 vecCross,vecMU;
                D3DXVec3Cross(&vecCross,&vecAngularVelocity,&vecPtoP);
                D3DXVECTOR3 vecPointVelocity=vecOldDirection+vecCross;
                vecMU=(vecPointVelocity+vecCollisionNormal*D3DXVec3Dot(&vecPointVelocity,&(-vecCollisionNormal)))*MU*Mass;
                float ImpulseNumenator,ImpulseDenominator;
                ImpulseNumenator=dotProduct(vecPointVelocity,vecCollisionNormal)*(-(1+this->Coefficient*Coefficient));
                ImpulseDenominator=(1.f/Mass)+dotProduct(crossProduct(matVec3Multiply(matInvInertiaTensor,crossProduct(vecPtoP,vecCollisionNormal)),vecPtoP),vecCollisionNormal);
                return vecCollisionNormal*(ImpulseNumenator/ImpulseDenominator)-vecMU;//*this->Time;
        }
        void ProcessCollisionWithQuadrate(EpObject *Quadrate)
        {
                D3DXVECTOR3 vecQuadratePosition=Quadrate->getPosition(),vecPlane[6];
                float lengthOfCenters=D3DXVec3Length(&(vecPosition-vecQuadratePosition));
                if(lengthOfCenters>sphereRadius+Quadrate->getSphereRadius()){
                        return;
                }
                D3DXPLANE anPlane[6];
                for(int i=0;i<6;i++){
                        anPlane[i]=Quadrate->getPlanes(i);
                        vecPlane[i]=D3DXVECTOR3(anPlane[i].a,anPlane[i].b,anPlane[i].c);
                }
                D3DXVECTOR3 vecDirToPoint,vecCollisionPlane[6],vecProj(0.f,0.f,0.f),vecResultPlane(0.f,0.f,0.f);
                int numberOfPlane,vecI=0,collisionCount=0;
                float resultDist=0.f,maxDist=-1.f,minDist=-1.f;
                for(int i=0;i<6;i++){
                        float dist=D3DXVec3Dot(&vecPosition,&vecPlane[i])-anPlane[i].d;
                        if((dist>=0)&&(dist>=maxDist)){
                                maxDist=dist;
                                numberOfPlane=i;
                                vecI++;
                        }
                }
                if(!vecI){
                        return;
                }
                vecProj-=vecPlane[numberOfPlane]*sphereRadius;
                vecProj+=vecPosition;
                for(int i=0;i<vecI;i++){
                        float dist=D3DXVec3Dot(&vecProj,&vecPlane[numberOfPlane])-anPlane[numberOfPlane].d;
                        if((dist<=0)&&(dist>=minDist)){
                                minDist=dist;
                                collisionCount++;
                        }
                }
                if(!collisionCount){
                        return;
                }
                D3DXVECTOR3 vecPtoP1,vecPtoP2,vecImpulse1,vecImpulse2;
                vecPtoP1=vecProj-vecPosition;
                vecPtoP2=vecProj-vecQuadratePosition;
                vecImpulse1=computeImpulse(vecPtoP1,vecPtoP2,vecPlane[numberOfPlane],Quadrate);
                vecImpulse2=Quadrate->computeImpulse(vecPtoP2,vecPtoP1,-vecPlane[numberOfPlane],this);
                addLinearImpulse(vecImpulse1,vecPtoP1,0);
                Quadrate->addLinearImpulse(vecImpulse2,vecPtoP2,1);
                addAngularImpulse(vecImpulse1,vecPtoP1);
                Quadrate->addAngularImpulse(vecImpulse2,vecPtoP2);
                vecPosition-=vecPlane[numberOfPlane]*minDist;
        }
        void ProcessCollisionWithSphere(EpObject *Sphere)
        {
                D3DXVECTOR3 vecSpherePos=Sphere->getPosition(),vecDirToSpherePos=vecSpherePos-vecPosition;
                if(sphereRadius+Sphere->getSphereRadius()<D3DXVec3Length(&(vecDirToSpherePos))){
                        return;
                }
                D3DXVECTOR3 vecCollisionNormal,vecPtoP1,vecPtoP2,vecImpulse1,vecImpulse2;
                D3DXVec3Normalize(&vecCollisionNormal,&vecDirToSpherePos);
                vecPtoP1=vecCollisionNormal*sphereRadius;
                vecPtoP2=-vecCollisionNormal*Sphere->getSphereRadius();
                vecImpulse1=computeImpulse(vecPtoP1,vecPtoP2,vecCollisionNormal,Sphere);
                vecImpulse2=Sphere->computeImpulse(vecPtoP2,vecPtoP1,-vecCollisionNormal,this);
                addLinearImpulse(vecImpulse1,vecPtoP1,1);
                addAngularImpulse(vecImpulse1,vecPtoP1);
                Sphere->addLinearImpulse(vecImpulse2,vecPtoP2,1);
                Sphere->addAngularImpulse(vecImpulse2,vecPtoP2);
                vecPosition-=vecCollisionNormal*(Sphere->getSphereRadius()-D3DXVec3Length(&(vecSpherePos-vecPosition-vecPtoP1)));
        }
        void ProcessCollisionWithPlane(D3DXPLANE Plane,float PLANECOEFFICIENT)
        {
                D3DXVECTOR3 vecPlane(Plane.a,Plane.b,Plane.c);
                float dist=D3DXVec3Dot(&vecPosition,&vecPlane)-Plane.d-sphereRadius;
                if(dist>0){
                        return;
                }
                isNewPlaneContact=true;
                D3DXVECTOR3 vecPtoP=-vecPlane*sphereRadius,vecImpulse;
                vecImpulse=computeImpulse(vecPtoP,vecPlane,PLANECOEFFICIENT);
                addLinearImpulse(vecImpulse,vecPtoP,0);
                addAngularImpulse(vecImpulse,vecPtoP);
                vecPosition-=vecPlane*dist;
        }
        void moveObject()
        {
                if(linearImpulseCount){
                        vecDirection+=vecLinearImpulse/(float)linearImpulseCount;
                        vecLinearImpulse=D3DXVECTOR3(0.f,0.f,0.f);
                }
                else{
                        vecDirection+=vecLinearImpulse;
                        vecLinearImpulse=D3DXVECTOR3(0.f,0.f,0.f);
                }
                angularImpulseCount=linearImpulseCount=0;
                vecOldDirection=vecDirection;
                vecPosition+=vecDirection*Time;
                if(isJoint){
                        setWorldPoints();
                        addForceToPoint((Parent->getVecParentOffset(numberForParent)-vecWorldPointJointOffset)*coeffOfSpring,vecWorldPointJointOffset-vecPosition);
                        vecPosition-=vecWorldPointJointOffset-Parent->getVecParentOffset(numberForParent);
                }
                if((isPlaneContact)&&(isNewPlaneContact)){
                        vecAngularMomentum-=vecAngularMomentum*MU*Time;
                }
                isPlaneContact=isNewPlaneContact;
                isNewPlaneContact=false;
                D3DXVECTOR3 vecVelocityNormalize;
                D3DXMATRIX matOrientation,matTransposedOrientation,matConversion;
                D3DXMatrixRotationQuaternion(&matOrientation,&quatOrientation);
                matTransposedOrientation=matOrientation;
                D3DXMatrixTranspose(&matOrientation,&matOrientation);
                matConversion=matOrientation*matInvInertiaTensor*matTransposedOrientation;
                D3DXVec3TransformCoord(&vecAngularVelocity,&vecAngularMomentum,&matConversion);
                vecVelocity=vecAngularVelocity*Time;
                float lengthOfVector=(float)sqrt(vecVelocity.x*vecVelocity.x+vecVelocity.y*vecVelocity.y+vecVelocity.z*vecVelocity.z),sinOfAngular=(float)sin(lengthOfVector/2.f),cosOfAngular=(float)cos(lengthOfVector/2.f);
                D3DXVec3Normalize(&vecVelocityNormalize,&vecVelocity);
                D3DXQUATERNION quatTemp(vecVelocityNormalize.x*sinOfAngular,vecVelocityNormalize.y*sinOfAngular,vecVelocityNormalize.z*sinOfAngular,cosOfAngular);
                D3DXQuaternionNormalize(&quatTemp,&quatTemp);
                D3DXQuaternionMultiply(&quatOrientation,&quatOrientation,&quatTemp);
                D3DXQuaternionNormalize(&quatOrientation,&quatOrientation);
        }
};

#endif // EPILEPSYSPHERE_H
