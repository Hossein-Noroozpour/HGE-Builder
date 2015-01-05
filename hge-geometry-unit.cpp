#include "hge-geometry-unit.hpp"
#include "hge-supplier.hpp"
hge::render::GeometryUnit::GeometryUnit(const QString &id, const QString &name) :
idString(id),
name(name),
id(0),
supplier(core::Supplier::defaultSupplier)
{
}
hge::render::GeometryUnit::~GeometryUnit()
{}
void hge::render::GeometryUnit::writeToFile(QFile &file)
{
#define HGE_WRITE_BTYPE(t) file.write((char *)(&t), sizeof(t))
#define HGE_WRITE_QSTRING(s)\
	{\
		QByteArray str = s.toLocal8Bit();\
		core::Protocol::Types::StringLengthType len = str.length();\
		HGE_WRITE_BTYPE(len);\
		file.write(str);\
	}
	// id string
	HGE_WRITE_QSTRING(idString);
	// name
	HGE_WRITE_QSTRING(name);
	// id number
	HGE_WRITE_BTYPE(id);
	// mesh
	mesh->writeToFile(file);
	matrix.writeToFile(file);
#define HGE_WRITE_BOOL(b) core::Protocol::Types::HgeBoolean B = b; HGE_WRITE_BTYPE(B);
	if (occlusionTestMesh.isNull())
	{
		HGE_WRITE_BOOL(core::Protocol::Values::HgeFalse);
	}
	else
	{
		HGE_WRITE_BOOL(core::Protocol::Values::HgeTrue);
		occlusionTestMesh->writeToFile(file);
		occlusionTestMatrix.writeToFile(file);
	}
#undef HGE_WRITE_BOOL
#undef HGE_WRITE_BTYPE
#undef HGE_WRITE_QSTRING
}
void hge::render::GeometryUnit::readFromFile(QFile &file)
{
	(void)file;
	/// \todo
}
void hge::render::GeometryUnit::setSupplier(core::Supplier *const &supplier)
{
	this->supplier = supplier;
}
hge::core::Supplier *hge::render::GeometryUnit::getSupplier()
{
	return supplier;
}
hge::core::Protocol::Types::ObjectTypeIdType hge::render::GeometryUnit::getTypeId()
{
	return core::Protocol::ObjectTypes::Geometry;
}
void hge::render::GeometryUnit::setDataId(const core::Protocol::Types::IdType &id)
{
	this->id = id;
}
hge::core::Protocol::Types::IdType hge::render::GeometryUnit::getDataId()
{
	return id;
}
void hge::render::GeometryUnit::setOcclusionQueryMesh(const QSharedPointer<MeshUnit> &mesh)
{
	occlusionTestMesh = mesh;
}
void hge::render::GeometryUnit::setMesh(const QSharedPointer<MeshUnit> &mesh)
{
	this->mesh = mesh;
}
const QSharedPointer<hge::render::MeshUnit> hge::render::GeometryUnit::getMesh()
{
	return mesh;
}
void hge::render::GeometryUnit::setMatrix(const math::Matrix4D<> &matrix)
{
	this->matrix = matrix;
}
void hge::render::GeometryUnit::setOcclusionTestMatrix(const math::Matrix4D<> &matrix)
{
	occlusionTestMatrix = matrix;
}