#pragma once
template <>
 struct _MM_ALIGN16 _matrix<float>
{
public:
	typedef float			TYPE;
	typedef _matrix<float>	Self;
	typedef Self& SelfRef;
	typedef const Self& SelfCRef;
	typedef _vector3<float>	Tvector;
public:
	_MM_ALIGN16 __m128 R[4];
	_matrix() {}
	_matrix(float a1,float a2,float a3,float a4,
		float b1, float b2, float b3, float b4,
		float c1, float c2, float c3, float c4,
		float d1, float d2, float d3, float d4
	)
	{
		R[0] = set_ps(a1, a2, a3, a4);
		R[1] = set_ps(b1, b2, b3, b4);
		R[2] = set_ps(c1, c2, c3, c4);
		R[3] = set_ps(d1, d2, d3, d4);
	}
	_matrix(const float*Matrix) 
	{
		R[0] = set_ps(Matrix[0], Matrix[1], Matrix[2], Matrix[3]);
		R[1] = set_ps(Matrix[0+4], Matrix[1+4], Matrix[2+4], Matrix[3+4]);
		R[2] = set_ps(Matrix[0+8], Matrix[1 + 8], Matrix[2 + 8], Matrix[3 + 8]);
		R[3] = set_ps(Matrix[0 + 12], Matrix[1 + 12], Matrix[2 + 12], Matrix[3 + 12]);
	}
	_matrix(const Self& right) { copy(right); }
	IC void set_row(size_t Row,const Fvector4&V)
	{
		set_row(Row,V.x,V.y,V.z,V.w);
	}
	IC void set_row(size_t Row, float x, float y, float z, float w)
	{
		VERIFY(Row < 4);
		R[Row] = set_ps(x, y, z, w);
	}
	IC void set_row(size_t Row, const Fvector3& V)
	{
		set_row(Row, V.x, V.y, V.z);
	}
	IC void set_row(size_t Row, float x, float y, float z)
	{
		VERIFY(Row < 4);
		R[Row] = _mm_mul_ps(R[Row], set_ps(0, 0, 0,1));
		R[Row] = _mm_add_ps(R[Row], set_ps(x, y, z, 0));
	}
	IC void set_i(const Fvector3& V)
	{
		set_row(0, V);
	}
	IC void set_j(const Fvector3& V)
	{
		set_row(1, V);
	}
	IC void set_k(const Fvector3& V)
	{
		set_row(2, V);
	}
	IC void set_c(const Fvector3& V)
	{
		set_row(3, V);
	}
	IC Fvector4 get_row(size_t Row) const
	{
		VERIFY(Row < 4);
		_MM_ALIGN16 float RowData[4];
		_mm_store_ps(RowData, R[Row]);
		return Fvector4().set(RowData[0], RowData[1], RowData[2], RowData[3]);
	}
	 
	IC Fvector got_row_as_vector3(size_t Row) const
	{
		VERIFY(Row < 4);
		_MM_ALIGN16 float RowData[4];
		_mm_store_ps(RowData, R[Row]);
		return Fvector().set(RowData[0], RowData[1], RowData[2]);
	}
	IC Fvector3 get_i()const 
	{
		return got_row_as_vector3(0);
	}
	IC Fvector3 get_j() const
	{
		return got_row_as_vector3(1);
	}
	IC Fvector3 get_k() const
	{
		return got_row_as_vector3(2);
	}
	IC Fvector3 get_c() const
	{
		return got_row_as_vector3(3);
	}
	IC void append_row(size_t Row, const Fvector4& V)
	{
		append_row(Row, V.x, V.y, V.z, V.w);
	}
	IC void append_row(size_t Row, const Fvector& V)
	{
		append_row(Row, V.x, V.y, V.z, 0);
	}
	IC void append_row(size_t Row, float x,float y,float z, float w)
	{
		VERIFY(Row < 4);
		R[Row] = _mm_add_ps(R[Row], set_ps(x, y, z, w));
	}
	IC void mul_row(size_t Row, float x, float y, float z, float w)
	{
		VERIFY(Row < 4);
		R[Row] = _mm_mul_ps(R[Row], set_ps(x, y, z, w));
	}
	IC float get_det() const
	{
		_MM_ALIGN16 __m128 R1 = swizzle<1, 0, 0, 0>(R[1]);
		_MM_ALIGN16 __m128 R2 = swizzle<2, 2, 1, 1>(R[2]);

		_MM_ALIGN16 __m128 R3 = swizzle<1, 0, 0, 0>(R[2]);
		_MM_ALIGN16 __m128 R4 = swizzle<2, 2, 1, 1>(R[1]);

		_MM_ALIGN16 __m128 R5 = _mm_sub_ps(_mm_mul_ps(R1, R2), _mm_mul_ps(R3, R4));
		R5 = _mm_mul_ps(R[0], R5);
		_MM_ALIGN16 float Result[4];
		_mm_store_ps(Result, R5);
		return Result[0] - Result[1] + Result[2];

	}
	IC Self& operator=(const Self& right)
	{
		copy(right);
		return *this;
	}
	IC Self& operator=(const float* Matrix)
	{
		R[0] = set_ps(Matrix[0], Matrix[1], Matrix[2], Matrix[3]);
		R[1] = set_ps(Matrix[0 + 4], Matrix[1 + 4], Matrix[2 + 4], Matrix[3 + 4]);
		R[2] = set_ps(Matrix[0 + 8], Matrix[1 + 8], Matrix[2 + 8], Matrix[3 + 8]);
		R[3] = set_ps(Matrix[0 + 12], Matrix[1 + 12], Matrix[2 + 12], Matrix[3 + 12]);
		return *this;
	}
	IC void copy(const Self& right)
	{
		R[0] = right.R[0];
		R[1] = right.R[1];
		R[2] = right.R[2];
		R[3] = right.R[3];
	}
	// Class members
	IC	SelfRef	set(const Self& a)
	{
		R[0] = a.R[0];
		R[1] = a.R[1];
		R[2] = a.R[2];
		R[3] = a.R[3];
		return *this;
	}
	IC	SelfRef	set(const Tvector& r, const Tvector& N, const Tvector& D, const Tvector& C)
	{
		R[0] = set_ps(r.x, r.y, r.z, 0);
		R[1] = set_ps(N.x, N.y, N.z, 0);
		R[2] = set_ps(D.x, D.y, D.z, 0);
		R[3] = set_ps(C.x, C.y, C.z, 1);
		return *this;
	}
	IC	SelfRef	identity(void)
	{
		R[0] = set_ps(1, 0, 0, 0);
		R[1] = set_ps(0, 1, 0, 0);
		R[2] = set_ps(0, 0, 1, 0);
		R[3] = set_ps(0, 0, 0, 1);
		return *this;
	}

