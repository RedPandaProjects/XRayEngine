//----------------------------------------------------
// file: StaticMesh.cpp
//----------------------------------------------------

#include "stdafx.h"
#pragma hdrstop

//#include "EditMeshVLight.h"
#include "EditMesh.h"
#include "EditObject.h"
#include "render.h"
//----------------------------------------------------
#define F_LIM (10000)
#define V_LIM (F_LIM*3)
//----------------------------------------------------
void CEditableMesh::GenerateRenderBuffers()
{
}
//----------------------------------------------------

void CEditableMesh::UnloadRenderBuffers()
{
	
}
//----------------------------------------------------

void CEditableMesh::FillRenderBuffer(IntVec& face_lst, int start_face, int num_face, const CSurface* surf, LPBYTE& src_data)
{
	LPBYTE data 		= src_data;
    u32 dwFVF 			= surf->_FVF();
	u32 dwTexCnt 		= ((dwFVF&D3DFVF_TEXCOUNT_MASK)>>D3DFVF_TEXCOUNT_SHIFT);
    for (int fl_i=start_face; fl_i<start_face+num_face; fl_i++){
        u32 f_index 	= face_lst[fl_i];
        VERIFY			(f_index<m_FaceCount);
    	st_Face& face 	= m_Faces[f_index];
        for (int k=0; k<3; k++){
            st_FaceVert& fv = face.pv[k];
            u32 norm_id = f_index*3+k;//fv.pindex;
	        VERIFY2(norm_id<m_FaceCount*3,"Normal index out of range.");
            VERIFY2(fv.pindex<(int)m_VertCount,"Point index out of range.");
            Fvector& PN = m_VertexNormals[norm_id];
			Fvector& V 	= m_Vertices[fv.pindex];
            int sz;
            if (dwFVF&D3DFVF_XYZ){
                sz=sizeof(Fvector);
                VERIFY2(fv.pindex<int(m_VertCount),"- Face index out of range.");
                CopyMemory(data,&V,sz);
                data+=sz;
            }
            if (dwFVF&D3DFVF_NORMAL){
                sz=sizeof(Fvector);
                CopyMemory(data,&PN,sz);
                data+=sz;
            }
            sz			= sizeof(Fvector2);
            int offs 	= 0;
            for (int t=0; t<(int)dwTexCnt; t++){
                VERIFY2((t+offs)<(int)m_VMRefs[fv.vmref].count,"- VMap layer index out of range");
            	st_VMapPt& vm_pt 	= m_VMRefs[fv.vmref].pts[t+offs];
                if (m_VMaps[vm_pt.vmap_index]->type!=vmtUV){
                	offs++;
                    t--;
                    continue;
                }
                VERIFY2	(vm_pt.vmap_index<int(m_VMaps.size()),"- VMap index out of range");
				st_VMap* vmap		= m_VMaps[vm_pt.vmap_index];
                VERIFY2	(vm_pt.index<vmap->size(),"- VMap point index out of range");
                CopyMemory(data,&vmap->getUV(vm_pt.index),sz); data+=sz;
//                Msg("%3.2f, %3.2f",vmap->getUV(vm_pt.index).x,vmap->getUV(vm_pt.index).y);
            }
        }
        if (surf->m_Flags.is(CSurface::sf2Sided)){
            for (int k=2; k>=0; k--){
                st_FaceVert& fv = face.pv[k];
	            Fvector& PN = m_VertexNormals[f_index*3+k];
                int sz;
                if (dwFVF&D3DFVF_XYZ){
                    sz=sizeof(Fvector);
	                VERIFY2(fv.pindex<int(m_VertCount),"- Face index out of range.");
                    CopyMemory(data,&m_Vertices[fv.pindex],sz);
                    data+=sz;
                }
                if (dwFVF&D3DFVF_NORMAL){
                    sz=sizeof(Fvector);
                    Fvector N; N.invert(PN);
                    CopyMemory(data,&N,sz);
                    data+=sz;
                }
                sz=sizeof(Fvector2);
				int offs = 0;
                for (int t=0; t<(int)dwTexCnt; t++){
	                VERIFY2((t+offs)<(int)m_VMRefs[fv.vmref].count,"- VMap layer index out of range");
                    st_VMapPt& vm_pt 	= m_VMRefs[fv.vmref].pts[t];
                    if (m_VMaps[vm_pt.vmap_index]->type!=vmtUV){
                        offs++;
                        t--;
                        continue;
                    }
	                VERIFY2(vm_pt.vmap_index<int(m_VMaps.size()),"- VMap index out of range");
                    st_VMap* vmap		= m_VMaps[vm_pt.vmap_index];
    	            VERIFY2(vm_pt.index<vmap->size(),"- VMap point index out of range");
                    CopyMemory(data,&vmap->getUV(vm_pt.index),sz); data+=sz;

//	                Msg("%3.2f, %3.2f",vmap->getUV(vm_pt.index).x,vmap->getUV(vm_pt.index).y);
                }
            }
        }
    }
}
//----------------------------------------------------
void CEditableMesh::Render(const Fmatrix& parent, CSurface* S)
{

}
//----------------------------------------------------
#define MAX_VERT_COUNT 0xFFFF
static Fvector RB[MAX_VERT_COUNT];
static int RB_cnt=0;

void CEditableMesh::RenderList(const Fmatrix& parent, u32 color, bool bEdge, IntVec& fl)
{
//
}
//----------------------------------------------------

void CEditableMesh::RenderSelection(const Fmatrix& parent, CSurface* s, u32 color)
{
  }
//----------------------------------------------------

void CEditableMesh::RenderEdge(const Fmatrix& parent, CSurface* s, u32 color)
{
   
}
//----------------------------------------------------

#define SKEL_MAX_FACE_COUNT 10000
struct svertRender
{
	Fvector		P;
	Fvector		N;
	Fvector2 	uv;
};
void CEditableMesh::RenderSkeleton(const Fmatrix&, CSurface* S)
{
    
}
//----------------------------------------------------


