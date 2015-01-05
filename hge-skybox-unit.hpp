#ifndef HGE_RENDER_HGESKYBOXUNIT_HPP
#define HGE_RENDER_HGESKYBOXUNIT_HPP
#include <QtCore/QSharedPointer>
#include "hge-geometry-unit.hpp"
#include "hge-math.hpp"
namespace hge
{
	namespace render
	{
		class SkyBoxUnit : public core::DataObject
		{
		private:
			core::Protocol::Types::IdType id;
			QSharedPointer<GeometryUnit> geometryPT;
			core::Supplier *supplier;
		public:
			SkyBoxUnit(const QSharedPointer<GeometryUnit> &geometryPT, const math::Matrix4D<> &matrix);
			~SkyBoxUnit();
			core::Protocol::Types::ObjectSizeType getDataSize();
			char *getData();
			void setData(const core::Protocol::Types::ObjectSizeType &dataSize, const char *const &data);
			void setSupplier(core::Supplier *const &supplier);
			core::Supplier *getSupplier();
			core::Protocol::Types::ObjectTypeIdType getTypeId();
			void setDataId(const core::Protocol::Types::IdType &id);
			core::Protocol::Types::IdType getDataId();
		};
	}
}
#endif
