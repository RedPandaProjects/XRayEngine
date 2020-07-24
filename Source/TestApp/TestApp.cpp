// TestApp.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "..\BearBundle\BearGraphics\BearGraphics.hpp"
inline void LoadShader(BearFactoryPointer<BearRHI::BearRHIShader>&Shader,const bchar*name)
{
	BearFileStream File;
	BearString Text, Error;
	BearMap<BearStringConteniar, BearStringConteniar> Defines;

	BEAR_ASSERT(File.Open(name));
	File.ToString(Text, BearEncoding::ANSI);

	BEAR_ASSERT(Shader->LoadAsText(*Text, Defines, Error, &GIncluderDefault));
}
int main()
{
	BearCore::Initialize();
	if (BearString::Exist(GetCommandLine(), "-vulkan"))
	{
		BEAR_ASSERT(BearRenderInterface::Initialize(TEXT("BearRenderVK10")));
	}
	else
	{
		if (!BearRenderInterface::Initialize(TEXT("BearRenderDX12_1")))
			if (!BearRenderInterface::Initialize(TEXT("BearRenderDX12")))
				if (!BearRenderInterface::Initialize(TEXT("BearRenderDX11")))
					BEAR_ASSERT(false);
	}
	{
		BearWindow Window(1024,768,false);
		BearFactoryPointer<BearRHI::BearRHIContext> Context;
		BearFactoryPointer<BearRHI::BearRHIViewport> Viewport;
		BearFactoryPointer<BearRHI::BearRHIShader> ShaderPixel;
		BearFactoryPointer<BearRHI::BearRHIShader> ShaderVertex;
		BearFactoryPointer<BearRHI::BearRHIRenderPass> RenderPass;
		BearFactoryPointer<BearRHI::BearRHIPipelineGraphics> Pipeline;
		BearFactoryPointer<BearRHI::BearRHIRootSignature> RootSignature;
		BearFactoryPointer<BearRHI::BearRHIVertexBuffer> VertexBuffer;

		{
			Context  = BearRenderInterface::CreateContext();
		}
		{
			BearViewportDescription Description;
			Description.Clear = true;
			Description.ClearColor = BearColor::Blue;
	
			Viewport = BearRenderInterface::CreateViewport(Window.GetWindowHandle(), Window.GetSize().x, Window.GetSize().y, false, Description);
		}
		{
			ShaderPixel = BearRenderInterface::CreatePixelShader();
			LoadShader(ShaderPixel, TEXT("test.ps.hlsl"));	
		}
		{
			ShaderVertex = BearRenderInterface::CreateVertexShader();
			LoadShader(ShaderVertex, TEXT("test.vs.hlsl"));
		}
		{
			BearRootSignatureDescription Description;
			RootSignature = BearRenderInterface::CreateRootSignature(Description);
		}
		{

			BearRenderPassDescription Description;
			Description.RenderTargets[0].Format = Viewport->GetFormat();
			RenderPass = BearRenderInterface::CreateRenderPass(Description);
		}
		{
			BearPipelineGraphicsDescription Description;
			Description.InputLayout.Elements[0] = BearInputLayoutElement("POS", VF_R32G32B32_FLOAT, 0);
			Description.Shaders.Pixel = ShaderPixel;
			Description.Shaders.Vertex = ShaderVertex;
			Description.RootSignature = RootSignature;
			Description.RenderPass = RenderPass;
			Pipeline = BearRenderInterface::CreatePipelineGraphics(Description);
		}
		{
			VertexBuffer = BearRenderInterface::CreateVertexBuffer();
			static float VertexBufferData[] =
			{
				0.0f, 0.5f, 0.5f,
				0.5f, -0.5f, 0.5f,
				-0.5f, -0.5f, 0.5f,
			};
			VertexBuffer->Create(sizeof(float) * 3, 3, false, VertexBufferData);
		}


		while (Window.Update())
		{
			BearEventWindows Event;
			while (Window.GetEvent(Event)){}
			Context->Reset();
			Context->Lock(Viewport);

			Context->SetViewportAsFrameBuffer(Viewport);
			Context->SetViewport(0, 0, 1024, 768);
			Context->SetScissor(false, 0, 0, 0, 0);
			Context->SetPipeline(Pipeline);
			Context->SetVertexBuffer(VertexBuffer);
			Context->Draw(3);

			Context->Unlock(Viewport);
			Context->Flush(Viewport, true);
		}
	}
	BearRenderInterface::Destroy();
	BearCore::Destroy();
}