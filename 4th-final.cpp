#include <windows.h>
#include <string>

HFONT hButtonFont, hFontBorder;
wchar_t textBuffer[512] = L"Text line 0"; // Buffer để chứa nội dung văn bản
wchar_t borderBuffer[512]; // Buffer để chứa nội dung viền
int buttonX, buttonY, buttonWidth = 200, buttonHeight = 60; // Biến toàn cục cho vị trí và kích thước nút

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static int step = 0;
    HWND hButton;

    switch (uMsg)
    {

    case WM_CREATE:
    {
        // Tạo font cho Texts
        hFontBorder = CreateFontW(32, 0, 0, 0, FW_NORMAL, TRUE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                            CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Montserrat"); //Font

        // Tạo font cho Button
        hButtonFont = CreateFontW(40, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                                  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Saturday Sweat");

        RECT rect;
        GetClientRect(hwnd, &rect);
        buttonX = (rect.right - rect.left - buttonWidth) / 2;
        buttonY = rect.bottom - buttonHeight - 40;

        hButton = CreateWindowW(L"Button", L"Next",
                                WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
                                buttonX, buttonY, buttonWidth, buttonHeight, // Căn giữa nút
                                hwnd, (HMENU)2, NULL, NULL); 
        break;
    }

    case WM_COMMAND:
        if (LOWORD(wParam) == 2)
        {
            step++;

            switch (step)
            {
            case 1:
                wcscpy_s(textBuffer, L"Text line 1");
                break;
            case 2:
                wcscpy_s(textBuffer, L"Text line 2");
                break;
            case 3:
                wcscpy_s(textBuffer, L"Text line 3");
                break;
            case 4:
                wcscpy_s(textBuffer, L"Text line 4");
                break;
            default:
                DestroyWindow(hwnd);  // Đóng cửa sổ sau khi kết thúc
                break;
            }

            InvalidateRect(hwnd, NULL, TRUE); // Yêu cầu vẽ lại toàn bộ cửa sổ
        }
        break;

    case WM_DRAWITEM:
        if ((UINT)wParam == 2)
        {
            LPDRAWITEMSTRUCT lpDrawItem = (LPDRAWITEMSTRUCT)lParam;

            // Tạo brush với màu - giúp tạo hiệu ứng khi Click Button
            HBRUSH hBrush;
            if (lpDrawItem->itemState & ODS_SELECTED)
            {
                hBrush = CreateSolidBrush(RGB(192,192,192)); // Tùy chỉnh màu khi Click
            }
            else
            {
                hBrush = CreateSolidBrush(RGB(255,255,255)); //Tùy chỉnh màu trước Click
            }

            HBRUSH hOldBrush = (HBRUSH)SelectObject(lpDrawItem->hDC, hBrush);

            // Tạo đường viền bo tròn
            RoundRect(lpDrawItem->hDC, lpDrawItem->rcItem.left, lpDrawItem->rcItem.top,
                      lpDrawItem->rcItem.right, lpDrawItem->rcItem.bottom, 20, 20);

            // Thiết lập màu chữ và font chữ
            SetTextColor(lpDrawItem->hDC, RGB(0, 0, 0)); // Màu chữ đen
            SetBkMode(lpDrawItem->hDC, TRANSPARENT);
            SelectObject(lpDrawItem->hDC, hButtonFont); // Font đậm cho văn bản trên nút

            // Lấy kích thước của text
            SIZE size;
            std::wstring text = L"< Next >";
            GetTextExtentPoint32(lpDrawItem->hDC, text.c_str(), text.length(), &size);

            // Vẽ văn bản căn giữa trên nút
            int textX = (lpDrawItem->rcItem.right - lpDrawItem->rcItem.left - size.cx) / 2;
            int textY = (lpDrawItem->rcItem.bottom - lpDrawItem->rcItem.top - size.cy) / 2;
            TextOut(lpDrawItem->hDC, textX, textY, text.c_str(), text.length());

            SelectObject(lpDrawItem->hDC, hOldBrush);
            DeleteObject(hBrush);
        }
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            RECT rect;
            GetClientRect(hwnd, &rect);
            FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW+1)); // Làm sạch toàn bộ vùng vẽ

            SelectObject(hdc, hFontBorder); // Áp dụng font

            // Tạo hai dòng viền trên và dưới bằng "♥"
            const int borderLength = 52; // Độ dài viền cố định
            std::wstring borderLine(borderLength, L'♥'); // Tạo viền trên và dưới có chiều dài cố định

            // Chuyển đổi sang kiểu wchar_t để dùng với DrawTextW
            wcscpy_s(borderBuffer, borderLine.c_str());

            // Vẽ viền trên
            SetTextColor(hdc, RGB(255, 0, 0)); // Đổi màu chữ thành đỏ
            rect.top = 40; // Khoảng cách từ đầu cửa sổ tới viền trên
            DrawTextW(hdc, borderBuffer, -1, &rect, DT_CENTER | DT_NOCLIP);

            // Vẽ văn bản chính
            SetTextColor(hdc, RGB(0,0,0));
            rect.top += 80; // Tăng khoảng cách giữa viền trên và văn bản
            DrawTextW(hdc, textBuffer, -1, &rect, DT_CENTER | DT_NOCLIP);

            // Vẽ viền dưới
            SetTextColor(hdc, RGB(255, 0, 0)); // Đổi màu chữ thành đỏ
            rect.top += 80; // Tăng khoảng cách giữa văn bản và viền dưới
            DrawTextW(hdc, borderBuffer, -1, &rect, DT_CENTER | DT_NOCLIP);

            EndPaint(hwnd, &ps);
        }
        break;

    case WM_DESTROY:
        DeleteObject(hFontBorder); // Xóa font khi cửa sổ bị đóng
        DeleteObject(hButtonFont); // Xóa font của button khi cửa sổ bị đóng
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    
    }

    return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASSW wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"Title-Goes-Here",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, // Không cho phép thay đổi kích thước
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 400, // Tăng chiều ngang và chiều cao của cửa sổ
        NULL,
        NULL,
        hInstance,
        NULL);

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
