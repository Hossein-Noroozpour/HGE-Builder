#include "hge-tab-manager.hpp"
#include "hge-profile-page.hpp"
#include "hge-texture-page.hpp"
#include "hge-material-page.hpp"
#include "hge-object-page.hpp"
#include "hge-action-page.hpp"
#include "hge-terrain-page.hpp"
hge::ui::TabManager::TabManager(QWidget *parent) :
	QTabWidget(parent)
{
	ProfilePage *profile_page = new ProfilePage();
	this->addTab(profile_page, tr("Profiler"));
	TexturePage *texture_page = new TexturePage();
	this->addTab(texture_page, tr("Textures"));
	MaterialPage *material_page = new MaterialPage();
	this->addTab(material_page, tr("Materials"));
	ObjectPage *object_page = new ObjectPage();
	this->addTab(object_page, tr("Objects"));
	terrain_page = new TerrainPage();
	this->addTab(terrain_page, tr("Terrain"));
	ActionPage *action_page = new ActionPage();
	this->addTab(action_page, tr("Actions"));
}
hge::ui::TerrainPage *hge::ui::TabManager::getTerrainPage()
{
	return terrain_page;
}
