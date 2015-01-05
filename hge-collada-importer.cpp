#include "hge-collada-importer.hpp"
#include "hge-skybox-unit.hpp"
#include <QtCore/QFile>
#include <QtCore/QSet>
#include <QtCore/QDebug>
#include <QtWidgets/QMessageBox>
#define HGEPRINTCODELINE qDebug() << "Debugging: file:" << __FILE__ << " line:" << __LINE__;
#define HGESEEN(item) qDebug() << "I've seen (a/an) " << item; HGEPRINTCODELINE
#define HGECOLLADAMAXVERTEXDATAELEMENTS 8
const QString hge::utility::ColladaImporter::colladaTagKey("COLLADA");
const QString hge::utility::ColladaImporter::libraryGeometryTagKey("library_geometries");
const QString hge::utility::ColladaImporter::geometryTagKey("geometry");
const QString hge::utility::ColladaImporter::meshTagKey("mesh");
const QString hge::utility::ColladaImporter::nameAttributeKey("name");
const QString hge::utility::ColladaImporter::idAttributeKey("id");
const QString hge::utility::ColladaImporter::skyKey("SkyBox");
const QString hge::utility::ColladaImporter::occlusionTestKey("OcclusionTest");
const QString hge::utility::ColladaImporter::positionIdPsfxAttVal("-positions");
const QString hge::utility::ColladaImporter::normalIdPsfxAttVal("-normals");
const QString hge::utility::ColladaImporter::mapIdPsfxAttVal("-map-");
const QString hge::utility::ColladaImporter::sourceTagKey("source");
const QString hge::utility::ColladaImporter::polylistTagKey("polylist");
const QString hge::utility::ColladaImporter::floatArrayTagKey("float_array");
const QString hge::utility::ColladaImporter::countAttributeKey("count");
const QString hge::utility::ColladaImporter::vCountTagKey("vcount");
const QString hge::utility::ColladaImporter::pTagKey("p");
class VertexData
{
public:
	float data[HGECOLLADAMAXVERTEXDATAELEMENTS];
	int maxElements = HGECOLLADAMAXVERTEXDATAELEMENTS;
	QVector<quint32> indices;
};
static inline bool operator == (const VertexData &v1, const VertexData &v2)
{
	int maxElements = v1.maxElements;
	if (v2.maxElements < maxElements) maxElements = v2.maxElements;
	for (int i = 0; i < maxElements; i++)
	{
		if (v1.data[i] != v2.data[i]) return false;
	}
	return true;
}
static inline uint qHash(const VertexData &key, const uint seed = 0)
{
	QByteArray bytes((const char *)(key.data), key.maxElements * sizeof(float));
	return ::qHash(bytes) + seed;
}
QVector<QSharedPointer<hge::core::DataObject> > hge::utility::ColladaImporter::importFromFile(const QString &fileName)
{
	QDomDocument colladaDoc(QObject::tr("ColladaDocument"));
	QFile colladaFile(fileName);
	if (!colladaFile.open(QFile::ReadOnly))
	{
		QMessageBox msgBox;
		msgBox.setText(QObject::tr("Failure in file openning procedure."));
		msgBox.setInformativeText(QObject::tr("The Collada file you have specified can not be opened!"));
		msgBox.setStandardButtons(QMessageBox::Close);
		msgBox.exec();
		throw FileCanNotBeOpened;
	}
	{
		QString errorMessage;
		int errorLineNumber;
		int errorColumnNumber;
		if (!colladaDoc.setContent(colladaFile.readAll(), false, &errorMessage, &errorLineNumber, &errorColumnNumber))
		{
			QMessageBox msgBox;
			msgBox.setText(QObject::tr("Collada XML error!"));
			msgBox.setInformativeText(errorMessage + QObject::tr(" in line:") + errorLineNumber + QObject::tr(" in column:") + errorColumnNumber);
			msgBox.setStandardButtons(QMessageBox::Close);
			msgBox.exec();
			throw ErrorInXMLFormat;
		}
	}
	QDomElement root = colladaDoc.documentElement();
	if (root.tagName() != colladaTagKey)
	{
		throw MalformColladaFormat;
	}
#ifdef HGECOLLADADEBUG
	HGESEEN(colladaTagKey);
#endif
		return colladaNode(root);
}
QVector<QSharedPointer<hge::core::DataObject> > hge::utility::ColladaImporter::colladaNode(const QDomElement &node)
{
	QVector<QSharedPointer<hge::core::DataObject> > result;
	for (QDomNode child = node.firstChild(); !child.isNull(); child = child.nextSibling())
	{
		if (child.toElement().tagName() == libraryGeometryTagKey)
		{
#ifdef HGECOLLADADEBUG
			HGESEEN(libraryGeometryTagKey)
#endif
				result += libraryGeometriesNode(child.toElement());
		}
	}
	return result;
}
QVector<QSharedPointer<hge::core::DataObject> > hge::utility::ColladaImporter::libraryGeometriesNode(const QDomElement &node)
{
	QVector<QSharedPointer<hge::core::DataObject> > result;
	for (QDomNode child = node.firstChild(); !child.isNull(); child = child.nextSibling())
	{
		if (geometryTagKey == child.toElement().tagName())
		{
			const QString namestr = child.toElement().attribute(nameAttributeKey);
#ifdef HGECOLLADADEBUG
			HGESEEN(geometryTagKey)
#endif
				if (namestr.startsWith(skyKey))
				{
#ifdef HGECOLLADADEBUG
				HGESEEN(skyKey)
#endif
					QSharedPointer<core::DataObject> geo = geometryNodePT(child.toElement());
				/// \todo QSharedPointer<render::SkyBoxUnit> sky(new render::SkyBoxUnit(geo->getMesh()));
				/// \todo scene->addSky(sky);
				}
				else if (!namestr.startsWith(occlusionTestKey))
				{
					QSharedPointer<render::GeometryUnit> geo = geometryNodePNT(child.toElement());
					const QString occQryStr = occlusionTestKey + namestr;
					for (QDomNode occchild = node.firstChild(); !occchild.isNull(); occchild = occchild.nextSibling())
					{
						if (geometryTagKey == occchild.toElement().tagName())
						{
							if (occQryStr == occchild.toElement().attribute(nameAttributeKey))
							{
								QSharedPointer<render::GeometryUnit> occgeo = geometryNodeP(occchild.toElement());
								geo->setOcclusionQueryMesh(occgeo->getMesh());
								break;
							}
						}
					}
					result.push_back(geo);
				}
		}
	}
	return result;
}
QSharedPointer<hge::render::GeometryUnit> hge::utility::ColladaImporter::geometryNodePNT(const QDomElement &node)
{
	const QString idstr(node.attribute(idAttributeKey));
	const QString namestr(node.attribute(nameAttributeKey));
	QSharedPointer<render::GeometryUnit> result(new render::GeometryUnit(idstr, namestr));
	for (QDomNode child = node.firstChild(); !child.isNull(); child = child.nextSibling())
	{
		if (meshTagKey == child.toElement().tagName())
		{
#ifdef HGECOLLADADEBUG
			HGESEEN("mesh for position, normal and texture coordinate.")
#endif
				QSharedPointer<render::MeshUnit> mesh = meshNodePNT(child.toElement(), idstr);
			result->setMesh(mesh);
		}
	}
	return result;
}
QSharedPointer<hge::render::GeometryUnit> hge::utility::ColladaImporter::geometryNodeP(const QDomElement &node)
{
	const QString idstr(node.attribute(idAttributeKey));
	const QString namestr(node.attribute(nameAttributeKey));
	QSharedPointer<render::GeometryUnit> result(new render::GeometryUnit(idstr, namestr));
	for (QDomNode child = node.firstChild(); !child.isNull(); child = child.nextSibling())
	{
		if (meshTagKey == child.toElement().tagName())
		{
#ifdef HGECOLLADADEBUG
			HGESEEN("mesh for position.")
#endif
				QSharedPointer<render::MeshUnit> mesh = meshNodeP(child.toElement(), idstr);
			result->setMesh(mesh);
		}
	}
	return result;
}
QSharedPointer<hge::render::GeometryUnit> hge::utility::ColladaImporter::geometryNodePT(const QDomElement &node)
{
	const QString idstr(node.attribute(idAttributeKey));
	const QString namestr(node.attribute(nameAttributeKey));
	QSharedPointer<render::GeometryUnit> result(new render::GeometryUnit(idstr, namestr));
	for (QDomNode child = node.firstChild(); !child.isNull(); child = child.nextSibling())
	{
		if (meshTagKey == child.toElement().tagName())
		{
#ifdef HGECOLLADADEBUG
			HGESEEN("mesh for position and texture coordinate.")
#endif
				QSharedPointer<render::MeshUnit> mesh = meshNodePT(child.toElement(), idstr);
			result->setMesh(mesh);
		}
	}
	return result;
}
QSharedPointer<hge::render::MeshUnit> hge::utility::ColladaImporter::meshNodePNT(const QDomElement &node, const QString &geoid)
{
	const quint32 vertexElements = 8;
	const QString positionStr = geoid + positionIdPsfxAttVal;
	const QString normalStr = geoid + normalIdPsfxAttVal;
	const QString uvStr = geoid + mapIdPsfxAttVal;
	QVector<core::Protocol::Types::VertexElementType> positions;
	QVector<core::Protocol::Types::VertexElementType> normals;
	QVector<QVector<core::Protocol::Types::VertexElementType> > uvs;
	QVector<core::Protocol::Types::IndexElementType> polylist;
	QSet<VertexData> vertexSet;
	for (QDomNode child = node.firstChild(); !child.isNull(); child = child.nextSibling())
	{
		if (sourceTagKey == child.toElement().tagName())
		{
			const QString idstr(child.toElement().attribute(idAttributeKey));
#ifdef HGECOLLADADEBUG
			HGESEEN(sourceTagKey);
#endif
			if (idstr == positionStr)
			{
				positions = sourceDataNode(child.toElement());
#ifdef HGECOLLADADEBUG1
				qDebug() << "Positions: ";
				foreach(core::Protocol::VertexElementType i, positions)
				{
					qDebug() << i;
				}
				HGEPRINTCODELINE;
#endif
			}
			else if (idstr == normalStr)
			{
				normals = sourceDataNode(child.toElement());
#ifdef HGECOLLADADEBUG1
				qDebug() << "Normals: ";
				foreach(core::Protocol::VertexElementType i, normals)
				{
					qDebug() << i;
				}
				HGEPRINTCODELINE;
#endif
			}
			else if (idstr.startsWith(uvStr))
			{
				/// \todo Check the order of UV in collada file, my approach in here is so blind.
				uvs.push_back(sourceDataNode(child.toElement()));
#ifdef HGECOLLADADEBUG1
				qDebug() << "UVs: ";
				foreach(core::Protocol::VertexElementType i, uvs[0])
				{
					qDebug() << i;
				}
				qDebug() << "Size of UVs: " << uvs[0].size();
				HGEPRINTCODELINE;
#endif
			}
		}
		else if (polylistTagKey == child.toElement().tagName())
		{
			polylist = sourcePolyListNode(child.toElement());
#ifdef HGECOLLADADEBUG1
			qDebug() << "polylist: ";
			foreach(core::Protocol::IndexElementType i, polylist)
			{
				qDebug() << i;
			}
			HGEPRINTCODELINE;
#endif
		}
	}
	quint32 indicesCount = 0;
	for (int i = 0, tmpint = 0; i < polylist.size(); indicesCount++)
	{
		VertexData tmpv;
		tmpv.maxElements = vertexElements;
		tmpint = polylist[i++] * 3;
		tmpv.data[0] = positions[tmpint++];
		tmpv.data[1] = positions[tmpint++];
		tmpv.data[2] = positions[tmpint];
		tmpint = polylist[i++] * 3;
		tmpv.data[3] = normals[tmpint++];
		tmpv.data[4] = normals[tmpint++];
		tmpv.data[5] = normals[tmpint];
		tmpint = polylist[i++] * 2;
		tmpv.data[6] = uvs[0][tmpint++];
		tmpv.data[7] = uvs[0][tmpint];
		if (!vertexSet.contains(tmpv))
		{
			tmpv.indices.push_back(indicesCount);
			vertexSet.insert(tmpv);
		}
		else
		{
			const_cast<QVector<quint32>*>(&(vertexSet.find(tmpv)->indices))->push_back(indicesCount);
		}
	}
#ifdef HGECOLLADADEBUG
	qDebug() << "Vertices size: " << vertexSet.size();
	qDebug() << "Indices size: " << indicesCount;
	HGEPRINTCODELINE;
#endif
#ifdef HGECOLLADADEBUG1
	foreach (VertexData i, vertexSet)
	{
		qDebug() << "VD: " << i.data[0] << i.data[1] << i.data[2] << i.data[3] << i.data[4] << i.data[5] << i.data[6] << i.data[7];
	}
	HGEPRINTCODELINE;
#endif
	quint32 *indices = new quint32[indicesCount];
	float *vertices = new float[vertexSet.size() * vertexElements];
	quint32 tmpint = 0, realIndex = 0;
	foreach(VertexData vertex, vertexSet)
	{
		for (quint32 i = 0; i < vertexElements; i++)
		{
			vertices[tmpint++] = vertex.data[i];
		}
		foreach(quint32 index, vertex.indices)
		{
			indices[index] = realIndex;
		}
		++realIndex;
	}
	QSharedPointer<render::MeshUnit>
		mesh(new render::MeshUnit(vertices,
		core::Protocol::Types::VerticesElementsCountType(vertexSet.size() * vertexElements),
		indices,
		core::Protocol::Types::IndicesCountType(indicesCount)));
	return mesh;
}
QSharedPointer<hge::render::MeshUnit> hge::utility::ColladaImporter::meshNodePT(const QDomElement &node, const QString &geoid)
{
	const quint32 vertexElements = 5;
	const QString positionStr = geoid + positionIdPsfxAttVal;
	const QString uvStr = geoid + mapIdPsfxAttVal;
	QVector<core::Protocol::Types::VertexElementType> positions;
	QVector<QVector<core::Protocol::Types::VertexElementType> > uvs;
	QVector<core::Protocol::Types::IndexElementType> polylist;
	QSet<VertexData> vertexSet;
	for (QDomNode child = node.firstChild(); !child.isNull(); child = child.nextSibling())
	{
		if (sourceTagKey == child.toElement().tagName())
		{
			const QString idstr(child.toElement().attribute(idAttributeKey));
#ifdef HGECOLLADADEBUG
			HGESEEN(sourceTagKey);
#endif
			if (idstr == positionStr)
			{
				positions = sourceDataNode(child.toElement());
#ifdef HGECOLLADADEBUG1
				qDebug() << "Positions: ";
				foreach(core::Protocol::VertexElementType i, positions)
				{
					qDebug() << i;
				}
				HGEPRINTCODELINE;
#endif
			}
			else if (idstr.startsWith(uvStr))
			{
				/// \todo Check the order of UV in collada file, my approach in here is so blind.
				uvs.push_back(sourceDataNode(child.toElement()));
#ifdef HGECOLLADADEBUG1
				qDebug() << "UVs: ";
				foreach(core::Protocol::VertexElementType i, uvs[0])
				{
					qDebug() << i;
				}
				qDebug() << "Size of UVs: " << uvs[0].size();
				HGEPRINTCODELINE;
#endif
			}
		}
		else if (polylistTagKey == child.toElement().tagName())
		{
			polylist = sourcePolyListNode(child.toElement());
#ifdef HGECOLLADADEBUG1
			qDebug() << "polylist: ";
			foreach(core::Protocol::IndexElementType i, polylist)
			{
				qDebug() << i;
			}
			HGEPRINTCODELINE;
#endif
		}
	}
	quint32 indicesCount = 0;
	for (int i = 0, tmpint = 0; i < polylist.size(); indicesCount++)
	{
		VertexData tmpv;
		tmpv.maxElements = vertexElements;
		tmpint = polylist[i++] * 3;
		int dataIndex = 0;
		tmpv.data[dataIndex++] = positions[tmpint++];
		tmpv.data[dataIndex++] = positions[tmpint++];
		tmpv.data[dataIndex++] = positions[tmpint];
		i++;
		tmpint = polylist[i++] * 2;
		tmpv.data[dataIndex++] = uvs[0][tmpint++];
		tmpv.data[dataIndex++] = uvs[0][tmpint];
		if (!vertexSet.contains(tmpv))
		{
			tmpv.indices.push_back(indicesCount);
			vertexSet.insert(tmpv);
		}
		else
		{
			const_cast<QVector<quint32>*>(&(vertexSet.find(tmpv)->indices))->push_back(indicesCount);
		}
	}
#ifdef HGECOLLADADEBUG
	qDebug() << "Vertices size: " << vertexSet.size();
	qDebug() << "Indices size: " << indicesCount;
	HGEPRINTCODELINE;
#endif
#ifdef HGECOLLADADEBUG1
	foreach(VertexData i, vertexSet)
	{
		qDebug() << "VD: " << i.data[0] << i.data[1] << i.data[2] << i.data[3] << i.data[4] << i.data[5] << i.data[6] << i.data[7];
	}
	HGEPRINTCODELINE;
#endif
	quint32 *indices = new quint32[indicesCount];
	float *vertices = new float[vertexSet.size() * vertexElements];
	quint32 tmpint = 0, realIndex = 0;
	foreach(VertexData vertex, vertexSet)
	{
		for (quint32 i = 0; i < vertexElements; i++)
		{
			vertices[tmpint++] = vertex.data[i];
		}
		foreach(quint32 index, vertex.indices)
		{
			indices[index] = realIndex;
		}
		++realIndex;
	}
	QSharedPointer<render::MeshUnit>
		mesh(new render::MeshUnit(vertices,
		core::Protocol::Types::VerticesElementsCountType(vertexSet.size() * vertexElements),
		indices,
		core::Protocol::Types::IndicesCountType(indicesCount)));
	return mesh;
}
QSharedPointer<hge::render::MeshUnit> hge::utility::ColladaImporter::meshNodeP(const QDomElement &node, const QString &geoid)
{

	const quint32 vertexElements = 3;
	const QString positionStr = geoid + positionIdPsfxAttVal;
	QVector<core::Protocol::Types::VertexElementType> positions;
	QVector<core::Protocol::Types::IndexElementType> polylist;
	QSet<VertexData> vertexSet;
	for (QDomNode child = node.firstChild(); !child.isNull(); child = child.nextSibling())
	{
		if (sourceTagKey == child.toElement().tagName())
		{
			const QString idstr(child.toElement().attribute(idAttributeKey));
#ifdef HGECOLLADADEBUG
			HGESEEN(sourceTagKey);
#endif
			if (idstr == positionStr)
			{
				positions = sourceDataNode(child.toElement());
#ifdef HGECOLLADADEBUG1
				qDebug() << "Positions: ";
				foreach(core::Protocol::VertexElementType i, positions)
				{
					qDebug() << i;
				}
				HGEPRINTCODELINE;
#endif
			}
		}
		else if (polylistTagKey == child.toElement().tagName())
		{
			polylist = sourcePolyListNode(child.toElement());
#ifdef HGECOLLADADEBUG1
			qDebug() << "polylist: ";
			foreach(core::Protocol::IndexElementType i, polylist)
			{
				qDebug() << i;
			}
			HGEPRINTCODELINE;
#endif
		}
	}
	quint32 indicesCount = 0;
	for (int i = 0, tmpint = 0; i < polylist.size(); indicesCount++)
	{
		VertexData tmpv;
		tmpv.maxElements = vertexElements;
		tmpint = polylist[i++] * 3;
		int dataIndex = 0;
		tmpv.data[dataIndex++] = positions[tmpint++];
		tmpv.data[dataIndex++] = positions[tmpint++];
		tmpv.data[dataIndex++] = positions[tmpint];
		i++;
		i++;
		if (!vertexSet.contains(tmpv))
		{
			tmpv.indices.push_back(indicesCount);
			vertexSet.insert(tmpv);
		}
		else
		{
			const_cast<QVector<quint32>*>(&(vertexSet.find(tmpv)->indices))->push_back(indicesCount);
		}
	}
#ifdef HGECOLLADADEBUG
	qDebug() << "Vertices size: " << vertexSet.size();
	qDebug() << "Indices size: " << indicesCount;
	HGEPRINTCODELINE;
#endif
#ifdef HGECOLLADADEBUG1
	foreach(VertexData i, vertexSet)
	{
		qDebug() << "VD: " << i.data[0] << i.data[1] << i.data[2] << i.data[3] << i.data[4] << i.data[5] << i.data[6] << i.data[7];
	}
	HGEPRINTCODELINE;
#endif
	quint32 *indices = new quint32[indicesCount];
	float *vertices = new float[vertexSet.size() * vertexElements];
	quint32 tmpint = 0, realIndex = 0;
	foreach(VertexData vertex, vertexSet)
	{
		for (quint32 i = 0; i < vertexElements; i++)
		{
			vertices[tmpint++] = vertex.data[i];
		}
		foreach(quint32 index, vertex.indices)
		{
			indices[index] = realIndex;
		}
		++realIndex;
	}
	QSharedPointer<render::MeshUnit>
		mesh(new render::MeshUnit(vertices,
		core::Protocol::Types::VerticesElementsCountType(vertexSet.size() * vertexElements),
		indices,
		core::Protocol::Types::IndicesCountType(indicesCount)));
	return mesh;
}
QVector<hge::core::Protocol::Types::VertexElementType> hge::utility::ColladaImporter::sourceDataNode(const QDomElement &node)
{
	QVector<core::Protocol::Types::VertexElementType> result;
	for (QDomElement child = node.firstChildElement(); !child.isNull(); child = child.nextSiblingElement())
	{
		if (floatArrayTagKey == child.tagName())
		{
			QStringList dataList = child.text().trimmed().split(" ", QString::SkipEmptyParts);
			foreach(QString dataVal, dataList)
			{
				bool isOK = true;
				result.push_back(core::Protocol::Types::VertexElementType(dataVal.toFloat(&isOK)));
				if (!isOK)
				{
					QMessageBox msgBox;
					msgBox.setText(QObject::tr("Failure in collada file reading procedure."));
					msgBox.setInformativeText(QObject::tr("The Collada file you have specified have malform source element."));
					msgBox.setStandardButtons(QMessageBox::Close);
					msgBox.exec();
					throw MalformDataElements;
				}
			}
		}
	}
	return result;
}
QVector<hge::core::Protocol::Types::IndexElementType> hge::utility::ColladaImporter::sourcePolyListNode(const QDomElement &node)
{
	QVector<quint32> result;
	for (QDomElement child = node.firstChildElement(); !child.isNull(); child = child.nextSiblingElement())
	{
		if (vCountTagKey == child.tagName())
		{
			QStringList dataList = child.text().trimmed().split(" ", QString::SkipEmptyParts);
			foreach(QString dataVal, dataList)
			{
				if (dataVal.toInt() != 3)
				{
					QMessageBox msgBox;
					msgBox.setText(QObject::tr("Failure in collada file reading procedure."));
					msgBox.setInformativeText(QObject::tr("The Collada file you have specified have malform "
						"polygon, all polygons must be triangle."));
					msgBox.setStandardButtons(QMessageBox::Close);
					msgBox.exec();
					throw NontrianglePolygon;
				}
			}
		}
		else if (pTagKey == child.tagName())
		{
			QStringList dataList = child.text().trimmed().split(" ", QString::SkipEmptyParts);
			foreach(QString dataVal, dataList)
			{
				bool isOK = true;
				result.push_back(core::Protocol::Types::IndexElementType(dataVal.toULongLong(&isOK)));
				if (!isOK)
				{
					QMessageBox msgBox;
					msgBox.setText(QObject::tr("Failure in collada file reading procedure."));
					msgBox.setInformativeText(QObject::tr("The Collada file you have specified have malform p element."));
					msgBox.setStandardButtons(QMessageBox::Close);
					msgBox.exec();
					throw MalformDataElements;
				}
			}
		}
	}
	return result;
}
