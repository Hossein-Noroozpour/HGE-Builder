#ifndef HGETRACEABLE_HPP
#define HGETRACEABLE_HPP
#include <QObject>
namespace hge
{
	namespace core
	{
		class Profiler;
		/// NOTE: Every iheritee must introduce itself to the profiler and update its state in profiler.
		/// For example an object must update profiler in its constructor and deconstructor and in its changes
		class Traceable
		{
		public:
			virtual void setProfiler(Profiler *const &profiler) = 0;
			virtual Profiler *getProfiler(void) = 0;
		};
	}
}
#endif
