#ifndef ShapeDataH
#define ShapeDataH

struct CShapeData
{
	enum{
    	cfSphere=0,
        cfBox
    };
	union shape_data
	{
		shape_data() { box.identity(); }
		Fsphere		sphere;
		Fmatrix		box;
	};
	struct shape_def
	{
		shape_def() {}
		shape_def(const shape_def& Right)
		{
			switch (Right.type)
			{
			case cfBox:
				data.box.copy(Right.data.box);
			default:
				data.sphere = Right.data.sphere;
				break;
			}
			type = Right.type;
		}
		u8			type;
		shape_data	data;
		inline shape_def& operator=(const shape_def& Right)
		{
			switch (type)
			{
			case cfBox:
				data.box.copy(Right.data.box);
			default:
				data.sphere = Right.data.sphere;
				break;
			}
			return*this;
		}
	};
    DEFINE_VECTOR					(shape_def,ShapeVec,ShapeIt);
	ShapeVec						shapes;
};

#endif