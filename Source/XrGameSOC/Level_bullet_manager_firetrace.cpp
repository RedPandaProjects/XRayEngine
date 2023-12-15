// Level_Bullet_Manager.cpp:	��� ����������� ������ ���� �� ����������
//								��� ���� � ������� ���������� ����
//								(��� �������� ������������ � �� ������������)
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Level_Bullet_Manager.h"
#include "entity.h"
#include "../XrEngine/gamemtllib.h"
#include "level.h"
#include "gamepersistent.h"
#include "game_cl_base.h"
#include "xrmessages.h"
#include "clsid_game.h"
#include "../XrEngine/Render/KinematicsAnimated.h"
#include "../XrEngine/Render/Kinematics.h"
#include "Actor.h"
#include "AI/Stalker/ai_stalker.h"
#include "character_info.h"
#include "game_cl_base_weapon_usage_statistic.h"
#include "../XrCDB/xr_collide_defs.h"
#include "../XrEngine/xr_collide_form.h"
#include "weapon.h"

//��������� shoot_factor, ������������ 
//��������� ���� ��� ������������ � ��������
#define RICOCHET_THRESHOLD		0.1
#define STUCK_THRESHOLD			0.4

//���������� �� �������� �������� ���� �� ������� ���� ��� �� ������
#define PARENT_IGNORE_DIST		3.f
extern float gCheckHitK;

