#include "gui_manager.h"
#include <windows.h>
#include "kiero.h"
#include <d3d11.h>
#include <dxgi.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx11.h>
#include <vector>

typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

bool _is_init = false;
Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

//MENUS VECTOR
std::vector<gui_menu*> menus = std::vector<gui_menu*>();
std::unique_ptr<gui_manager> manager = nullptr;

HRESULT __stdcall hooked_present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!_is_init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			//ImGUI INIT:

			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
			ImGui_ImplWin32_Init(window);
			ImGui_ImplDX11_Init(pDevice, pContext);


			_is_init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	for (gui_menu* menu : menus)
	{
		if (!menu) continue;
		menu->menu();
	}

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}

gui_manager::gui_manager()
{
	if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
	{
		kiero::bind(8, (void**)&oPresent, hooked_present);
	}
}

gui_manager::~gui_manager()
{
	kiero::shutdown(8);
}

gui_manager* gui_manager::get()
{
	if (!manager) manager = std::make_unique<gui_manager>();
	return manager.get();
}

void gui_manager::remove_menu(gui_menu* menu)
{
	for (int i = 0; i < menus.size(); i++)
	{
		if (menus[i] == menu) menus.erase(menus.begin() + i);
	}
}

void gui_manager::add_menu(gui_menu* menu)
{
	menus.push_back(menu);
}