	IC	SelfRef	rotation(const _quaternion<float>& Q);
	IC	SelfRef	mk_xform(const _quaternion<float>& Q, const Tvector& V);

	// Multiply RES = A[4x4]*B[4x4] (WITH projection)
	IC	SelfRef	mul(const Self& A, const Self& B)
	{
		VERIFY((this != &A) && (this != &B));
	
		R[0] = _mm_mul_ps(A.R[0], swizzle<0, 0, 0, 0>(B.R[0]));
		R[0] = _mm_add_ps(R[0], _mm_mul_ps(A.R[1], swizzle<1, 1, 1, 1>(B.R[0])));
		R[0] = _mm_add_ps(R[0], _mm_mul_ps(A.R[2], swizzle<2, 2, 2, 2>(B.R[0])));
		R[0] = _mm_add_ps(R[0], _mm_mul_ps(A.R[3], swizzle<3, 3, 3, 3>(B.R[0])));

		R[1] = _mm_mul_ps(A.R[0], swizzle<0, 0, 0, 0>(B.R[1]));
		R[1] = _mm_add_ps(R[1], _mm_mul_ps(A.R[1], swizzle<1, 1, 1, 1>(B.R[1])));
		R[1] = _mm_add_ps(R[1], _mm_mul_ps(A.R[2], swizzle<2, 2, 2, 2>(B.R[1])));
		R[1] = _mm_add_ps(R[1], _mm_mul_ps(A.R[3], swizzle<3, 3,3, 3>(B.R[1])));

		R[2] = _mm_mul_ps(A.R[0], swizzle<0, 0, 0, 0>(B.R[2]));
		R[2] = _mm_add_ps(R[2], _mm_mul_ps(A.R[1], swizzle<1, 1, 1, 1>(B.R[2])));
		R[2] = _mm_add_ps(R[2], _mm_mul_ps(A.R[2], swizzle<2, 2, 2, 2>(B.R[2])));
		R[2] = _mm_add_ps(R[2], _mm_mul_ps(A.R[3], swizzle<3, 3, 3, 3>(B.R[2])));

		R[3] = _mm_mul_ps(A.R[0], swizzle<0, 0, 0, 0>(B.R[3]));
		R[3] = _mm_add_ps(R[3], _mm_mul_ps(A.R[1], swizzle<1, 1, 1, 1>(B.R[3])));
		R[3] = _mm_add_ps(R[3], _mm_mul_ps(A.R[2], swizzle<2, 2, 2, 2>(B.R[3])));
		R[3] = _mm_add_ps(R[3], _mm_mul_ps(A.R[3], swizzle<3, 3, 3, 3>(B.R[3])));

		return *this;
	}

