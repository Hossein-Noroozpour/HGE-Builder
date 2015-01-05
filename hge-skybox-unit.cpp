#include "hge-skybox-unit.hpp"
#include "hge-supplier.hpp"
hge::render::SkyBoxUnit::SkyBoxUnit(const QSharedPointer<GeometryUnit> &geometryPT, const math::Matrix4D<> &matrix):
id(0),
supplier(core::Supplier::defaultSupplier),
geometryPT(geometryPT)
{
	geometryPT->setMatrix(matrix);
}
void hge::render::SkyBoxUnit::setSupplier(core::Supplier *const &supplier)
{
	this->supplier = supplier;
}
hge::core::Supplier *hge::render::SkyBoxUnit::getSupplier()
{
	return supplier;
}
hge::core::Protocol::Types::ObjectTypeIdType hge::render::SkyBoxUnit::getTypeId()
{
	return core::Protocol::ObjectTypes::SkyBox;
}
void hge::render::SkyBoxUnit::setDataId(const core::Protocol::Types::IdType &id)
{
	this->id = id;
}
hge::core::Protocol::Types::IdType hge::render::SkyBoxUnit::getDataId()
{
	return id;
}
