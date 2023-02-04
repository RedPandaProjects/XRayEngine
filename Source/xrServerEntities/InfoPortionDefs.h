#pragma once
#if defined(GAME_SOC)||defined(GAME_CS)
#include "..\xrEngine\alife_space.h"
#include "../XrEngine/object_interfaces.h"

struct INFO_DATA : public IPureSerializeObject<IReader, IWriter>
{
	INFO_DATA() :info_id(NULL), receive_time(0) {};
	INFO_DATA(shared_str id, ALife::_TIME_ID time) :info_id(id), receive_time(time) {};

	virtual void		load(IReader& stream);
	virtual void		save(IWriter&);

	shared_str			info_id;
	//����� ��������� ����� ������ ����������
	ALife::_TIME_ID		receive_time;
};

DEFINE_VECTOR(INFO_DATA, KNOWN_INFO_VECTOR, KNOWN_INFO_VECTOR_IT);
#else
typedef shared_str INFO_DATA;
DEFINE_VECTOR(INFO_DATA, KNOWN_INFO_VECTOR, KNOWN_INFO_VECTOR_IT);

class CFindByIDPred
{
public:
	CFindByIDPred(shared_str element_to_find) {element = element_to_find;}
	IC bool operator () (const INFO_DATA& data) const {return data == element;}
private:
	shared_str element;
};
#endif