	// Multiply RES = A[4x3]*B[4x3] (no projection), faster than ordinary multiply
	IC	SelfRef	mul_43(const Self& A, const Self& B)
	{
		VERIFY((this != &A) && (this != &B));

		_MM_ALIGN16 __m128 RMask = set_ps(1, 1, 1, 0);

		R[0] = _mm_mul_ps(A.R[0], swizzle<0, 0, 0, 0>(B.R[0]));
		R[0] = _mm_add_ps(R[0], _mm_mul_ps(A.R[1], swizzle<1, 1, 1, 1>(B.R[0])));
		R[0] = _mm_add_ps(R[0], _mm_mul_ps(A.R[2], swizzle<2, 2, 2, 2>(B.R[0])));
		R[0] = _mm_mul_ps(R[0], RMask);

		R[1] = _mm_mul_ps(A.R[0], swizzle<0, 0, 0, 0>(B.R[1]));
		R[1] = _mm_add_ps(R[1], _mm_mul_ps(A.R[1], swizzle<1, 1, 1, 1>(B.R[1])));
		R[1] = _mm_add_ps(R[1], _mm_mul_ps(A.R[2], swizzle<2, 2, 2, 2>(B.R[1])));
		R[1] = _mm_mul_ps(R[1], RMask);

		R[2] = _mm_mul_ps(A.R[0], swizzle<0, 0, 0, 0>(B.R[2]));
		R[2] = _mm_add_ps(R[2], _mm_mul_ps(A.R[1], swizzle<1, 1, 1, 1>(B.R[2])));
		R[2] = _mm_add_ps(R[2], _mm_mul_ps(A.R[2], swizzle<2, 2, 2, 2>(B.R[2])));
		R[2] = _mm_mul_ps(R[2], RMask);

		R[3] = _mm_mul_ps(A.R[0], swizzle<0, 0, 0, 0>(B.R[3]));
		R[3] = _mm_add_ps(R[3], _mm_mul_ps(A.R[1], swizzle<1, 1, 1, 1>(B.R[3])));
		R[3] = _mm_add_ps(R[3], _mm_mul_ps(A.R[2], swizzle<2, 2, 2, 2>(B.R[3])));
		R[3] = _mm_add_ps(R[3], A.R[3]);
		R[3] = _mm_mul_ps(R[3], RMask);
		R[3] = _mm_add_ps(R[3], set_ps(0, 0, 0, 1));

		return *this;
	}
	IC	SelfRef	mulA_44(const Self& A)			// mul after 
	{
		Self B; B.set(*this); 	mul(A, B);
		return *this;
	};
	IC	SelfRef	mulB_44(const Self& B)			// mul before
	{
		Self A; A.set(*this); 	mul(A, B);
		return *this;
	};
	IC	SelfRef	mulA_43(const Self& A)			// mul after (no projection)
	{
		Self B; B.set(*this); 	mul_43(A, B);
		return *this;
	};
	IC	SelfRef	mulB_43(const Self& B)			// mul before (no projection)
	{
		Self A; A.set(*this); 	mul_43(A, B);
		return *this;
	};
	IC	SelfRef	invert(const Self& a)
	{		
		
		_MM_ALIGN16 __m128 R1 = swizzle<1, 0, 0, 0>(a.R[1]);
		_MM_ALIGN16 __m128 R2 = swizzle<2, 2, 1, 1>(a.R[2]);

		_MM_ALIGN16 __m128 R3 = swizzle<1, 0, 0, 0>(a.R[2]);
		_MM_ALIGN16 __m128 R4 = swizzle<2, 2, 1, 1>(a.R[1]);

		_MM_ALIGN16 __m128 R5 = _mm_sub_ps(_mm_mul_ps(R1, R2), _mm_mul_ps(R3, R4));
		R5 = _mm_mul_ps(a.R[0], R5);
#ifdef _DEBUG
		{
			_MM_ALIGN16 float Result[4];
			_mm_store_ps(Result, R5);
			float fDetInv = Result[0] - Result[1] + Result[2];
			VERIFY(_abs(fDetInv) > flt_zero);
		}
#endif 

		R1 = _mm_add_ps(_mm_sub_ps(swizzle<0, 0, 0, 0>(R5), swizzle<1, 1, 1, 1>(R5)), swizzle<2, 2, 2, 2>(R5));
		_MM_ALIGN16 __m128 R7 = _mm_rcp_ps(R1);
		/*
		_11 = fDetInv * (a._22 * a._33 - a._23 * a._32);
		_12 = -fDetInv * (a._12 * a._33 - a._13 * a._32);
		_13 = fDetInv * (a._12 * a._23 - a._13 * a._22);
		_14 = 0.0f;

		_21 = -fDetInv * (a._21 * a._33 - a._23 * a._31);
		_22 = fDetInv * (a._11 * a._33 - a._13 * a._31);
		_23 = -fDetInv * (a._11 * a._23 - a._13 * a._21);
		_24 = 0.0f;

		_31 = fDetInv * (a._21 * a._32 - a._22 * a._31);
		_32 = -fDetInv * (a._11 * a._32 - a._12 * a._31);
		_33 = fDetInv * (a._11 * a._22 - a._12 * a._21);
		_34 = 0.0f;

		_41 = -(a._41 * _11 + a._42 * _21 + a._43 * _31);
		_42 = -(a._41 * _12 + a._42 * _22 + a._43 * _32);
		_43 = -(a._41 * _13 + a._42 * _23 + a._43 * _33);
		_44 = 1.0f;*/
		/*
		R1
		[1][0] 
		[0][0] 
		[0][0]
		R2
		[1][1] 
		[0][1] 
		[0][1]
		R3
		[1][2] 
		[0][2] 
		[0][2]
		R4
		[2][0]
		[2][0]
		[1][0]
		R5
		[2][1]
		[2][1]
		[1][1]
		R6
		[2][2]
		[2][2]
		[1][2] 	
		*/
		/*
		[0][0]->[1][0] 
		[0][1]->[0][0] 
		[1][0]->[0][0] 
		[1][1]->[0][0] 
		*/
		R1 = _mm_movelh_ps(a.R[0], a.R[1]);
		R1 = swizzle<2, 0, 0, 0>(R1);
		/*
		[0][0]->[1][1] 
		[0][1]->[0][1] 
		[1][0]->[0][1] 
		[1][1]->[0][1] 
		*/
		R2 = _mm_movelh_ps(a.R[0], a.R[1]);
		R2 = swizzle<3, 1, 1, 1>(R2);
		/*
		[1][2]->[1][2]
		[1][3]->[0][2]
		[0][2]->[0][2]
		[0][3]->[0][2]
		*/
		R3 = _mm_movehl_ps(a.R[0], a.R[1]);
		R3 = swizzle<0, 2, 2, 2>(R3);
		/*
		[1][0]->[2][0]
		[1][1]->[2][0]
		[2][0]->[1][0]
		[2][1]->[1][0]
		*/
		R4 = _mm_movelh_ps(a.R[1], a.R[2]);
		R4 = swizzle<2, 2, 0, 0>(R4);

		/*
		[1][0]->[2][1]
		[1][1]->[2][1]
		[2][0]->[1][1]
		[2][1]->[1][1]
		*/
		R5 = _mm_movelh_ps(a.R[1], a.R[2]);
		R5 = swizzle<3, 3, 1, 1>(R5);
		/*
		[2][2]->[2][2]
		[2][3]->[2][2]
		[1][2]->[1][2]
		[1][3]->[1][2]
		*/
		_MM_ALIGN16 __m128 R6 = _mm_movehl_ps(a.R[1], a.R[2]);
		R6 = swizzle<0, 0, 2, 2>(R6);
		/*
		R2*R6-R3*R5
		M[0][0] = fDetInv * (a.M[1][1] * a.M[2][2] - a.M[1][2] * a.M[2][1]);
		M[0][1] = -fDetInv *(a.M[0][1] * a.M[2][2] - a.M[0][2] * a.M[2][1]);
		M[0][2] = fDetInv * (a.M[0][1] * a.M[1][2] - a.M[0][2] * a.M[1][1]);
		_14 = 0.0f;

		R1*R6-R3*R4
		M[1][0] = -fDetInv * (a.M[1][0] * a.M[2][2] - a.M[1][2] * a.M[2][0]);
		M[1][1] =  fDetInv * (a.M[0][0] * a.M[2][2] - a.M[0][2] * a.M[2][0]);
		M[1][2] = -fDetInv * (a.M[0][0] * a.M[1][2] - a.M[0][2] * a.M[1][0]);
		_24 = 0.0f;
		R1*R5-R2*R4
		M[2][0] = fDetInv * (a.M[1][0] * a.M[2][1] - a.M[1][1] * a.M[2][0]);
		M[2][1] = -fDetInv * (a.M[0][0] * a.M[2][1] - a.M[0][1] * a.M[2][0]);
		M[2][2] = fDetInv * (a.M[0][0] * a.M[1][1] - a.M[0][1] * a.M[1][0]);
		_34 = 0.0f;
		*/
		R[0] = _mm_mul_ps(_mm_mul_ps(_mm_sub_ps(_mm_mul_ps(R2, R6), _mm_mul_ps(R3, R5)),R7), set_ps(1, -1, 1, 0));
		R[1] = _mm_mul_ps(_mm_mul_ps(_mm_sub_ps(_mm_mul_ps(R1, R6), _mm_mul_ps(R3, R4)), R7), set_ps(-1, 1, -1, 0));
		R[2] = _mm_mul_ps(_mm_mul_ps(_mm_sub_ps(_mm_mul_ps(R1, R5), _mm_mul_ps(R2, R4)), R7), set_ps(1, -1, 1, 0));
		R[3] = _mm_mul_ps(swizzle<0, 0, 0, 0>(a.R[3]), R[0]);
		R[3] = _mm_add_ps(R[3], _mm_mul_ps(swizzle<1, 1, 1, 1>(a.R[3]), R[1]));
		R[3] = _mm_add_ps(R[3], _mm_mul_ps(swizzle<2, 2, 2, 2>(a.R[3]), R[2]));
		R[3] = _mm_mul_ps(R[3], set_ps(-1, -1, -1, 0));
		R[3] = _mm_add_ps(R[3], set_ps(0, 0, 0, 1));
		
		return *this;
	}

