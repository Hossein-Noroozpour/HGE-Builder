#ifndef HGEDATAOBJECT_HPP
#define HGEDATAOBJECT_HPP
#include "hge-serializable.hpp"
namespace hge
{
	namespace core
	{
		class Supplier;
		/// NOTE: Every iheritee must introduce itself to the supplier and update its state in supplier.
		/// For example an object must update supplier in its constructor and deconstructor and in its changes
		class DataObject : public Serializable
		{
		public:
			virtual void setSupplier(hge::core::Supplier *const &supplier) = 0;
			virtual hge::core::Supplier *getSupplier() = 0;
			virtual hge::core::Protocol::Types::ObjectTypeIdType getTypeId() = 0;
			virtual void setDataId(const hge::core::Protocol::Types::IdType &id) = 0;
			virtual hge::core::Protocol::Types::IdType getDataId() = 0;
		};
	}
}
#endif
