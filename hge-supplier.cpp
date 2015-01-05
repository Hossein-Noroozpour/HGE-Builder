#include "hge-supplier.hpp"
#include "hge-terrain.hpp"
#include <QtCore/QSharedPointer>
#include <QtCore/QFile>
#include <QtCore/QDebug>
hge::core::Supplier *hge::core::Supplier::defaultSupplier;
hge::core::Supplier::Supplier():
	lastID(8)
{
}
void hge::core::Supplier::addObject(const QSharedPointer<DataObject> &obj)
{
	obj->setDataId(lastID++);
	objects.push_back(obj);
}
void hge::core::Supplier::deleteObject(const QSharedPointer<DataObject> &obj)
{
	(void)obj;
	/// TODO
}
void hge::core::Supplier::writeToFile(QFile &file)
{
	foreach(QSharedPointer<DataObject> obj, objects)
	{
		core::Protocol::Types::ObjectTypeIdType typeId;
		typeId = obj->getTypeId();
		file.write((char *)(&typeId), sizeof(typeId));
		obj->writeToFile(file);
	}
}
