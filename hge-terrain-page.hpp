#ifndef HGETERRAINPAGE_HPP
#define HGETERRAINPAGE_HPP
#include <QWidget>
QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE
namespace hge
{
	namespace render
	{
		class Terrain;
	}
	namespace ui
	{
		class TerrainPage : public QWidget
		{
			Q_OBJECT
		public:
			TerrainPage(QWidget *parent = 0);
			~TerrainPage();
			void setTerrain(const QSharedPointer<render::Terrain> &terrain);
			QSharedPointer<render::Terrain> getTerrain();
		signals:
		public slots:
		private:
			QSharedPointer<render::Terrain> terrain;
			QLabel *sizebl;
			QLabel *sizecl;
			QLabel *aspectl;
		};
	}
}
#endif
