#include "DrawStrategy.h"
#include "qopengl.h"

void DrawingStrategyMesh::Draw()
{

}

void DrawingStrategyWireFrame::Draw()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}