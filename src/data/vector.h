#ifndef __VECTOR__
#define __VECTOR__
#include <stdarg.h>
//#include <stddef.h>
#include <math.h>
#include <iostream>
#include <float.h>
#include <cmath>
#include <array>
//#include "Matrix.h"

//#include "../Exceptions/normalizeZeroVectorException.h"

namespace dogEngine
{
	class NormalizeZeroVectorException: public std::exception{
		private:
		public:
		NormalizeZeroVectorException(){}
		virtual const char* what() const throw(){
			return "Normalize Zero Vector Exception";
		}
	};


	template<std::size_t N>
	class CVector;

	template<std::size_t N>
	std::ostream& operator<<(std::ostream& input,const CVector<N>& vector);

	template<std::size_t N>
	class CVector
	{
	//friend std::ostream& operator<<(std::ostream& input,const CVector<N>& vector);
	friend std::ostream& operator<< <>(std::ostream& input,CVector<N> const& vector);
	protected:
		//std::array<float,N> vec;
		float vec[N];

	public:
		CVector();
		CVector(int numParam,...);
		//CVector(float* arr);
		CVector(std::array<float,N> arr);

		//void getArray(float* arr);
		std::array<float,N> getArray();

		CVector<N> operator+(CVector<N> vector);
		CVector<N> operator-(CVector<N> vector);
		CVector<N> operator*(float skalar);
		float operator*(CVector<N> vector);
		CVector<N> operator/(float skalar);
		void operator+=(CVector<N> vector);
		void operator-=(CVector<N> vector);
		void operator*=(float skalar);
		void operator/=(float skalar);
		bool operator==(CVector<N> vec);
		bool operator!=(CVector<N> vec);
		float& operator[](int index);
		CVector<N> crossProduct(CVector<N> vector);
		CVector<N> minFunc(CVector<N> vector);
		CVector<N> maxFunc(CVector<N> vector);

		float getValue(int index) const;

		float getLength();

		void normalize();

		int getSize();

	};



	class CVector2:public CVector<2>
	{
	public:
		CVector2();
		CVector2(float x,float y);
		CVector2(const CVector<2>& vector);
		void setX(float value);
		void setY(float value);
		float getX();
		float getY();
	};

	typedef CVector2 CVector2D;
	typedef CVector2 CVector2d;
	//----------------------------------------
	class CVector3:public CVector<3>
	{
	public:
		CVector3();
		CVector3(float x,float y,float z);
		CVector3(const CVector<3>& vector);
		void setX(float value);
		void setY(float value);
		void setZ(float value);
		void set(CVector3* vecToCopy);
		void set(CVector3 vecToCopy);
		float getX();
		float getY();
		float getZ();

		double getLength3();

		CVector3 crossProduct(CVector3 vector);
		CVector3 operator%(CVector3 vector);
		bool operator==(CVector3 vector);
	};

