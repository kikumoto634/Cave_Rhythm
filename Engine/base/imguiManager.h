#pragma once
#include "Window.h"
#include "DirectXCommon.h"

#include <wrl.h>
#include <d3d12.h>

class imguiManager
{
public:
	//������
	void Initialize(Window* winApp, DirectXCommon* dxCommon);

	//ImGui��t�J�n
	void Begin();

	//ImGui��t�I��
	void End();

	//�`��
	void Draw();

	//�㏈��
	void Finalize();

private:
	Window* winApp = nullptr;
	DirectXCommon* dxCommon = nullptr;

	//SRV�p�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;
};

