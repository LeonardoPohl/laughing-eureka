#include "Point.h"
#include "ColorMaps.h"

std::array<float, 3> Point::getColorFromDepth()
{
	// TODO: (Possibly) Add color schemes if bored
	float grey = (Depth + 1) / 2.0f;

	if (grey == 1.0f)
		grey = 0.0f;

	return { grey, grey, grey };
}

void Point::updateDepth(float depth)
{
	this->Depth = depth;
	std::array<float, 3> Color = getColorFromDepth();

	Vertices[0].Position[2] = -HalfLength + depth;
	Vertices[0].Color = Color;
	Vertices[1].Position[2] = -HalfLength + depth;
	Vertices[1].Color = Color;
	Vertices[2].Position[2] = HalfLength + depth;
	Vertices[2].Color = Color;
	Vertices[3].Position[2] = HalfLength + depth;
	Vertices[3].Color = Color;

	Vertices[4].Position[2] = -HalfLength + depth;
	Vertices[4].Color = Color;
	Vertices[5].Position[2] = -HalfLength + depth;
	Vertices[5].Color = Color;
	Vertices[6].Position[2] = HalfLength + depth;
	Vertices[6].Color = Color;
	Vertices[7].Position[2] = HalfLength + depth;
	Vertices[7].Color = Color;
}

unsigned int *Point::getIndices(int i)
{
	std::array<unsigned int, IndexCount> indices
	{
		0 + i * VertexCount, 1 + i * VertexCount, 2 + i * VertexCount,
		0 + i * VertexCount, 2 + i * VertexCount, 3 + i * VertexCount,
		0 + i * VertexCount, 1 + i * VertexCount, 5 + i * VertexCount,
		0 + i * VertexCount, 5 + i * VertexCount, 4 + i * VertexCount,
		0 + i * VertexCount, 3 + i * VertexCount, 7 + i * VertexCount,
		0 + i * VertexCount, 7 + i * VertexCount, 4 + i * VertexCount,
		1 + i * VertexCount, 2 + i * VertexCount, 6 + i * VertexCount,
		1 + i * VertexCount, 6 + i * VertexCount, 5 + i * VertexCount,
		2 + i * VertexCount, 3 + i * VertexCount, 7 + i * VertexCount,
		2 + i * VertexCount, 7 + i * VertexCount, 6 + i * VertexCount,
		4 + i * VertexCount, 5 + i * VertexCount, 6 + i * VertexCount,
		4 + i * VertexCount, 6 + i * VertexCount, 7 + i * VertexCount
	};

	return &indices[0];
}

void Point::updateVertexArray()
{
	std::array<float, 3> Color = getColorFromDepth();

	/*
		7      6
	   .+------+
	4.' |  5 .'|
	+---+--+'  |
	|   |  |   |
	|  .+--+---+2
	|.' 3  | .'Length = 2 * HalfLength
	+------+'
	0	   1
	*/
	Vertices[0] = { { -HalfLength + Position[0], -HalfLength + Position[1], -HalfLength + Depth }, Color };
	Vertices[1] = { {  HalfLength + Position[0], -HalfLength + Position[1], -HalfLength + Depth }, Color };
	Vertices[2] = { {  HalfLength + Position[0], -HalfLength + Position[1],  HalfLength + Depth }, Color };
	Vertices[3] = { { -HalfLength + Position[0], -HalfLength + Position[1],  HalfLength + Depth }, Color };

	Vertices[4] = { { -HalfLength + Position[0],  HalfLength + Position[1], -HalfLength + Depth }, Color };
	Vertices[5] = { {  HalfLength + Position[0],  HalfLength + Position[1], -HalfLength + Depth }, Color };
	Vertices[6] = { {  HalfLength + Position[0],  HalfLength + Position[1],  HalfLength + Depth }, Color };
	Vertices[7] = { { -HalfLength + Position[0],  HalfLength + Position[1],  HalfLength + Depth }, Color };
}