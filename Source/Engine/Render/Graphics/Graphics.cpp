#include "precomp.h"
#include "Graphics.h"

using namespace Microsoft::WRL;
using namespace DirectX;



bool Engine::Graphics::Init(HWND hwnd)
{
    pm_hwnd = hwnd;

    bool retval =  InitDevice() && InitCommandQueue() 
        && InitSwapChain() && InitRTV() 
        && InitCommandList() && InitFence()
        ;

    if(retval)
        m_running = true;

    return retval;
}




void Engine::Graphics::Render()
{
	HRESULT hr;

	UpdatePipeline();
					  
	ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
	
	m_commandQ->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
	
	hr = m_commandQ->Signal(m_fence[m_frameIndex].Get(), m_fenceValue[m_frameIndex]);
	if (FAILED(hr))
	{
		m_running = false;
	}

	// present the current backbuffer
	hr = m_swapChain->Present(0, 0);
	if (FAILED(hr))
	{
		m_running = false;
	}
}




bool Engine::Graphics::IsRunning()
{
	return m_running;
}




void Engine::Graphics::UpdatePipeline()
{
	HRESULT hr;
	
	WaitForPreviousFrame();
	
	hr = m_commandAllocator[m_frameIndex]->Reset();
	if (FAILED(hr))
	{
		m_running = false;
	}

	hr = m_commandList->Reset(m_commandAllocator[m_frameIndex].Get(), NULL);
	if (FAILED(hr))
	{
		m_running = false;
	}	
	
	auto resBarrier{ CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET) };
	m_commandList->ResourceBarrier(1, &resBarrier);
	
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);
	
	m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
	
	const float backColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	m_commandList->ClearRenderTargetView(rtvHandle, backColor, 0, nullptr);
	
	resBarrier= CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	m_commandList->ResourceBarrier(1, &resBarrier);

	hr = m_commandList->Close();
	if (FAILED(hr))
	{
		m_running = false;
	}
}




void Engine::Graphics::Cleanup()
{
	// wait for the gpu to finish all frames
	for (int i = 0; i < WindowData::BufferNum; ++i)
	{
		m_frameIndex = i;
		WaitForPreviousFrame();
	}

	// get swapchain to windowed before exiting
	BOOL fs = false;
	if( SUCCEEDED( m_swapChain->GetFullscreenState(&fs, NULL) ))
		m_swapChain->SetFullscreenState(false, NULL);
	
}




void Engine::Graphics::WaitForPreviousFrame()
{
	HRESULT hr;
	
	if (m_fence[m_frameIndex]->GetCompletedValue() < m_fenceValue[m_frameIndex])
	{		
		hr = m_fence[m_frameIndex]->SetEventOnCompletion(m_fenceValue[m_frameIndex], m_fenceEvent);
		if (FAILED(hr))
		{
			m_running = false;
		}
		
		WaitForSingleObject(m_fenceEvent, INFINITE);
	}
	
	m_fenceValue[m_frameIndex]++;
	
	m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
}




