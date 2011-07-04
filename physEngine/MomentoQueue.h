#ifndef MOMENTOQUEUE_H
#define MOMENTOQUEUE_H

class MomentoQueue
{
private:
        struct Momento{
                D3DXVECTOR3 Position;
                D3DXQUATERNION Orientation;
        };

        std::vector<Momento*> moments;

public:
        MomentoQueue();
        MomentoQueue(D3DXVECTOR3 pos, D3DXQUATERNION or);
        void setMoment(D3DXVECTOR3 pos, D3DXQUATERNION or);

        D3DXVECTOR3 getVecMomentPos(int sec);
        D3DXQUATERNION getVecMomentOr(int sec);
};

#endif // MOMENTOQUEUE_H
