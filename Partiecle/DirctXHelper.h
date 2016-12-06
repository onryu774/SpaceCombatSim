#pragma once

#include<winnt.h>
#include<intsafe.h>
#include<d3d11.h>
#include <ppltasks.h>	// create_task �p

namespace DX
{
	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			// Win32 API �G���[���L���b�`���邽�߂̃u���[�N�|�C���g�����̍s�ɐݒ肵�܂��B
			//throw Platform::Exception::CreateException(hr);
			throw hr;
		}
	}

	//	// �o�C�i�� �t�@�C������񓯊��ɓǂݎ��֐��B
	//	inline Concurrency::task<std::vector<byte>> ReadDataAsync(const std::wstring& filename)
	//	{
	//		using namespace Windows::Storage;
	//		using namespace Concurrency;
	//
	////		auto folder = Windows::ApplicationModel::Package::Current->InstalledLocation;
	//		wchar_t folder[MAX_PATH];
	//		GetCurrentDirectory(MAX_PATH, folder);
	//		return create_task(folder->GetFileAsync(Platform::StringReference(filename.c_str()))).then([] (StorageFile^ file) 
	//		{
	//			return FileIO::ReadBufferAsync(file);
	//		}).then([] (Streams::IBuffer^ fileBuffer) -> std::vector<byte> 
	//		{
	//			std::vector<byte> returnBuffer;
	//			returnBuffer.resize(fileBuffer->Length);
	//			Streams::DataReader::FromBuffer(fileBuffer)->ReadBytes(Platform::ArrayReference<byte>(returnBuffer.data(), fileBuffer->Length));
	//			return returnBuffer;
	//		});
	//	}

	// �f�o�C�X�Ɉˑ����Ȃ��s�N�Z���P�� (DIP) �̒����𕨗��I�ȃs�N�Z���̒����ɕϊ����܂��B
	inline float ConvertDipsToPixels(float dips, float dpi)
	{
		static const float dipsPerInch = 96.0f;
		return floorf(dips * dpi / dipsPerInch + 0.5f); // �ł��߂������l�Ɋۂ߂܂��B
	}

#if defined(_DEBUG)
	// SDK ���C���[�̃T�|�[�g���m�F���Ă��������B
	inline bool SdkLayersAvailable()
	{
		HRESULT hr = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_NULL,       // ���ۂ̃n�[�h�E�F�A �f�o�C�X���쐬����K�v�͂���܂���B
			0,
			D3D11_CREATE_DEVICE_DEBUG,  // SDK ���C���[���m�F���Ă��������B
			nullptr,                    // �ǂ̋@�\���x���ł��Ή��ł��܂��B
			0,
			D3D11_SDK_VERSION,          // Windows �X�g�A �A�v���ł́A����ɂ͏�� D3D11_SDK_VERSION ��ݒ肵�܂��B
			nullptr,                    // D3D �f�o�C�X�̎Q�Ƃ�ێ�����K�v�͂���܂���B
			nullptr,                    // �@�\���x���𒲂ׂ�K�v�͂���܂���B
			nullptr                     // D3D �f�o�C�X�̃R���e�L�X�g�Q�Ƃ�ێ�����K�v�͂���܂���B
			);

		return SUCCEEDED(hr);
	}
#endif
}