	IC	bool	invert_b(const Self& a) 
	{	
		_MM_ALIGN16 __m128 R1 = swizzle<1, 0, 0, 0>(a.R[1]);
		_MM_ALIGN16 __m128 R2 = swizzle<2, 2, 1, 1>(a.R[2]);

		_MM_ALIGN16 __m128 R3 = swizzle<1, 0, 0, 0>(a.R[2]);
		_MM_ALIGN16 __m128 R4 = swizzle<2, 2, 1, 1>(a.R[1]);

		_MM_ALIGN16 __m128 R5 = _mm_sub_ps(_mm_mul_ps(R1, R2), _mm_mul_ps(R3, R4));
		R5 = _mm_mul_ps(a.R[0], R5);

		R1 = _mm_add_ps(_mm_sub_ps(swizzle<0, 0, 0, 0>(R5), swizzle<1, 1, 1, 1>(R5)), swizzle<2, 2, 2, 2>(R5));

		float fDetInv;
		_mm_store_ps1(&fDetInv, R1);
		if (_abs(fDetInv) <= flt_zero)	return	false;

		_MM_ALIGN16 __m128 R7 = _mm_rcp_ps(R1);
		/*
		[0][0]->[1][0]
		[0][1]->[0][0]
		[1][0]->[0][0]
		[1][1]->[0][0]
		*/
		R1 = _mm_movelh_ps(a.R[0], a.R[1]);
		R1 = swizzle<2, 0, 0, 0>(R1);
		/*
		[0][0]->[1][1]
		[0][1]->[0][1]
		[1][0]->[0][1]
		[1][1]->[0][1]
		*/
		R2 = _mm_movelh_ps(a.R[0], a.R[1]);
		R2 = swizzle<3, 1, 1, 1>(R2);
		/*
		[1][2]->[1][2]
		[1][3]->[0][2]
		[0][2]->[0][2]
		[0][3]->[0][2]
		*/
		R3 = _mm_movehl_ps(a.R[0], a.R[1]);
		R3 = swizzle<0, 2, 2, 2>(R3);
		/*
		[1][0]->[2][0]
		[1][1]->[2][0]
		[2][0]->[1][0]
		[2][1]->[1][0]
		*/
		R4 = _mm_movelh_ps(a.R[1], a.R[2]);
		R4 = swizzle<2, 2, 0, 0>(R4);

		/*
		[1][0]->[2][1]
		[1][1]->[2][1]
		[2][0]->[1][1]
		[2][1]->[1][1]
		*/
		R5 = _mm_movelh_ps(a.R[1], a.R[2]);
		R5 = swizzle<3, 3, 1, 1>(R5);
		/*
		[2][2]->[2][2]
		[2][3]->[2][2]
		[1][2]->[1][2]
		[1][3]->[1][2]
		*/
		_MM_ALIGN16 __m128 R6 = _mm_movehl_ps(a.R[1], a.R[2]);
		R6 = swizzle<0, 0, 2, 2>(R6);
		R[0] = _mm_mul_ps(_mm_mul_ps(_mm_sub_ps(_mm_mul_ps(R2, R6), _mm_mul_ps(R3, R5)), R7), set_ps(1, -1, 1, 0));
		R[1] = _mm_mul_ps(_mm_mul_ps(_mm_sub_ps(_mm_mul_ps(R1, R6), _mm_mul_ps(R3, R4)), R7), set_ps(-1, 1, -1, 0));
		R[2] = _mm_mul_ps(_mm_mul_ps(_mm_sub_ps(_mm_mul_ps(R1, R5), _mm_mul_ps(R2, R4)), R7), set_ps(1, -1, 1, 0));
		R[3] = _mm_mul_ps(swizzle<0, 0, 0, 0>(a.R[3]), R[0]);
		R[3] = _mm_add_ps(R[3], _mm_mul_ps(swizzle<1, 1, 1, 1>(a.R[3]), R[1]));
		R[3] = _mm_add_ps(R[3], _mm_mul_ps(swizzle<2, 2, 2, 2>(a.R[3]), R[2]));
		R[3] = _mm_mul_ps(R[3], set_ps(-1, -1, -1, 0));
		R[3] = _mm_add_ps(R[3], set_ps(0, 0, 0, 1));
		return true;
	}

