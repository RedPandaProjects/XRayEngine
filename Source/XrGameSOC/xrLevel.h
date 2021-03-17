#pragma once
#include "..\XrEngine\xrLevel.h"
#pragma pack(push,1)
#pragma pack(1)
#ifndef _EDITOR
class NodePosition {
	u8	data[5];

	ICF	void xz(u32 value) { CopyMemory(data, &value, 3); }
	ICF	void y(u16 value) { CopyMemory(data + 3, &value, 2); }
public:
	ICF	u32	xz() const {
		return			((*((u32*)data)) & 0x00ffffff);
	}
	ICF	u32	x(u32 row) const {
		return			(xz() / row);
	}
	ICF	u32	z(u32 row) const {
		return			(xz() % row);
	}
	ICF	u32	y() const {
		return			(*((u16*)(data + 3)));
	}

	friend class	CLevelGraph;
	friend struct	CNodePositionCompressor;
	friend struct	CNodePositionConverter;
};

struct NodeCompressed {
public:
	u8				data[12];
private:

	ICF	void link(u8 link_index, u32 value)
	{
		value &= 0x007fffff;
		switch (link_index) {
		case 0: {
			value |= (*(u32*)data) & 0xff800000;
			CopyMemory(data, &value, sizeof(u32));
			break;
		}
		case 1: {
			value <<= 7;
			value |= (*(u32*)(data + 2)) & 0xc000007f;
			CopyMemory(data + 2, &value, sizeof(u32));
			break;
		}
		case 2: {
			value <<= 6;
			value |= (*(u32*)(data + 5)) & 0xe000003f;
			CopyMemory(data + 5, &value, sizeof(u32));
			break;
		}
		case 3: {
			value <<= 5;
			value |= (*(u32*)(data + 8)) & 0xf000001f;
			CopyMemory(data + 8, &value, sizeof(u32));
			break;
		}
		}
	}

	ICF	void light(u8 value)
	{
		data[11] &= 0x0f;
		data[11] |= value << 4;
	}

public:
	u16				cover0 : 4;
	u16				cover1 : 4;
	u16				cover2 : 4;
	u16				cover3 : 4;
	u16				plane;
	NodePosition	p;
	// 4 + 4 + 4 + 4 + 16 + 40 + 96 = 168 bits = 21 byte

	ICF	u32	link(u8 index) const
	{
		switch (index) {
		case 0:	return	((*(u32*)data) & 0x007fffff);
		case 1:	return	(((*(u32*)(data + 2)) >> 7) & 0x007fffff);
		case 2:	return	(((*(u32*)(data + 5)) >> 6) & 0x007fffff);
		case 3:	return	(((*(u32*)(data + 8)) >> 5) & 0x007fffff);
		default:	NODEFAULT;
		}
#ifdef DEBUG
		return			(0);
#endif
	}

	ICF	u8	light() const
	{
		return			(data[11] >> 4);
	}

	ICF	u16	cover(u8 index) const
	{
		switch (index) {
		case 0: return(cover0);
		case 1: return(cover1);
		case 2: return(cover2);
		case 3: return(cover3);
		default: NODEFAULT;
		}
#ifdef DEBUG
		return				(u8(-1));
#endif
	}

	friend class	CLevelGraph;
	friend struct	CNodeCompressed;
	friend class	CNodeRenumberer;
	friend class	CRenumbererConverter;
};									// 2+2+5+12 = 21b
#endif

#ifdef PRIQUEL
const u32 XRAI_CURRENT_VERSION = 9;
#else // PRIQUEL
const u32 XRAI_CURRENT_VERSION = 8;
#endif // PRIQUEL