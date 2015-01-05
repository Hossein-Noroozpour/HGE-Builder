#ifndef HGETERRAINIMAGEWINDOW_HPP
#define HGETERRAINIMAGEWINDOW_HPP
#include <QWidget>
QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QPushButton;
QT_END_NAMESPACE
namespace hge
{
	namespace core
	{
		class Profiler;
		class Supplier;
	}
	namespace ui
	{
		class TerrainPage;
		class TerrainImageWindow : public QWidget
		{
			Q_OBJECT
		private:
			QLabel      *img_l;
			QLineEdit   *x_unit_e;
			QLineEdit   *y_unit_e;
			QLineEdit   *z_unit_e;
			QLineEdit   *x_start_e;
			QLineEdit   *y_start_e;
			QLineEdit   *z_start_e;
			QPushButton *load_b;
			core::Profiler *profiler;
			core::Supplier *supplier;
			TerrainPage *terrain_page;
		public:
			TerrainImageWindow(
					TerrainPage *const &terrain_page,
					core::Profiler *const &profiler,
					core::Supplier *const &supplier,
					QWidget *parent = 0);
			~TerrainImageWindow();
		signals:
			void new_terrain(QString file_name, float x_u, float y_u, float z_u);
		public slots:
		private slots:
			void on_load();
		};
	}
}
#endif