	IC	SelfRef	invert()					// slower than invert other matrix
	{
		Self a;	a.set(*this);	invert(a);
		return *this;
	}
	IC	SelfRef	transpose(const Self& matSource)	// faster version of transpose
	{
		copy(matSource);
		_MM_TRANSPOSE4_PS(R[0], R[1], R[2], R[3]);
		return *this;
	}
	IC	SelfRef	transpose()						// self transpose - slower
	{
		_MM_TRANSPOSE4_PS(R[0], R[1], R[2], R[3]);
		return *this;
	}
	IC	SelfRef	translate(const Tvector& Loc)		// setup translation matrix
	{
		R[0] = set_ps(1, 0, 0, 0);
		R[1] = set_ps(0, 1, 0, 0);
		R[2] = set_ps(0, 0, 1, 0);
		R[3] = set_ps(Loc.x, Loc.y, Loc.z, 1);
		return *this;
	}
	IC	SelfRef	translate(float _x, float _y, float _z) // setup translation matrix
	{
		R[0] = set_ps(1, 0, 0, 0);
		R[1] = set_ps(0, 1, 0, 0);
		R[2] = set_ps(0, 0, 1, 0);
		R[3] = set_ps(_x, _y, _z, 1);
		return *this;
	}
	IC	SelfRef	translate_over(const Tvector& Loc)	// modify only translation
	{
		R[3] = set_ps(Loc.x, Loc.y, Loc.z, R[3].m128_f32[3]);
		return *this;
	}
	IC	SelfRef	translate_over(float _x, float _y, float _z) // modify only translation
	{
		R[3] = set_ps(_x, _y, _z, R[3].m128_f32[3]);
		return *this;
	}
	IC	SelfRef	translate_add(const Tvector& Loc)	// combine translation
	{
		R[3] = _mm_add_ps(R[3], set_ps(Loc.x, Loc.y, Loc.z, 0));
		return *this;
	}
	IC	SelfRef	scale(float x, float y, float z)	// setup scale matrix
	{
		R[0] = set_ps(x, 0, 0, 0);
		R[1] = set_ps(0, y, 0, 0);
		R[2] = set_ps(0, 0, z, 0);
		R[3] = set_ps(0, 0, 0, 1);
		return *this;
	}
	IC	SelfRef	scale(const Tvector& v)			// setup scale matrix
	{
		return scale(v.x, v.y, v.z);
	}

	IC	SelfRef	rotateX(float Angle)				// rotation about X axis
	{
		float cosa = _cos(Angle);
		float sina = _sin(Angle);

		R[0] = set_ps(1, 0, 0, 0);
		R[1] = set_ps(0, cosa, sina, 0);
		R[2] = set_ps(0, -sina, cosa, 0);
		R[3] = set_ps(0, 0, 0, 1);
		return *this;
	}
	IC	SelfRef	rotateY(float Angle)				// rotation about Y axis
	{
		float cosa = _cos(Angle);
		float sina = _sin(Angle);

		R[0] = set_ps(cosa, 0, -sina,0);
		R[1] = set_ps(0, 1, 0, 0);
		R[2] = set_ps(sina, 0, cosa, 0);
		R[3] = set_ps(0, 0, 0, 1);

		return *this;
	}
	IC	SelfRef	rotateZ(float Angle)				// rotation about Z axis
	{
		float cosa = _cos(Angle);
		float sina = _sin(Angle);

		R[0] = set_ps(cosa, sina, 0, 0);
		R[1] = set_ps(-sina, cosa, 0, 0);
		R[2] = set_ps(0, 0, 1, 0);
		R[3] = set_ps(0, 0, 0, 1);

		return *this;
	}

	IC	SelfRef	rotation(const Tvector& vdir, const Tvector& vnorm) {
		Tvector vright;
		vright.crossproduct(vnorm, vdir).normalize();
		R[0] = set_ps(vright.x, vright.y, vright.z, 0);
		R[1] = set_ps(vnorm.x, vnorm.y, vnorm.z, 0);
		R[2] = set_ps(vdir.x, vdir.y, vdir.z, 0);
		R[3] = set_ps(0, 0, 0, 1);
		return *this;
	}

	IC	SelfRef	mapXYZ() 
	{
		R[0] = set_ps(1, 0, 0, 0);
		R[1] = set_ps(0, 1, 0, 0);
		R[2] = set_ps(0, 0, 1, 0);
		R[3] = set_ps(0, 0, 0, 1);
		return *this; 
	}
	IC	SelfRef	mapXZY()
	{
		R[0] = set_ps(1, 0, 0, 0);
		R[1] = set_ps(0, 0, 1, 0);
		R[2] = set_ps(0, 1, 0, 0);
		R[3] = set_ps(0, 0, 0, 1);
		return *this;
	}
	IC	SelfRef	mapYXZ() 
	{
		R[0] = set_ps(0, 1, 0, 0);
		R[1] = set_ps(1, 0, 0, 0);
		R[2] = set_ps(0, 0, 1, 0);
		R[3] = set_ps(0, 0, 0, 1);
		return *this; 
	}
	IC	SelfRef	mapYZX() 
	{
		R[0] = set_ps(0, 1, 0, 0);
		R[1] = set_ps(0, 0, 1, 0);
		R[2] = set_ps(1, 0, 0, 0);
		R[3] = set_ps(0, 0, 0, 1);
		return *this; }
	IC	SelfRef	mapZXY() 
	{
		R[0] = set_ps(0, 0, 1, 0);
		R[1] = set_ps(1, 0, 0, 0);
		R[2] = set_ps(0, 1, 0, 0);
		R[3] = set_ps(0, 0, 0, 1);
		return *this; 
	}
	IC	SelfRef	mapZYX() 
	{
		R[0] = set_ps(0, 0, 1, 0);
		R[1] = set_ps(0, 1, 0, 0);
		R[2] = set_ps(1, 0, 0, 0);
		R[3] = set_ps(0, 0, 0, 1);
		return *this; 
	}

	IC	SelfRef	rotation(const Tvector& axis, float Angle) 
	{
		float Cosine = _cos(Angle);
		float Sine = _sin(Angle);
		_MM_ALIGN16 float M[4];
		M[0] = axis.x * axis.x + (1 - axis.x * axis.x) * Cosine;
		M[1] = axis.x * axis.y * (1 - Cosine) + axis.z * Sine;
		M[2] = axis.x * axis.z * (1 - Cosine) - axis.y * Sine;
		M[3] = 0;

		R[0] = _mm_load_ps(M);
		M[0] = axis.x * axis.y * (1 - Cosine) - axis.z * Sine;
		M[1] = axis.y * axis.y + (1 - axis.y * axis.y) * Cosine;
		M[2] = axis.y * axis.z * (1 - Cosine) + axis.x * Sine;
		M[3] = 0;
		R[1] = _mm_load_ps(M);

		M[0] = axis.x * axis.z * (1 - Cosine) + axis.y * Sine;
		M[1] = axis.y * axis.z * (1 - Cosine) - axis.x * Sine;
		M[2] = axis.z * axis.z + (1 - axis.z * axis.z) * Cosine;
		M[3] = 0;
		R[2] = _mm_load_ps(M);

		R[3] = set_ps(0,0,0,1);
		return *this;
	}

