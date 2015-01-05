#include "hge-profiler.hpp"
hge::core::Profiler::Profiler(QObject *parent) :
	QObject(parent),
	vertices_count(0),
	normals_count(0),
	uvs_count(0),
	tangents_count(0),
	bitangents_count(0),
	triangles_count(0),
	indices_count(0),
	draw_calls_per_frame_count(0),
	draw_calls_total_count(0)
{
}
void hge::core::Profiler::added_vertices(  const int &count)
{
	(void) count;
}
void hge::core::Profiler::added_normals(   const int &count)
{
	(void) count;
}
void hge::core::Profiler::added_uvs(       const int &count)
{
	(void) count;
}
void hge::core::Profiler::added_tangents(  const int &count)
{
	(void) count;
}
void hge::core::Profiler::added_bitangents(const int &count)
{
	(void) count;
}
void hge::core::Profiler::added_triangles( const int &count)
{
	(void) count;
}
void hge::core::Profiler::added_indices(   const int &count)
{
	(void) count;
}
