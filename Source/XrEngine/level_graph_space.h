////////////////////////////////////////////////////////////////////////////
//	Module 		: level_graph_space.h
//	Created 	: 02.10.2001
//  Modified 	: 08.12.2004
//	Author		: Dmitriy Iassenev
//	Description : Level graph space
////////////////////////////////////////////////////////////////////////////

#pragma once

namespace LevelGraph
{
	class CHeader :
#ifndef MASTER_GOLD
		public
#else
		private
#endif
		hdrNODES 
	{
	private:
		friend class CRenumbererConverter;

	public:
		ICF	u32				version					() const;
		ICF	u32				vertex_count			() const;
		ICF	float			cell_size				() const;
		ICF	float			factor_y				() const;
		ICF	const Fbox		&box					() const;
		ICF const xrGUID	&guid					() const;
	};

	typedef NodePosition	CPosition;

	class CVertex : 
		
#ifndef MASTER_GOLD
		public
#else
		private
#endif
		NodeCompressed 
	{
	private:
		friend class CRenumbererConverter;

	public:
		ICF	void			link(u8 link_index, u32 value) { NodeCompressed::link(link_index, value); }
		ICF	u32				link					(int i) const;
		ICF	u16				high_cover				(u8 index) const;
		ICF	u16				low_cover				(u8 index) const;
		ICF	u16				plane					() const;
		ICF	const CPosition &position				() const;
		ICF	bool			operator<				(const LevelGraph::CVertex &vertex) const;
		ICF	bool			operator>				(const LevelGraph::CVertex &vertex) const;
		ICF	bool			operator==				(const LevelGraph::CVertex &vertex) const;
		friend class ILevelGraph;
	};

	struct SSegment 
	{
		Fvector v1;
		Fvector v2;
	};

	struct SContour : public SSegment
	{
		Fvector v3;
		Fvector v4;
	};
};
