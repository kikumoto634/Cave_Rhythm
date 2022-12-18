#include "imguiManager.h"

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>
#include <cassert>

void imguiManager::Initialize(Window* winApp, DirectXCommon* dxCommon)
{
	assert(winApp);
	this->winApp = winApp;
	this->dxCommon = dxCommon;

	//ImGui�̃R���e�L�X�g�𐶐�
	ImGui::CreateContext();
	//ImGui�̃X�^�C����ݒ�
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(this->winApp->GetHwnd());

	// �f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	// �f�X�N���v�^�q�[�v����
	HRESULT result = this->dxCommon->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));

	ImGui_ImplDX12_Init(
    this->dxCommon->GetDevice(),
    static_cast<int>(this->dxCommon->GetBackBuffersCount()),
    DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, srvHeap.Get(),
    srvHeap->GetCPUDescriptorHandleForHeapStart(),
    srvHeap->GetGPUDescriptorHandleForHeapStart()
    );

	ImGuiIO& io = ImGui::GetIO();
	// �W���t�H���g��ǉ�����
	io.Fonts->AddFontDefault();
}

void imguiManager::Begin()
{
	//ImGui�t���[���J�n
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void imguiManager::End()
{
	//�`��O����
	ImGui::Render();
}

void imguiManager::Draw()
{
	ID3D12GraphicsCommandList* commandList = dxCommon->GetCommandList();
	//�f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = {srvHeap.Get()};
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//�`��R�}���h
	 ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
}

void imguiManager::Finalize()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
