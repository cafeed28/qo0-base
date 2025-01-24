#include "inputsystem.h"

#include "core/variables.h"
#include "core/menu.h"
#include "core/hooks.h"

#include "core/interfaces.h"
#include "sdk/interfaces/iinputsystem.h"

bool IPT::Setup()
{
	// get directx swapchain data
	D3DDEVICE_CREATION_PARAMETERS creationParameters = {};
	if (FAILED(I::DirectDevice->GetCreationParameters(&creationParameters)))
		return false;

	// store window pointer
	hWindow = creationParameters.hFocusWindow;
	if (hWindow == nullptr)
		return false;

	// change window message handle to our
	pOldWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(hWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(H::WndProc)));
	if (pOldWndProc == nullptr)
		return false;

	return true;
}

void IPT::Destroy()
{
	if (pOldWndProc != nullptr)
	{
		SetWindowLongPtrW(hWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(pOldWndProc));
		pOldWndProc = nullptr;
	}

	// reset input state
	I::InputSystem->EnableInput(true);
}

extern ImGuiKey ImGui_ImplWin32_KeyEventToImGuiKey(WPARAM wParam, LPARAM lParam);
bool IPT::OnWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// current active key
	ImGuiKey uKey = ImGuiKey_None;
	// current active key state
	KeyState_t state = KEY_STATE_NONE;

	switch (uMsg)
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (wParam < 256U)
		{
			uKey = ImGui_ImplWin32_KeyEventToImGuiKey(wParam, lParam);
			state = KEY_STATE_DOWN;
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		if (wParam < 256U)
		{
			uKey = ImGui_ImplWin32_KeyEventToImGuiKey(wParam, lParam);
			state = KEY_STATE_UP;
		}
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
		uKey = ImGuiKey_MouseLeft;
		state = uMsg == WM_LBUTTONUP ? KEY_STATE_UP : KEY_STATE_DOWN;
		break;
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
		uKey = ImGuiKey_MouseRight;
		state = uMsg == WM_RBUTTONUP ? KEY_STATE_UP : KEY_STATE_DOWN;
		break;
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDBLCLK:
		uKey = ImGuiKey_MouseMiddle;
		state = uMsg == WM_MBUTTONUP ? KEY_STATE_UP : KEY_STATE_DOWN;
		break;
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_XBUTTONDBLCLK:
		uKey = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? ImGuiKey_MouseX1 : ImGuiKey_MouseX2);
		state = uMsg == WM_XBUTTONUP ? KEY_STATE_UP : KEY_STATE_DOWN;
		break;
	default:
		return false;
	}

	if (uKey == ImGuiKey_None)
		return false;

	Q_ASSERT(uKey >= ImGuiKey_NamedKey_BEGIN && uKey <= ImGuiKey_NamedKey_END);

	// prevent process when e.g. binding something in-menu
	if (MENU::bMainOpened && uKey != C::Get<KeyBind_t>(Vars.iMenuKey).uKey && uKey != C::Get<KeyBind_t>(Vars.iPanicKey).uKey)
		return false;

	int nIndex = uKey - ImGuiKey_NamedKey_BEGIN;
	Q_ASSERT(nIndex >= 0 && nIndex < ImGuiKey_NamedKey_COUNT);

	// save key states
	if (arrKeyState[nIndex] == KEY_STATE_UP && state == KEY_STATE_DOWN)
		arrKeyState[nIndex] = KEY_STATE_PRESSED;
	else
		arrKeyState[nIndex] = state;

	return true;
}

bool IPT::GetBindState(KeyBind_t& keyBind)
{
	if (keyBind.uKey == ImGuiKey_None)
		return false;

	switch (keyBind.nMode)
	{
	case EKeyBindMode::HOLD:
		keyBind.bEnable = IsKeyDown(keyBind.uKey);
		break;
	case EKeyBindMode::TOGGLE:
		if (IsKeyPressed(keyBind.uKey))
			keyBind.bEnable = !keyBind.bEnable;
		break;
	}

	return keyBind.bEnable;
}
