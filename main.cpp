
#include	"main.h"
#include	"renderer.h"
#include	"Manager.h"
#include	"keyboard.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "debugUI.h"

//===================================
// ���C�u�����̃����N
//===================================
#pragma	comment (lib, "d3d11.lib")
#pragma	comment (lib, "d3dcompiler.lib")
#pragma	comment (lib, "winmm.lib")
#pragma	comment (lib, "dxguid.lib")
#pragma	comment (lib, "dinput8.lib")

//=================================
//�}�N����`
//=================================
#define		CLASS_NAME		"GameProject Window"
#define		WINDOW_CAPTION	"GameProject   <<3D�̒��̐l>>"

//===================================
//�v���g�^�C�v�錾
//===================================
//�R�[���o�b�N�֐��������l���Ăяo���Ă����֐�
LRESULT	CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//�������֐�
HRESULT	Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
//�I������
void	Finalize(void);
//�X�V����
void	Update(void);
//�`�揈��
void	Draw(void);

//==================================
//�O���[�o���ϐ�
//==================================
#ifdef _DEBUG	//�f�o�b�O���[�h���̂ݕϐ������
int	g_CountFPS;							//FPS�J�E���^�[
char g_DebugStr[2048] = WINDOW_CAPTION;	//�\��������ݒ�
#endif

//=====================================
//���C���֐�
//======================================
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance, LPSTR lpCmd, int nCmdShow)
{
	//����������
	srand((UINT)timeGetTime());
	//�t���[�����[�g�v���p�ϐ�
	DWORD	dwExecLastTime;
	DWORD	dwFPSLastTime;
	DWORD	dwCurrentTime;
	DWORD	dwFrameCount;
	//COM�R���|�[�l���g�̏����i�@�\�𕔕i�����ĊO���̃v���O�������狤�L���p����d�g�݁j
	HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);

	//�E�B���h�E�N���X�̓o�^�i�E�B���h�E�̎d�l�I�ȕ������߂�Windows�փZ�b�g����j
	WNDCLASS	wc;	//�\���̂�����
	ZeroMemory(&wc, sizeof(WNDCLASS));//���e���O�ŏ�����
	wc.lpfnWndProc = WndProc;	//�R�[���o�b�N�֐��̃|�C���^�[
	wc.lpszClassName = CLASS_NAME;	//���̎d�l���̖��O
	wc.hInstance = hInstance;	//���̃A�v���P�[�V�����̂���
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);//�J�[�\���̎��
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);//�E�B���h�E�̔w�i�F
	RegisterClass(&wc);	//�\���̂�Windows�փZ�b�g


	//�E�B���h�E�T�C�Y�̒���
	//             ����@�@�E��
	RECT	rc = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };//��1280 �c720
	//�`��̈悪1280X720�ɂȂ�悤�ɃT�C�Y�𒲐�����
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME|WS_MAXIMIZEBOX|WS_MINIMIZEBOX), FALSE);

	//�E�B���h�E�̍쐬
	HWND	hWnd = CreateWindow(
		CLASS_NAME,	//��肽���E�B���h�E
		WINDOW_CAPTION,	//�E�B���h�E�ɕ\������^�C�g��
		WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX),//�W���I�Ȍ`��̃E�B���h�E �T�C�Y�ύX�֎~
		CW_USEDEFAULT,		//�f�t�H���g�ݒ�ł��܂���
		CW_USEDEFAULT,
		rc.right - rc.left,//CW_USEDEFAULT,//�E�B���h�E�̕�
		rc.bottom - rc.top,//CW_USEDEFAULT,//�E�B���h�E�̍���
		NULL,
		NULL,
		hInstance,		//�A�v���P�[�V�����̃n���h��
		NULL
	);

	//����������
	if (FAILED(Init(hInstance, hWnd, true)))
	{
		return -1;//���������s
	}
	Keyboard_Initialize();

	//�쐬�����E�B���h�E��\������
	ShowWindow(hWnd, nCmdShow);//�����ɏ]���ĕ\���A�܂��͔�\��
	//�E�B���h�E�̓��e�������\��
	UpdateWindow(hWnd);

	//���b�Z�[�W���[�v
	MSG	msg;
	ZeroMemory(&msg, sizeof(MSG));//���b�Z�[�W�\���̂��쐬���ď�����
	//�t���[�����[�g�v��������
	timeBeginPeriod(1);	//�^�C�}�[�̕���\��ݒ�
	dwExecLastTime = dwFPSLastTime = timeGetTime();//���݂̃^�C�}�[�l
	dwCurrentTime = dwFrameCount = 0;

	//�I�����b�Z�[�W������܂Ń��[�v����
	//�Q�[�����[�v
	while (1)
	{	//���b�Z�[�W�̗L�����`�F�b�N
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{ //Windows���烁�b�Z�[�W�������I
			if (msg.message == WM_QUIT)//���S�I�����܂������b�Z�[�W
			{
				break;	//while���[�v����ʂ���
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);	//WndProc���Ăяo�����
			}
	
		}
		else //Windows���烁�b�Z�[�W�����Ă��Ȃ�
		{
			dwCurrentTime = timeGetTime();//���݂̃^�C�}�[�l���擾
			if ((dwCurrentTime - dwFPSLastTime) >= 1000)//1�b�o�߂�����
			{
#ifdef _DEBUG
				g_CountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;	//���݂̃^�C�}�[�l��ۑ�
				dwFrameCount = 0;				//�t���[���J�E���g���N���A
			}
			if ((dwCurrentTime - dwExecLastTime) >= ((float)1000 / 60)) // 1/60�b�o�߂�����
			{
				dwExecLastTime = dwCurrentTime;	//���݂̎��Ԃ�ۑ�
#ifdef _DEBUG
				wsprintf(g_DebugStr, WINDOW_CAPTION);//�z��ɃL���v�V������������i�[
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " FPS:%d", g_CountFPS);
				SetWindowText(hWnd, g_DebugStr);//�L���v�V���������̏�������
#endif

				Update();	//�X�V����
				Draw();		//�`�揈��
				keycopy();	//�L�[�����p

				dwFrameCount++;	//�t���[���J�E���g��i�߂�
			}

		}
	}//while

	//�I������
	Finalize();

	//�I������
	return (int)msg.wParam;

}

