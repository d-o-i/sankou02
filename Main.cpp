//=============================================================================
//
// ���C������ [Main.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "SceneManager.h"
#include "DebugWindow.h"
#include "Icon.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// �E�C���h�E�̃N���X��
#define WINDOW_NAME		"BrushRunner"		// �E�C���h�E�̃L���v�V������

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
bool SetWindowCenter(HWND hWnd);

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
HWND hWnd;											// �E�C���h�E�n���h��
LPDIRECT3D9			g_pD3D = NULL;					// Direct3D �I�u�W�F�N�g
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;			// Device�I�u�W�F�N�g(�`��ɕK�v)

#ifdef _DEBUG
int					FPSCount;						// FPS�J�E���^
bool ShowAnotherWindow = false;						// imGui�p�ʃE�C���h�E�̃t���O
#endif

SceneManager *pSceneManager;						// �V�[���Ǘ�

//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	srand((unsigned)time(NULL));

	// �x��
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	DWORD dwExecLastTime;							// �ŏI���s����
	DWORD dwFPSLastTime;							// ���݃t���[��
	DWORD dwCurrentTime;							// ���ݎ���
	DWORD dwFrameCount;								// �t���[���J�E���g

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),							// �\���̂̃T�C�Y
		CS_CLASSDC,									// �E�C���h�E�X�^�C���FCS_HREDRAW / CS_VREDRAW
		WndProc,									// �E�C���h�E�v���V�[�W���̃A�h���X
		0,											// �\��������
		0,											// �E�C���h�E�I�u�W�F�N�g�쐬���Ɋm�ۂ���郁�����T�C�Y
		hInstance,									// �C���X�^���X�n���h��
		LoadIcon(hInstance, (LPCTSTR)IDI_ICON1),	// �A�v���̃V���[�g�J�b�g�ȂǂŎg�p�i�A�C�R���̐ݒ�j
		LoadCursor(NULL, IDC_ARROW),				// �E�C���h�E�̃N���C�A���g��̃}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),					// �E�C���h�E�̃N���C�A���g�̈�̔w�i�F
		NULL,										// ���j���[��
		CLASS_NAME,									// �E�C���h�E�N���X�̖��O
		LoadIcon(hInstance, (LPCTSTR)IDI_ICON1)		// �E�C���h�E�̃A�C�R��
	};
	MSG msg;										// �E�C���h�E�v���V�[�W���ɓn�����b�Z�[�W

													// �E�B���h�E�N���X�̓o�^
	if (RegisterClassEx(&wcex) == 0)
	{
		return false;
	}

	// �E�B���h�E�̍쐬
	hWnd = CreateWindowEx(0,
		CLASS_NAME,																					// �N���X��
		WINDOW_NAME,																				// �E�C���h�E��
		WS_OVERLAPPEDWINDOW,																		// �E�C���h�E�X�^�C��
		CW_USEDEFAULT,																				// �E�C���h�E�̍�����W�iX�j
		CW_USEDEFAULT,																				// �E�C���h�E�̍�����W�iY�j
		SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,											// �E�C���h�E����
		SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),		// �E�C���h�E�c��
		NULL,																						// �I�[�i�[�E�C���h�E�̃n���h��
		NULL,																						// ���j���[�n���h��
		hInstance,																					// �A�v���P�[�V�����C���X�^���X�̃n���h��
		NULL);																						// �E�C���h�E�쐬�f�[�^

																									// ����������(�E�B���h�E���쐬���Ă���s��)
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	//�t���[���J�E���g������
	timeBeginPeriod(1);				// ����\��ݒ�
	dwExecLastTime =
		dwFPSLastTime = timeGetTime();
	dwCurrentTime =
		dwFrameCount = 0;

	// �E�C���h�E�̕\��(�����������̌�ɌĂ΂Ȃ��Ƒʖ�)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &mat);

	// --------------------------------------  ���b�Z�[�W���[�v---------------------------------------------
	while (1)
	{
		int scene = GetScene();
		// �Q�[���I������
		if (scene == nSceneExit)
		{
			break;
		}

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)						//�A�v���P�[�V�����̏I���v��
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		// �Q�[������
		else
		{
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5�b���ƂɎ��s
			{
#ifdef _DEBUG
				FPSCount = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;

				// �X�V����
				Update();

				// �`�揈��
				Draw();

				dwFrameCount++;
			}
		}
	}
	// -----------------------------------------------------------------------------------------------------

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// �I������
	Uninit();

	timeEndPeriod(1);				// ����\��߂�

	return (int)msg.wParam;
}

