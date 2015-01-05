#ifndef HGEPROFILER_HPP
#define HGEPROFILER_HPP
#include <QObject>
namespace hge
{
	namespace core
	{
		class Profiler : public QObject
		{
			Q_OBJECT
		private:
			int vertices_count;
			int normals_count;
			int uvs_count;
			int tangents_count;
			int bitangents_count;
			int triangles_count;
			int indices_count;
			int draw_calls_per_frame_count;/// TODO
			int draw_calls_total_count;/// TODO
		public:
			Profiler(QObject *parent = 0);
			void add_vertices(  const int &count);
			void add_normals(   const int &count);
			void add_uvs(       const int &count);
			void add_tangents(  const int &count);
			void add_bitangents(const int &count);
			void add_triangles( const int &count);
			void add_indices(   const int &count);
		signals:
		public slots:
			void added_vertices(  const int &count);
			void added_normals(   const int &count);
			void added_uvs(       const int &count);
			void added_tangents(  const int &count);
			void added_bitangents(const int &count);
			void added_triangles( const int &count);
			void added_indices(   const int &count);
		};
	}
}
#endif
