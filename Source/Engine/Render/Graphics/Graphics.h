#pragma once

namespace Engine {

	class Graphics
	{
	public:
		Graphics() = default;
		~Graphics() = default;
		// disabling instances	
		Graphics(const Graphics& other) = delete;
		Graphics operator=(const Graphics& other) = delete;
		
		bool Init(HWND hwnd);
		void Render();

		bool IsRunning();
		
		// bool Release();

	private:		

		Microsoft::WRL::ComPtr<IDXGIFactory4> m_dxgiFactory;
		HWND pm_hwnd;
		bool m_running = false;
		
		// Device		
		Microsoft::WRL::ComPtr<IDXGIAdapter1> m_adapter;		
		Microsoft::WRL::ComPtr<ID3D12Device> m_device;

		// Command Queue
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQ;

		// Swap Chain
		Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swapChain;
		UINT m_frameIndex = 0;

		// RTV
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvDescriptorHeap;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_renderTargets[WindowData::BufferNum];
		UINT m_rtvDescriptorSize;

		// Command allocator
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocator[WindowData::BufferNum];
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;

		// Fence
		Microsoft::WRL::ComPtr<ID3D12Fence> m_fence[WindowData::BufferNum];
		HANDLE m_fenceEvent;		
		UINT64 m_fenceValue[WindowData::BufferNum]; 
									
		
		// Pipeline
		bool InitDevice();
		bool InitCommandQueue();
		bool InitSwapChain();	
		bool InitRTV();
		bool InitCommandList();
		bool InitFence();	

		//
		void UpdatePipeline();
		void Cleanup();
		void WaitForPreviousFrame();
	};

} // Engine