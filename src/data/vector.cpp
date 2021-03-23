#include "vector.h"

namespace dogEngine
{
	//CVector2
	//-----------------------------------
	CVector2::CVector2()
	: CVector()
	{
	}
	CVector2::CVector2(float x,float y)
	: CVector(2,x,y)
	{
	}
	CVector2::CVector2(const CVector<2>& vector)
	{
		vec[0] = vector.getValue(0);
		vec[1] = vector.getValue(1);
	}
	void CVector2::setX(float value)
	{
		vec[0] = value;
	}
	void CVector2::setY(float value)
	{
		vec[1] = value;
	}
	float CVector2::getX()
	{
		return vec[0];
	}
	float CVector2::getY()
	{
		return vec[1];
	}
	//CVector3
	//-----------------------------------
	CVector3::CVector3()
	: CVector()
	{
	}
	CVector3::CVector3(float x,float y,float z)
	: CVector(3,x,y,z)
	{
	}
	CVector3::CVector3(const CVector<3>& vector)
	{
		vec[0] = vector.getValue(0);
		vec[1] = vector.getValue(1);
		vec[2] = vector.getValue(2);
	}
	void CVector3::setX(float value)
	{
		vec[0] = value;
	}
	void CVector3::setY(float value)
	{
		vec[1] = value;
	}
	void CVector3::setZ(float value)
	{
		vec[2] = value;
	}

	void CVector3::set(CVector3* vecToCopy)
	{
		vec[0] = vecToCopy->getX();
		vec[1] = vecToCopy->getY();
		vec[2] = vecToCopy->getZ();
	}
	void CVector3::set(CVector3 vecToCopy)
	{
		vec[0] = vecToCopy.getX();
		vec[1] = vecToCopy.getY();
		vec[2] = vecToCopy.getZ();
	}

	float CVector3::getX()
	{
		return vec[0];
	}
	float CVector3::getY()
	{
		return vec[1];
	}
	float CVector3::getZ()
	{
		return vec[2];
	}

	double CVector3::getLength3()
	{
		return getLength();
	}

	CVector3 CVector3::crossProduct(CVector3 vector)
	{
		CVector3 ret;
		ret[0] = vec[1]*vector[2] - vec[2]*vector[1];
		ret[1] = vec[2]*vector[0] - vec[0]*vector[2];
		ret[2] = vec[0]*vector[1] - vec[1]*vector[0];
		return ret;
	}

	CVector3 CVector3::operator%(CVector3 vector)
	{
		return crossProduct(vector);
	}

	bool CVector3::operator==(CVector3 vector)
	{
		CVector3d thisVec = (*this);
		CVector3D diff = thisVec-vector;
		return (std::abs(diff.getX()) < FLT_EPSILON ) &&
				(std::abs(diff.getY()) < FLT_EPSILON ) &&
				(std::abs(diff.getZ()) < FLT_EPSILON );
	}

	float getAngle(CVector3 a,CVector3 b)
	{
		float skalar = a*b;
		return acos(skalar / (a.getLength()*b.getLength()));
	}

	double angle(CVector3D vec1,CVector3D vec2)
	{
		return getAngle(vec1,vec2);
	}

	double distance(CVector3* vecA,CVector3* vecB)
	{
		return (*vecB - *vecA).getLength();
	}

	double dot3(CVector3D vec1,CVector3D vec2)
	{
		return vec1.getX() * vec2.getX() +
			   vec1.getY() * vec2.getY() +
			   vec1.getZ() * vec2.getZ();
	}

	CVector3 operator*(double skalar,CVector3 vec)
	{
		CVector3 ret;
		for(int i=0;i<3;i++)
		{
			ret[i] = vec[i] * skalar;
		}
		return ret;
	}

	//CVector4
	//----------------------------------
	CVector4::CVector4()
	: CVector()
	{
	}
	CVector4::CVector4(float x,float y,float z,float w)
	: CVector(4,x,y,z,w)
	{
	}

	CVector4::CVector4(const CVector<4>& vector)
	{
		vec[0] = vector.getValue(0);
		vec[1] = vector.getValue(1);
		vec[2] = vector.getValue(2);
		vec[3] = vector.getValue(3);
	}
	CVector4::CVector4(CVector3 vector)
	{
		vec[0] = vector.getValue(0);
		vec[1] = vector.getValue(1);
		vec[2] = vector.getValue(2);
		vec[3] = 1;
	}

	void CVector4::setX(float value)
	{
		vec[0] = value;
	}
	void CVector4::setY(float value)
	{
		vec[1] = value;
	}
	void CVector4::setZ(float value)
	{
		vec[2] = value;
	}
	void CVector4::setW(float value)
	{
		vec[3] = value;
	}
	float CVector4::getX()
	{
		return vec[0];
	}
	float CVector4::getY()
	{
		return vec[1];
	}
	float CVector4::getZ()
	{
		return vec[2];
	}
	float CVector4::getW()
	{
		return vec[3];
	}
}