//test callback ������� 
//  object - object for testing
//return TRUE-����������� ������ / FALSE-���������� ������
BOOL CBulletManager::test_callback(const collide::ray_defs& rd, CObject* object, LPVOID params)
{
	bullet_test_callback_data* pData	= (bullet_test_callback_data*)params;
	SBullet* bullet = pData->pBullet;

	if( (object->ID() == bullet->parent_id)		&&  
		(bullet->fly_dist<PARENT_IGNORE_DIST)	&&
		(!bullet->flags.ricochet_was))			return FALSE;

	BOOL bRes						= TRUE;
	if (object){
		CEntity*	entity			= smart_cast<CEntity*>(object);
		if (entity&&entity->g_Alive()&&(entity->ID()!=bullet->parent_id)){
			ICollisionForm*	cform	= entity->collidable.model;
			if ((NULL!=cform) && (cftObject==cform->Type())){
				CActor* actor		= smart_cast<CActor*>(entity);
				CAI_Stalker* stalker= smart_cast<CAI_Stalker*>(entity);
				// � ���� ������?
				if (actor && IsGameTypeSingle()/**/||stalker/**/){
					// ������ � ������ ��� ��������
					Fsphere S		= cform->getSphere();
					entity->XFORM().transform_tiny	(S.P)	;
					float dist		= rd.range;
					// �������� ������ �� �� � ����������� ����� 
					if (Fsphere::rpNone!=S.intersect_full(bullet->pos, bullet->dir, dist)){
						// �� ������, ������ ��� �������
						bool play_whine				= true;
						CObject* initiator			= Level().Objects.net_Find	(bullet->parent_id);
						if (actor){
							// ������ � ������
							float hpf				= 1.f;
							float ahp				= actor->HitProbability();
#if 1
#	if 0
							CObject					*weapon_object = Level().Objects.net_Find	(bullet->weapon_id);
							if (weapon_object) {
								CWeapon				*weapon = smart_cast<CWeapon*>(weapon_object);
								if (weapon) {
									float fly_dist		= bullet->fly_dist+dist;
									float dist_factor	= _min(1.f,fly_dist/Level().BulletManager().m_fHPMaxDist);
									ahp					= dist_factor*weapon->hit_probability() + (1.f-dist_factor)*1.f;
								}
							}
#	else
							float					game_difficulty_hit_probability = actor->HitProbability();
							CAI_Stalker				*stalker = smart_cast<CAI_Stalker*>(initiator);
							if (stalker)
								hpf					= stalker->SpecificCharacter().hit_probability_factor();

							float					dist_factor = 1.f;
							CObject					*weapon_object = Level().Objects.net_Find	(bullet->weapon_id);
							if (weapon_object) {
								CWeapon				*weapon = smart_cast<CWeapon*>(weapon_object);
								if (weapon) {
									game_difficulty_hit_probability = weapon->hit_probability();
									float fly_dist	= bullet->fly_dist+dist;
									dist_factor		= _min(1.f,fly_dist/Level().BulletManager().m_fHPMaxDist);
								}
							}

							ahp						= dist_factor*game_difficulty_hit_probability + (1.f-dist_factor)*1.f;
#	endif
#else
							CAI_Stalker* i_stalker	= smart_cast<CAI_Stalker*>(initiator);
							// ���� ������� �������, ��������� - hit_probability_factor �������a ����� - 1.0
							if (i_stalker) {
								hpf					= i_stalker->SpecificCharacter().hit_probability_factor();
								float fly_dist		= bullet->fly_dist+dist;
								float dist_factor	= _min(1.f,fly_dist/Level().BulletManager().m_fHPMaxDist);
								ahp					= dist_factor*actor->HitProbability() + (1.f-dist_factor)*1.f;
							}
#endif
							if (Random.randF(0.f,1.f)>(ahp*hpf)){ 
								bRes				= FALSE;	// don't hit actor
								play_whine			= true;		// play whine sound
							}else{
								// real test actor CFORM
								Level().BulletManager().m_rq_results.r_clear();

								if (cform->_RayQuery(rd,Level().BulletManager().m_rq_results)){
									bRes			= TRUE;		// hit actor
									play_whine		= false;	// don't play whine sound
								}else{
									bRes			= FALSE;	// don't hit actor
									play_whine		= true;		// play whine sound
								}
							}
						}
						// play whine sound
						if (play_whine){
							Fvector					pt;
							pt.mad					(bullet->pos, bullet->dir, dist);
							Level().BulletManager().PlayWhineSound				(bullet,initiator,pt);
						}
					}else{
						// don't test this object again (return FALSE)
						bRes		= FALSE;
					}

				}
			}
		}
	}
	
	return bRes;
}
//callback ������� 
//	result.O;		// 0-static else CObject*
//	result.range;	// range from start to element 
//	result.element;	// if (O) "num tri" else "num bone"
//	params;			// user defined abstract data
//	Device->Statistic.TEST0.End();
//return TRUE-���������� ����������� / FALSE-��������� �����������
BOOL  CBulletManager::firetrace_callback(collide::rq_result& result, LPVOID params)
{
	bullet_test_callback_data* pData	= (bullet_test_callback_data*)params;
	pData->bStopTracing					= true;
	SBullet* bullet						= pData->pBullet;

	//��������� ����� ���������
	Fvector end_point;
	end_point.mad(bullet->pos, bullet->dir, result.range);

	u16 hit_material_idx = GAMEMTL_NONE_IDX;

	//������������ ������
	if(result.O){
		//�������� �������� � �� ��������
		IKinematics* V = 0;
		//���� �� ������ �� �������� �� ������ ��
		//�����, �� ������������ ���, ��� ��� ��� ��
		//� �������
		VERIFY( !(result.O->ID() == bullet->parent_id &&  bullet->fly_dist<PARENT_IGNORE_DIST) );
		if (0!=(V=CastToIKinematics(result.O->Visual()))){
			const IBoneData& B = V->GetBoneData((u16)result.element);
			hit_material_idx = B.get_game_mtl_idx();
			Level().BulletManager().RegisterEvent(EVENT_HIT, TRUE,bullet, end_point, result, hit_material_idx);
		}
	} else {
		//����������� ������
		//�������� ����������� � ������ ��� ��������
		CDB::TRI* T			= Level().ObjectSpace.GetStaticTris()+result.element;
		hit_material_idx	= T->material;
		

		SGameMtl* mtl = GameMaterialLibrary->GetMaterialByIdx(hit_material_idx);
		if( fsimilar(mtl->fShootFactor,1.0f,EPS) )//���� �������� ��������� ���������������
		{
			pData->bStopTracing		= false;
		}else
			Level().BulletManager().RegisterEvent(EVENT_HIT, FALSE,bullet, end_point, result, hit_material_idx);

	}

	//��������� ���������� �� ���� ����, ����� ��������� ������
	if(bullet->speed<m_fMinBulletSpeed || bullet->flags.ricochet_was)
		return FALSE;
	else
		return TRUE;
}

