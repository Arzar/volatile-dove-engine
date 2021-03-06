#pragma once

#include <wrl.h>
#include <d3d11_1.h>
#include <d2d1_1.h>
#include <d2d1effects.h>
#include <dwrite_1.h>
#include <wincodec.h>
#ifndef VERSION_WINDOWS_8_0
#include <DXGI1_3.h>
#endif

//#include <agile.h>

class AbstractWindow;
class DXMain;

// Helper class that initializes DirectX APIs
ref class DirectXBase abstract
{
public:
	
	virtual void BeginDraw();
	virtual void EndDraw();

	void getWindowSize(int* width, int* height);
	void uninitializeWindow();

	void cleanup();

	void OnSuspending(Object ^sender, Windows::ApplicationModel::SuspendingEventArgs ^e);
	void OnResuming(Platform::Object^ sender, Platform::Object^ args);
	void Clear(Windows::UI::Color color);

	void startComputingFrameDuration();
	int64_t computeFrameDuration();

	virtual ~DirectXBase();

internal:
	bool isDrawing() const;
	const AbstractWindow* getWindow() const;
	virtual void Initialize(AbstractWindow* abstractWindow, IUnknown* imageSourceOrNull, int imageSourceWidth, int imageSourceHeight);

protected:
	virtual void UpdateForWindowSizeChange();
	virtual void CreateDeviceResources();
	virtual void CreateDeviceIndependentResources();
	virtual void CreateWindowSizeDependentResources();

	virtual void SetDpi(float dpi);
	virtual void Trim();

private:

	void cleanupSizeDependentResources();

	virtual void Present() sealed;

	bool m_isSuspended;
	int64_t m_msNow;

//private protected:
private:

	DXMain* m_directXMainObjects;
};
