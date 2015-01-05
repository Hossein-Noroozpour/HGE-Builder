#ifndef HGETABMANAGER_HPP
#define HGETABMANAGER_HPP
#include <QTabWidget>
namespace hge
{
	namespace ui
	{
		class TerrainPage;
		class TabManager : public QTabWidget
		{
			Q_OBJECT
		public:
			TabManager(QWidget *parent = 0);
			TerrainPage *getTerrainPage();
		signals:
		public slots:
		private:
			TerrainPage *terrain_page;
		};
	}
}
#endif