//=============================================================================
// �v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// �ϐ��錾
	static HANDLE Thread_Handle;

	switch (uMsg)
	{
		// �j��
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
		// ��V�X�e���L�[���b�Z�[�W
	case WM_KEYDOWN:
		switch (wParam)
		{
			// ESC�L�[�������ꂽ�ꍇ
		case VK_ESCAPE:
			DestroyWindow(hWnd);	// �E�C���h�E��j������
			break;
		}
		break;
	default:
		break;
	}

	// �f�o�b�O�E�B���h�E
	DebugWindPrcHandler(hWnd, uMsg, wParam, lParam);

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// ����������
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// �E�C���h�E�̕\���ʒu����ʒ����ɐݒ�
	SetWindowCenter(hWnd);

	// Direct3D�I�u�W�F�N�g�̍쐬
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ���[�N���[���N���A
	d3dpp.BackBufferCount = 1;						// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth = SCREEN_WIDTH;				// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;			// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;				// �o�b�N�o�b�t�@�t�H�[�}�b�g�̓f�B�X�v���C���[�h�ɍ��킹�Ďg��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// �f���M���ɓ������ăt���b�v����
	d3dpp.Windowed = bWindow;					// �E�B���h�E���[�h
	d3dpp.EnableAutoDepthStencil = TRUE;						// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;				// �f�v�X�o�b�t�@�Ƃ���16bit���g��

	if (bWindow)
	{// �E�B���h�E���[�h
		d3dpp.FullScreen_RefreshRateInHz = 0;								// ���t���b�V�����[�g
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// �C���^�[�o��
	}
	else
	{// �t���X�N���[�����[�h
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// ���t���b�V�����[�g
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// �C���^�[�o��
	}

	// �f�o�C�X�I�u�W�F�N�g�̐���
	// [�f�o�C�X�쐬����]<�`��>��<���_����>���n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &g_pD3DDevice)))
	{
		// ��L�̐ݒ肪���s������
		// [�f�o�C�X�쐬����]<�`��>���n�[�h�E�F�A�ōs���A<���_����>��CPU�ōs�Ȃ�
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &g_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// [�f�o�C�X�쐬����]<�`��>��<���_����>��CPU�ōs�Ȃ�
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &g_pD3DDevice)))
			{
				// ���������s
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�p�����[�^�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ���ʂ��J�����O
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��

	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�

	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O����
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����

	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

#ifdef _DEBUG
	InitDebugWindow(hWnd, g_pD3DDevice);
#endif
	pSceneManager = new SceneManager(hInstance, hWnd);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Uninit(void)
{
#ifdef _DEBUG
	UninitDebugWindow(0);
#endif

	delete pSceneManager;
	// �f�o�C�X�̊J��
	SAFE_RELEASE(g_pD3DDevice);

	// Direct3D�I�u�W�F�N�g�̊J��
	SAFE_RELEASE(g_pD3D);

}

//=============================================================================
// �X�V����
//=============================================================================
void Update(void)
{
#ifdef _DEBUG
	// �����J�n�̎��Ԃ��L�^
	BeginTimerCount();

	UpdateDebugWindow();
#endif

	pSceneManager->Update();

#ifdef _DEBUG
	// �����I���̎��Ԃ�\��
	BeginDebugWindow("Information");

	ImGui::SetNextWindowPos(ImVec2(5, 600), ImGuiSetCond_Once);

	DebugText("UpdateTime = %.3f ms", GetProgressTimerCount());

	EndDebugWindow("Information");
#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void Draw(void)
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

#ifdef _DEBUG
	// �����J�n�̎��Ԃ��L�^
	BeginTimerCount();
#endif

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		pSceneManager->Draw();

#ifdef _DEBUG
		// �����I���̎��Ԃ�\��
		BeginDebugWindow("Information");

		DebugText("DrawTime = %.3f ms\nFPS = %d\n", GetProgressTimerCount(), FPSCount);

		EndDebugWindow("Information");

		DrawDebugWindow();
#endif

		// Direct3D�ɂ��`��̏I��
		g_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// �f�o�C�X�̎擾
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//=====================================================================================================
// �E�C���h�E�̕\���ʒu����ʒ�����
//=====================================================================================================
bool SetWindowCenter(HWND hWnd)
{
	// �ϐ��錾
	RECT WindowRegion;		//	�E�B���h�E�̈�
	RECT DesktopRegion;	//	�f�X�N�g�b�v�̈�
	int WindowPositionX;	//	�E�B���h�E�ʒu X
	int WindowPositionY;	//	�E�B���h�E�ʒu Y
	int WindowSizeX;		//	�E�B���h�E�T�C�Y X
	int WindowSizeY;		//	�E�B���h�E�T�C�Y Y

							// �e�T�C�Y�̎擾
	GetMonitorRect(&DesktopRegion);				//	�f�X�N�g�b�v�̃T�C�Y���擾
	GetWindowRect(hWnd, &WindowRegion);	//	�E�B���h�E�̃T�C�Y���擾

										// �e���W�̊���o��
	WindowSizeX = (WindowRegion.right - WindowRegion.left);														//	�E�C���h�E�̉����̊���o��
	WindowSizeY = (WindowRegion.bottom - WindowRegion.top);														//	�E�C���h�E�̏c���̊���o��
	WindowPositionX = (((DesktopRegion.right - DesktopRegion.left) - WindowSizeX) / 2 + DesktopRegion.left);	//	�������̒������W���̊���o��
	WindowPositionY = (((DesktopRegion.bottom - DesktopRegion.top) - WindowSizeY) / 2 + DesktopRegion.top);	//	�c�����̒������W���̊���o��

																											// �E�B���h�E����ʒ����Ɉړ�
	return SetWindowPos	//	SetWindowPos�֐��F�q�E�B���h�E�A�|�b�v�A�b�v�E�B���h�E�A�܂��̓g�b�v���x���E�B���h�E�̃T�C�Y�A�ʒu�A����� Z �I�[�_�[��ύX����
						//						�����̃E�B���h�E�́A���̉�ʏ�ł̕\���ɏ]���ď��������߂���A
						//						�őO�ʂɂ���E�B���h�E�͍ł����������N��^�����AZ�I�[�_�[�̐擪�ɒu�����
						(
							hWnd,											//	�E�B���h�E�n���h��
							NULL,											//	�z�u�����̃n���h���F��s����E�B���h�E�̃n���h�����w��
							WindowPositionX,								//	�E�B���h�E�������"X"���W���w��F�������̈ʒu X
							WindowPositionY,								//	�E�B���h�E�������"Y"���W���w��F�c�����̈ʒu Y
							WindowSizeX,									//	�E�B���h�E�̉������w�� X
							WindowSizeY,									//	�E�B���h�E�̏c�����w�� Y
							(SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER)	//	�E�B���h�E�ʒu�̃I�v�V�����F�E�B���h�E�̃T�C�Y��A�ʒu�̕ύX�Ɋւ���t���O���w��
						);

}