	// mirror X
	IC	SelfRef	mirrorX() 
	{
		R[0] = set_ps(-1, 0, 0, 0);
		R[1] = set_ps(0, 1, 0, 0);
		R[2] = set_ps(0, 0, 1, 0);
		R[3] = set_ps(0, 0, 0, 1);
		return *this;
	}
	IC	SelfRef	mirrorX_over()
	{
		R[0] = _mm_mul_ps(R[0], set_ps(0, 1, 1,1));
		R[0] = _mm_add_ps(R[0], set_ps(-1, 0, 0, 0));
		return *this;
	}
	IC	SelfRef	mirrorX_add()
	{
		R[0] = _mm_mul_ps(R[0], set_ps(-1, 0, 0, 0));
		return *this;
	}

	// mirror Y
	IC	SelfRef	mirrorY() 
	{
		R[0] = set_ps(1, 0, 0, 0);
		R[1] = set_ps(0, -1, 0, 0);
		R[2] = set_ps(0, 0, 1, 0);
		R[3] = set_ps(0, 0, 0, 1);
		return *this;
	}
	IC	SelfRef	mirrorY_over() 
	{
		R[1] = _mm_mul_ps(R[1], set_ps(1, 0, 1, 1));
		R[1] = _mm_add_ps(R[1], set_ps(0, -1, 0, 0));
		return *this;
	}
	IC	SelfRef	mirrorY_add() 
	{
		R[1] = _mm_mul_ps(R[1], set_ps(0, -1, 0, 0));
		return *this;
	}

	// mirror Z
	IC	SelfRef	mirrorZ() 
	{
		R[0] = set_ps(1, 0, 0, 0);
		R[1] = set_ps(0, 1, 0, 0);
		R[2] = set_ps(0, 0, -1, 0);
		R[3] = set_ps(0, 0, 0, 1);
		return *this;
	}
	IC	SelfRef	mirrorZ_over() 
	{
		R[2] = _mm_mul_ps(R[2], set_ps(1, 1, 0, 1));
		R[2] = _mm_add_ps(R[2], set_ps(0, 0, -1, 0));
		return *this;
	}
	IC	SelfRef	mirrorZ_add()
	{
		R[2] = _mm_mul_ps(R[2], set_ps(0, 0, -1, 0));
		return *this;
	}
	IC	SelfRef	mul(const Self& A, float v) 
	{
		_MM_ALIGN16 __m128 V = set_ps(v, v, v, v);

		R[0] = _mm_mul_ps(A.R[0], V);
		R[1] = _mm_mul_ps(A.R[1], V);
		R[2] = _mm_mul_ps(A.R[2], V);
		R[3] = _mm_mul_ps(A.R[3], V);
		return *this;
	}
	IC	SelfRef	mul(float v) {
		_MM_ALIGN16 __m128 V = set_ps(v, v, v, v);

		R[0] = _mm_mul_ps(R[0], V);
		R[1] = _mm_mul_ps(R[1], V);
		R[2] = _mm_mul_ps(R[2], V);
		R[3] = _mm_mul_ps(R[3], V);
		return *this;
	}
	IC	SelfRef	div(const Self& A, float v) 
	{
		VERIFY(_abs(v) > 0.000001f);
		return mul(A, 1.0f / v);
	}
	IC	SelfRef	div(float v)
	{
		VERIFY(_abs(v) > 0.000001f);
		return mul(1.0f / v);
	}
	// fov
	IC	SelfRef	build_projection(float fFOV, float fAspect, float fNearPlane, float fFarPlane) 
	{
		return build_projection_HAT(tanf(fFOV / 2.f), fAspect, fNearPlane, fFarPlane);
	}
	// half_fov-angle-tangent
	IC	SelfRef	build_projection_HAT(float HAT, float fAspect, float fNearPlane, float fFarPlane)
	{
		VERIFY(_abs(fFarPlane - fNearPlane) > EPS_S);
		VERIFY(_abs(HAT) > EPS_S);

		float cot = float(1) / HAT;
		float w = fAspect * cot;
		float h = float(1) * cot;
		float Q = fFarPlane / (fFarPlane - fNearPlane);
		R[0] = set_ps(w, 0, 0, 0);
		R[1] = set_ps(0, h, 0, 0);
		R[2] = set_ps(0, 0, Q, 1.f);
		R[3] = set_ps(0, 0, -Q*fNearPlane, 0);
		return *this;
	}
	IC	SelfRef	build_projection_ortho(float w, float h, float zn, float zf)
	{

		R[0] = set_ps(float(2) / w, 0, 0, 0);
		R[1] = set_ps(0, float(2) / h, 0, 0);
		R[2] = set_ps(0, 0, float(1) / (zf - zn), 0);
		R[3] = set_ps(0, 0, zn / (zn - zf), 1);
		return *this;
	}
	IC	SelfRef	build_camera(const Tvector& vFrom, const Tvector& vAt, const Tvector& vWorldUp)
	{
		// Get the z basis vector3, which points straight ahead. This is the
		// difference from the eyepoint to the lookat point.
		Tvector vView;
		vView.sub(vAt, vFrom).normalize();

		// Get the dot product, and calculate the projection of the z basis
		// vector3 onto the up vector3. The projection is the y basis vector3.
		float fDotProduct = vWorldUp.dotproduct(vView);

		Tvector vUp;
		vUp.mul(vView, -fDotProduct).add(vWorldUp).normalize();

		// The x basis vector3 is found simply with the cross product of the y
		// and z basis vectors
		Tvector vRight;
		vRight.crossproduct(vUp, vView);

		// Start building the Device.mView. The first three rows contains the basis
		// vectors used to rotate the view to point at the lookat point

		R[0] = set_ps(vRight.x, vUp.x, vView.x, 0);
		R[1] = set_ps(vRight.y, vUp.y, vView.y, 0);
		R[2] = set_ps(vRight.z, vUp.z, vView.z, 0);
		R[3] = set_ps(-vFrom.dotproduct(vRight), -vFrom.dotproduct(vUp), -vFrom.dotproduct(vView), 1);
		return *this;
	}
	IC	SelfRef	build_camera_dir(const Tvector& vFrom, const Tvector& vView, const Tvector& vWorldUp)
	{
		// Get the dot product, and calculate the projection of the z basis
		// vector3 onto the up vector3. The projection is the y basis vector3.
		float fDotProduct = vWorldUp.dotproduct(vView);

		Tvector vUp;
		vUp.mul(vView, -fDotProduct).add(vWorldUp).normalize();

		// The x basis vector3 is found simply with the cross product of the y
		// and z basis vectors
		Tvector vRight;
		vRight.crossproduct(vUp, vView);

		// Start building the Device.mView. The first three rows contains the basis
		// vectors used to rotate the view to point at the lookat point
		R[0] = set_ps(vRight.x, vUp.x, vView.x, 0);
		R[1] = set_ps(vRight.y, vUp.y, vView.y, 0);
		R[2] = set_ps(vRight.z, vUp.z, vView.z, 0);
		R[3] = set_ps(-vFrom.dotproduct(vRight), -vFrom.dotproduct(vUp), -vFrom.dotproduct(vView), 1);
		return *this;
	}

