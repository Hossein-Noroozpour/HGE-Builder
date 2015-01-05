#include "hge-main-window.hpp"
#include "hge-tab-manager.hpp"
#include "hge-terrain-image-window.hpp"
#include <QAction>
#include <QStatusBar>
#include <QMenu>
#include <QContextMenuEvent>
#include <QMessageBox>
#include <QMenuBar>
#include <QCoreApplication>
#include <QDebug>
#include <QtWidgets/QFileDialog>
#include "hge-profiler.hpp"
#include "hge-supplier.hpp"
#include "hge-collada-importer.hpp"
hge::ui::MainWindow::MainWindow(core::Profiler *const &profiler, core::Supplier *const &supplier):
	profiler(profiler),
	supplier(supplier)
{
	tabs = new TabManager(this);
	this->setCentralWidget(tabs);
	create_actions();
	create_menus();
	statusBar()->showMessage(tr("Started."));
	setWindowTitle(tr("Hulixerian Game Engine File Builder"));
	setMinimumSize(300, 300);
	resize(480, 320);
}
void hge::ui::MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu menu(this);
	menu.addAction(cut_action);
	menu.addAction(copy_action);
	menu.addAction(paste_action);
	menu.exec(event->globalPos());
}
void hge::ui::MainWindow::on_new_file()
{
}
void hge::ui::MainWindow::on_open()
{
}
void hge::ui::MainWindow::on_save()
{
	if(projectFileName.isEmpty())
	{
		on_save_as();
		return;
	}
	QFile f(projectFileName);
	if(!f.open(QFile::WriteOnly))
	{
		QMessageBox msgBox;
		msgBox.setText("The file you have specified can not be opened.");
		msgBox.setInformativeText("Do you want to save your changes?");
		msgBox.setStandardButtons(QMessageBox::Retry | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Retry);
		int ret = msgBox.exec();
		if(QMessageBox::Retry == ret)
		{
			on_save_as();
		}
		return;
	}
	int endian_test_var = 1;
	if(1 != f.write((char *)(&endian_test_var), 1))
	{
		QMessageBox msgBox;
		msgBox.setText("The file you have specified can not be written in.");
		msgBox.setInformativeText("Do you want to save your changes?");
		msgBox.setStandardButtons(QMessageBox::Retry | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Retry);
		int ret = msgBox.exec();
		if(QMessageBox::Retry == ret)
		{
			on_save_as();
		}
		return;
	}
	supplier->writeToFile(f);
}
void hge::ui::MainWindow::on_save_as()
{
	QString filename =
			QFileDialog::getSaveFileName(
				(QWidget *)this,
				tr("Save HGE file"),
				QDir::currentPath(),
				tr("HGE File(*.hge)"));
	if(filename.isEmpty()) return;
	projectFileName = filename;
	on_save();
}
void hge::ui::MainWindow::on_print()
{
}
void hge::ui::MainWindow::on_import()
{
	QStringList filenames =
		QFileDialog::getOpenFileNames(
		(QWidget *)this,
		tr("Open 3D format file(s)"),
		QDir::currentPath(),
		tr("Collada (*.dae)"));
	QVector<QSharedPointer<core::DataObject> > objects;
	foreach(QString filename, filenames)
	{
		objects += utility::ColladaImporter::importFromFile(filename);
	}
	foreach(QSharedPointer<core::DataObject> object, objects)
	{
		supplier->addObject(object);
	}
}
void hge::ui::MainWindow::on_export()
{
}
void hge::ui::MainWindow::on_undo()
{
}
void hge::ui::MainWindow::on_redo()
{
}
void hge::ui::MainWindow::on_cut()
{
}
void hge::ui::MainWindow::on_copy()
{
}
void hge::ui::MainWindow::on_paste()
{
}
void hge::ui::MainWindow::on_bold()
{
}
void hge::ui::MainWindow::on_italic()
{
}
void hge::ui::MainWindow::on_left_align()
{
}
void hge::ui::MainWindow::on_right_align()
{
}
void hge::ui::MainWindow::on_justify()
{
}
void hge::ui::MainWindow::on_center()
{
}
void hge::ui::MainWindow::on_set_line_spacing()
{
}
void hge::ui::MainWindow::on_set_paragraph_spacing()
{
}
void hge::ui::MainWindow::on_terrain_new()
{

}
void hge::ui::MainWindow::on_terrain_new_image()
{
	try
	{
		TerrainImageWindow *img = new TerrainImageWindow(tabs->getTerrainPage(), profiler, supplier);
		/*connect(img, SIGNAL(new_terrain(QString, float, float, float)), this,
				SLOT(new_terrain_image(QString, float, float, float)));*/
	}
	catch(...)
	{

	}
}
void hge::ui::MainWindow::new_terrain_image(QString file_name, float x_u, float y_u, float z_u)
{
	///WARNING: For now
	(void) file_name;
	(void) x_u;
	(void) y_u;
	(void) z_u;
}
void hge::ui::MainWindow::on_terrain_new_nasa()
{

}
void hge::ui::MainWindow::on_terrain_edit()
{

}
void hge::ui::MainWindow::on_about()
{
	QMessageBox::about(this, tr("About"),
			tr("The <b>Hulixerian Game Engine file Builder</b>\nBy Hossein Noroozpour."));
}
void hge::ui::MainWindow::create_actions()
{
	new_action = new QAction(tr("&New"), this);
	new_action->setShortcuts(QKeySequence::New);
	new_action->setStatusTip(tr("Create a new file"));
	connect(new_action, SIGNAL(triggered()), this, SLOT(on_new_file()));

	open_action = new QAction(tr("&Open..."), this);
	open_action->setShortcuts(QKeySequence::Open);
	open_action->setStatusTip(tr("Open an existing file"));
	connect(open_action, SIGNAL(triggered()), this, SLOT(on_open()));

	save_action = new QAction(tr("&Save"), this);
	save_action->setShortcuts(QKeySequence::Save);
	save_action->setStatusTip(tr("Save the document to disk"));
	connect(save_action, SIGNAL(triggered()), this, SLOT(on_save()));

	print_action = new QAction(tr("&Print..."), this);
	print_action->setShortcuts(QKeySequence::Print);
	print_action->setStatusTip(tr("Print the document"));
	connect(print_action, SIGNAL(triggered()), this, SLOT(on_print()));

	import_action = new QAction(tr("&Import"), this);
	import_action->setShortcut(tr("Ctrl+Shift+I"));
	import_action->setStatusTip("Import a scene.");
	connect(import_action, SIGNAL(triggered()), this, SLOT(on_import()));

	export_action = new QAction(tr("&Export"), this);
	export_action->setShortcut(tr("Ctrl+Shift+E"));
	export_action->setStatusTip("Export current scene.");
	connect(export_action, SIGNAL(triggered()), this, SLOT(on_export()));

	exit_action = new QAction(tr("E&xit"), this);
	exit_action->setShortcuts(QKeySequence::Quit);
	exit_action->setStatusTip(tr("Exit the application"));
	connect(exit_action, SIGNAL(triggered()), this, SLOT(onExit()));

	undo_action = new QAction(tr("&Undo"), this);
	undo_action->setShortcuts(QKeySequence::Undo);
	undo_action->setStatusTip(tr("Undo the last operation"));
	connect(undo_action, SIGNAL(triggered()), this, SLOT(on_undo()));

	redo_action = new QAction(tr("&Redo"), this);
	redo_action->setShortcuts(QKeySequence::Redo);
	redo_action->setStatusTip(tr("Redo the last operation"));
	connect(redo_action, SIGNAL(triggered()), this, SLOT(on_redo()));

	cut_action = new QAction(tr("Cu&t"), this);
	cut_action->setShortcuts(QKeySequence::Cut);
	cut_action->setStatusTip(tr("Cut the current selection's contents to the "
							"clipboard"));
	connect(cut_action, SIGNAL(triggered()), this, SLOT(on_cut()));

	copy_action = new QAction(tr("&Copy"), this);
	copy_action->setShortcuts(QKeySequence::Copy);
	copy_action->setStatusTip(tr("Copy the current selection's contents to the "
							 "clipboard"));
	connect(copy_action, SIGNAL(triggered()), this, SLOT(on_copy()));

	paste_action = new QAction(tr("&Paste"), this);
	paste_action->setShortcuts(QKeySequence::Paste);
	paste_action->setStatusTip(tr("Paste the clipboard's contents into the current "
							  "selection"));
	connect(paste_action, SIGNAL(triggered()), this, SLOT(on_paste()));

	bold_action = new QAction(tr("&Bold"), this);
	bold_action->setCheckable(true);
	bold_action->setShortcut(QKeySequence::Bold);
	bold_action->setStatusTip(tr("Make the text bold"));
	connect(bold_action, SIGNAL(triggered()), this, SLOT(on_bold()));

	QFont boldFont = bold_action->font();
	boldFont.setBold(true);
	bold_action->setFont(boldFont);

	italic_action = new QAction(tr("&Italic"), this);
	italic_action->setCheckable(true);
	italic_action->setShortcut(QKeySequence::Italic);
	italic_action->setStatusTip(tr("Make the text italic"));
	connect(italic_action, SIGNAL(triggered()), this, SLOT(on_italic()));

	QFont italicFont = italic_action->font();
	italicFont.setItalic(true);
	italic_action->setFont(italicFont);

	set_line_spacing_action = new QAction(tr("Set &Line Spacing..."), this);
	set_line_spacing_action->setStatusTip(tr("Change the gap between the lines of a "
									   "paragraph"));
	connect(set_line_spacing_action, SIGNAL(triggered()), this, SLOT(on_set_line_spacing()));

	set_paragraph_spacing_action = new QAction(tr("Set &Paragraph Spacing..."), this);
	set_paragraph_spacing_action->setStatusTip(tr("Change the gap between paragraphs"));
	connect(set_paragraph_spacing_action, SIGNAL(triggered()), this, SLOT(on_set_paragraph_spacing()));

	about_action = new QAction(tr("&About"), this);
	about_action->setStatusTip(tr("Show the application's About box"));
	connect(about_action, SIGNAL(triggered()), this, SLOT(on_about()));

	left_align_action = new QAction(tr("&Left Align"), this);
	left_align_action->setCheckable(true);
	left_align_action->setShortcut(tr("Ctrl+L"));
	left_align_action->setStatusTip(tr("Left align the selected text"));
	connect(left_align_action, SIGNAL(triggered()), this, SLOT(on_left_align()));

	right_align_action = new QAction(tr("&Right Align"), this);
	right_align_action->setCheckable(true);
	right_align_action->setShortcut(tr("Ctrl+R"));
	right_align_action->setStatusTip(tr("Right align the selected text"));
	connect(right_align_action, SIGNAL(triggered()), this, SLOT(on_right_align()));

	justify_action = new QAction(tr("&Justify"), this);
	justify_action->setCheckable(true);
	justify_action->setShortcut(tr("Ctrl+J"));
	justify_action->setStatusTip(tr("Justify the selected text"));
	connect(justify_action, SIGNAL(triggered()), this, SLOT(on_justify()));

	center_action = new QAction(tr("&Center"), this);
	center_action->setCheckable(true);
	center_action->setShortcut(tr("Ctrl+E"));
	center_action->setStatusTip(tr("Center the selected text"));
	connect(center_action, SIGNAL(triggered()), this, SLOT(on_center()));

	alignment_group = new QActionGroup(this);
	alignment_group->addAction(left_align_action);
	alignment_group->addAction(right_align_action);
	alignment_group->addAction(justify_action);
	alignment_group->addAction(center_action);
	left_align_action->setChecked(true);

	terrain_new_action = new QAction(tr("&New"), this);
	connect(terrain_new_action, SIGNAL(triggered()), this, SLOT(on_terrain_new()));

	terrain_new_image_action = new QAction(tr("New with &Image height map file"), this);
	connect(terrain_new_image_action, SIGNAL(triggered()), this, SLOT(on_terrain_new_image()));

	terrain_new_nasa_action = new QAction(tr("New with N&ASA height map file"), this);
	connect(terrain_new_nasa_action, SIGNAL(triggered()), this, SLOT(on_terrain_new_nasa()));

	terrain_edit_action = new QAction(tr("&Edit"), this);
	connect(terrain_edit_action, SIGNAL(triggered()), this, SLOT(on_terrain_edit()));
}
void hge::ui::MainWindow::create_menus()
{
	file_menu = menuBar()->addMenu(tr("&File"));
	file_menu->addAction(new_action);
	file_menu->addAction(open_action);
	file_menu->addAction(save_action);
	file_menu->addAction(print_action);
	file_menu->addSeparator();
	file_menu->addAction(import_action);
	file_menu->addAction(export_action);
	file_menu->addSeparator();
	file_menu->addAction(exit_action);

	edit_menu = menuBar()->addMenu(tr("&Edit"));
	edit_menu->addAction(undo_action);
	edit_menu->addAction(redo_action);
	edit_menu->addSeparator();
	edit_menu->addAction(cut_action);
	edit_menu->addAction(copy_action);
	edit_menu->addAction(paste_action);
	edit_menu->addSeparator();

	terrain_menu = menuBar()->addMenu(tr("&Terrain"));
	terrain_menu->addAction(terrain_new_action);
	terrain_menu->addAction(terrain_new_image_action);
	terrain_menu->addAction(terrain_new_nasa_action);
	terrain_menu->addAction(terrain_edit_action);

	help_menu = menuBar()->addMenu(tr("&Help"));
	help_menu->addAction(about_action);

	format_menu = edit_menu->addMenu(tr("&Format"));
	format_menu->addAction(bold_action);
	format_menu->addAction(italic_action);
	format_menu->addSeparator()->setText(tr("Alignment"));
	format_menu->addAction(left_align_action);
	format_menu->addAction(right_align_action);
	format_menu->addAction(justify_action);
	format_menu->addAction(center_action);
	format_menu->addSeparator();
	format_menu->addAction(set_line_spacing_action);
	format_menu->addAction(set_paragraph_spacing_action);
}
void hge::ui::MainWindow::onExit()
{
	QMessageBox msgBox;
	msgBox.setText("The project has been modified.");
	msgBox.setInformativeText("Do you want to save your changes?");
	msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Save);
	int ret = msgBox.exec();
	if(QMessageBox::Save == ret)
	{
		on_save();
		QCoreApplication *app = QCoreApplication::instance();
		app->quit();
	}
	else if(QMessageBox::Discard == ret)
	{
		QCoreApplication *app = QCoreApplication::instance();
		app->quit();
	}
	else if(QMessageBox::Cancel == ret)
	{
		return;
	}
}
void hge::ui::MainWindow::closeEvent(QCloseEvent *event)
{
	QMessageBox msgBox;
	msgBox.setText("The project has been modified.");
	msgBox.setInformativeText("Do you want to save your changes?");
	msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Save);
	int ret = msgBox.exec();
	if(QMessageBox::Save == ret)
	{
		on_save();
		QMainWindow::closeEvent(event);
		QCoreApplication *app = QCoreApplication::instance();
		app->quit();
	}
	else if(QMessageBox::Discard == ret)
	{
		QMainWindow::closeEvent(event);
		QCoreApplication *app = QCoreApplication::instance();
		app->quit();
	}
	else if(QMessageBox::Cancel == ret)
	{
		return;
	}
}
