#pragma once
#ifdef DEBUG
xr_string get_string( bool v );
xr_string get_string( const Fvector& v );
xr_string get_string( const Fmatrix& dop );
xr_string get_string(const Fbox &box);
xr_string dbg_object_base_dump_string( const CObject *obj );
xr_string dbg_object_poses_dump_string( const CObject *obj );
xr_string dbg_object_visual_geom_dump_string( const CObject *obj );
xr_string dbg_object_props_dump_string( const CObject *obj );
xr_string dbg_object_full_dump_string( const CObject *obj );
xr_string dbg_object_full_capped_dump_string( const CObject *obj );
#endif