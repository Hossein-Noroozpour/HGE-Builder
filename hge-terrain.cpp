#include "hge-terrain.hpp"
#include "hge-profiler.hpp"
#include <iostream>
#include "hge-math.hpp"
const hge::core::Protocol::Types::TerrainVboComponentsCountType hge::render::Terrain::vbo_components_count = 12;
hge::render::Terrain::Terrain(const unsigned char *const &pixels, const int &line_bytes,
							  const int &aspect, const float &x_scale, const float &y_scale, const float &z_scale,
							  const float &x_start, const float &y_start, const float &z_start, core::Supplier *const &supplier,
							  core::Profiler *const &profiler) :
	aspect(aspect),
	supplier(supplier),
	profiler(profiler)
{
	int pixel_components = line_bytes / aspect;
	int blank = 0;
	if(pixel_components * aspect != line_bytes)
	{
		blank = line_bytes - pixel_components * aspect;
	}
	vbo = new float[aspect * aspect * vbo_components_count];
	for(int i = 0, index = 0, vbo_index = 0; i < aspect; i++, index += blank)
	{
		for(int j = 0; j < aspect; j++, vbo_index += vbo_components_count)
		{
			float height = 0.0f;
			for(int c = 0; c < pixel_components; c++, index++)
			{
				long int byte_index = 1 << (c * 8);
				height += float(byte_index) * float(pixels[index]);
			}
			vbo[vbo_index + 0] = x_start + x_scale * float(j);
			vbo[vbo_index + 1] = y_start + y_scale * float(i);
			vbo[vbo_index + 2] = z_start + z_scale * height;
		}
	}
	calculate();
	/// TODO: Update profiler about its state
}
hge::render::Terrain::Terrain(core::Supplier *const &supplier, core::Profiler *const &profiler):
	aspect(0),
	vbo(0),
	supplier(supplier),
	profiler(profiler)
{}
hge::render::Terrain::~Terrain()
{
	if(vbo != 0)
	{
		delete [] vbo;
	}
	/// TODO: Update profiler about its state
	/// TODO: Update supplier about its state
}
void hge::render::Terrain::calculate()
{
#define get_index(ii, jj) (((ii * aspect) + jj) * vbo_components_count)
#define get_vector(i1, j1, i2, j2) math::Vector3D<>(\
	vbo[get_index(i1, j1)    ] - vbo[get_index(i2, j2)    ],\
	vbo[get_index(i1, j1) + 1] - vbo[get_index(i2, j2) + 1],\
	vbo[get_index(i1, j1) + 2] - vbo[get_index(i2, j2) + 2])

	for(int i = 0, vboIndex = 0; i < aspect; i++)
	{
		for(int j = 0; j < aspect; j++)
		{
			math::Vector3D<> nv(0.0f, 0.0f, 0.0f);
			if(i > 0)
			{
				if(j > 0)
				{
					math::Vector3D<> N  = get_vector((i - 1), (j    ), (i), (j));
					math::Vector3D<> W  = get_vector((i    ), (j - 1), (i), (j));
					math::Vector3D<> NW = get_vector((i - 1), (j - 1), (i), (j));
					nv += math::Vector3D<>::normalize(math::Vector3D<>::cross(N, NW));
					nv += math::Vector3D<>::normalize(math::Vector3D<>::cross(NW, W));
				}
				if(j < aspect - 1)
				{
					//std::cout << "j " << j << "  i " << i << std::endl << std::flush;
					math::Vector3D<> N  = get_vector((i - 1), (j    ), (i), (j));
					math::Vector3D<> NE = get_vector((i - 1), (j + 1), (i), (j));
					math::Vector3D<> E  = get_vector((i    ), (j + 1), (i), (j));
					nv += math::Vector3D<>::normalize(math::Vector3D<>::cross(NE, N));
					nv += math::Vector3D<>::normalize(math::Vector3D<>::cross(E, NE));
				}
			}
			if(i < aspect - 1)
			{
				if(j > 0)
				{
					math::Vector3D<> S  = get_vector((i + 1), (j    ), (i), (j));
					math::Vector3D<> SW = get_vector((i + 1), (j - 1), (i), (j));
					math::Vector3D<> W  = get_vector((i    ), (j - 1), (i), (j));
					nv += math::Vector3D<>::normalize(math::Vector3D<>::cross(SW, S));
					nv += math::Vector3D<>::normalize(math::Vector3D<>::cross(W, SW));
				}
				if(j < aspect - 1)
				{
					math::Vector3D<> S  = get_vector((i + 1), (j    ), (i), (j));
					math::Vector3D<> E  = get_vector((i    ), (j + 1), (i), (j));
					math::Vector3D<> SE = get_vector((i + 1), (j + 1), (i), (j));
					nv += math::Vector3D<>::normalize(math::Vector3D<>::cross(S, SE));
					nv += math::Vector3D<>::normalize(math::Vector3D<>::cross(SE, E));
				}
			}
#undef get_index
#undef get_vector
			nv = math::Vector3D<>::normalize(nv);
			math::Vector3D<> tv = math::Vector3D<>::normalize(math::Vector3D<>(0.0f, 1.0f, 0.0f) - (nv * (nv.vec[1])));
			math::Vector3D<> btv = math::Vector3D<>::cross(nv, tv);
			vboIndex += 3;
			vbo[vboIndex++] = nv.vec[0];
			vbo[vboIndex++] = nv.vec[1];
			vbo[vboIndex++] = nv.vec[2];
			vbo[vboIndex++] = tv.vec[0];
			vbo[vboIndex++] = tv.vec[1];
			vbo[vboIndex++] = tv.vec[2];
			vbo[vboIndex++] = btv.vec[0];
			vbo[vboIndex++] = btv.vec[1];
			vbo[vboIndex++] = btv.vec[2];
		}
	}
}
void hge::render::Terrain::setProfiler(core::Profiler *const &profiler)
{
	this->profiler = profiler;
}
hge::core::Profiler *hge::render::Terrain::getProfiler(void)
{
	return profiler;
}
void hge::render::Terrain::setSupplier(core::Supplier *const &supplier)
{
	this->supplier = supplier;
}
hge::core::Supplier *hge::render::Terrain::getSupplier()
{
	return supplier;
}
hge::core::Protocol::Types::ObjectTypeIdType hge::render::Terrain::getTypeId()
{
	return core::Protocol::ObjectTypes::Terrain;
}
void hge::render::Terrain::setDataId(const core::Protocol::Types::IdType &id)
{
	this->id = id;
}
hge::core::Protocol::Types::IdType hge::render::Terrain::getDataId()
{
	return this->id;
}
void hge::render::Terrain::writeToFile(QFile &file)
{
#define HGE_WRITE_BTYPE(t) file.write((char *)(&t), sizeof(t))
	HGE_WRITE_BTYPE(aspect);
	HGE_WRITE_BTYPE(id);
	HGE_WRITE_BTYPE(vbo_components_count);
	file.write((char *)(vbo), aspect * aspect * vbo_components_count * sizeof(core::Protocol::Types::VertexElementType));
#undef HGE_WRITE_BTYPE
}
void hge::render::Terrain::readFromFile(QFile &file)
{
	(void)file;
	/// \todo
}