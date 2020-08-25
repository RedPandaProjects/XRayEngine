
REGISTER(D3DFVF_RESERVED0, 0x001, 0, "",BearVertexFormat::None)
REGISTER(D3DFVF_POSITION_MASK, 0x400E, 0, "",BearVertexFormat::None)
REGISTER(D3DFVF_XYZ, 0x002, sizeof(float) * 3, "position_xyz",BearVertexFormat::R32G32B32_FLOAT)
REGISTER(D3DFVF_XYZRHW, 0x004, sizeof(float) * 4, "position_xyzrhw",BearVertexFormat::R32G32B32A32_FLOAT)
REGISTER(D3DFVF_XYZB1, 0x006, 0, "",BearVertexFormat::None)
REGISTER(D3DFVF_XYZB2, 0x008, 0, "",BearVertexFormat::None)
REGISTER(D3DFVF_XYZB3, 0x00a, 0, "",BearVertexFormat::None)
REGISTER(D3DFVF_XYZB4, 0x00c, 0, "",BearVertexFormat::None)
REGISTER(D3DFVF_XYZB5, 0x00e, 0, "",BearVertexFormat::None)
REGISTER(D3DFVF_XYZW, 0x4002, sizeof(float) * 4, "position_xyzw",BearVertexFormat::R32G32B32A32_FLOAT)

REGISTER(D3DFVF_NORMAL, 0x010, sizeof(float) * 3, "normal",BearVertexFormat::R32G32B32_FLOAT)
REGISTER(D3DFVF_PSIZE, 0x020, sizeof(float), "",BearVertexFormat::None)
REGISTER(D3DFVF_DIFFUSE, 0x040, sizeof(int), "diffuse",BearVertexFormat::R8G8B8A8)
REGISTER(D3DFVF_SPECULAR, 0x080, sizeof(int), "specular",BearVertexFormat::R8G8B8A8)

REGISTER(D3DFVF_TEXCOUNT_MASK, 0xf00, 0, "",BearVertexFormat::None)
REGISTER(D3DFVF_TEXCOUNT_SHIFT, 8, 0, "",BearVertexFormat::None)
//REGISTER(D3DFVF_TEX0            ,0x000, sizeof(float)*2, "uv0",BearVertexFormat::R32G32_FLOAT)
REGISTER(D3DFVF_TEX1, 0x100, sizeof(float) * 2, "uv1t",BearVertexFormat::R32G32_FLOAT)
REGISTER(D3DFVF_TEX2, 0x200, sizeof(float) * 2, "uv2t",BearVertexFormat::R32G32_FLOAT)
REGISTER(D3DFVF_TEX3, 0x300, sizeof(float) * 2, "uv3t",BearVertexFormat::R32G32_FLOAT)
REGISTER(D3DFVF_TEX4, 0x400, sizeof(float) * 2, "uv4t",BearVertexFormat::R32G32_FLOAT)
REGISTER(D3DFVF_TEX5, 0x500, sizeof(float) * 2, "uv5t",BearVertexFormat::R32G32_FLOAT)
REGISTER(D3DFVF_TEX6, 0x600, sizeof(float) * 2, "uv6t",BearVertexFormat::R32G32_FLOAT)
REGISTER(D3DFVF_TEX7, 0x700, sizeof(float) * 2, "uv7t",BearVertexFormat::R32G32_FLOAT)
REGISTER(D3DFVF_TEX8, 0x800, sizeof(float) * 2, "uv8t",BearVertexFormat::R32G32_FLOAT)

REGISTER(D3DFVF_LASTBETA_UBYTE4, 0x1000, 0, "",BearVertexFormat::None)
REGISTER(D3DFVF_LASTBETA_D3DCOLOR, 0x8000, 0, "",BearVertexFormat::None)

REGISTER(D3DFVF_RESERVED2, 0x6000, 0, "",BearVertexFormat::None) // 2 reserved bits
REGISTER(D3DFVF_COUNT, 28, 0, "",BearVertexFormat::None) // 2 reserved bits