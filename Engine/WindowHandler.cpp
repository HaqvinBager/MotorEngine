#include "WindowHandler.h"
#include "Input.h"

LRESULT CWindowHandler::WinProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    static CWindowHandler* windowHandler = nullptr;
    CREATESTRUCT* createStruct;
    switch (uMsg)
    {
        case WM_CLOSE:

        case WM_DESTROY:       
            PostQuitMessage(0);
            return 0;

        case WM_CREATE:
            createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
            windowHandler = reinterpret_cast<CWindowHandler*>(createStruct->lpCreateParams);
            break;
        default:
            Input::GetInstance()->update_events(uMsg, wParam, lParam);
            break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

CWindowHandler::CWindowHandler()
{
    myWindowHandle = 0;
}

CWindowHandler::~CWindowHandler()
{
    myWindowHandle = 0;
    UnregisterClass(L"3DEngine", GetModuleHandle(nullptr));
}

bool CWindowHandler::Init(CWindowHandler::SWindowData someWindowData)
{
    myWindowData = someWindowData;

    HCURSOR customCursor = LoadCursorFromFileW(L"Assets/3D/UI/Ingame/UI_In_Cursor_01_19G4.cur");

    WNDCLASSEX windowclass = {};
    windowclass.cbSize = sizeof(WNDCLASSEX);
    windowclass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
    windowclass.lpfnWndProc = CWindowHandler::WinProc;
    windowclass.cbClsExtra = 0;
    windowclass.cbWndExtra = 0;
    windowclass.hInstance = GetModuleHandle(nullptr);
    windowclass.hIcon = (HICON)LoadImage(NULL, L"ironwrought.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
    windowclass.hCursor = customCursor;
    windowclass.lpszClassName = L"3DEngine";
    RegisterClassEx(&windowclass);

    myWindowHandle = CreateWindow(L"3DEngine", L"IronWrought",
        WS_OVERLAPPEDWINDOW | WS_POPUP | WS_VISIBLE,
        someWindowData.myX, someWindowData.myY, someWindowData.myWidth, someWindowData.myHeight,
        nullptr, nullptr, nullptr, this);

    myResolution = new Vector2();
    // //Start in fullscreen
    //myWindowHandle = CreateWindow(L"3DEngine", L"IronWrought", 
    //    WS_POPUP | WS_VISIBLE,
    //    0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
    //    NULL, NULL, GetModuleHandle(nullptr), NULL);

    return true;
}

const HWND CWindowHandler::GetWindowHandle() const
{
    return myWindowHandle;
}

UINT CWindowHandler::GetWidth() const {
    LPRECT rect = new RECT{ 0, 0, 0, 0 };
    if (GetClientRect(myWindowHandle, rect) != 0) {
        return rect->right;
    }

    return myWindowData.myWidth;
}

UINT CWindowHandler::GetHeight() const {
    LPRECT rect = new RECT{ 0, 0, 0, 0 };
    if (GetClientRect(myWindowHandle, rect) != 0) {
        return rect->bottom;
    }
    return myWindowData.myHeight;
}

DirectX::SimpleMath::Vector2 CWindowHandler::GetResolution()
{
    return *myResolution;
}

void CWindowHandler::SetResolution()
{
    LPRECT rect = new RECT{ 0, 0, 0, 0 };
    if (GetClientRect(myWindowHandle, rect) != 0) {
        myResolution->x = static_cast<float>(rect->right);
        myResolution->y = static_cast<float>(rect->bottom);
    }
}

void CWindowHandler::SetWindowTitle(std::string aString)
{
    SetWindowTextA(myWindowHandle, aString.c_str());
}