void CBulletManager::FireShotmark (SBullet* bullet, const Fvector& vDir, const Fvector &vEnd, collide::rq_result& R, u16 target_material, const Fvector& vNormal, bool ShowMark)
{
	SGameMtlPair* mtl_pair	= GameMaterialLibrary->GetMaterialPair(bullet->bullet_material_idx, target_material);
	Fvector particle_dir;

	if (R.O)
	{
		particle_dir		 = vDir;
		particle_dir.invert	();

		//�� ������� ������ ������� �� ������
		if(Level().CurrentEntity() && Level().CurrentEntity()->ID() == R.O->ID()) return;

		/*ui_shader* pWallmarkShader = (!mtl_pair || mtl_pair->CollideMarks.empty()) ?
						NULL:&mtl_pair->CollideMarks[::Random.randI(0,mtl_pair->CollideMarks.size())];;*/

		if (mtl_pair && !mtl_pair->m_pCollideMarks->empty()&& ShowMark)
		{
			//�������� ������� �� ���������
			Fvector p;
			p.mad(bullet->pos,bullet->dir,R.range-0.01f);
			::Render->add_SkeletonWallmark	(&R.O->renderable.xform, 
							PKinematics(R.O->Visual()), &*mtl_pair->m_pCollideMarks,
							p, bullet->dir, bullet->wallmark_size);
		}		
	} 
	else 
	{
		//��������� ������� � ���������� �����������
		particle_dir		= vNormal;
		Fvector*	pVerts	= Level().ObjectSpace.GetStaticVerts();
		CDB::TRI*	pTri	= Level().ObjectSpace.GetStaticTris()+R.element;

		/*ui_shader* pWallmarkShader = (!mtl_pair || mtl_pair->CollideMarks.empty()) ?
										NULL:&mtl_pair->CollideMarks[::Random.randI(0,mtl_pair->CollideMarks.size())];;*/

		if (mtl_pair && !mtl_pair->m_pCollideMarks->empty() && ShowMark)
		{
			//�������� ������� �� ���������
			::Render->add_StaticWallmark	(&*mtl_pair->m_pCollideMarks, vEnd, bullet->wallmark_size, pTri, pVerts);
		}
	}

	ref_sound* pSound = (!mtl_pair || mtl_pair->CollideSounds.empty())?
						NULL:&mtl_pair->CollideSounds[::Random.randI(0,mtl_pair->CollideSounds.size())];

	//��������� ����
	if(pSound && ShowMark)
	{
		CObject* O			= Level().Objects.net_Find(bullet->parent_id );
		bullet->m_mtl_snd	= *pSound;
		bullet->m_mtl_snd.play_at_pos(O, vEnd, 0);
	}

	LPCSTR ps_name = (!mtl_pair || mtl_pair->CollideParticles.empty())?
NULL:*mtl_pair->CollideParticles[::Random.randI(0,mtl_pair->CollideParticles.size())];

	SGameMtl*	tgt_mtl = GameMaterialLibrary->GetMaterialByIdx(target_material);
	BOOL bStatic = !tgt_mtl->Flags.test(SGameMtl::flDynamic);

	if( (ps_name && ShowMark) || (bullet->flags.explosive && bStatic) )
	{
		Fmatrix pos;
		pos.k.normalize(particle_dir);
		Fvector::generate_orthonormal_basis(pos.k, pos.j, pos.i);
		pos.c.set(vEnd);
		if(ps_name && ShowMark){
			//�������� �������� ��������� � ��������
			CParticlesObject* ps = CParticlesObject::Create(ps_name,TRUE);

			ps->UpdateParent(pos,zero_vel);
			GamePersistent().ps_needtoplay.push_back(ps);
		}

		if(bullet->flags.explosive&&bStatic)
			PlayExplodePS(pos);
	}
}

void CBulletManager::StaticObjectHit	(CBulletManager::_event& E)
{
//	Fvector hit_normal;
	FireShotmark(&E.bullet, E.bullet.dir,	E.point, E.R, E.tgt_material, E.normal);
//	ObjectHit	(&E.bullet,					E.point, E.R, E.tgt_material, hit_normal);
}

