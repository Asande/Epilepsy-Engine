#include "MomentoQueue.h"

MomentoQueue::MomentoQueue()
{
}

MomentoQueue::MomentoQueue(D3DXVECTOR3 pos, D3DXQUATERNION or)
{
        Momento *moment = new Momento;
        moment->Position = pos;
        moment->Orientation = or;

        moments.push_back(moment);
}

void MomentoQueue::setMoment(D3DXVECTOR3 pos,D3DXQUATERNION or)
{
    Momento *moment = new Momento;
    moment->Position = pos;
    moment->Orientation = or;

    moments.push_back(moment);
}

D3DXVECTOR3 MomentoQueue::getVecMomentPos(int sec)
{
        return moments[sec]->Position;
}

D3DXQUATERNION MomentoQueue::getVecMomentOr(int sec)
{
        return moments[sec]->Orientation;
}