	typedef CVector3 CVector3D;
	typedef CVector3 CVector3d;
	float getAngle(CVector3 a,CVector3 b);
	double angle(CVector3D vec1,CVector3D vec2);
	double distance(CVector3* vecA,CVector3* vecB);
	double dot3(CVector3D vec1,CVector3D vec2);
	CVector3 operator*(double skalar,CVector3 vec);
	//--------------------------------------------
	class CVector4:public CVector<4>
	{
	public:
		CVector4();
		CVector4(float x,float y,float z,float w);
		CVector4(const CVector<4>& vector);
		CVector4(CVector3 vec);
		void setX(float value);
		void setY(float value);
		void setZ(float value);
		void setW(float value);
		float getX();
		float getY();
		float getZ();
		float getW();
	};
	typedef CVector4 CVector4D;
	typedef CVector4 CVector4d;
	//----------------------------------------------------------
	template<std::size_t N>
	std::ostream& operator <<(std::ostream& input,const CVector<N>& vector)
	{
		//return (input << "[" << vector.getX()<< "\t" << vector.getY()<<"\t" << vector.getZ() <<"]");
		input << "[";
		for(int i=0;i<N;i++)
		{
			input << vector.getValue(i) << "\t";
		}
		input << "]";
		return input;
	}
	//----------------------------------------------------------
	template<std::size_t N>
	CVector<N>::CVector()
	{
		for(int i=0;i<N;i++)
		{
			this->vec[i] = 0;
		}
	}
	//----------------------------------------------------------
	template<std::size_t N>
	CVector<N>::CVector(int numParam,...)
	{
		float value;
		va_list vl;
		va_start(vl,numParam);
		if(numParam >= N)
		{
			for(int i=0;i<N;i++)
			{
				value = (float) va_arg(vl,double);
				this->vec[i] = value;
			}
		}
		else
		{
			for(int i=0;i<numParam;i++)
			{
				value = (float)va_arg(vl,double);
				this->vec[i] = value;
			}
			int rest = N-numParam;
			for(int i=numParam;i<N;i++)
			{
				this->vec[i] = 0;
			}
		}
		va_end(vl);
	}
	//----------------------------------------------------------
	template<std::size_t N>
	//CVector<N>::CVector(float* arr)
	CVector<N>::CVector(std::array<float,N> arr)
	{
		for(int i=0;i<N;i++)
		{
			this->vec[i] = arr[i];
		}
	}
	//----------------------------------------------------------
	template<std::size_t N>
	//void CVector<N>::getArray(float* arr)
	std::array<float,N> CVector<N>::getArray()
	{
		std::array<float,N> arr;
		for(int i=0;i<N;i++)
		{
			arr[i] = this->vec[i];
		}
		return arr;
	}
	//----------------------------------------------------------
	template<std::size_t N>
	CVector<N> CVector<N>::operator+(CVector<N> vector)
	{
		CVector<N> ret;
		for(int i=0;i<N;i++)
		{
			ret[i] = this->vec[i] + vector[i];
		}
		return ret;
	}
	//----------------------------------------------------------
	template<std::size_t N>
	CVector<N> CVector<N>::operator-(CVector vector)
	{
		CVector<N> ret;
		for(int i=0;i<N;i++)
		{
			ret[i] = this->vec[i] - vector[i];
		}
		return ret;
	}
	//----------------------------------------------------------
	template<std::size_t N>
	CVector<N> CVector<N>::operator*(float skalar)
	{
		CVector<N> ret;
		for(int i=0;i<N;i++)
		{
			ret[i] = this->vec[i] * skalar;
		}
		return ret;
	}
	//----------------------------------------------------------
	template<std::size_t N>
	float CVector<N>::operator*(CVector vector)
	{
		float ret = 0;
		for(int i=0;i<N;i++)
		{
			ret += this->vec[i] * vector[i];
		}
		return ret;
	}
	//----------------------------------------------------------
	template<std::size_t N>
	CVector<N> CVector<N>::operator/(float skalar)
	{
		CVector<N> ret;
		for(int i=0;i<N;i++)
		{
			ret[i] = this->vec[i] / skalar;
		}
		return ret;
	}
	//----------------------------------------------------------
	template<std::size_t N>
	void CVector<N>::operator+=(CVector<N> vector)
	{
		for(int i=0;i<N;i++)
		{
			vec[i]+=vector[i];
		}
	}
	//----------------------------------------------------------
	template<std::size_t N>
	void CVector<N>::operator-=(CVector<N> vector)
	{
		for(int i=0;i<N;i++)
		{
			vec[i]-=vector[i];
		}
	}
	//----------------------------------------------------------
	template<std::size_t N>
	void CVector<N>::operator*=(float skalar)
	{
		for(int i=0;i<N;i++)
		{
			vec[i]*=skalar;
		}
	}
	//----------------------------------------------------------
	template<std::size_t N>
	void CVector<N>::operator/=(float skalar)
	{
		for(int i=0;i<N;i++)
		{
			vec[i]/=skalar;
		}
	}
	//----------------------------------------------------------
	template<std::size_t N>
	float& CVector<N>::operator[](int index)
	{
		return this->vec[index];
	}
	//----------------------------------------------------------
	template<std::size_t N>
	CVector<N> CVector<N>::minFunc(CVector<N> vector)
	{
		CVector<N> ret;
		for(int i=0;i<N;i++)
		{
			ret[i] = vec[i] < vector[i] ? vec[i]:vector[i];
		}
		return ret;
	}
	//----------------------------------------------------------
	template<std::size_t N>
	CVector<N> CVector<N>::maxFunc(CVector<N> vector)
	{
		CVector<N> ret;
		for(int i=0;i<N;i++)
		{
			ret[i] = vec[i] > vector[i] ? vec[i]:vector[i];
		}
		return ret;
	}
	//----------------------------------------------------------
	template<std::size_t N>
	CVector<N> CVector<N>::crossProduct(CVector vector)
	{

	}
	//----------------------------------------------------------
	template<std::size_t N>
	float CVector<N>::getValue(int index) const
	{
		const float value = vec[index];
		return value;
	}
	//----------------------------------------------------------
	template<std::size_t N>
	float CVector<N>::getLength()
	{
		float ret = 0;
		for(int i=0;i<N;i++)
		{
			ret += this->vec[i] * this->vec[i];
		}
		ret = sqrt(ret);
		return ret;
	}
	//----------------------------------------------------------
	template<std::size_t N>
	void CVector<N>::normalize()
	{
		float length = getLength();
		if(abs(length) < FLT_EPSILON){
			//we are going to divide by zero. This must be prevented.
			throw NormalizeZeroVectorException();
		}
		for(int i=0;i<N;i++)
		{
			this->vec[i] /= length;
		}
	}
	//----------------------------------------------------------
	template<std::size_t N>
	int getSize()
	{
		return N;
	}
	//----------------------------------------------------------
	template<std::size_t N>
	bool CVector<N>::operator==(CVector<N> vec)
	{
		bool ret = true;
		for(int i=0;i<N;i++)
		{
			if(std::abs(getValue(i) - vec[i]) > FLT_EPSILON)
			{
				ret = false;
				break;
			}
		}
		return ret;
	}
	//----------------------------------------------------------
	template<std::size_t N>
	bool CVector<N>::operator!=(CVector<N> vec)
	{
		bool ret = false;
		for(int i=0;i<N;i++)
		{
			if(std::abs(getValue(i) - vec[i]) > FLT_EPSILON)
			{
				ret = true;
				break;
			}
		}
		return ret;
	}
}

#endif
