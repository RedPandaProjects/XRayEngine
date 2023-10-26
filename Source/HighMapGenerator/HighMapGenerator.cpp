#include "../XrCore/xrCore.h"
#include "../Editors/XrECore/stdafx.h"
#include "../Editors/XrECore/Editor/EditObject.h"
#include "../Editors/XrECore/Editor/Library.h"
#include "RedImageTool/RedImage.hpp"
class XRayMemory:public XRayMemoryInterface
{
public:
	u32 mem_usage() override
	{
		return 0;
	}
	void* mem_alloc(size_t size) override
	{
		return malloc(size);
	}
	void* mem_realloc(void* p, size_t size) override
	{
		return realloc(p,size);
	}
	void mem_free(void* p) override
	{
		free(p);
	}
};
class XRayLog:public XRayLogInterface
{
public:
	void Log(const char* Text) override
	{
		printf_s("%s\r\n",Text);
	}
};

class XRayDebug:public XRayDebugInterface
{
public:
	void CriticalError(const char* Text) override
	{
		exit(-1);
	}
};

class InputParser{
    public:
        InputParser (int &argc, char **argv){
            for (int i=1; i < argc; ++i)
                this->tokens.push_back(std::string(argv[i]));
        }
        /// @author iain
        const std::string& getCmdOption(const std::string &option) const{
            std::vector<std::string>::const_iterator itr;
            itr =  std::find(this->tokens.begin(), this->tokens.end(), option);
            if (itr != this->tokens.end() && ++itr != this->tokens.end()){
                return *itr;
            }
            static const std::string empty_string("");
            return empty_string;
        }
        /// @author iain
        bool cmdOptionExists(const std::string &option) const{
            return std::find(this->tokens.begin(), this->tokens.end(), option)
                   != this->tokens.end();
        }
    private:
        std::vector <std::string> tokens;
};

int main(int argc, char** argv)
{
	XRayLog Log;
	XRayMemory Memory;
	XRayDebug Debug;
    Core.Initialize(&Memory,&Log,&Debug,nullptr,true);
	InputParser input(argc, argv);
	if(input.cmdOptionExists("-h")||!input.cmdOptionExists("-f")||(input.cmdOptionExists("-f")&&input.getCmdOption("-f").size()==0)||(input.cmdOptionExists("-s")&&input.getCmdOption("-s").size()==0))
	{
        printf("Using: HighMapGenerator.exe [-s 1024] -f escape_terrain.object\r\n");
        printf("Parameters: -s is texture out size, default 1024 \r\n");
        printf("            -f is in object file \r\n");
		exit(0);
	}
	XRC = xr_new<xrXRC>();
	GXRayObjectLibrary = xr_new<XRayObjectLibrary>();
	size_t TextureSizeX = 1024;
	size_t TextureSizeY = 1024;
	
    Msg("Startup...");
	std::string Size = input.getCmdOption("-s");
	if(Size.size())
	{
		TextureSizeX = TextureSizeY = atoll(Size.c_str());
	}
	Msg("Out texture size [%d]x[%d]",TextureSizeX,TextureSizeY);
	if(TextureSizeX==0||TextureSizeY==0||TextureSizeX>8192||TextureSizeY>8192)
	{
		Msg("Error invalid out texture size!");
		exit(-1);
	}

	size_t MaxTextureSize = TextureSizeY>TextureSizeY?TextureSizeY:TextureSizeX;
	RedImageTool::RedImage TestImage(TextureSizeX,TextureSizeY);
    {
		
		std::string File = input.getCmdOption("-f");
	    CEditableObject TerrainObject("Terrain");
		
		 Msg("Load:%s",File.c_str());
        if(TerrainObject.Load(File.c_str()))
        {
			Fbox TerrainBound = TerrainObject.GetBox();
			Fvector TerrainSize;
			TerrainBound.getsize(TerrainSize);
			float StepX = 0,StepY = 0;

			Fvector ResultScale;
			if(TerrainSize.x>TerrainSize.z)
			{
				StepX = TerrainSize.x/static_cast<float>(MaxTextureSize);
				StepY = TerrainSize.x/static_cast<float>(MaxTextureSize);
			}
			else
			{
				StepX = TerrainSize.z/static_cast<float>(MaxTextureSize);
				StepY = TerrainSize.z/static_cast<float>(MaxTextureSize);
			}
			ResultScale.set(TerrainSize.x,TerrainSize.z,TerrainSize.y/5.12f);
			std::vector<uint16_t> RawData;
			RawData.resize(TextureSizeX*TextureSizeY);
			
			Msg("Start generate!");
			for(size_t x=0;x<TextureSizeX;x++)
			{
				for(size_t z=0;z<TextureSizeY;z++)
				{
					Fvector StartPoint,Direction;
					StartPoint.set(static_cast<float>(x)*StepX + TerrainBound.x1,TerrainBound.y2,static_cast<float>(z)*StepY + TerrainBound.z1);
					Direction.set(0,-1,0);
					float dist = FLT_MAX;
					if(TerrainObject.RayPick(dist,StartPoint,Direction,Fidentity))
					{
						RawData[TextureSizeX*z + (TextureSizeX-x-1)] = static_cast<uint16_t>((1.f - clampr((dist / TerrainSize.y), 0.f, 1.f)) * 65535.f);
						TestImage.SetPixel(RedImageTool::RedColor((1.f - clampr((dist / TerrainSize.y), 0.f, 1.f)),(1.f - clampr((dist / TerrainSize.y), 0.f, 1.f)),(1.f - clampr((dist / TerrainSize.y), 0.f, 1.f))),x,z);
					}
					else
					{
						TestImage.SetPixel(RedImageTool::RedColor(0,0,0,1),x,z);
						RawData[TextureSizeX*z + (TextureSizeX-x-1)] = 0;
					}
				}
			}
			Msg("Completed!");
			Msg("Result scale [%f,%f,%f]", ResultScale.x/static_cast<float>(TextureSizeX)*100,ResultScale.y/static_cast<float>(TextureSizeY)*100,ResultScale.z);
			string2048 OutR16FileName,OutPNGFileName;

			xr_strcpy(OutR16FileName,File.c_str());
			if(strext(OutR16FileName))
			{
				strext(OutR16FileName)[0] = 0;
			}
			xr_strcpy(OutPNGFileName,OutR16FileName);
			xr_strcat(OutPNGFileName,".png");
			xr_strcat(OutR16FileName,".r16");
			
			Msg("Save %s",OutPNGFileName);
			Msg("Save %s",OutR16FileName);
			TestImage.SaveToPng(OutPNGFileName);
			IWriter* F 		= FS.w_open			(OutR16FileName);
			if(F)
			{
				F->w(RawData.data(),RawData.size()*2);
				FS.w_close(F);
			}
		


        }
		else
		{
			Msg("Error can't open file %s",File.c_str());
			exit(-1);
		}
    }
	xr_delete(GXRayObjectLibrary);
	xr_delete(XRC);
    Core.Destroy();
}