static bool g_clear = false;
void CBulletManager::DynamicObjectHit	(CBulletManager::_event& E)
{
	//������ ��� ������������ ��������
	VERIFY(E.R.O);
	if (g_clear) E.Repeated = false;
	if (GameID() == GAME_SINGLE) E.Repeated = false;
	bool NeedShootmark = true;//!E.Repeated;
	
	if (E.R.O->CLS_ID == CLSID_OBJECT_ACTOR)
	{
		game_PlayerState* ps = Game().GetPlayerByGameID(E.R.O->ID());
		if (ps && ps->testFlag(GAME_PLAYER_FLAG_INVINCIBLE))
		{
			NeedShootmark = false;
		};
	}
	
	//���������� ����������� ��������� �� �������
//	Fvector			hit_normal;
	FireShotmark	(&E.bullet, E.bullet.dir, E.point, E.R, E.tgt_material, E.normal, NeedShootmark);
	
	Fvector original_dir = E.bullet.dir;
	float power, impulse;
	std::pair<float,float> hit_result = E.result; //ObjectHit(&E.bullet, E.end_point, E.R, E.tgt_material, hit_normal);
	power = hit_result.first;
	impulse = hit_result.second;

	// object-space
	//��������� ���������� ���������
	Fvector				p_in_object_space,position_in_bone_space;
	Fmatrix				m_inv;
	m_inv.invert		(E.R.O->XFORM());
	m_inv.transform_tiny(p_in_object_space, E.point);

	// bone-space
	IKinematics* V = CastToIKinematics(E.R.O->Visual());

	if(V)
	{
		VERIFY3(V->LL_GetBoneVisible(u16(E.R.element)),*E.R.O->cNameVisual(),V->LL_BoneName_dbg(u16(E.R.element)));
		Fmatrix m_bone = (V->LL_GetBoneInstance(u16(E.R.element))).GetTransform();
		Fmatrix  m_inv_bone;
		m_inv_bone.invert(m_bone);
		(V->LL_GetBoneInstance(u16(E.R.element))).SetTransform(m_bone);
		m_inv_bone.transform_tiny(position_in_bone_space, p_in_object_space);
	}
	else
	{
		position_in_bone_space.set(p_in_object_space);
	}

	//��������� ��� ����������� �������
	if (E.bullet.flags.allow_sendhit && !E.Repeated)
	{
		//-------------------------------------------------
		bool AddStatistic = false;
		if (GameID() != GAME_SINGLE && E.bullet.flags.allow_sendhit && E.R.O->CLS_ID == CLSID_OBJECT_ACTOR
			&& Game().m_WeaponUsageStatistic->CollectData())
		{
			CActor* pActor = smart_cast<CActor*>(E.R.O);
			if (pActor)// && pActor->g_Alive())
			{
				Game().m_WeaponUsageStatistic->OnBullet_Hit(&E.bullet, E.R.O->ID(), (s16)E.R.element, E.point);
				AddStatistic = true;
			};
		};
/*		
		NET_Packet		P;
//		CGameObject::u_EventGen	(P,(AddStatistic)? GE_HIT_STATISTIC : GE_HIT,E.R.O->ID());
		P.w_u16			(E.bullet.parent_id);
		P.w_u16			(E.bullet.weapon_id);
		P.w_dir			(original_dir);
		P.w_float		(power);
		P.w_s16			((s16)E.R.element);
		P.w_vec3		(position_in_bone_space);
		P.w_float		(impulse);
		P.w_u16			(u16(E.bullet.hit_type));
		if (E.bullet.hit_type == ALife::eHitTypeFireWound)
			P.w_float	(E.bullet.ap);

		if (AddStatistic)
			P.w_u32(E.bullet.m_dwID);

		CGameObject::u_EventSend (P);
*/

		SHit	Hit = SHit(	power, 
							original_dir, 
							NULL, 
							u16(E.R.element), 
							position_in_bone_space, 
							impulse, 
							E.bullet.hit_type,
							E.bullet.ap,
							E.bullet.flags.aim_bullet);

		Hit.GenHeader(u16((AddStatistic)? GE_HIT_STATISTIC : GE_HIT)&0xffff, E.R.O->ID());
		Hit.whoID			= E.bullet.parent_id;
		Hit.weaponID		= E.bullet.weapon_id;
		Hit.BulletID		= E.bullet.m_dwID;

		NET_Packet			np;
		Hit.Write_Packet	(np);
		
//		Msg("Hit sended: %d[%d,%d]", Hit.whoID, Hit.weaponID, Hit.BulletID);
		CGameObject::u_EventSend(np);
	}
}

#ifdef DEBUG
FvectorVec g_hit[3];
#endif

extern void random_dir	(Fvector& tgt_dir, const Fvector& src_dir, float dispersion);

