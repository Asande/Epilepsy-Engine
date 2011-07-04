#include "Graphics.h"
#include <math.h>

Graphics::Graphics(){
	d3d = 0;
        device = 0;
        light = 0;
        text = 0;
        calculated = false;
        scenesecond = 0;
        mainCam = new FreeCamera();
}

Graphics::~Graphics(){
    if(d3d) d3d->Release();
    if(device) device->Release();
    if(text) text->Release();
    if(light) light->Release();
    delete mainCam;
}

bool Graphics::CreateDX(HWND hWnd, int width, int height){
    if( NULL == ( d3d = Direct3DCreate9( D3D_SDK_VERSION ) ) ){
            //MessageBox(NULL,L"Не могу создать Direct3D9!",L"ОШИБКА",MB_OK|MB_ICONEXCLAMATION);
        return false;
    }

    ZeroMemory( &d3dpp, sizeof(d3dpp) ); // Очищаем память из-под d3dpp

    d3dpp.FullScreen_RefreshRateInHz=0;
    d3dpp.PresentationInterval=0;
    d3dpp.hDeviceWindow = hWnd;
    d3dpp.BackBufferFormat = D3DFMT_R5G6B5;		// Формат заднего буфера
    d3dpp.EnableAutoDepthStencil = true;		// Использовать Буфер глубины и стенсил буфер
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	// Формат буфера глубины и стенсил буфера
    d3dpp.Windowed = true;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferWidth  = width;  // Укажите ваше значение ширины
    d3dpp.BackBufferHeight = height; // Укажите ваше значение высоты

    // Пытаемся создать устройство рендеринга с использованием возможностей видеокарты
    if(FAILED( d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                    D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &device)))
    {
        if(FAILED( d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
                D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &device)))
        {
                return false;
        }
    }
    //ResizeDX(width, height);
    return true;
}

bool Graphics::InitDX()
{
  if(!device)return false;

  //device->SetFVF(D3DFVF_XYZ);
  device->SetRenderState( D3DRS_LIGHTING, FALSE );

  initGrid();

  D3DXCreateTextureFromFile(device, L"img.jpg", &text);
  D3DXCreateEffectFromFile(device, L"light2.fx", NULL, NULL, NULL, NULL, &light, NULL);

  return true;
}

bool Graphics::ResizeDX(int width, int height)
{
  if (height==0)										// Предотвращаем деление на ноль
    height=1;
  D3DXMATRIX m;
  D3DXMatrixPerspectiveFovLH(&m, 45.0f, (float)width/(float)height, 0.1f, 1000.0f );
  if( device )
    device->SetTransform(D3DTS_PROJECTION, &m);
  return true;
}

bool Graphics::UpdateDX(float timeDelta)
{
    if(device)
        device->SetTransform(D3DTS_VIEW, &mainCam->getViewMatrix());
    return true;
}

bool Graphics::CleanData()
{
    if(light) light->Release();
    light = 0;
    if(text) text->Release();
    text = 0;
    return true;
}

bool Graphics::RenderDX()
{
    QMap<QString,IObject*>::iterator it;

    if( device )
    {
        device->BeginScene();
        device->Clear(0, 0, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xcccccc, 1.0f, 0);

        drawGrid();

        UINT Pass, Passes;
        for(it = scene.begin(); it != scene.end();++it ){
            if(calculated)
            {
                initShader(it.value()->queue->getVecMomentPos(scenesecond), it.value()->getSecWorld(scenesecond));
            }
            else
            {
                initShader(it.value()->getPosition(), it.value()->getWorld()); // Инициализируем шейдер
            }
            light->Begin(&Passes,0);
            for (Pass = 0; Pass < Passes; Pass++)
            {
                light->BeginPass(Pass);
                it.value()->draw();
                light->EndPass();
            }
            light->End();

        }

        device->EndScene();
        device->Present(NULL, NULL, NULL, NULL);
    }
    return true;
}

bool Graphics::initShader(D3DXVECTOR3 value, D3DXMATRIX world){
    D3DXMATRIX proj;
    device->GetTransform(D3DTS_PROJECTION, &proj);
    float *lightdir = new float[4];
    D3DXVECTOR3 lightpos;
    D3DXVec3Normalize(&lightpos, &(mainCam->getPosition() - value));

    lightdir[0] = lightpos.x; lightdir[1] = lightpos.y;
    lightdir[2] = lightpos.z; lightdir[3] = 0;

    light->SetTechnique( "LIGHT" );
    light->SetTexture( "tex", text );
    light->SetFloat("light", 1.5f);
    light->SetMatrix( "mvp", &(world * mainCam->getViewMatrix() * proj) );
    light->SetMatrix( "m", &world);
    light->SetFloatArray("vlight", lightdir, 4);
    delete lightdir;
    return true;
}

void Graphics::initGrid(){
    gridLines = new myLineVertex[126];

    for(int i = 0 ; i <= 60 ; i+=2 )
    {
        gridLines[i].x = -60.0f;
        gridLines[i].y = -60.0f + i*2.0f ;
        gridLines[i].z = 0.0f;
        gridLines[i].color = 0xaaaaaa;
        if(i == 30)
            gridLines[i].color = 0x000000;

        gridLines[i+1].x = 60.0f;
        gridLines[i+1].y = -60.0f + i*2.0f ;
        gridLines[i+1].z = 0.0f;
        gridLines[i+1].color = 0xaaaaaa;
        if(i == 30)
            gridLines[i+1].color = 0x000000;
    }
    for(int i = 62 ; i <= 122 ; i+=2)
    {
        gridLines[i].x = -60.0f + (i-62)*2.0f ;
        gridLines[i].y = -60.0f;
        gridLines[i].z = 0.0f;
        gridLines[i].color = 0xaaaaaa;
        if(i == 92)
            gridLines[i].color = 0x000000;

        gridLines[i+1].x = -60.0f + (i-62)*2.0f ;
        gridLines[i+1].y = 60.0f;
        gridLines[i+1].z = 0.0f;
        gridLines[i+1].color = 0xaaaaaa;
        if(i == 92)
            gridLines[i+1].color = 0x000000;
    }
    gridLines[124].x = 0.0f ;
    gridLines[124].y = 0.0f;
    gridLines[124].z = -5000.0f;
    gridLines[124].color = 0x00ff00;
    gridLines[125].x = 0.0f ;
    gridLines[125].y = 0.0f;
    gridLines[125].z = 5000.0f;
    gridLines[125].color = 0x00ff00;


    if( FAILED( device->CreateVertexBuffer(
                    (126)*sizeof(myLineVertex),	// Размер буфера
                    D3DUSAGE_WRITEONLY,		// Используем буфер только для чтения (быстрее)
                    D3DFVF_XYZ | D3DFVF_DIFFUSE,// Формат наших вершин
                    D3DPOOL_MANAGED,		// DirectX сам управляет пересозданием буфера
                    &buf,		// Наш указатель на буфер
                    NULL			// Оставим NULL
                    ) ) )
        return;			// Если произошла ошибка сообщить об этом

    void * pBuf;

    buf->Lock( 0, (126) * sizeof(myLineVertex), &pBuf, 0 );
    memcpy( pBuf, gridLines, (126) * sizeof(myLineVertex));
    buf->Unlock();
    delete[] gridLines;
}

void Graphics::drawGrid(){
    device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
    device->SetStreamSource(0,buf,0,sizeof(myLineVertex));
    device->DrawPrimitive(D3DPT_LINELIST, 0,63);
}
