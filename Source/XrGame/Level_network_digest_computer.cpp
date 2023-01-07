#include "stdafx.h"
#include "Level.h"

void CLevel::SendClientDigestToServer()
{
#if 0
	string128 tmp_digest;
	NET_Packet P;
	P.w_begin			(M_SV_DIGEST);
	m_client_digest	=	ComputeClientDigest(tmp_digest);
	P.w_stringZ			(m_client_digest);
	SecureSend			(P, net_flags(TRUE, TRUE, TRUE, TRUE));
#endif
}