#ifndef HGE_RENDER_HGEGEOMETRYUNIT_HPP
#define HGE_RENDER_HGEGEOMETRYUNIT_HPP
#include <QtCore/QString>
#include <QtCore/QSharedPointer>
#include "hge-mesh-unit.hpp"
#include "hge-math.hpp"
namespace hge
{
	namespace render
	{
		class GeometryUnit : public core::DataObject
		{
		private:
			QString idString;
			QString name;
			QSharedPointer<MeshUnit> mesh;
			QSharedPointer<MeshUnit> occlusionTestMesh;
			core::Protocol::Types::IdType id;
			math::Matrix4D<> matrix;
			math::Matrix4D<> occlusionTestMatrix;
			core::Supplier *supplier;
		public:
			GeometryUnit(const QString &id, const QString &name);
			~GeometryUnit();
			// serializable part
			void writeToFile(QFile &file);
			void readFromFile(QFile &file);
			// end of serializable part
			// data object inherited part
			void setSupplier(core::Supplier *const &supplier);
			core::Supplier *getSupplier();
			core::Protocol::Types::ObjectTypeIdType getTypeId();
			void setDataId(const core::Protocol::Types::IdType &id);
			core::Protocol::Types::IdType getDataId();
			// end of data object inherited part
			void setOcclusionQueryMesh(const QSharedPointer<MeshUnit> &mesh);
			void setMesh(const QSharedPointer<MeshUnit> &mesh);
			const QSharedPointer<MeshUnit> getMesh();
			void setMatrix(const math::Matrix4D<> &matrix);
			void setOcclusionTestMatrix(const math::Matrix4D<> &matrix);
		};
	}
}
#endif
