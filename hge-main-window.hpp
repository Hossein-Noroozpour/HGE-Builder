#ifndef HGEMAINWINDOW_HPP
#define HGEMAINWINDOW_HPP
#include <QMainWindow>
class QAction;
class QActionGroup;
class QLabel;
class QMenu;
namespace hge
{
	namespace core
	{
		class Profiler;
		class Supplier;
	}
	namespace ui
	{
		class TabManager;
		class MainWindow : public QMainWindow
		{
			Q_OBJECT
		private:
			core::Profiler *profiler;
			core::Supplier *supplier;
			QString projectFileName;
		public:
			MainWindow(core::Profiler *const &profiler, hge::core::Supplier *const &supplier);
		protected:
			void contextMenuEvent(QContextMenuEvent *event);
		public slots:
			void new_terrain_image(QString file_name, float x_u, float y_u, float z_u);
		private slots:
			void on_new_file();
			void on_open();
			void on_save();
			void on_save_as();
			void on_import();
			void on_export();
			void on_print();
			void on_undo();
			void on_redo();
			void on_cut();
			void on_copy();
			void on_paste();
			void on_bold();
			void on_italic();
			void on_left_align();
			void on_right_align();
			void on_justify();
			void on_center();
			void on_set_line_spacing();
			void on_set_paragraph_spacing();
			void on_about();
			void on_terrain_new();
			void on_terrain_new_image();
			void on_terrain_new_nasa();
			void on_terrain_edit();
			void onExit();
		private:
			void create_actions();
			void create_menus();
			void closeEvent(QCloseEvent *event);
			QMenu *file_menu;
			QMenu *edit_menu;
			QMenu *terrain_menu;
			QMenu *format_menu;
			QMenu *help_menu;
			QActionGroup *alignment_group;
			QAction *new_action;
			QAction *open_action;
			QAction *save_action;
			QAction *print_action;
			QAction *import_action;
			QAction *export_action;
			QAction *exit_action;
			QAction *undo_action;
			QAction *redo_action;
			QAction *cut_action;
			QAction *copy_action;
			QAction *paste_action;
			QAction *bold_action;
			QAction *italic_action;
			QAction *left_align_action;
			QAction *right_align_action;
			QAction *justify_action;
			QAction *center_action;
			QAction *set_line_spacing_action;
			QAction *set_paragraph_spacing_action;
			QAction *terrain_new_nasa_action;
			QAction *terrain_new_image_action;
			QAction *terrain_new_action;
			QAction *terrain_edit_action;
			QAction *about_action;
			QLabel *infoLabel;
			TabManager *tabs;
		};
	}
}
#endif
