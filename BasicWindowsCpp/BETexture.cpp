#include "BETexture.h"
#include "BECommon.h"
#include <wincodec.h>
#include <wrl.h>

using namespace DirectX;

BETexture::BETexture(std::wstring filename)
{
	Microsoft::WRL::ComPtr<IWICImagingFactory> pFactory = nullptr;

	HRESULT hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pFactory)
	);

	BE_HR_CHECK(hr);

	Microsoft::WRL::ComPtr<IWICBitmapDecoder> pDecoder = nullptr;

	hr = pFactory->CreateDecoderFromFilename(filename.c_str(),
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnDemand,
		&pDecoder);

	BE_HR_CHECK(hr);

	Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> pFrame = nullptr;

	hr = pDecoder->GetFrame(0u, &pFrame);

	BE_HR_CHECK(hr);

	hr = pFrame->GetSize(&width, &height);

	BE_HR_CHECK(hr);

	Microsoft::WRL::ComPtr<IWICFormatConverter> pConverter = nullptr;

	hr = pFactory->CreateFormatConverter(&pConverter);

	BE_HR_CHECK(hr);

	// for reference
	// DXGI_FORMAT_R8G8B8A8_SNORM      GUID_WICPixelFormat32bppRGBA
	// DXGI_FORMAT_R32G32B32A32_FLOAT  GUID_WICPixelFormat128bppRGBAFloat

	hr = pConverter->Initialize(pFrame.Get(),
		GUID_WICPixelFormat128bppRGBAFloat,
		WICBitmapDitherTypeNone,
		nullptr,
		0.0f,
		WICBitmapPaletteTypeCustom);

	BE_HR_CHECK(hr);

	// storing the image in the BECanvas member
	surface = new BESurface2D<XMVECTOR>(width, height);

	hr = pConverter->CopyPixels(nullptr,
		surface->GetPitchBytes(),
		surface->GetTotalBytes(),
		(byte*)surface->GetData());

	BE_HR_CHECK(hr);
}
