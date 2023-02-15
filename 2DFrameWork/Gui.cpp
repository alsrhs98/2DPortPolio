#include "Framework.h"

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT Gui::MsgProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	return ImGui_ImplWin32_WndProcHandler(handle, message, wParam, lParam);
}
Gui::Gui()
{
	fileID = 0;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(App.handle);
	ImGui_ImplDX11_Init(D3D->GetDevice(), D3D->GetDC());
	ImGui::GetIO().ConfigWindowsResizeFromEdges = true;
	ImGui::GetIO().FontAllowUserScaling = true;
	ImGui::StyleColorsDark();
}

Gui::~Gui()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Gui::ResizeScreen()
{
	ImGui_ImplDX11_InvalidateDeviceObjects();
	ImGui_ImplDX11_CreateDeviceObjects();
}

void Gui::Update()
{
	fileID = 0;
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void Gui::Render()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

bool Gui::FileImGui(string button, string name, string fiter, string minPath)
{
	string strId = to_string(fileID);
	fileID++;


	// open Dialog Simple
	if (ImGui::Button(button.c_str()))
		ImGuiFileDialog::Instance()->OpenDialog(strId, name.c_str(), fiter.c_str(), minPath, ".");

	// display
	if (ImGuiFileDialog::Instance()->Display(strId))
	{
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			ImGuiFileDialog::Instance()->Close();
			return true;
			// action
		}
		// close
		ImGuiFileDialog::Instance()->Close();
	}
	return false;
}

