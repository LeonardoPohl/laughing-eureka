#pragma once

#include <array>

class Point
{
public:
	struct Vertex
	{
		std::array<float, 3> Position;
		std::array<float, 3> Color;

		Vertex() :Position{ 0.0f,0.0f,0.0f }, Color{ 0.0f,0.0f,0.0f }
		{
		};
		Vertex(std::array<float, 3> position,
			   std::array<float, 3> color)
			: Position(position), Color(color)
		{
		}
	};

	static const int VertexCount = 8;
	static const int IndexCount = 3 * 12;

	std::array<float, 2> Position;
	float Depth{ 0 };
	std::array<Vertex, VertexCount> Vertices;
	float HalfLength;

	Point() : Position{0.0f, 0.0f}
	{
	}

	inline std::array<float, 3> getColorFromDepth()
	{
		// TODO: (Possibly) Add color schemes if bored
		float grey =  (Depth + 1) / 2.0f;

		if (grey == 1.0f)
			grey = 0.0f;
		
		return { grey, grey, grey };
	}

	inline void updateDepth(float depth)
	{
		this->Depth = depth;

		Vertices[0].Position[2] = -HalfLength + depth;
		Vertices[1].Position[2] = -HalfLength + depth;
		Vertices[2].Position[2] =  HalfLength + depth;
		Vertices[3].Position[2] =  HalfLength + depth;

		Vertices[4].Position[2] = -HalfLength + depth;
		Vertices[5].Position[2] = -HalfLength + depth;
		Vertices[6].Position[2] =  HalfLength + depth;
		Vertices[7].Position[2] =  HalfLength + depth;
	}

	static inline unsigned int *getIndices(int i)
	{
		std::array<unsigned int, IndexCount> indices
		{
			/*
			0 + i * VertexCount, 1 + i * VertexCount, 2 + i * VertexCount,
			0 + i * VertexCount, 2 + i * VertexCount, 3 + i * VertexCount,
			0 + i * VertexCount, 1 + i * VertexCount, 4 + i * VertexCount,
			1 + i * VertexCount, 2 + i * VertexCount, 4 + i * VertexCount,
			2 + i * VertexCount, 3 + i * VertexCount, 4 + i * VertexCount,
			3 + i * VertexCount, 0 + i * VertexCount, 4 + i * VertexCount
			*/
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

	inline void updateVertexArray()
	{
		std::array<float, 3> Color = getColorFromDepth();

		/*
		   4
		   ^
		  /|\
		 /0| \
		/.-|-.\
	   1'-.|.-'3
		   2
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
		/*
		Vertices[0] = { { -HalfLength + Position[0],       0.0f + Position[1],  HalfLength }, Color };
		Vertices[1] = { { -HalfLength + Position[0],       0.0f + Position[1], -HalfLength }, Color };
		Vertices[2] = { {  HalfLength + Position[0],       0.0f + Position[1], -HalfLength }, Color };
		Vertices[3] = { {  HalfLength + Position[0],       0.0f + Position[1],  HalfLength }, Color };
		Vertices[4] = { {        0.0f + Position[0], HalfLength + Position[1],        0.0f }, Color };*/
		Vertices[0] = { { -HalfLength + Position[0], -HalfLength + Position[1], -HalfLength + Depth }, Color };
		Vertices[1] = { {  HalfLength + Position[0], -HalfLength + Position[1], -HalfLength + Depth }, Color };
		Vertices[2] = { {  HalfLength + Position[0], -HalfLength + Position[1],  HalfLength + Depth }, Color };
		Vertices[3] = { { -HalfLength + Position[0], -HalfLength + Position[1],  HalfLength + Depth }, Color };

		Vertices[4] = { { -HalfLength + Position[0],  HalfLength + Position[1], -HalfLength + Depth }, Color };
		Vertices[5] = { {  HalfLength + Position[0],  HalfLength + Position[1], -HalfLength + Depth }, Color };
		Vertices[6] = { {  HalfLength + Position[0],  HalfLength + Position[1],  HalfLength + Depth }, Color };
		Vertices[7] = { { -HalfLength + Position[0],  HalfLength + Position[1],  HalfLength + Depth }, Color };
	}

};