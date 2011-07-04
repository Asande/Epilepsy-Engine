#include "EpilepsiyaObject.h"
#ifndef __EPCONTROLLER_H__
#define __EPCONTROLLER_H__
class EpController
{
private:
	EpObject **Obj;
        int timeOfCicle,numberOfPrimitives,primitiveCount,numberOfPlanes,planeCount ,stepCount;
	D3DXPLANE *borderPlanes;
	float linearDumping,angularDumping,HZ,numberOfSeconds,PLANECOEFFICIENT,MUCOEFFICIENT;
	bool isGravity,isDumping;
	D3DXVECTOR3 vecGravity;
public:
	EpController();
	EpController(int numberOfPrimitives,float HZ,float numberOfSeconds,bool isGravity,bool isDumping,float planecoef,float mu)
	{
                primitiveCount=0; stepCount = 0;
		PLANECOEFFICIENT=planecoef;
		MUCOEFFICIENT=mu;
		Obj=new EpObject*[numberOfPrimitives];
		timeOfCicle=(int)(HZ*numberOfSeconds);
		planeCount=6;
		borderPlanes=new D3DXPLANE[6];
		borderPlanes[0]=D3DXPLANE(0.f,0.f,1.f,0.f);
		borderPlanes[1]=D3DXPLANE(1.f,0.f,0.f,-51.5f);
		borderPlanes[2]=D3DXPLANE(-1.f,0.f,0.f,-50.f);
		borderPlanes[3]=D3DXPLANE(0.f,1.f,0.f,-50.f);
		borderPlanes[4]=D3DXPLANE(0.f,-1.f,0.f,-51.5f);
		borderPlanes[5]=D3DXPLANE(0.f,0.f,-1.f,-30.f);
		this->numberOfPrimitives=numberOfPrimitives;
		this->HZ=HZ;
		this->numberOfSeconds=numberOfSeconds;
		linearDumping=-0.5f;
		angularDumping=-0.4f;
		vecGravity=D3DXVECTOR3(0.f,0.f,-98.f);
		this->isGravity=isGravity;
		this->isDumping=isDumping;
	}
	void addQuadratePrimitive(D3DXVECTOR3 vecPosition,D3DXVECTOR3 vecSize,D3DXVECTOR3 vecDirection,float Coefficient,float Dentisty)
	{
		Obj[primitiveCount++]=new EpQuadrate(vecPosition,vecSize,vecDirection,Coefficient,1.f/HZ,Dentisty,MUCOEFFICIENT);
	}
	void addSpherePrimitive(D3DXVECTOR3 vecPosition,FLOAT sphereRadius,D3DXVECTOR3 vecDirection,float Coefficient,float Dentesty)
	{
                Obj[primitiveCount++]=new EpBall(vecPosition,sphereRadius,vecDirection,Coefficient,1.f/HZ,Dentesty,MUCOEFFICIENT);
	}
	void switchGravity(){isGravity=!isGravity;}
	void switchDumping(){isDumping=!isDumping;}
	momentoQueue *getObject(int i){return Obj[i]->moment;}
	bool setNumberOfJointToPrimitive(int numberOfPrimitive,int numberOfJoint)
	{
		if(numberOfPrimitive>this->numberOfPrimitives){
			return 0;
		}
		Obj[numberOfPrimitive]->setNumberOfJoint(numberOfJoint);
	}
	void setSpring(int numberOfParent,D3DXVECTOR3 vecParentPoint,int numberOfJoint,D3DXVECTOR3 vecJointPoint,float powerOfSpring)
	{
		Obj[numberOfJoint]->setSpring(Obj[numberOfParent],vecParentPoint,vecJointPoint,powerOfSpring);
	}
	void addGravity(D3DXVECTOR3 vecGravity)
	{
		this->vecGravity=vecGravity;
		isGravity=true;
	}
	void addGravityForce(int i)
	{
		Obj[i]->addGravityForce(vecGravity);
	}
	void addDumpingForce(int i)
	{
		Obj[i]->addForceToObject(Obj[i]->getDirection()*linearDumping,Obj[i]->getAngularVelocity()*angularDumping);
	}
        int colculateStep()
        { stepCount++;
		for(int j=0;j<numberOfPrimitives;j++)
		{
			Obj[j]->moveObject();
			if(isGravity)addGravityForce(j);
			if(isDumping)addDumpingForce(j);
			for(int k=0;k<6;k++)
			{
				Obj[j]->ProcessCollisionWithPlane(borderPlanes[k],PLANECOEFFICIENT);
			}
			for(int k=0;k<numberOfPrimitives;k++)
			{
				if(k!=j){
					if(Obj[k]->getType())Obj[j]->ProcessCollisionWithQuadrate(Obj[k]);
					else Obj[j]->ProcessCollisionWithSphere(Obj[k]);
				}
			}
			Obj[j]->setMoment();
		}
                return stepCount;
	}
	void colculateScene()
	{
		for(int i=0;i<timeOfCicle;i++)
		{
			for(int j=0;j<primitiveCount;j++)
			{
				Obj[j]->moveObject();
				if(isGravity)addGravityForce(j);
				if(isDumping)addDumpingForce(j);
				for(int k=0;k<6;k++)
				{
					Obj[j]->ProcessCollisionWithPlane(borderPlanes[k],PLANECOEFFICIENT);
				}
				for(int k=0;k<numberOfPrimitives;k++)
				{
					if(k!=j){
						if(Obj[k]->getType())Obj[j]->ProcessCollisionWithQuadrate(Obj[k]);
						else Obj[j]->ProcessCollisionWithSphere(Obj[k]);
					}
				}
				Obj[j]->setMoment();
			}
		}
	}
};
#endif
