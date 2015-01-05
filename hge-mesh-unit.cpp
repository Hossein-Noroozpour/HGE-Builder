#include "hge-mesh-unit.hpp"
#include <cstring>
#include "hge-supplier.hpp"
hge::render::MeshUnit::MeshUnit(core::Protocol::Types::VertexElementType *const &vertices,
	const core::Protocol::Types::VerticesElementsCountType &verticesElementsCount,
	core::Protocol::Types::IndexElementType *const &indices,
	const core::Protocol::Types::IndicesCountType &indicesCount) :
	vertices(vertices),
	verticesElementsCount(verticesElementsCount),
	indices(indices),
	indicesCount(indicesCount),
	supplier(core::Supplier::defaultSupplier),
	id(0)
{
}
hge::render::MeshUnit::~MeshUnit()
{
	delete[] indices;
	delete[] vertices;
}

void hge::render::MeshUnit::setSupplier(core::Supplier *const &supplier)
{
	this->supplier = supplier;
}
hge::core::Supplier *hge::render::MeshUnit::getSupplier()
{
	return supplier;
}
hge::core::Protocol::Types::ObjectTypeIdType hge::render::MeshUnit::getTypeId()
{
	return core::Protocol::ObjectTypes::Mesh;
}
void hge::render::MeshUnit::setDataId(const core::Protocol::Types::IdType &id)
{
	this->id = id;
}
hge::core::Protocol::Types::IdType hge::render::MeshUnit::getDataId()
{
	return id;
}
void hge::render::MeshUnit::writeToFile(QFile &file)
{
#define HGE_WRITE_BTYPE(t) file.write((char *)(&t), sizeof(t))
	HGE_WRITE_BTYPE(verticesElementsCount);
	file.write((char *)(vertices), sizeof(core::Protocol::Types::VertexElementType) * verticesElementsCount);
	HGE_WRITE_BTYPE(indicesCount);
	file.write((char *)(indices), sizeof(core::Protocol::Types::IndexElementType) * indicesCount);
#undef HGE_WRITE_BTYPE
}
void hge::render::MeshUnit::readFromFile(QFile &file)
{
	(void)file;
	/// \todo
}