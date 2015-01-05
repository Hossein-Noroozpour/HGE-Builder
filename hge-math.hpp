#ifndef HGE_MATH_HPP
#define HGE_MATH_HPP
#include <cmath>
#include "hge-serializable.hpp"
namespace hge
{
	namespace math
	{
		template<typename element_type=float> class Vector3D;
		template<typename element_type=float> class Vector4D;
		template<typename element_type=float> class Matrix4D;

		template<typename element_type>
		class Vector3D
		{
		public:
			element_type vec [3];
			Vector3D(element_type x, element_type y, element_type z)
			{
				vec[0] = x;
				vec[1] = y;
				vec[2] = z;
			}
			Vector3D(element_type e)
			{
				vec[0] = e;
				vec[1] = e;
				vec[2] = e;
			}
			Vector3D(const Vector3D<element_type> &v)
			{
				vec[0] = v.vec[0];
				vec[1] = v.vec[1];
				vec[2] = v.vec[2];
			}
			void operator=(const Vector3D<element_type> &v)
			{
				vec[0] = v.vec[0];
				vec[1] = v.vec[1];
				vec[2] = v.vec[2];
			}
			element_type dot(const Vector3D &another) const
			{
				return  (vec[0] * another.vec[0]) +
						(vec[1] * another.vec[1]) +
						(vec[2] * another.vec[2]);
			}
			static element_type dot(const Vector3D &a, const Vector3D &b)
			{
				return  (b.vec[0] * a.vec[0]) +
						(b.vec[1] * a.vec[1]) +
						(b.vec[2] * a.vec[2]);
			}
			Vector3D<element_type> cross(const Vector3D<element_type> &a) const
			{
				return Vector3D<element_type>((vec[1] * a.vec[2]) - (vec[2] * a.vec[1]),
					(vec[2] * a.vec[0]) - (vec[0] * a.vec[2]),
					(vec[0] * a.vec[1]) - (vec[1] * a.vec[0]));
			}
			static Vector3D<element_type> cross(const Vector3D<element_type> &a, const Vector3D<element_type> &b)
			{
				return Vector3D<element_type>((a.vec[1] * b.vec[2]) - (a.vec[2] * b.vec[1]),
					(a.vec[2] * b.vec[0]) - (a.vec[0] * b.vec[2]),
					(a.vec[0] * b.vec[1]) - (a.vec[1] * b.vec[0]));
			}
			void normalize()
			{
				element_type len = (element_type)sqrt((double)(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]));
				vec[0] /= len;
				vec[1] /= len;
				vec[2] /= len;
			}
			static Vector3D<element_type> normalize(const Vector3D<element_type> &a)
			{
				element_type len = (element_type)sqrt((double)(
						a.vec[0] * a.vec[0] + a.vec[1] * a.vec[1] + a.vec[2] * a.vec[2]));
				return Vector3D<element_type>(a.vec[0] / len, a.vec[1] / len, a.vec[2] / len);
			}
			Vector3D<element_type> operator+(const Vector3D<element_type> &a) const
			{
				return Vector3D<element_type>(vec[0] + a.vec[0], vec[1] + a.vec[1], vec[2] + a.vec[2]);
			}
			Vector3D<element_type> operator-(const Vector3D<element_type> &a) const
			{
				return Vector3D<element_type>(vec[0] - a.vec[0], vec[1] - a.vec[1], vec[2] - a.vec[2]);
			}
			Vector3D<element_type> operator*(const Vector3D<element_type> &a) const
			{
				return Vector3D<element_type>(vec[0] * a.vec[0], vec[1] * a.vec[1], vec[2] * a.vec[2]);
			}
			Vector3D<element_type> operator*(const element_type &a) const
			{
				return Vector3D<element_type>(vec[0] * a, vec[1] * a, vec[2] * a);
			}
			Vector3D<element_type> operator/(const Vector3D<element_type> &a) const
			{
				return Vector3D<element_type>(vec[0] / a.vec[0], vec[1] / a.vec[1], vec[2] / a.vec[2]);
			}
			void operator+=(const Vector3D<element_type> &a)
			{
				vec[0] += a.vec[0];
				vec[1] += a.vec[1];
				vec[2] += a.vec[2];
			}
			void operator-=(const Vector3D<element_type> &a)
			{
				vec[0] -= a.vec[0];
				vec[1] -= a.vec[1];
				vec[2] -= a.vec[2];
			}
			void operator*=(const Vector3D<element_type> &a)
			{
				vec[0] *= a.vec[0];
				vec[1] *= a.vec[1];
				vec[2] *= a.vec[2];
			}
			void operator/=(const Vector3D<element_type> &a)
			{
				vec[0] /= a.vec[0];
				vec[1] /= a.vec[1];
				vec[2] /= a.vec[2];
			}
			Vector3D<element_type> operator-() const
			{
				return Vector3D<element_type>(vec[0], vec[1], vec[2]);
			}
			Matrix4D<element_type> createRotationMatrix(const element_type &degree) const
			{
				element_type sinus = element_type(sin(degree));
				element_type cosinus = element_type(cos(degree));
				element_type oneminuscos = element_type(1.0 - cosinus);
				Vector3D<element_type> w = Vector3D<element_type>::normalize(*this);
				element_type wx2 = w.vec[0] * w.vec[0];
				element_type wxy = w.vec[0] * w.vec[1];
				element_type wxz = w.vec[0] * w.vec[2];
				element_type wy2 = w.vec[1] * w.vec[1];
				element_type wyz = w.vec[1] * w.vec[2];
				element_type wz2 = w.vec[2] * w.vec[2];
				element_type wxyonemincos = wxy * oneminuscos;
				element_type wxzonemincos = wxz * oneminuscos;
				element_type wyzonemincos = wyz * oneminuscos;
				element_type wxsin = w.vec[0] * sinus;
				element_type wysin = w.vec[1] * sinus;
				element_type wzsin = w.vec[2] * sinus;
				Matrix4D<element_type> m;
				m.mat[0 ] = cosinus + (wx2 * oneminuscos);
				m.mat[1 ] = wxyonemincos - wzsin;
				m.mat[2 ] = wysin + wxzonemincos;
				m.mat[3 ] = element_type(0.0);
				m.mat[4 ] = wzsin + wxyonemincos;
				m.mat[5 ] = cosinus + (wy2 * oneminuscos);
				m.mat[6 ] = wyzonemincos - wxsin;
				m.mat[7 ] = element_type(0.0);
				m.mat[8 ] = wxzonemincos - wysin;
				m.mat[9 ] = wxsin + wyzonemincos;
				m.mat[10] = cosinus + (wz2 * oneminuscos);
				m.mat[11] = element_type(0.0);
				m.mat[12] = element_type(0.0);
				m.mat[13] = element_type(0.0);
				m.mat[14] = element_type(0.0);
				m.mat[15] = element_type(1.0);
				return m;
			}
			element_type length()
			{
				return element_type(sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]));
			}
		};

		template<typename element_type>
		class Vector4D
		{
		public:
			element_type vec [4];
			Vector4D(element_type x, element_type y, element_type z,  element_type w)
			{
				vec[0] = x;
				vec[1] = y;
				vec[2] = z;
				vec[3] = w;
			}
			Vector4D(element_type e)
			{
				vec[0] = e;
				vec[1] = e;
				vec[2] = e;
				vec[3] = e;
			}
			element_type dot(const Vector4D &another) const
			{
				return  (vec[0] * another.vec[0]) +
						(vec[1] * another.vec[1]) +
						(vec[2] * another.vec[2]) +
						(vec[3] * another.vec[3]);
			}
			static element_type dot(const Vector4D &a, const Vector4D &b)
			{
				return  (b.vec[0] * a.vec[0]) +
						(b.vec[1] * a.vec[1]) +
						(b.vec[2] * a.vec[2]) +
						(b.vec[3] * a.vec[3]);
			}
			Vector4D<element_type> cross(const Vector4D<element_type> &a) const
			{
				return Vector4D<element_type>((vec[1] * a.vec[2]) - (vec[2] * a.vec[1]),
					(vec[2] * a.vec[0]) - (vec[0] * a.vec[2]),
					(vec[0] * a.vec[1]) - (vec[1] * a.vec[0]),
					vec[3] * a.vec[3]);
			}
			static Vector4D<element_type> cross(const Vector4D<element_type> &a, const Vector4D<element_type> &b)
			{
				return Vector4D<element_type>((a.vec[1] * b.vec[2]) - (a.vec[2] * b.vec[1]),
					(a.vec[2] * b.vec[0]) - (a.vec[0] * b.vec[2]),
					(a.vec[0] * b.vec[1]) - (a.vec[1] * b.vec[0]),
					a.vec[3] * b.vec[3]);
			}
			void normalize()
			{
				element_type len = (element_type)sqrt((double)(
						vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2] + vec[3] * vec[3]));
				vec[0] /= len;
				vec[1] /= len;
				vec[2] /= len;
				vec[3] /= len;
			}
			static Vector4D<element_type> normalize(const Vector4D<element_type> &a)
			{
				element_type len = (element_type)sqrt((double)(
						a.vec[0] * a.vec[0] + a.vec[1] * a.vec[1] + a.vec[2] * a.vec[2] + a.vec[3] * a.vec[3]));
				return Vector4D<element_type>(a.vec[0] / len, a.vec[1] / len, a.vec[2] / len, a.vec[3] / len);
			}
		};

		template<typename element_type>
		class Matrix4D : public core::Serializable
		{
		public:
			element_type mat [16];
			Matrix4D(element_type e)
			{
				mat[0 ] = e;
				mat[1 ] = element_type(0);
				mat[2 ] = element_type(0);
				mat[3 ] = element_type(0);
				mat[4 ] = element_type(0);
				mat[5 ] = e;
				mat[6 ] = element_type(0);
				mat[7 ] = element_type(0);
				mat[8 ] = element_type(0);
				mat[9 ] = element_type(0);
				mat[10] = e;
				mat[11] = element_type(0);
				mat[12] = element_type(0);
				mat[13] = element_type(0);
				mat[14] = element_type(0);
				mat[15] = e;
			}
			Matrix4D()
			{
				mat[0] = element_type(1);
				mat[1] = element_type(0);
				mat[2] = element_type(0);
				mat[3] = element_type(0);
				mat[4] = element_type(0);
				mat[5] = element_type(1);
				mat[6] = element_type(0);
				mat[7] = element_type(0);
				mat[8] = element_type(0);
				mat[9] = element_type(0);
				mat[10] = element_type(1);
				mat[11] = element_type(0);
				mat[12] = element_type(0);
				mat[13] = element_type(0);
				mat[14] = element_type(0);
				mat[15] = element_type(1);
			}
			Matrix4D(const Matrix4D<element_type> &m)
			{
				for(int i = 0; i < 16; i++)
				{
					mat[i] = m.mat[i];
				}
			}
			/// WARNING: It is not tested yet!
			static Matrix4D<element_type> lookAt(
					const Vector3D<element_type> &position,
					const Vector3D<element_type> &target,
					const Vector3D<element_type> &up)
			{
				Vector3D<element_type> z = (target - position);
				z.normalize();
				Vector3D<element_type> x = up.cross(z);
				x.normalize();
				Vector3D<element_type> y = z.cross(x);
				Matrix4D<element_type> m;
				m.mat[0 ] = x.vec[0];
				m.mat[1 ] = x.vec[1];
				m.mat[2 ] = x.vec[2];
				m.mat[3 ] = -x.dot(position);
				m.mat[4 ] = y.vec[0];
				m.mat[5 ] = y.vec[1];
				m.mat[6 ] = y.vec[2];
				m.mat[7 ] = -y.dot(position);
				m.mat[8 ] = z.vec[0];
				m.mat[9 ] = z.vec[1];
				m.mat[10] = z.vec[2];
				m.mat[11] = -z.dot(position);
				m.mat[12] = element_type(0.0);
				m.mat[13] = element_type(0.0);
				m.mat[14] = element_type(0.0);
				m.mat[15] = element_type(1.0);
				return m;
			}
			static Matrix4D<element_type> translate(const Matrix4D<element_type> &m, const Vector3D<element_type> &v)
			{
				Matrix4D<element_type> r;
				for(unsigned int i = 0; i < 16; i++)
				{
					r.mat[i] = m.mat[i];
				}
				r.mat[3 ] += v.vec[0];
				r.mat[7 ] += v.vec[1];
				r.mat[11] += v.vec[2];
				return r;
			}
			static Matrix4D<element_type> translate(const Matrix4D<element_type> &m, const Vector4D<element_type> &v)
			{
				Matrix4D<element_type> r;
				for(unsigned int i = 0; i < 16; i++)
				{
					r.mat[i] = m.mat[i];
				}
				r.mat[3 ] += v.vec[0];
				r.mat[7 ] += v.vec[1];
				r.mat[11] += v.vec[2];
				return r;
			}
			void translate(const Vector3D<element_type> &v)
			{
				mat[3 ] += v.vec[0];
				mat[7 ] += v.vec[1];
				mat[11] += v.vec[2];
			}
			void translate(const Vector4D<element_type> &v)
			{
				mat[3 ] += v.vec[0];
				mat[7 ] += v.vec[1];
				mat[11] += v.vec[2];
			}
			Vector3D<element_type> operator*(const Vector3D<element_type> &v) const
			{
				return Vector3D<element_type>(mat[0] * v.vec[0] + mat[1] * v.vec[1] + mat[2 ] * v.vec[2] + mat[3 ],
											  mat[4] * v.vec[0] + mat[5] * v.vec[1] + mat[6 ] * v.vec[2] + mat[7 ],
											  mat[8] * v.vec[0] + mat[9] * v.vec[1] + mat[10] * v.vec[2] + mat[11]);
			}
			Vector4D<element_type> operator*(const Vector4D<element_type> &v) const
			{
				return Vector4D<element_type>(mat[0 ] * v.vec[0] + mat[1 ] * v.vec[1] + mat[2 ] * v.vec[2] + mat[3 ] * v.vec[3],
											  mat[4 ] * v.vec[0] + mat[5 ] * v.vec[1] + mat[6 ] * v.vec[2] + mat[7 ] * v.vec[3],
											  mat[8 ] * v.vec[0] + mat[9 ] * v.vec[1] + mat[10] * v.vec[2] + mat[11] * v.vec[3],
											  mat[12] * v.vec[0] + mat[13] * v.vec[1] + mat[14] * v.vec[2] + mat[15] * v.vec[3]);
			}
			Matrix4D<element_type> operator*(const Matrix4D<element_type> &m) const
			{
				Matrix4D<element_type> r;
				for(int i = 0, ri = 0; i < 16; i += 4)
				{
					for(int j = 0; j < 4; j++, ri++)
					{
						r.mat[ri] = mat[i    ] * m.mat[j     ] +
									mat[i + 1] * m.mat[j + 4 ] +
									mat[i + 2] * m.mat[j + 8 ] +
									mat[i + 3] * m.mat[j + 12];
					}
				}
				return r;
//				r.mat[0 ] = mat[0] * m.mat[0] + mat[1] * m.mat[4] + mat[2] * m.mat[8 ] + mat[3] * m.mat[12];
//				r.mat[1 ] = mat[0] * m.mat[1] + mat[1] * m.mat[5] + mat[2] * m.mat[9 ] + mat[3] * m.mat[13];
//				r.mat[2 ] = mat[0] * m.mat[2] + mat[1] * m.mat[6] + mat[2] * m.mat[10] + mat[3] * m.mat[14];
//				r.mat[3 ] = mat[0] * m.mat[3] + mat[1] * m.mat[7] + mat[2] * m.mat[11] + mat[3] * m.mat[15];
//				r.mat[4 ] = mat[] * m.mat[] + mat[] * m.mat[] + mat[] * m.mat[] + mat[] * m.mat[];
//				r.mat[5 ] = mat[] * m.mat[] + mat[] * m.mat[] + mat[] * m.mat[] + mat[] * m.mat[];
//				r.mat[6 ] = mat[] * m.mat[] + mat[] * m.mat[] + mat[] * m.mat[] + mat[] * m.mat[];
//				r.mat[7 ] = mat[] * m.mat[] + mat[] * m.mat[] + mat[] * m.mat[] + mat[] * m.mat[];
//				r.mat[8 ] = mat[] * m.mat[] + mat[] * m.mat[] + mat[] * m.mat[] + mat[] * m.mat[];
//				r.mat[9 ] = mat[] * m.mat[] + mat[] * m.mat[] + mat[] * m.mat[] + mat[] * m.mat[];
//				r.mat[10] = mat[] * m.mat[] + mat[] * m.mat[] + mat[] * m.mat[] + mat[] * m.mat[];
//				r.mat[11] = mat[] * m.mat[] + mat[] * m.mat[] + mat[] * m.mat[] + mat[] * m.mat[];
//				r.mat[12] = mat[] * m.mat[] + mat[] * m.mat[] + mat[] * m.mat[] + mat[] * m.mat[];
//				r.mat[13] = mat[] * m.mat[] + mat[] * m.mat[] + mat[] * m.mat[] + mat[] * m.mat[];
//				r.mat[14] = mat[] * m.mat[] + mat[] * m.mat[] + mat[] * m.mat[] + mat[] * m.mat[];
//				r.mat[15] = mat[] * m.mat[] + mat[] * m.mat[] + mat[] * m.mat[] + mat[] * m.mat[];
			}
			void operator=(const Matrix4D<element_type> &m)
			{
				for(int i = 0; i < 16; i++)
				{
					mat[i] = m.mat[i];
				}
			}
			void scale(const element_type &f)
			{
				for(unsigned int i = 0; i < 16; i++)
				{
					mat[i] *= f;
				}
			}
			static Matrix4D<element_type> perspective(
					const element_type &width,
					const element_type &height,
					const element_type &near,
					const element_type &far)
			{
				Matrix4D<element_type> r;
				r.mat[0 ] = element_type((2.0 * near) / width);
				r.mat[1 ] = element_type(0.0);
				r.mat[2 ] = element_type(0.0);
				r.mat[3 ] = element_type(0.0);
				r.mat[4 ] = element_type(0.0);
				r.mat[5 ] = element_type((2.0 * near) / height);
				r.mat[6 ] = element_type(0.0);
				r.mat[7 ] = element_type(0.0);
				r.mat[8 ] = element_type(0.0);
				r.mat[9 ] = element_type(0.0);
				r.mat[10] = element_type((far + near) / (near - far));
				r.mat[11] = element_type((2.0 * far * near) / (near - far));
				r.mat[12] = element_type(0.0);
				r.mat[13] = element_type(0.0);
				r.mat[14] = element_type(-1.0);
				r.mat[15] = element_type(0.0);
				return r;
			}
			void writeToFile(QFile &file)
			{
				file.write((char *)(mat), sizeof(mat));
			}
			void readFromFile(QFile &file)
			{
				file.read((char *)(mat), sizeof(mat));
			}
		};
	}
}
#endif
