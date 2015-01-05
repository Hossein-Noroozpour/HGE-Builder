#include "hge-terrain-image-window.hpp"
#include "hge-terrain.hpp"
#include "hge-supplier.hpp"
#include "hge-terrain-page.hpp"
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QImage>
#include <QFileDialog>
#include <QMessageBox>
#include <QScrollArea>
#include <QPushButton>
#include <cmath>
#include <exception>
#include <QProgressDialog>
#include <QtCore\QDebug>
hge::ui::TerrainImageWindow::TerrainImageWindow(
		TerrainPage *const &terrain_page,
		core::Profiler *const &profiler,
		core::Supplier *const &supplier,
		QWidget *parent):
	QWidget(parent),
	profiler(profiler),
	supplier(supplier),
	terrain_page(terrain_page)
{
	QString file_name = QFileDialog::getOpenFileName(this, tr("Choose height map image file"), QDir::currentPath(),
		tr("Image Files (*.png *jpg *.jpeg *.bmp)"));
	if(file_name.length() == 0 ||  file_name == QString::null)
	{
		QMessageBox msg;
		msg.setText(tr("Please specify an image."));
		msg.setIcon(QMessageBox::Critical);
		msg.setButtonText(QMessageBox::Close, tr("Close"));
		msg.exec();
		throw std::exception();
	}
	QImage img(file_name);
	if(int(pow( 2.0, log2(double(img.height())))) != img.height() || img.height() != img.width())
	{
		QMessageBox msg;
		msg.setText(tr("Please use an image that is square and have power of 2 aspects."));
		msg.setIcon(QMessageBox::Critical);
		msg.setButtonText(QMessageBox::Close, tr("Close"));
		msg.exec();
		throw std::exception();
	}
	QGridLayout *grid = new QGridLayout(this);
	this->setLayout(grid);
	QLabel *x_unit_l = new QLabel(tr("X unit scale"), this);
	x_unit_e = new QLineEdit(tr("1"), this);
	x_unit_l->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	x_unit_e->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	grid->addWidget(x_unit_l, 0, 0, 1, 1, Qt::AlignLeft);
	grid->addWidget(x_unit_e, 0, 1, 1, 1, Qt::AlignLeft);
	QLabel *y_unit_l = new QLabel(tr("Y unit scale"), this);
	y_unit_e = new QLineEdit(tr("1"), this);
	y_unit_l->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	y_unit_e->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	grid->addWidget(y_unit_l, 1, 0, 1, 1, Qt::AlignLeft);
	grid->addWidget(y_unit_e, 1, 1, 1, 1, Qt::AlignLeft);
	QLabel *z_unit_l = new QLabel(tr("Z unit scale"), this);
	z_unit_e = new QLineEdit(tr("1"), this);
	z_unit_l->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	z_unit_e->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	grid->addWidget(z_unit_l, 2, 0, 1, 1, Qt::AlignLeft);
	grid->addWidget(z_unit_e, 2, 1, 1, 1, Qt::AlignLeft);

	QLabel *x_start_l = new QLabel(tr("X strat"), this);
	x_start_e = new QLineEdit(tr("0"), this);
	x_start_l->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	x_start_e->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	grid->addWidget(x_start_l, 3, 0, 1, 1, Qt::AlignLeft);
	grid->addWidget(x_start_e, 3, 1, 1, 1, Qt::AlignLeft);
	QLabel *y_start_l = new QLabel(tr("Y start"), this);
	y_start_e = new QLineEdit(tr("0"), this);
	y_start_l->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	y_start_e->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	grid->addWidget(y_start_l, 4, 0, 1, 1, Qt::AlignLeft);
	grid->addWidget(y_start_e, 4, 1, 1, 1, Qt::AlignLeft);
	QLabel *z_start_l = new QLabel(tr("Z start"), this);
	z_start_e = new QLineEdit(tr("0"), this);
	z_start_l->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	z_start_e->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	grid->addWidget(z_start_l, 5, 0, 1, 1, Qt::AlignLeft);
	grid->addWidget(z_start_e, 5, 1, 1, 1, Qt::AlignLeft);

	QWidget *spring = new QWidget(this);
	spring->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	grid->addWidget(spring, 6, 0, 1, 1);

	load_b = new QPushButton(tr("&Load"), this);
	grid->addWidget(load_b, 7, 0, 1, 1, Qt::AlignCenter);
	load_b->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	connect(load_b, SIGNAL(clicked()), this, SLOT(on_load()));

	QScrollArea *img_s = new QScrollArea(this);
	img_l = new QLabel();
	QGridLayout *img_g = new QGridLayout(img_s);
	QWidget *img_w = new QWidget();
	img_s->setWidget(img_w);
	img_w->setLayout(img_g);
	img_s->setWidgetResizable(true);
	img_g->addWidget(img_l, 0, 0, 1, 1);
	img_l->setPixmap(QPixmap::fromImage(img));
	grid->addWidget(img_s, 0, 2, 8, 1);

	setMinimumSize(500, 200);
	setWindowTitle(tr("Height map window"));
	show();
}
void hge::ui::TerrainImageWindow::on_load()
{
	load_b->setDisabled(true);
	QProgressDialog progress("Constructing terrain object", "Cancel", 0, 100, this);
	const QPixmap *pixmap = img_l->pixmap();
	QImage img = pixmap->toImage();
	int height = img.height();
	int width = img.width();
	if(int(pow( 2.0, log2(height))) != height || width != height)
	{
		QMessageBox msg;
		msg.setText(tr("Please use an image that is square and have power of 2 aspects."));
		msg.setIcon(QMessageBox::Critical);
		msg.setButtonText(QMessageBox::Close, tr("Close"));
		msg.exec();
		load_b->setEnabled(true);
		return;
	}
	progress.setValue(5);
	unsigned char *pixels = img.bits();
	int line_bytes = img.bytesPerLine();
	float x_scale = x_unit_e->text().toFloat();
	float y_scale = y_unit_e->text().toFloat();
	float z_scale = z_unit_e->text().toFloat();
	float x_start = x_start_e->text().toFloat();
	float y_start = y_start_e->text().toFloat();
	float z_start = z_start_e->text().toFloat();
	progress.setValue(10);
	QSharedPointer<render::Terrain>terrain(new render::Terrain(
			pixels, line_bytes, height, x_scale, y_scale, z_scale, x_start, y_start, z_start, supplier, profiler));
	supplier->addObject(terrain);
	terrain_page->setTerrain(terrain);
	load_b->setEnabled(true);
}
hge::ui::TerrainImageWindow::~TerrainImageWindow()
{
	delete img_l;
	delete x_unit_e;
	delete y_unit_e;
	delete z_unit_e;
	delete x_start_e;
	delete y_start_e;
	delete z_start_e;
	delete load_b;
}
