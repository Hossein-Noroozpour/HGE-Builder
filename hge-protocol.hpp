#ifndef HGEPROTOCOL_HPP
#define HGEPROTOCOL_HPP
#include <QtCore/QObject>
namespace hge
{
	namespace core
	{
		class Protocol
		{
		public:
			class Types
			{
			public:
				typedef quint8 HgeBoolean;
				typedef quint16 TerrainAspectType;
				typedef quint8  TerrainVboComponentsCountType;
				typedef quint32 IdType;
				typedef quint16 ObjectTypeIdType;
				typedef quint16 InstructionsType;
				typedef quint32 ObjectSizeType;
				typedef float VertexElementType;
				typedef quint32 IndexElementType;
				typedef quint32 VerticesElementsCountType;
				typedef quint32 IndicesCountType;
				typedef quint16 StringLengthType;
			};
			class ObjectTypes
			{
			public:
				const static Types::ObjectTypeIdType Terrain;
				const static Types::ObjectTypeIdType Geometry;
				const static Types::ObjectTypeIdType SkyBox;
				const static Types::ObjectTypeIdType Mesh;
			};
			class Values
			{
			public:
				const static Types::HgeBoolean HgeFalse;
				const static Types::HgeBoolean HgeTrue;
			};
		};
	}
}
#endif