	IC	SelfRef	inertion(const Self& mat, float v)
	{
		_MM_ALIGN16 __m128 RegA = _mm_load_ps1(&v);
		float iv = 1.f - v;
		_MM_ALIGN16 __m128 RegB = _mm_load_ps1(&iv);

		R[0] = _mm_add_ps(_mm_mul_ps(R[0], RegA), _mm_mul_ps(mat.R[0], RegB));
		R[1] = _mm_add_ps(_mm_mul_ps(R[1], RegA), _mm_mul_ps(mat.R[1], RegB));
		R[2] = _mm_add_ps(_mm_mul_ps(R[2], RegA), _mm_mul_ps(mat.R[2], RegB));
		R[3] = _mm_add_ps(_mm_mul_ps(R[3], RegA), _mm_mul_ps(mat.R[3], RegB));
		
		return *this;
	}
	IC	void	transform_tiny(Tvector& dest, const Tvector& v)	const // preferred to use
	{

		_MM_ALIGN16 __m128 RegV = set_ps(v.x, v.y, v.z, 0);
		_MM_ALIGN16 __m128 RegResult = _mm_mul_ps(swizzle<0,0,0,0>(RegV), R[0]);
		RegResult = _mm_add_ps(RegResult, _mm_mul_ps(swizzle<1,1,1,1>(RegV), R[1]));
		RegResult = _mm_add_ps(RegResult, _mm_mul_ps(swizzle<2,2,2,2>(RegV), R[2]));
		RegResult = _mm_add_ps(RegResult, R[3]);
		_MM_ALIGN16 float Result[4];
		_mm_store_ps(Result, RegResult);
		dest.set(Result[0], Result[1], Result[2]);
		
	}
	IC	void	transform_tiny32(Fvector2& dest, const Tvector& v)	const // preferred to use
	{
		_MM_ALIGN16 __m128 RegV = set_ps(v.x, v.y, v.z, 0);
		_MM_ALIGN16 __m128 RegResult = _mm_mul_ps(swizzle<0,0,0,0>(RegV), R[0]);
		RegResult = _mm_add_ps(RegResult, _mm_mul_ps(swizzle<1,1,1,1>(RegV), R[1]));
		RegResult = _mm_add_ps(RegResult, _mm_mul_ps(swizzle<2,2,2,2>(RegV), R[2]));
		RegResult = _mm_add_ps(RegResult, R[3]);
		_MM_ALIGN16 float Result[4];
		_mm_store_ps(Result, RegResult);
		dest.set(Result[0], Result[1]);
	}
	IC	void	transform_tiny23(Tvector& dest, const Fvector2& v)	const // preferred to use
	{
		_MM_ALIGN16 __m128 RegV = set_ps(v.x, v.y, 0, 0);
		_MM_ALIGN16 __m128 RegResult = _mm_mul_ps(swizzle<0,0,0,0>(RegV), R[0]);
		RegResult = _mm_add_ps(RegResult, _mm_mul_ps(swizzle<1,1,1,1>(RegV), R[1]));
		RegResult = _mm_add_ps(RegResult, R[3]);
		_MM_ALIGN16 float Result[4];
		_mm_store_ps(Result, RegResult);
		dest.set(Result[0], Result[1], Result[2]);
	}
	IC	void	transform_dir(Tvector& dest, const Tvector& v)	const 	// preferred to use
	{
		_MM_ALIGN16 __m128 RegV = set_ps(v.x, v.y, v.z, 0);
		_MM_ALIGN16 __m128 RegResult = _mm_mul_ps(swizzle<0,0,0,0>(RegV), R[0]);
		RegResult = _mm_add_ps(RegResult, _mm_mul_ps(swizzle<1,1,1,1>(RegV), R[1]));
		RegResult = _mm_add_ps(RegResult, _mm_mul_ps(swizzle<2,2,2,2>(RegV), R[2]));
		_MM_ALIGN16 float Result[4];
		_mm_store_ps(Result, RegResult);
		dest.set(Result[0], Result[1], Result[2]);
	}
	IC	void	transform(Fvector4& dest, const Tvector& v)	const 	// preferred to use
	{
		Fvector4 V; V.set(v.x, v.y, v.z);
		transform(dest, V);

		float iw = 1.f / dest.w;
		dest.x = dest.x * iw;
		dest.y = dest.y * iw;
		dest.z = dest.z * iw;
	}
	IC	void	transform(Tvector& dest, const Tvector& v)	const 	// preferred to use
	{
		Fvector4 V,Dest; V.set(v.x, v.y, v.z);
		transform(Dest, V);

		float iw = 1.f / Dest.w;
		dest.x = Dest.x *iw;
		dest.y = Dest.y * iw;
		dest.z = Dest.z * iw;
	}

