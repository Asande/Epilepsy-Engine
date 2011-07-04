#pragma once
#pragma comment(lib, "C:\\Program Files\\Microsoft DirectX SDK (August 2009)\\Lib\\x86\\d3dx9.lib")
#pragma comment(lib, "C:\\Program Files\\Microsoft DirectX SDK (August 2009)\\Lib\\x86\\d3d9.lib")
#pragma comment(lib, "C:\\Program Files\\Microsoft DirectX SDK (August 2009)\\Lib\\x86\\dxguid.lib")

#include "C:\Program Files\Microsoft DirectX SDK (August 2009)\Include\d3dx9.h"
#include <Windows.h>
#include <QMap>
#include <QString>

#include "iobject.h"
#include "ICamera.h"
#include "FreeCamera.h"
#include "EpController.h"



class Graphics{
protected:

	LPDIRECT3D9             d3d;
	LPDIRECT3DDEVICE9       device;
        D3DPRESENT_PARAMETERS   d3dpp;

        ID3DXEffect *light;
        LPDIRECT3DTEXTURE9 text;

        EpController *controller;

        void drawGrid();
        void initGrid();

        typedef struct mylinevertex
        {
            float x,y,z;
            D3DCOLOR color;
        }myLineVertex;

        myLineVertex *gridLines;
        IDirect3DVertexBuffer9 *buf;

        ICamera *mainCam;
        QMap<QString, IObject*> scene;

        bool calculated;
        int scenesecond;

public:
        
	Graphics();
	virtual ~Graphics();

	bool CreateDX(HWND hWnd, int width, int height);

        virtual bool InitDX();
        virtual bool ResizeDX(int width, int height);
        virtual bool UpdateDX(float timeDelta);
        virtual bool RenderDX();
        virtual bool CleanData();
        bool initShader(D3DXVECTOR3 value, D3DXMATRIX world);

        LPDIRECT3DDEVICE9 getDevice(){return this->device;};

};
