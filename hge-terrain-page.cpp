#include "hge-terrain-page.hpp"
#include "hge-terrain.hpp"
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
hge::ui::TerrainPage::TerrainPage(QWidget *parent) :
	QWidget(parent),
	terrain(0)
{
	QGridLayout *grid = new QGridLayout(this);
	this->setLayout(grid);
	QGroupBox *desgrp = new QGroupBox(tr("Description"));
	grid->addWidget(desgrp, 0, 0, 1, 1);
	QGridLayout *desgrd = new QGridLayout(desgrp);
	desgrp->setLayout(desgrd);
	QLabel *sizebll = new QLabel(tr("Terrain size(in bytes)"));
	desgrd->addWidget(sizebll, 0, 0, 1, 1);
	sizebll->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	sizebl = new QLabel(tr("-"));
	desgrd->addWidget(sizebl, 0, 1, 1, 1);
	sizebl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	QLabel *sizecll = new QLabel(tr("Terrain component count"));
	desgrd->addWidget(sizecll, 1, 0, 1, 1);
	sizecll->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	sizecl = new QLabel(tr("-"));
	desgrd->addWidget(sizecl, 1, 1, 1, 1);
	sizecl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	QLabel *aspectll = new QLabel(tr("Terrain aspect"));
	desgrd->addWidget(aspectll, 2, 0, 1, 1);
	aspectll->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	aspectl = new QLabel(tr("-"));
	desgrd->addWidget(aspectl, 2, 1, 1, 1);
	aspectl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	QWidget *desspacer = new QWidget();
	desgrd->addWidget(desspacer, 3, 0, 1, 2);
	desspacer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	QGroupBox *edtgrp = new QGroupBox(tr("Edit"));
	grid->addWidget(edtgrp, 1, 0, 1, 1);
	QGridLayout *edtgrd = new QGridLayout(edtgrp);
	edtgrp->setLayout(edtgrd);
}
hge::ui::TerrainPage::~TerrainPage()
{
}
void hge::ui::TerrainPage::setTerrain(const QSharedPointer<render::Terrain> &terrain)
{
	this->terrain = terrain;
	sizebl->setText(QString::number(666)); /// \todo
}
QSharedPointer<hge::render::Terrain> hge::ui::TerrainPage::getTerrain()
{
	return this->terrain;
}
