#ifndef HGESUPPLIER_HPP
#define HGESUPPLIER_HPP
#include <QtCore/QVector>
QT_BEGIN_NAMESPACE
template <typename data>
class QSharedPointer;
class QFile;
QT_END_NAMESPACE
#define HGE_CORE_SUPPLIER_DEBUG_MODE_ON
namespace hge
{
	namespace core
	{
		class DataObject;
		class Supplier
		{
		public:
			static Supplier *defaultSupplier;
			Supplier();
			void addObject(const QSharedPointer<DataObject> &obj);
			void deleteObject(const QSharedPointer<DataObject> &obj);
			void writeToFile(QFile &file);
		private:
			unsigned int lastID;
			QVector<QSharedPointer<DataObject> > objects;
		};
	}
}
#endif