	IC	void	transform(Fvector4& dest, const Fvector4& v)	const 	// preferred to use
	{
		_MM_ALIGN16 __m128 RegV = set_ps(v.x, v.y, v.z, v.w);
		_MM_ALIGN16 __m128 RegResult = _mm_mul_ps(swizzle<0,0,0,0>(RegV), R[0]);
		RegResult = _mm_add_ps(RegResult, _mm_mul_ps(swizzle<1, 1, 1, 1>(RegV), R[1]));
		RegResult = _mm_add_ps(RegResult, _mm_mul_ps(swizzle<2, 2, 2, 2>(RegV), R[2]));
		_MM_ALIGN16 float Result[4];
		_mm_store_ps(Result, RegResult);
		dest.set(Result[0], Result[1], Result[2], Result[4]);
	}

	IC	void	transform_tiny(Tvector& v) const
	{
		Tvector			res;
		transform_tiny(res, v);
		v.set(res);
	}
	IC	void	transform(Tvector& v) const
	{
		Tvector			res;
		transform(res, v);
		v.set(res);
	}
	IC	void	transform_dir(Tvector& v) const
	{
		Tvector			res;
		transform_dir(res, v);
		v.set(res);
	}
	IC	SelfRef	setHPB(float h, float p, float b)
	{
		float _ch, _cp, _cb, _sh, _sp, _sb, _cc, _cs, _sc, _ss;

		_sh = _sin(h); _ch = _cos(h);
		_sp = _sin(p); _cp = _cos(p);
		_sb = _sin(b); _cb = _cos(b);
		_cc = _ch * _cb; _cs = _ch * _sb; _sc = _sh * _cb; _ss = _sh * _sb;

		R[0] = set_ps(_cc - _sp * _ss, -_cp * _sb, _sp * _cs + _sc, 0);
		R[1] = set_ps(_sp * _sc + _cs, _cp * _cb, _ss - _sp * _cc, 0);
		R[2] = set_ps(-_cp * _sh, _sp, _cp * _ch, 0);
		R[3] = set_ps(0, 0, 0, 1);

		return *this;
	}
	IC	SelfRef	setXYZ(float x, float y, float z) { return setHPB(y, x, z); }
	IC	SelfRef	setXYZ(Tvector const& xyz) { return setHPB(xyz.y, xyz.x, xyz.z); }
	IC	SelfRef	setXYZi(float x, float y, float z) { return setHPB(-y, -x, -z); }
	IC	SelfRef	setXYZi(Tvector const& xyz) { return setHPB(-xyz.y, -xyz.x, -xyz.z); }
	//
	IC	void	getHPB(float& h, float& p, float& b) const
	{
		Fvector i = get_i();
		Fvector j = get_j();
		Fvector k = get_k();
		float cy = _sqrt(j.y * j.y + i.y * i.y);
		if (cy > 16.0f * type_epsilon(float)) 
		{
			h = (float)-atan2(k.x, k.z);
			p = (float)-atan2(-k.y, cy);
			b = (float)-atan2(i.y, j.y);
		}
		else 
		{
			h = (float)-atan2(-i.z, i.x);
			p = (float)-atan2(-k.y, cy);
			b = 0;
		}
	}
	IC	void	getHPB(Tvector& hpb) const { getHPB(hpb.x, hpb.y, hpb.z); }
	IC	void	getXYZ(float& x, float& y, float& z) const { getHPB(y, x, z); }
	IC	void	getXYZ(Tvector& xyz) const { getXYZ(xyz.x, xyz.y, xyz.z); }
	IC	void	getXYZi(float& x, float& y, float& z) const { getHPB(y, x, z); x *= -1.f; y *= -1.f; z *= -1.f; }
	IC	void	getXYZi(Tvector& xyz) const { getXYZ(xyz.x, xyz.y, xyz.z); xyz.mul(-1.f); }

	IC  float	get_w(Tvector const& xyz)
	{
		_MM_ALIGN16 __m128 R1 = _mm_movelh_ps(R[0], R[1]);
		_MM_ALIGN16 __m128 R2 = _mm_movehl_ps(R[2], R[3]);
		_MM_ALIGN16 __m128 R3 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(1, 2, 3, 0));
		R1 = set_ps(xyz.x, xyz.y, xyz.z, 1);
		R2 = _mm_mul_ps(R1, R3);
		_MM_ALIGN16 float Result[4];
		_mm_store_ps(Result,R2);
		return Result[0] + Result[1] + Result[2] + Result[3];
		/*pos.x*EDevice.mFullTransform._14 + pos.y*EDevice.mFullTransform._24 + pos.z*EDevice.mFullTransform._34 + EDevice.mFullTransform._44;*/
	}
private:
	template<int x,int y,int z,int w>
	static IC  __m128 swizzle(__m128 in)
	{
		return _mm_shuffle_ps(in, in, _MM_SHUFFLE(w,z,y,x));
	}
	template<>
	static IC  __m128 swizzle<0,1,2,3>(__m128 in)
	{
		return in;
	}
	template<>
	static IC  __m128 swizzle<2,2,3,3>(__m128 in)
	{
		return _mm_unpackhi_ps(in,in);
	}
	template<>
	static IC  __m128 swizzle<0,0,1,1>(__m128 in)
	{
		return _mm_unpacklo_ps(in, in);
	}
	template<>
	static IC  __m128 swizzle<2,3,2,3>(__m128 in)
	{
		return _mm_movehl_ps(in, in);
	}
	template<>
	static IC  __m128 swizzle<0,1,0,1>(__m128 in)
	{
		return _mm_movelh_ps(in, in);
	}
	static IC  __m128 set_ps(float x, float y, float z, float w)
	{
		_MM_ALIGN16 float Result[4] =  { x,y,z,w };
		return _mm_load_ps(Result);
	}
};


template <class T>
IC BOOL	_valid(const _matrix<T>& m)
{
	return
		_valid(m.i) && _valid(m._14_) &&
		_valid(m.j) && _valid(m._24_) &&
		_valid(m.k) && _valid(m._34_) &&
		_valid(m.c) && _valid(m._44_)
		;
}

template <>
IC BOOL _valid<float>(const _matrix<float>& m)
{
	bool Result = true;

	for (int i = 0; i < 4; i++)
	{
		_MM_ALIGN16 float M[4];
		_mm_store_ps(M, m.R[i]);
		Result = Result && _valid(M[0]) && _valid(M[1]) && _valid(M[2]) && _valid(M[3]);
		if (!Result)return false;
	}

	return Result;
}