#ifndef HGECOLLADAIMPORTER_HPP
#define	HGECOLLADAIMPORTER_HPP
#include <QtCore/QString>
#include <QtCore/QVector>
#include <QtCore/QSharedPointer>
#include <QtXml/QDomElement>
#include "hge-geometry-unit.hpp"
//#define HGECOLLADADEBUG
//#define HGECOLLADADEBUG1
//#define HGECOLLADADEBUG2
namespace hge
{
	namespace utility
	{
		class ColladaImporter
		{
		public:
			enum ColladaExceptions
			{
				FileCanNotBeOpened,
				ErrorInXMLFormat,
				MalformColladaFormat,
				MalformDataElements,
				NontrianglePolygon
			};
			static QVector<QSharedPointer<core::DataObject> > importFromFile(const QString &fileName);
		private:
			static const QString colladaTagKey;
			static const QString libraryGeometryTagKey;
			static const QString geometryTagKey;
			static const QString skyKey;
			static const QString occlusionTestKey;
			static const QString meshTagKey;
			static const QString nameAttributeKey;
			static const QString idAttributeKey;
			static const QString positionIdPsfxAttVal; /// Position ID postfix attribute value
			static const QString normalIdPsfxAttVal; /// Normal ID postfix attribute value
			static const QString mapIdPsfxAttVal; /// Map ID postfix attribute value
			static const QString sourceTagKey;
			static const QString polylistTagKey;
			static const QString floatArrayTagKey;
			static const QString countAttributeKey;
			static const QString vCountTagKey;
			static const QString pTagKey;

			static QVector<QSharedPointer<core::DataObject> > colladaNode(const QDomElement &node);
			static QVector<QSharedPointer<core::DataObject> > libraryGeometriesNode(const QDomElement &node);
			/// This function will be used for every thing that only positions,
			/// normals and texture coordinates is important for them.
			static QSharedPointer<render::GeometryUnit> geometryNodePNT(const QDomElement &node);
			/// This function will be used for occlusion query object and every
			/// thing that only position is important for them.
			static QSharedPointer<render::GeometryUnit> geometryNodeP(const QDomElement &node);
			/// This function will be used for sky box and every thing that only
			/// positions and texture coordinate is important for them.
			static QSharedPointer<render::GeometryUnit> geometryNodePT(const QDomElement &node);
			///\warning I assumed that all UVs are sorted.
			///\warning It does not support multichannel texture coordinating.
			///\todo Support multichannel texture coordinating.
			static QSharedPointer<render::MeshUnit> meshNodePNT(const QDomElement &node, const QString &geoid);
			static QSharedPointer<render::MeshUnit> meshNodePT(const QDomElement &node, const QString &geoid);
			static QSharedPointer<render::MeshUnit> meshNodeP(const QDomElement &node, const QString &geoid);
			static QVector<core::Protocol::Types::VertexElementType> sourceDataNode(const QDomElement &node);
			static QVector<core::Protocol::Types::IndexElementType> sourcePolyListNode(const QDomElement &node);
		};
	}
}
#endif
