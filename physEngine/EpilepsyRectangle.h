#ifndef EPILEPSYRECTANGLE_H
#define EPILEPSYRECTANGLE_H
#include "EpilepsyObject.h"

class EpilepsyRectangle : public EpilepsyObject
{
private:
        D3DXVECTOR3 vecLocalPoints[8], vecWorldPoints[8], vecOldWorldPoints[8];
        D3DXMATRIX matInvInertiaTensor;
        D3DXPLANE planes[6], oldPlanes[6];
        int Indexes[18], momentoCount, count;
public:
        EpQuadrate(){}
        EpQuadrate(D3DXVECTOR3 vecPosition, D3DXVECTOR3 vecSize,
                   D3DXVECTOR3 vecDirection, float Coefficient,
                   float Time, float Dentisty, float MU)
        {
                count=0;
                this->vecPosition=vecPosition;
                this->MU=MU;
                D3DXVECTOR3 vecHalf=vecSize*0.5f;
                sphereRadius=D3DXVec3Length(&vecHalf);
                vecLocalPoints[0]=D3DXVECTOR3(-vecHalf.x,-vecHalf.y,-vecHalf.z);
                vecLocalPoints[1]=D3DXVECTOR3(-vecHalf.x,vecHalf.y,-vecHalf.z);
                vecLocalPoints[2]=D3DXVECTOR3(vecHalf.x,-vecHalf.y,-vecHalf.z);
                vecLocalPoints[3]=D3DXVECTOR3(vecHalf.x,vecHalf.y,-vecHalf.z);
                vecLocalPoints[4]=D3DXVECTOR3(-vecHalf.x,-vecHalf.y,vecHalf.z);
                vecLocalPoints[5]=D3DXVECTOR3(-vecHalf.x,vecHalf.y,vecHalf.z);
                vecLocalPoints[6]=D3DXVECTOR3(vecHalf.x,-vecHalf.y,vecHalf.z);
                vecLocalPoints[7]=D3DXVECTOR3(vecHalf.x,vecHalf.y,vecHalf.z);
                Indexes[0]=0;
                Indexes[1]=1;
                Indexes[2]=2;
                Indexes[3]=0;
                Indexes[4]=2;
                Indexes[5]=4;
                Indexes[6]=2;
                Indexes[7]=3;
                Indexes[8]=6;
                Indexes[9]=1;
                Indexes[10]=5;
                Indexes[11]=3;
                Indexes[12]=0;
                Indexes[13]=4;
                Indexes[14]=1;
                Indexes[15]=4;
                Indexes[16]=6;
                Indexes[17]=5;
                Mass=vecSize.x*vecSize.y*vecSize.z*Dentisty;
                this->vecDirection=vecDirection;
                vecOldDirection=vecDirection;
                D3DXMatrixIdentity(&matInvInertiaTensor);
                float xs=vecSize.x*vecSize.x,ys=vecSize.y*vecSize.y,zs=vecSize.z*vecSize.z;
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
                setWorldPoints();
                setPlanes();
                angularImpulseCount=linearImpulseCount=numberForParent=numberOfJoint=jointCount=0;
                moment=new momentoQueue(vecPosition,quatOrientation);
        }
        bool getType(){return 1;}
        void setWorldPoints()
        {
                D3DXMATRIX matOrientation;
                D3DXVECTOR4 m;
                D3DXMatrixRotationQuaternion(&matOrientation,&quatOrientation);
                for(DWORD i=0;i<8;i++){
                        vecWorldPoints[i];
                        D3DXVec3Transform(&m,&vecLocalPoints[i],&matOrientation);
                        vecWorldPoints[i].x=m.x;
                        vecWorldPoints[i].y=m.y;
                        vecWorldPoints[i].z=m.z;
                        vecWorldPoints[i]+=vecPosition;
                }
                if(isParent){
                        for(int i=0;i<numberOfJoint;i++){
                            D3DXVec3Transform(&m,&vecLocalPointParentOffset[i],&matOrientation);
                            vecWorldPointParentOffset[i].x=m.x;
                            vecWorldPointParentOffset[i].y=m.y;
                            vecWorldPointParentOffset[i].z=m.z;
                            vecWorldPointParentOffset[i]+=vecPosition;
                        }
                }
                if(isJoint){
                        D3DXVec3Transform(&m,&vecLocalPointJointOffset,&matOrientation);
                        vecWorldPointJointOffset.x=m.x;
                        vecWorldPointJointOffset.y=m.y;
                        vecWorldPointJointOffset.z=m.z;
                        vecWorldPointJointOffset+=vecPosition;
                }
        }
        void setPlanes()
        {
                D3DXVECTOR3 v,v1,v2;
                for(int i=0;i<6;i++){
                        v1=vecWorldPoints[Indexes[i*3+1]]-vecWorldPoints[Indexes[i*3]];
                        v2=vecWorldPoints[Indexes[i*3+2]]-vecWorldPoints[Indexes[i*3]];
                        D3DXVec3Cross(&v,&v1,&v2);
                        D3DXVec3Normalize(&v,&v);
                        planes[i]=D3DXPLANE(v.x,v.y,v.z,D3DXVec3Dot(&v,&vecWorldPoints[Indexes[i*3]]));
                }
        }
        void setNumberOfJoint(int numberOfJoint)
        {
                this->numberOfJoint=numberOfJoint;
                Joint=new EpObject*[numberOfJoint];
                vecLocalPointParentOffset=new D3DXVECTOR3[numberOfJoint];
                vecWorldPointParentOffset=new D3DXVECTOR3[numberOfJoint];
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
        D3DXVECTOR3 getVecParentOffset(int numberOfJoint)
        {
                return vecWorldPointParentOffset[numberOfJoint];
        }
        D3DXPLANE getPlanes(int i){return planes[i];}
        D3DXVECTOR3 getOldDirection(){return vecOldDirection;}
        D3DXVECTOR3 getDirection(){return vecDirection;}
        D3DXVECTOR3 getPosition(){return vecPosition;}
        D3DXVECTOR3 getAngularVelocity(){return vecAngularVelocity;}
        D3DXVECTOR3 getAngularMomentum(){return vecAngularMomentum;}
        float getMass(){return Mass;}
        float getSphereRadius(){return sphereRadius;}
        float getCoefficient(){return Coefficient;}
        D3DXVECTOR3 getWorldPoints(int numberOfPoint){return vecWorldPoints[numberOfPoint];}
        void setTime(float Time){this->Time=Time;}
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
                D3DXVECTOR3 vecCross=crossProduct(vecPtoP,vecAngularImpulse);
                vecAngularMomentum+=vecCross;
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
        void ProcessCollisionWithSphere(EpObject *Sphere)
        {
                Sphere->ProcessCollisionWithQuadrate(this);
        }
        void ProcessCollisionWithPlane(D3DXPLANE Plane,float PLANECOEFFICIENT)
        {
                D3DXVECTOR3 vecPtoPLeft(0.f,0.f,0.f),vecPtoPRight(0.f,0.f,0.f),vecTemp,vecPlane,vecResultImpulse(0.f,0.f,0.f),vecPtoP[8],vecResultPtoP=D3DXVECTOR3(0.f,0.f,0.f),vecLeftImpulse,vecRightImpulse(0.f,0.f,0.f);
                int vecI=0,maxDistNumber=0,leftImpulseCount=0,rightImpulseCount=0;
                float maxDist=1.f;
                vecPlane=D3DXVECTOR3(Plane.a,Plane.b,Plane.c);
                for(int i=0;i<8;i++){
                        float dist=D3DXVec3Dot(&vecPlane,&vecWorldPoints[i])-Plane.d;
                        if(dist<=0){
                                vecPtoP[vecI]=vecWorldPoints[i]-vecPosition;
                                addAngularImpulse(computeImpulse(vecPtoP[vecI],vecPlane,PLANECOEFFICIENT),vecPtoP[vecI]);
                                vecI++;
                                if(maxDist>=dist){
                                        maxDist=dist;
                                        maxDistNumber=i;
                                }
                        }
                }
                if(!vecI){
                        return;
                }
                for(int i=0;i<vecI;i++){
                        vecResultPtoP+=vecPtoP[i];
                }
                //vecResultImpulse/=(float)vecI;
                vecResultPtoP/=(float)vecI;
                vecResultImpulse=computeImpulse(vecResultPtoP,vecPlane,PLANECOEFFICIENT);
                addLinearImpulse(vecResultImpulse,vecResultPtoP,1);

                if(maxDist<=0.f){
                        vecPosition-=(D3DXVec3Dot(&vecPlane,&vecWorldPoints[maxDistNumber])-Plane.d)*vecPlane;
                    setWorldPoints();
                    setPlanes();
                }
        }
        void ProcessCollisionWithQuadrate(EpObject *Quadrate)
        {
                float lengthOfCenters=D3DXVec3Length(&(vecPosition-Quadrate->getPosition()));
                if(lengthOfCenters>sphereRadius+Quadrate->getSphereRadius()){
                        return;
                }
                D3DXPLANE anPlane[6];
                D3DXVECTOR3 vecPtoP1[8],vecPtoP2[8],vecPlane[6],vecResultImpulse1,vecResultImpulse2,vecResultPtoP1,vecResultPtoP2,vecOldDirectionNormal;
                vecResultImpulse1=vecResultImpulse2=vecResultPtoP1=vecResultPtoP2=D3DXVECTOR3(0.f,0.f,0.f);
                int vecI=0,numberOfPlane,maxDistNumber,indexOfPointPos[8];
                D3DXVec3Normalize(&vecOldDirectionNormal,&vecOldDirection);
                for(int i=0;i<6;i++){
                        anPlane[i]=Quadrate->getPlanes(i);
                        vecPlane[i]=D3DXVECTOR3(anPlane[i].a,anPlane[i].b,anPlane[i].c);
                }
                float maxDist=1.f,minDist=-100.f,minLocalDist,distForPoint[8][6];
                for(int i=0,collisionCount;i<8;i++){
                        collisionCount=0;
                        minLocalDist=-100.f;
                        for(int j=0;j<6;j++){
                            float dist=D3DXVec3Dot(&vecPlane[j],&vecWorldPoints[i])-anPlane[j].d;
                                if(dist<=0.1){
                                        collisionCount++;
                                        distForPoint[vecI][j]=dist;
                                }
                                if(dist>0.1){
                                        break;
                                }
                        }
                        if(collisionCount>=6){
                                vecPtoP1[vecI]=vecWorldPoints[i]-vecPosition;
                                vecPtoP2[vecI]=vecWorldPoints[i]-Quadrate->getPosition();
                                indexOfPointPos[vecI++]=i;
                        }
                }
//---------------------------------------------------------------------------
                if(!vecI){
                    D3DXVECTOR3 vecWorldPoint[8],vecCollisionNormal,vecOldPlane[6];
                    for(int i=0;i<8;i++){
                            vecWorldPoint[i]=Quadrate->getWorldPoints(i);
                    }
                        float ang=-1.f;
                        D3DXPLANE anOldPlane[6];
                    for(int i=0;i<6;i++){
                            anPlane[i]=planes[i];
                                anOldPlane[i]=oldPlanes[i];
                            vecPlane[i]=D3DXVECTOR3(anPlane[i].a,anPlane[i].b,anPlane[i].c);
                                vecOldPlane[i]=D3DXVECTOR3(anOldPlane[i].a,anOldPlane[i].b,anOldPlane[i].c);
                    }
                    for(int i=0,collisionCount;i<8;i++){
                            collisionCount=0;
                            minLocalDist=-100.f;
                            for(int j=0;j<6;j++){
                                float dist=D3DXVec3Dot(&vecPlane[j],&vecWorldPoint[i])-anPlane[j].d;
                                    if(dist<0){
                                            collisionCount++;
                                            distForPoint[vecI][j]=dist;
                                    }
                                    if(dist>=0){
                                            break;
                                    }
                            }
                            if(collisionCount>=6){
                                    vecPtoP1[vecI]=vecWorldPoint[i]-vecPosition;
                                    vecPtoP2[vecI]=vecWorldPoint[i]-Quadrate->getPosition();
                                    indexOfPointPos[vecI++]=i;
                            }
                    }
                        if(!vecI){
                                return;
                        }
                        maxDist=100.f;
                        for(int i=0;i<vecI;i++){
                                vecResultPtoP1+=vecPtoP1[i];
                                vecResultPtoP2+=vecPtoP2[i];
                                float dist=D3DXVec3Length(&(vecPosition-vecWorldPoint[indexOfPointPos[i]]));
                                if(maxDist>=dist){
                                        maxDist=dist;
                                        maxDistNumber=indexOfPointPos[i];
                                }
                        }
                        vecResultPtoP1/=(float)vecI;
                        vecResultPtoP2/=(float)vecI;
                        maxDist=-100.0f;
                        numberOfPlane=-1;
                        for(int j=0;j<6;j++){
                                float dist=D3DXVec3Dot(&vecOldPlane[j],&vecWorldPoint[maxDistNumber])-anOldPlane[j].d;
                                if(dist>=0){
                                        numberOfPlane=j;
                                }
                        }
                        if(numberOfPlane==-1)
                        for(int i=0;i<6;i++){
                                float dist=D3DXVec3Dot(&vecPlane[i],&vecWorldPoint[maxDistNumber])-anPlane[i].d;
                                if((dist>=maxDist)){//&&(D3DXVec3Dot(&vecPlane[i],&vecOldDirection)/(D3DXVec3Length(&vecPlane[i])*D3DXVec3Length(&vecOldDirection))>0.1)){
                                        maxDist=dist;
                                        numberOfPlane=i;
                                }
                        }
                        D3DXVECTOR3 vecTemp=(D3DXVec3Dot(&vecPlane[numberOfPlane],&vecWorldPoint[maxDistNumber])-anPlane[numberOfPlane].d)*vecPlane[numberOfPlane];
                        vecPosition+=vecTemp;
                        vecResultPtoP1-=vecTemp;
                        vecResultImpulse1=computeImpulse(vecResultPtoP1,vecResultPtoP2,-vecPlane[numberOfPlane],Quadrate);
                        vecResultImpulse2=Quadrate->computeImpulse(vecResultPtoP2,vecResultPtoP1,vecPlane[numberOfPlane],this);
                        addLinearImpulse(vecResultImpulse1,vecResultPtoP1,1);
                        Quadrate->addLinearImpulse(vecResultImpulse2,vecResultPtoP2,1);
                        addAngularImpulse(vecResultImpulse1,vecResultPtoP1);
                        Quadrate->addAngularImpulse(vecResultImpulse2,vecResultPtoP2);
                    setWorldPoints();
                    setPlanes();
                        return;
                }
//----------------------------------------------------------------------------------------------
                minDist=100.f;
                for(int i=0;i<vecI;i++){
                        float dist=D3DXVec3Length(&(vecOldWorldPoints[indexOfPointPos[i]]-Quadrate->getPosition()));
                        if(minDist>dist){
                                minDist=dist;
                                maxDistNumber=indexOfPointPos[i];
                        }
                }
                minDist=-100.f;
                for(int i=0;i<6;i++){
                        float dist=D3DXVec3Dot(&vecOldWorldPoints[maxDistNumber],&vecPlane[i])-anPlane[i].d;
                        if(minDist<=dist){
                                minDist=dist;
                                numberOfPlane=i;
                        }
                }
                maxDist=1.f;
                for(int i=0;i<vecI;i++){
                        float dist=D3DXVec3Dot(&vecWorldPoints[indexOfPointPos[i]],&vecPlane[numberOfPlane])-anPlane[numberOfPlane].d;
                        if(maxDist>=dist){
                                maxDist=dist;
                                maxDistNumber=indexOfPointPos[i];
                        }
                }
                for(int i=0;i<vecI;i++){
        //		addAngularImpulse(computeImpulse(vecPtoP1[i],vecPtoP2[i],vecPlane[numberOfPlane],Quadrate),vecPtoP1[i]);
        //		Quadrate->addAngularImpulse(Quadrate->computeImpulse(vecPtoP2[i],vecPtoP1[i],vecPlane[numberOfPlane],this),vecPtoP2[i]);
                        vecResultPtoP1+=vecPtoP1[i];
                        vecResultPtoP2+=vecPtoP2[i];
                }
                if(vecI){
                        vecResultPtoP1/=(float)vecI;
                        vecResultPtoP2/=(float)vecI;
                        vecResultImpulse1=computeImpulse(vecResultPtoP1,vecResultPtoP2,vecPlane[numberOfPlane],Quadrate);
                        vecResultImpulse2=Quadrate->computeImpulse(vecResultPtoP2,vecResultPtoP1,-vecPlane[numberOfPlane],this);
                }
                addLinearImpulse(vecResultImpulse1,vecResultPtoP1,1);
                Quadrate->addLinearImpulse(vecResultImpulse2,vecResultPtoP2,1);
                addAngularImpulse(vecResultImpulse1,vecResultPtoP1);
                Quadrate->addAngularImpulse(vecResultImpulse2,vecResultPtoP2);
                if((maxDist<=0.f)&&(vecI)){
                        vecPosition-=(D3DXVec3Dot(&vecPlane[numberOfPlane],&vecWorldPoints[maxDistNumber])-anPlane[numberOfPlane].d)*vecPlane[numberOfPlane];
                    setWorldPoints();
                    setPlanes();
                }
        }
        void moveObject()
        {
                //if(angularImpulseCount)vecAngularMomentum/=angularImpulseCount;
                if(linearImpulseCount){
                        vecDirection+=vecLinearImpulse/(float)linearImpulseCount;
                        vecLinearImpulse=D3DXVECTOR3(0.f,0.f,0.f);
                }
                angularImpulseCount=linearImpulseCount=0;
                vecOldDirection=vecDirection;
                for(int i=0;i<8;i++){
                        vecOldWorldPoints[i]=vecWorldPoints[i];
                }
                for(int i=0;i<6;i++){
                        oldPlanes[i]=planes[i];
                }
                vecPosition+=vecDirection*Time;
                if(isJoint){
                        setWorldPoints();
                        addForceToPoint((Parent->getVecParentOffset(numberForParent)-vecWorldPointJointOffset)*coeffOfSpring,vecWorldPointJointOffset-vecPosition);
                        vecPosition-=vecWorldPointJointOffset-Parent->getVecParentOffset(numberForParent);
                }
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
                setWorldPoints();
                setPlanes();
        }
};

#endif // EPILEPSYRECTANGLE_H
