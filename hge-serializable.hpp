#ifndef HGE_CORE_HGESERIALIZABLE_HPP
#define HGE_CORE_HGESERIALIZABLE_HPP
#include "hge-protocol.hpp"
#include <QtCore\QFile>
namespace hge
{
	namespace core
	{
		class Serializable
		{
		public:
			virtual void writeToFile(QFile &file) = 0;
			virtual void readFromFile(QFile &file) = 0;
		};
	}
}
#endif