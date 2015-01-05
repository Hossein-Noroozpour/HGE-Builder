#ifndef HGE_RENDER_HGEMESHUNIT_HPP
#define HGE_RENDER_HGEMESHUNIT_HPP
#include "hge-data-object.hpp"
namespace hge
{
	namespace render
	{
		class MeshUnit :
			public core::DataObject
		{
		private:
			core::Protocol::Types::VertexElementType *vertices;
			core::Protocol::Types::VerticesElementsCountType verticesElementsCount;
			core::Protocol::Types::IndexElementType *indices;
			core::Protocol::Types::IndicesCountType indicesCount;
			core::Protocol::Types::IdType id;
			core::Supplier *supplier;
		public:
			MeshUnit(
				core::Protocol::Types::VertexElementType *const &vertices,
				const core::Protocol::Types::VerticesElementsCountType &verticesElementsCount,
				core::Protocol::Types::IndexElementType *const &indices,
				const core::Protocol::Types::IndicesCountType &indicesCount);
			~MeshUnit();
			// serializable part
			void writeToFile(QFile &file);
			void readFromFile(QFile &file);
			// end of serializable
			// data object part
			void setSupplier(hge::core::Supplier *const &supplier);
			hge::core::Supplier *getSupplier();
			hge::core::Protocol::Types::ObjectTypeIdType getTypeId();
			void setDataId(const hge::core::Protocol::Types::IdType &id);
			hge::core::Protocol::Types::IdType getDataId();
			// end of data object part
		};
	}
}
#endif