std::pair<float, float>  CBulletManager::ObjectHit	(SBullet* bullet, const Fvector& end_point, 
									collide::rq_result& R, u16 target_material, 
									Fvector& hit_normal)
{
	//----------- normal - start
	if (R.O){
		//������� ������� �� ������� ������ ��������
		CCF_Skeleton* skeleton = smart_cast<CCF_Skeleton*>(R.O->CFORM());
		if(skeleton){
			Fvector			e_center;
			hit_normal.set	(0,0,0);
			if (skeleton->_ElementCenter((u16)R.element,e_center))
				hit_normal.sub							(end_point, e_center);
			float len		= hit_normal.square_magnitude();
			if (!fis_zero(len))	hit_normal.div		(_sqrt(len));
			else				hit_normal.invert	(bullet->dir);
		}
	} else {
		//��������� ������� � �����������
		Fvector*	pVerts	= Level().ObjectSpace.GetStaticVerts();
		CDB::TRI*	pTri	= Level().ObjectSpace.GetStaticTris()+R.element;
		hit_normal.mknormal	(pVerts[pTri->verts[0]],pVerts[pTri->verts[1]],pVerts[pTri->verts[2]]);
	}		
	//----------- normal - end
	float old_speed, energy_lost;
	old_speed = bullet->speed;

	//����������� ���������� ���� � �������� ��������
	float speed_factor = bullet->speed/bullet->max_speed;
	//�������� ���� ���� �������� � ������ �������
	float power = bullet->hit_power*speed_factor;
	
	SGameMtl* mtl = GameMaterialLibrary->GetMaterialByIdx(target_material);

	//shoot_factor: ����������� ����������� �� ������� �������� ���� 
	//(���� ������ 1, �� ���� ���� ���������(���� ������� ��� �� �����������), ���� �������� � ������� 
	//�������, ���� ������ 1, �� ���� ��������� ������)
	float shoot_factor = mtl->fShootFactor * bullet->pierce*speed_factor;

	float impulse	= 0.f;

#ifdef DEBUG
	Fvector dbg_bullet_pos;
	dbg_bullet_pos.mad(bullet->pos,bullet->dir,R.range);
	int bullet_state		= 0;
#endif

	if (fsimilar(mtl->fShootFactor,1.0f,EPS))//���� �������� ��������� ���������������, ��
	{
		#ifdef DEBUG
		bullet_state = 2;
		#endif	
		return std::make_pair(power, impulse);
	}

	//�������
	Fvector			new_dir;
	new_dir.reflect	(bullet->dir,hit_normal);
	Fvector			tgt_dir;
	random_dir		(tgt_dir, new_dir, deg2rad(10.f));

	float ricoshet_factor	= bullet->dir.dotproduct(tgt_dir);

	float f			= Random.randF	(0.5f,1.f);
	//float f				= Random.randF	(0.0f,0.3);
//	if(shoot_factor<RICOCHET_THRESHOLD &&  )
	if (((f+shoot_factor)<ricoshet_factor) && bullet->flags.allow_ricochet)	{
		//���������� �������� ������ � ����������� 
		//�� ���� ������� ���� (��� ������ ����, ��� ������ ������)
		float scale = 1.f -_abs(bullet->dir.dotproduct(hit_normal))*m_fCollisionEnergyMin;
		clamp(scale, 0.f, m_fCollisionEnergyMax);

		//���������� ��������, �������� ������� ������,
		//�.�. ���� �������� � ����� ������������
		//� ����� ������� �� RayQuery()
		bullet->dir.set	(tgt_dir)		;
		bullet->pos		= end_point		;
		bullet->flags.ricochet_was = 1	;

		//��������� �������� � ����������� �� �����������������
		bullet->speed *= (1-mtl->fShootFactor)*scale;
		//������� ������� � ��������� �������� ���� ��� ������������
		float energy_lost = 1.f - bullet->speed/old_speed;
		//������� ���������� ������� ����� ������������������� ���������� �������
		impulse = bullet->hit_impulse*speed_factor*energy_lost;

		#ifdef DEBUG
		bullet_state = 0;
		#endif		
	} else if(shoot_factor <  1.0) {
		//����������� ���� � ���������
		bullet->speed  = 0.f;
		//�������� ���� ������� �������
		impulse = bullet->hit_impulse*speed_factor;
		#ifdef DEBUG
		bullet_state = 1;
		#endif		
	} else {
		//���������� ���������
		//��������� �������� ��������������� ����������� ��������
		//float speed_lost = fis_zero(bullet->hit_impulse) ?	1.f : 		1.f - impulse/bullet->hit_impulse;
		//clamp (speed_lost, 0.f , 1.f);
		//float speed_lost = shoot_factor;
		
		bullet->speed *=mtl->fShootFactor;
		energy_lost = 1.f - bullet->speed/old_speed;
		impulse = bullet->hit_impulse*speed_factor*energy_lost;
		
		bullet->pos.mad(bullet->pos,bullet->dir,EPS);//fake
		//������ ����������� ����������� ��� ��������������
		Fvector rand_normal;
		rand_normal.random_dir(bullet->dir, deg2rad(5.f)*energy_lost, Random);
		bullet->dir.set(rand_normal);
		#ifdef DEBUG
		bullet_state = 2;
		#endif		
	}
#ifdef DEBUG
	extern BOOL g_bDrawBulletHit;
	if(g_bDrawBulletHit)
		g_hit[bullet_state].push_back(dbg_bullet_pos);
#endif 

	return std::make_pair(power, impulse);
}