//=========================================
//�E�B���h�E�v���V�[�W��
// ���b�Z�[�W���[�v���ŌĂяo�����
//=========================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;
	

	switch (uMsg)
	{
		case WM_ACTIVATEAPP:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			Keyboard_ProcessMessage(uMsg, wParam, lParam);
			break;
		case WM_KEYDOWN:	//�L�[�������ꂽ
			if (wParam == VK_ESCAPE)//�����ꂽ�̂�ESC�L�[
			{
				//�E�B���h�E����������N�G�X�g��Windows�ɑ���
				SendMessage(hWnd, WM_CLOSE, 0, 0);
			}
			Keyboard_ProcessMessage(uMsg, wParam, lParam);
			break;
		case WM_CLOSE:	//�E�B���h�E����Ȃ�������				
			if (
				MessageBox(hWnd, "�{���ɏI�����Ă�낵���ł����H",
					"�m�F", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK
				)
			{//OK�������ꂽ�Ƃ�
				DestroyWindow(hWnd);//�I������葱����Windows�փ��N�G�X�g
			}
			else
			{
				return 0;	//����ς�I���Ȃ�
			}

			break;
		case WM_DESTROY:	//�I������OK�ł���
			PostQuitMessage(0);		//�����̃��b�Z�[�W�ɂO�𑗂�
			break;

	}

	//�K�p�̖������b�Z�[�W�͓K���ɏ��������ďI��
	return DefWindowProc(hWnd, uMsg, wParam, lParam);

}

//==================================
//������
//==================================
HRESULT	Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//DirectX�֘A�̏�����
	InitRenderer(hInstance, hWnd, bWindow);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // imgui�̃L�[�{�[�h����
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // imgui�̃p�b�h����


	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(GetDevice(), GetDeviceContext());

	// ���{��t�H���g�̎w��
	//���ۂ�\�����������{��\������O�̃_�u���N�H�[�e�[�V������u8������iUnicode�w��j
	io.Fonts->AddFontFromFileTTF(u8"c:\\Windows\\Fonts\\meiryo.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());

	//�L�[���͏�����
//	Keyboard_Initialize();
	//�}�l�[�W��������
	InitManager();


	return	S_OK;
}

//====================================
//	�I������
//====================================
void	Finalize(void)
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	//�}�l�[�W���I��
	FinalizeManager();
	//DirectX�֘A�̏I������
	FinalizeRenderer();
}

//===================================
//�X�V����
//====================================
void	Update(void)
{
	// Imgui�̉�ʂ����
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//Imgui_debugUI
	ShowDebugUI();

	//�}�l�[�W���X�V
	UpdateManager();

}

//==================================
//�`�揈��
//==================================
void	Draw(void)
{
	//�o�b�N�o�b�t�@�̃N���A
	Clear();
	//�}�l�[�W���`��
	DrawManager();

	// Imgui�̕`��
	// (Your code clears your framebuffer, renders your other stuff etc.)
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	//�o�b�N�o�b�t�@���t�����g�o�b�t�@�փR�s�[
	Present();

}


