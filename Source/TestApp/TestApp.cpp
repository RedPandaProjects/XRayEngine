//// TestApp.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
////
//
#include "..\XrEngine\stdafx.h"
#include "..\BearBundle\BearGraphics\BearGraphics.hpp"
///*
//inline void LoadShader(BearFactoryPointer<BearRHI::BearRHIShader>&Shader,const char*Entry,const bchar*name)
//{
//	BearFileStream File;
//	BearString Text, Error;
//	BearMap<BearStringConteniar, BearStringConteniar> Defines;
//
//	BEAR_ASSERT(File.Open(name));
//	File.ToString(Text, BearEncoding::ANSI);
//
//	BEAR_ASSERT(Shader->LoadAsText(*Text, Entry, Defines, Error, &GIncluderDefault));
//	BearString new_file;
//	new_file.append(name).append(".spr");
//	BEAR_ASSERT(File.Open(*new_file, File.M_Write));
//	File.WriteBuffer(Shader->GetPointer(), Shader->GetSize());
//}
//inline void TestRTX()
//{
//	BearWindow Window(1024, 768, false);
//
//	BearFactoryPointer<BearRHI::BearRHIShader> ShaderRaygen;
//	BearFactoryPointer<BearRHI::BearRHIShader> ShaderHit;
//	BearFactoryPointer<BearRHI::BearRHIShader> ShaderMiss;
//	BearFactoryPointer<BearRHI::BearRHIPipelineRayTracing> RayPipeline;
//	BearFactoryPointer<BearRHI::BearRHIRootSignature> RootSignature;
//	BearFactoryPointer<BearRHI::BearRHIDescriptorHeap> DescriptorHeap;
//	BearFactoryPointer<BearRHI::BearRHIRayTracingBottomLevel> BottomLevel;
//	BearFactoryPointer<BearRHI::BearRHIVertexBuffer> VertexBuffer;
//	BearFactoryPointer<BearRHI::BearRHIRayTracingTopLevel> TopLevel;
//
//	BearFactoryPointer<BearRHI::BearRHITexture2D> Texture2DUVA;
//
//
//	BearFactoryPointer<BearRHI::BearRHIRayTracingShaderTable> RayShaderTable;
//
//
//	BearFactoryPointer<BearRHI::BearRHIContext> Context;
//
//	BearFactoryPointer<BearRHI::BearRHIViewport> Viewport;
//
//
//	BearFactoryPointer<BearRHI::BearRHIShader> Output_ShaderPixel;
//	BearFactoryPointer<BearRHI::BearRHIShader> Output_ShaderVertex;
//	BearFactoryPointer<BearRHI::BearRHIRenderPass> Output_RenderPass;
//	BearFactoryPointer<BearRHI::BearRHIPipelineGraphics> Output_Pipeline;
//	BearFactoryPointer<BearRHI::BearRHIRootSignature> Output_RootSignature;
//	BearFactoryPointer<BearRHI::BearRHIVertexBuffer> Output_VertexBuffer;
//	BearFactoryPointer<BearRHI::BearRHIDescriptorHeap> Output_DescriptorHeap;
//	BearFactoryPointer<BearRHI::BearRHISampler> Output_Sampler;
//
//	{
//		Context = BearRenderInterface::CreateContext();
//	}
//	{
//		BearViewportDescription Description;
//		Description.Clear = true;
//		Description.ClearColor = BearColor::Black;
//
//		Viewport = BearRenderInterface::CreateViewport(Window.GetWindowHandle(), Window.GetSize().x, Window.GetSize().y, false, Description);
//	}
//	{
//		ShaderRaygen = BearRenderInterface::CreateRayTracingShader();
//		ShaderHit = BearRenderInterface::CreateRayTracingShader();
//		ShaderMiss = BearRenderInterface::CreateRayTracingShader();
//		LoadShader(ShaderRaygen,TEXT(""),TEXT("raygen.hlsl"));
//		LoadShader(ShaderHit, TEXT(""), TEXT("hit.hlsl"));
//		LoadShader(ShaderMiss, TEXT(""), TEXT("miss.hlsl"));
//	}
//	{
//		BearRootSignatureDescription Description;
//		Description.SRVResources[0].Shader = BearShaderType::RayTracing;
//		Description.UAVResources[0].Shader = BearShaderType::RayTracing;
//		Description.SRVResources[0].DescriptorType =BearSRVDescriptorType::AccelerationStructure;
//		RootSignature = BearRenderInterface::CreateRootSignature(Description);
//	}
//
//	{
//		BearPipelineRayTracingDescription Description;
//		Description.Shaders.resize(3);
//		Description.Shaders[0].Exports.resize(1);
//		Description.Shaders[0].Exports[0].NameExport = L"RayGen";
//		Description.Shaders[0].Exports[0].NameFunction = L"Main";
//		Description.Shaders[0].Exports[0].Type = BearRayTracingShaderType::RayGeneration;
//		Description.Shaders[0].Shader = ShaderRaygen;
//
//
//		Description.Shaders[1].Exports.resize(1);
//		Description.Shaders[1].Exports[0].NameExport = L"Miss";
//		Description.Shaders[1].Exports[0].NameFunction = L"Main";
//		Description.Shaders[1].Exports[0].Type = BearRayTracingShaderType::Miss;
//		Description.Shaders[1].Shader = ShaderMiss;
//
//
//		Description.Shaders[2].Exports.resize(1);
//		Description.Shaders[2].Exports[0].NameExport = L"ClosestHit";
//		Description.Shaders[2].Exports[0].NameFunction = L"Main";
//		Description.Shaders[2].Exports[0].Type = BearRayTracingShaderType::ClosestHit;
//		Description.Shaders[2].Shader = ShaderHit;
//
//		Description.HitGroups.resize(1);
//		Description.HitGroups[0].ClosestHitShaderImport = L"ClosestHit";
//		Description.HitGroups[0].NameExport = L"Hit";
//
//		Description.GlobalRootSignature = RootSignature;
//
//	/*	Description.LocalRootSignatures.resize(1);
//		Description.LocalRootSignatures[0].Exports.resize(3);
//		Description.LocalRootSignatures[0].Exports[0] = L"RayGen";
//		Description.LocalRootSignatures[0].Exports[1] = L"Miss";
//		Description.LocalRootSignatures[0].Exports[2] = L"ClosestHit";
//		Description.LocalRootSignatures[0].RootSignature = LocalRootSignature;*/
//		RayPipeline = BearRenderInterface::CreatePipelineRayTracing(Description);
//	}
//	{
//		VertexBuffer = BearRenderInterface::CreateVertexBuffer();
//		static float VertexBufferData[] =
//		{
//			0.0f, 0.5f, 0.5f,
//			0.5f, -0.5f, 0.5f,
//			-0.5f, -0.5f, 0.5f,
//		};
//		VertexBuffer->Create(sizeof(float) * 3, 3, false, VertexBufferData);
//	}
//	{
//		BearRayTracingBottomLevelDescription Description;
//		Description.GeometryDescriptions.resize(1);
//		Description.GeometryDescriptions[0].Triangles.VertexBuffer = VertexBuffer;
//		Description.GeometryDescriptions[0].Triangles.VertexCount = 3;
//		Description.GeometryDescriptions[0].Triangles.VertexFormat =BearVertexFormat::R32G32B32_FLOAT;
//		Description.BuildFlags = (uint32)BearAccelerationStructureBuildFlags::PreferFastTrace;
//		BottomLevel = BearRenderInterface::CreateRayTracingBottomLevel(Description);
//	}
//	{
//		BearRayTracingTopLevelDescription Description;
//		Description.InstanceDescriptions.resize(1);
//		Description.InstanceDescriptions[0].BottomLevel = BottomLevel;
//		Description.BuildFlags = (uint32)BearAccelerationStructureBuildFlags::PreferFastTrace;
//		TopLevel = BearRenderInterface::CreateRayTracingTopLevel(Description);
//	}
//	{
//		Texture2DUVA = BearRenderInterface::CreateTexture2D(1024, 768, 1, 1, BearTexturePixelFormat::R32G32B32A32F, BearTextureUsage::Storage, 0);
//	}
//	{
//		BearDescriptorHeapDescription Description;
//		//Description.UAVResources[0].Shader = BearGraphics::BearShaderType::RayGeneration;
//		Description.RootSignature = RootSignature;
//		DescriptorHeap = BearRenderInterface::CreateDescriptorHeap(Description);
//		DescriptorHeap->SetShaderResource(0, TopLevel);
//		DescriptorHeap->SetUnorderedAccess(0, Texture2DUVA);
//	}
//
//	{
//		BearRayTracingShaderTableDescription Description;
//		Description.Pipeline = RayPipeline;
//		Description.RayGenerateShader = L"RayGen";
//		Description.MissShader = L"Miss";
//		Description.HitGroups.push_back(L"Hit");
//		RayShaderTable = BearRenderInterface::CreateRayTracingShaderTable(Description);
//	}
//	{
//		{
//			Output_ShaderPixel = BearRenderInterface::CreatePixelShader();
//			LoadShader(Output_ShaderPixel, TEXT("main"), TEXT("texture.ps.hlsl"));
//		}
//		{
//			Output_ShaderVertex = BearRenderInterface::CreateVertexShader();
//			LoadShader(Output_ShaderVertex, TEXT("main"), TEXT("texture.vs.hlsl"));
//		}
//		{
//			BearRootSignatureDescription Description;
//			Description.SRVResources[0].Shader = BearShaderType::Pixel;
//			Description.Samplers[0].Shader = BearShaderType::Pixel;
//			Output_RootSignature = BearRenderInterface::CreateRootSignature(Description);
//		}
//		{
//			BearRenderPassDescription Description;
//			Description.RenderTargets[0].Format = Viewport->GetFormat();
//			Output_RenderPass = BearRenderInterface::CreateRenderPass(Description);
//		}
//		{
//			BearPipelineGraphicsDescription Description;
//			Description.InputLayout.Elements[0] = BearInputLayoutElement("POS", BearVertexFormat::R32G32B32_FLOAT, 0);
//			Description.InputLayout.Elements[1] = BearInputLayoutElement("UV", BearVertexFormat::R32G32_FLOAT, sizeof(float)*3);
//			Description.Shaders.Pixel = Output_ShaderPixel;
//			Description.Shaders.Vertex = Output_ShaderVertex;
//			Description.RootSignature = Output_RootSignature;
//			Description.RenderPass = Output_RenderPass;
//			Output_Pipeline = BearRenderInterface::CreatePipelineGraphics(Description);
//		}
//		{
//
//			BearSamplerDescription Description;
//			Output_Sampler = BearRenderInterface::CreateSampler(Description);
//		}
//		{
//			BearDescriptorHeapDescription Description;
//			Description.RootSignature = Output_RootSignature;
//			Output_DescriptorHeap = BearRenderInterface::CreateDescriptorHeap(Description);
//			Output_DescriptorHeap->SetShaderResource(0,Texture2DUVA);
//			Output_DescriptorHeap->SetSampler(0, Output_Sampler);
//		}
//		{
//			Output_VertexBuffer = BearRenderInterface::CreateVertexBuffer();
//			static float VertexBufferData[] =
//			{
//				-1, -1, 0 ,0 ,0,
//				-1 ,  1, 0 ,0 ,-1,
//				1,  1, 0, 1, -1,
//
//					-1, -1, 0 ,0 ,0,
//					1,  1, 0, 1, -1,
//				1,  -1, 0 ,1, 0,
//			};
//			Output_VertexBuffer->Create(sizeof(float) *5, 6, false, VertexBufferData);
//		}
//	}
//
//	while (Window.Update())
//	{
//		BearEventWindows Event;
//		while (Window.GetEvent(Event)) {}
//		Context->Reset();
//		Context->SetViewport(0, 0, 1024, 768);
//		Context->SetScissor(false, 0, 0, 0, 0);
//		Context->SetPipeline(RayPipeline);
//		Context->SetDescriptorHeap(DescriptorHeap);
//		Context->DispatchRays(1024,768,1,RayShaderTable);
//
//		Context->Lock(Viewport);
//		Context->Lock(Texture2DUVA.cast<BearRHI::BearRHIUnorderedAccess>());
//		Context->SetViewportAsFrameBuffer(Viewport);
//		Context->SetPipeline(Output_Pipeline);
//		Context->SetVertexBuffer(Output_VertexBuffer);
//		Context->SetDescriptorHeap(Output_DescriptorHeap);
//		Context->Draw(6);
//		Context->ClearFrameBuffer();
//		Context->Unlock(Texture2DUVA.cast<BearRHI::BearRHIUnorderedAccess>());
//
//		Context->Unlock(Viewport);
//		Context->Flush(Viewport, true);
//	}
//}
//inline void TestGraphics()
//{
//	BearWindow Window(1024, 768, false);
//	BearFactoryPointer<BearRHI::BearRHIContext> Context;
//	BearFactoryPointer<BearRHI::BearRHIViewport> Viewport;
//	BearFactoryPointer<BearRHI::BearRHIShader> ShaderPixel;
//	BearFactoryPointer<BearRHI::BearRHIShader> ShaderVertex;
//	BearFactoryPointer<BearRHI::BearRHIRenderPass> RenderPass;
//	BearFactoryPointer<BearRHI::BearRHIPipelineGraphics> Pipeline;
//	BearFactoryPointer<BearRHI::BearRHIRootSignature> RootSignature;
//	BearFactoryPointer<BearRHI::BearRHIVertexBuffer> VertexBuffer;
//	{
//		Context = BearRenderInterface::CreateContext();
//	}
//	{
//		BearViewportDescription Description;
//		Description.Clear = true;
//		Description.ClearColor = BearColor::Blue;
//		Viewport = BearRenderInterface::CreateViewport(Window.GetWindowHandle(), Window.GetSize().x, Window.GetSize().y, false, Description);
//	}
//	{
//		ShaderPixel = BearRenderInterface::CreatePixelShader();
//		LoadShader(ShaderPixel, TEXT("main"), TEXT("test.ps.hlsl"));
//	}
//	{
//		ShaderVertex = BearRenderInterface::CreateVertexShader();
//		LoadShader(ShaderVertex, TEXT("main"), TEXT("test.vs.hlsl"));
//	}
//	{
//		BearRootSignatureDescription Description;
//		RootSignature = BearRenderInterface::CreateRootSignature(Description);
//	}
//	{
//		BearRenderPassDescription Description;
//		Description.RenderTargets[0].Format = Viewport->GetFormat();
//		RenderPass = BearRenderInterface::CreateRenderPass(Description);
//	}
//	{
//		BearPipelineGraphicsDescription Description;
//		Description.InputLayout.Elements[0] = BearInputLayoutElement("POS", BearVertexFormat::R32G32B32_FLOAT, 0);
//		Description.Shaders.Pixel = ShaderPixel;
//		Description.Shaders.Vertex = ShaderVertex;
//		Description.RootSignature = RootSignature;
//		Description.RenderPass = RenderPass;
//		Pipeline = BearRenderInterface::CreatePipelineGraphics(Description);
//	}
//	{
//		VertexBuffer = BearRenderInterface::CreateVertexBuffer();
//		static float VertexBufferData[] =
//		{
//			0.0f, 0.5f, 0.5f,
//			0.5f, -0.5f, 0.5f,
//			-0.5f, -0.5f, 0.5f,
//		};
//		VertexBuffer->Create(sizeof(float) * 3, 3, false, VertexBufferData);
//	}
//
//
//	while (Window.Update())
//	{
//		BearEventWindows Event;
//		while (Window.GetEvent(Event)) {}
//		Context->Reset();
//		Context->Lock(Viewport);
//
//		Context->SetViewportAsFrameBuffer(Viewport);
//		Context->SetViewport(0, 0, 1024, 768);
//		Context->SetScissor(false, 0, 0, 0, 0);
//		Context->SetPipeline(Pipeline);
//		Context->SetVertexBuffer(VertexBuffer);
//		Context->Draw(3);
//
//		Context->Unlock(Viewport);
//		Context->Flush(Viewport, true);
//	}
//}*/
BearString GetFileName(const bchar* Name)
{
	BearString FileName = BearFileManager::GetPathFile(Name);
	FileName.append(BEAR_PATH);
	FileName.append(BearFileManager::GetFileName(Name));
	return FileName;
}
void Make(BearString& Name)
{
	if (BearString::FindWithEnd(*Name, TEXT("_ao.png") )&&BearString::CompareWithoutCase(BearString::FindWithEnd(*Name, TEXT("_ao.png")), TEXT("_ao.png")) == 0)return;
	if (BearString::FindWithEnd(*Name, TEXT("_h.png"))&&BearString::CompareWithoutCase(BearString::FindWithEnd(*Name, TEXT("_h.png")), TEXT("_h.png")) == 0)return;
	if (BearString::FindWithEnd(*Name, TEXT("_m.png")) && BearString::CompareWithoutCase(BearString::FindWithEnd(*Name, TEXT("_m.png")), TEXT("_m.png")) == 0)return;
	if (BearString::FindWithEnd(*Name, TEXT("_n.png")) && BearString::CompareWithoutCase(BearString::FindWithEnd(*Name, TEXT("_n.png")), TEXT("_n.png")) == 0)return;
	if (BearString::FindWithEnd(*Name, TEXT("_r.png")) && BearString::CompareWithoutCase(BearString::FindWithEnd(*Name, TEXT("_r.png")), TEXT("_r.png")) == 0)return;
	BearString FileName = GetFileName(*Name);

	BearString FileNameAO;
	FileNameAO.append(FileName).append(TEXT("_ao.png"));
	BearString FileNameH;
	FileNameH.append(FileName).append(TEXT("_h.png"));
	BearString FileNameM;
	FileNameM.append(FileName).append(TEXT("_m.png"));
	BearString FileNameN;
	FileNameN.append(FileName).append(TEXT("_n.png"));
	BearString FileNameR;
	FileNameR.append(FileName).append(TEXT("_r.png"));

	if (!BearFileManager::FileExists(*FileNameAO))
		return;
	if (!BearFileManager::FileExists(*FileNameH))
		return;
	if (!BearFileManager::FileExists(*FileNameM))
		return;
	if (!BearFileManager::FileExists(*FileNameN))
		return;
	if (!BearFileManager::FileExists(*FileNameR))
		return;

	BEAR_PRINTF("Make %s\n\r", *Name);
	BearImage Base;BEAR_ASSERT(Base.LoadFromFile(*Name));
	BearImage BaseN; BEAR_ASSERT(BaseN.LoadFromFile(*FileNameN));
	BearImage BaseH; BEAR_ASSERT(BaseH.LoadFromFile(*FileNameH));
	BearImage BaseAO; BEAR_ASSERT(BaseAO.LoadFromFile(*FileNameAO));
	BearImage BaseR; BEAR_ASSERT(BaseR.LoadFromFile(*FileNameR));
	BearImage BaseM; BEAR_ASSERT(BaseM.LoadFromFile(*FileNameM));


	FileName.append(".dds");
	if (!BearFileManager::FileExists(*FileName))
	{
		BEAR_PRINTF("Save %s\n\r", *FileName);
		Base.GenerateMipmap();
		Base.Convert(BearTexturePixelFormat::BC3);
		Base.SaveToDds(*FileName);
	}
	BearImage BaseAORM;
	BaseAORM.Create(Base.GetSize().x, Base.GetSize().y, 1, 1);
	BearImage BaseNormalHeight;
	BaseNormalHeight.Create(Base.GetSize().x, Base.GetSize().y, 1, 1);
	for (bsize x = 0; x < Base.GetSize().x; x++)
	{
		for (bsize y  = 0; y < Base.GetSize().y; y++)
		{
			{
				BearColor Pixel;
				Pixel.SetAsUint8(BaseAO.GetPixel(x, y).R8U, BaseR.GetPixel(x, y).R8U, BaseM.GetPixel(x, y).R8U);
				BaseAORM.SetPixel(Pixel, x, y);
			}
			{
				BearColor Pixel;
				Pixel.SetAsUint8(BaseN.GetPixel(x, y).R8U, BaseN.GetPixel(x, y).G8U, BaseN.GetPixel(x, y).B8U, BaseH.GetPixel(x, y).R8U);
				BaseNormalHeight.SetPixel(Pixel, x, y);
			}
		}
	}
	FileName = GetFileName(*Name);
	FileName.append("_AORM.dds");
	if (!BearFileManager::FileExists(*FileName))
	{
		BEAR_PRINTF("Save %s\n\r", *FileName);
		BaseAORM.GenerateMipmap();
		BaseAORM.Convert(BearTexturePixelFormat::BC3);

		BaseAORM.SaveToDds(*FileName);
	}


	FileName = GetFileName(*Name);
	FileName.append("_NH.dds");
	if (!BearFileManager::FileExists(*FileName))
	{
		BaseNormalHeight.GenerateMipmap();
		BaseNormalHeight.Convert(BearTexturePixelFormat::BC3);
		BEAR_PRINTF("Save %s\n\r", *FileName);
		BaseNormalHeight.SaveToDds(*FileName);
	}

}
int main()
{
	BearCore::Initialize();
	{
		/*BearStringPath WorkPath;
		BearFileManager::GetWorkPath(WorkPath);
		BearStringPath File1="";
		BearStringPath File2 = "";
		BearStringPath File3 = "";
		BearStringPath File4 = "";
		BearStringPath File5 = "";
		BearStringPath File6 = "";
		BearFileManager::PathCombine(File1, WorkPath, "1.png");
		BearFileManager::PathCombine(File2, WorkPath, "2.png");
		BearFileManager::PathCombine(File3, WorkPath, "3.png");
		BearFileManager::PathCombine(File4, WorkPath, "4.png");
		BearFileManager::PathCombine(File5, WorkPath, "5.png");
		BearFileManager::PathCombine(File6, WorkPath, "6.png");

		BearImage CubeMap;
		CubeMap.Create(512, 512, 1, 1, BearTexturePixelFormat::R8G8B8A8, true);
		CubeMap.LoadFromFile(0, File1);
		CubeMap.LoadFromFile(1, File2);
		CubeMap.LoadFromFile(2, File3);
		CubeMap.LoadFromFile(3, File4);
		CubeMap.LoadFromFile(4, File5);
		CubeMap.LoadFromFile(5, File6);
		CubeMap.GenerateMipmap();
		CubeMap.Convert(BearTexturePixelFormat::BC3);
		CubeMap.SaveToDds("Cube.dds");*/
		BearVector<BearString> Files;
		BearStringPath WorkPath;
		BearFileManager::GetWorkPath(WorkPath);

		BearFileManager::FindFiles(Files, WorkPath, TEXT("*.png"));
		for (BearString& Name : Files)
		{
			Make(Name);
		}

	}
	/*if (BearString::Exist(GetCommandLine(), "-vulkan"))
	{
		BEAR_ASSERT(BearRenderInterface::Initialize(TEXT("BearRenderVK11")));
	}
	else
	{
		if (!BearRenderInterface::Initialize(TEXT("BearRenderDX12_1")))
			if (!BearRenderInterface::Initialize(TEXT("BearRenderDX12")))
				if (!BearRenderInterface::Initialize(TEXT("BearRenderDX11")))
					BEAR_ASSERT(false);
	}
	{
		if (BearRenderInterface::RTXSupport())
		{
			TestRTX();
		}
		else
		{
			TestGraphics();
		}
		
		//
	}
	BearRenderInterface::Destroy();*/
	BearCore::Destroy();
}