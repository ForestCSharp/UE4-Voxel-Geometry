/*

Implementations of Octree member functions.

Copyright (C) 2011  Tao Ju

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public License
(LGPL) as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#pragma once

#include "UnrealMath.h"
#include "QEF.h"

struct MeshVertex
{
	MeshVertex(const FVector& _xyz, const FVector& _normal)
		: xyz(_xyz)
		, normal(_normal)
	{
	}

	FVector xyz;
	FVector normal;
};

typedef TArray<MeshVertex> VertexBuffer;
typedef TArray<int32> IndexBuffer;

// ----------------------------------------------------------------------------

enum OctreeNodeType
{
	Node_None,
	Node_Internal,
	Node_Psuedo,
	Node_Leaf,
};

// ----------------------------------------------------------------------------

struct OctreeDrawInfo
{
	OctreeDrawInfo()
		: index(-1)
		, corners(0)
	{
	}

	int 			index;
	int				corners;
	FVector			position;
	FVector			averageNormal;
	svd::QefData	qef;
};

// ----------------------------------------------------------------------------

class OctreeNode
{
public:

	OctreeNode()
		: type(Node_None)
		, min(0, 0, 0)
		, size(0)
		, drawInfo(nullptr)
	{
		for (int i = 0; i < 8; i++)
		{
			children[i] = nullptr;
		}
	}

	OctreeNode(const OctreeNodeType _type)
		: type(_type)
		, min(0, 0, 0)
		, size(0)
		, drawInfo(nullptr)
	{
		for (int i = 0; i < 8; i++)
		{
			children[i] = nullptr;
		}
	}

	OctreeNodeType	type;
	FVector		    min;
	int32			size;
	OctreeNode*		children[8];
	OctreeDrawInfo*	drawInfo;
};

// ----------------------------------------------------------------------------

OctreeNode* BuildOctree(const FVector& Min, const int32 size, const float threshold);
void DestroyOctree(OctreeNode* node);
void GenerateMeshFromOctree(OctreeNode* node, VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer);

// ----------------------------------------------------------------------------

