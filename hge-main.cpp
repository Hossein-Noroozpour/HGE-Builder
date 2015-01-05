#include "hge-main-window.hpp"
#include "hge-profiler.hpp"
#include "hge-supplier.hpp"
#include <QApplication>
#include <QSurfaceFormat>
#include <QProcess>

int main(int argc, char ** argv)
{
	QApplication a(argc, argv);
	hge::core::Profiler profiler;
	hge::core::Supplier supplier;
	hge::ui::MainWindow main_window(&profiler, &supplier);
	main_window.show();
	return a.exec();
}
