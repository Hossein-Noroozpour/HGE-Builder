#ifndef HGETERRAIN_HPP
#define HGETERRAIN_HPP
#include "hge-traceable.hpp"
#include "hge-data-object.hpp"
namespace hge
{
	namespace render
	{
		class Terrain : public core::Traceable, public core::DataObject
		{
		private:
			core::Protocol::Types::TerrainAspectType aspect;
			core::Protocol::Types::VertexElementType *vbo;
			const static core::Protocol::Types::TerrainVboComponentsCountType vbo_components_count;
			core::Protocol::Types::IdType id;
			core::Supplier *supplier;
			core::Profiler *profiler;
			void calculate();
		public:
			Terrain(
					const unsigned char *const &pixels, const int &line_bytes, const int &aspect,
					const float &x_scale, const float &y_scale, const float &z_scale,
					const float &x_start, const float &y_start, const float &z_start,
					core::Supplier *const &supplier,
					core::Profiler *const &profiler);
			Terrain(core::Supplier *const &supplier, core::Profiler *const &profiler);
			~Terrain();
			// tracable part
			void setProfiler(core::Profiler *const &profiler);
			core::Profiler *getProfiler(void);
			// end of tracable part
			// data object part
			void setSupplier(core::Supplier *const &supplier);
			core::Supplier *getSupplier();
			core::Protocol::Types::ObjectTypeIdType getTypeId();
			void setDataId(const core::Protocol::Types::IdType &id);
			core::Protocol::Types::IdType getDataId();
			// end of data object part
			// serializable part
			void writeToFile(QFile &file);
			void readFromFile(QFile &file);
			// end of serializable part
		};
	}
}
#endif