bool Engine::Graphics::InitDevice()
{
    HRESULT hr;     
    
    hr = CreateDXGIFactory1(IID_PPV_ARGS(m_dxgiFactory.GetAddressOf()));

    if(FAILED(hr))    
        return false;    

    int adapterIdx = 0;     
    
    for(; m_dxgiFactory->EnumAdapters1(adapterIdx, m_adapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; adapterIdx++)
    {
        DXGI_ADAPTER_DESC1 desc;
        m_adapter->GetDesc1(&desc);

        if(desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
        {            
                adapterIdx++; // skipping software devices
            continue;
        }

        // Level 11 and more are compatible 
        // Finding good adapter, not creating device yet
        hr = D3D12CreateDevice(m_adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr);
        if(SUCCEEDED(hr))
            break;                        
    }

    if(FAILED(hr))
        return false;
    
    hr = D3D12CreateDevice(m_adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(m_device.GetAddressOf()));

    if(FAILED(hr))
        return false;

    return true;
}




bool Engine::Graphics::InitCommandQueue()
{        
    HRESULT hr;
    D3D12_COMMAND_QUEUE_DESC commandq_Desc = {}; // default values

    hr = m_device->CreateCommandQueue(&commandq_Desc, IID_PPV_ARGS(m_commandQ.GetAddressOf())); 

    if(FAILED(hr))    
        return false;    

    return true;
}




bool Engine::Graphics::InitSwapChain()
{   
    if(pm_hwnd == nullptr)
    {
        LOG_CORE_FATAL("Window handler is NULL");
        return false;
    }

    HRESULT hr;
    
    DXGI_MODE_DESC backBufferDesc = {};
    backBufferDesc.Width = WindowData::Width;
    backBufferDesc.Height = WindowData::Height;
    backBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    
    // multisampling, 1 is off
    DXGI_SAMPLE_DESC sampleDesc = {};
    sampleDesc.Count = 1; 
    
    
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = WindowData::BufferNum; 
    swapChainDesc.BufferDesc = backBufferDesc; 
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; 
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; 
    swapChainDesc.OutputWindow = pm_hwnd; 
    swapChainDesc.SampleDesc = sampleDesc; // our multi-sampling description
    swapChainDesc.Windowed = !WindowData::Fullscreen; 
    
    ComPtr<IDXGISwapChain> tmpSwapChain;
    
    hr = m_dxgiFactory->CreateSwapChain(
        m_commandQ.Get(), 
        &swapChainDesc, 
        &tmpSwapChain
        );       

    if(FAILED(hr))
    {
        LOG_CORE_FATAL("Failed to init Swap Chain");
        return false;
    }    

    hr = tmpSwapChain.As(&m_swapChain); 
    if(FAILED(hr))
    {
        LOG_CORE_FATAL("Failed to init Swap Chain");
        return false;
    }

    
    m_frameIndex = m_swapChain->GetCurrentBackBufferIndex(); 

    return true;
}




bool Engine::Graphics::InitRTV()
{
    HRESULT hr;

    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = WindowData::BufferNum;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
													  													  
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	hr = m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(m_rtvDescriptorHeap.GetAddressOf()));

	if(FAILED(hr))
	    return false;
	
	m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	// Create a RTV for each buffer
	for (int i = 0; i < WindowData::BufferNum; i++)
	{		
		hr = m_swapChain->GetBuffer(i, IID_PPV_ARGS(m_renderTargets[i].GetAddressOf()));
		if (FAILED(hr))					
			return false;		
		
		m_device->CreateRenderTargetView(m_renderTargets[i].Get(), nullptr, rtvHandle);
		
		rtvHandle.Offset(1, m_rtvDescriptorSize);
    }

    return true;
}




bool Engine::Graphics::InitCommandList()
{
    HRESULT hr;

    for (int i = 0; i < WindowData::BufferNum; i++)
	{
		hr = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator[i]));
		if (FAILED(hr))
		{			
			return false;
		}
	}	

	// create the command list with the first allocator
	hr = m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator[m_frameIndex].Get(), NULL, IID_PPV_ARGS(&m_commandList));
	if (FAILED(hr))
	{		
		return false;
	}

    hr = m_commandList->Close();
    if(FAILED(hr))
    {
        return false;
    }

    return true;
}




bool Engine::Graphics::InitFence()
{
    HRESULT hr;
    // create the fences
	for (int i = 0; i < WindowData::BufferNum; i++)
	{
		hr = m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence[i]));
		if (FAILED(hr))
		{
			// // Running= false;
			return false;
		}
		m_fenceValue[i] = 0; // set the initial fence value to 0
	}

	// create a handle to a fence event
	m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (m_fenceEvent == nullptr)
	{
		// // Running= false;
		return false;
	}    

    return true;
}


