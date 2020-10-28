#include "stdafx.h"
#include "AStar.h"
#include "Heap.h"

struct DrefTriangleLessComparer 
{
	bool operator()(STriangle* lhs, STriangle* rhs) const 
	{
		return lhs->myF < rhs->myF;
	}
};

float CalculateH(DirectX::SimpleMath::Vector3& aStartCentroid, DirectX::SimpleMath::Vector3& anEndCentroid)
{
	return (abs(aStartCentroid.x - anEndCentroid.x) + abs(aStartCentroid.y - anEndCentroid.y) + abs(aStartCentroid.z - anEndCentroid.z));
}

std::vector<DirectX::SimpleMath::Vector3> CAStar::AStar(SNavMesh* aNavmesh, STriangle* aStartTriangle, STriangle* anEndTriangle)
{
	// Create nodes from tiles
	std::vector<STriangle*> triangles = aNavmesh->myTriangles;
	for (int i = 0u; i < triangles.size(); ++i) {
		triangles[i]->myG = FLT_MAX / 2.0f;
		triangles[i]->myIndex = i;
	}

	// Setup heap
	CommonUtilities::CHeap<STriangle*, DrefTriangleLessComparer> openSet;

	aStartTriangle->myG = 0;
	aStartTriangle->myF = aStartTriangle->myG + CalculateH(aStartTriangle->myCenterPosition, anEndTriangle->myCenterPosition);
	openSet.Enqueue(aStartTriangle);

	float traversalWeight = 1.0f;

	while (openSet.Size() > 0) {
		STriangle* current = openSet.GetTop();
		if (current == anEndTriangle) {
			break;
		}

		openSet.Resort();
		STriangle* v = openSet.Dequeue();
		v->myStatus = EStatus::CLOSED;

		for (auto& u : v->myNeighbors) {
			if (u->myStatus == EStatus::CLOSED) {
				continue;
			}

			float gCost = v->myG + traversalWeight;

			if (u->myStatus != EStatus::OPEN) {
				u->myStatus = EStatus::OPEN;
				openSet.Enqueue(u);
				u->myPredecessor = v;
				u->myG = gCost;
				u->myF = u->myG + CalculateH(u->myCenterPosition, anEndTriangle->myCenterPosition);
			}
			else if (gCost < u->myG) {
				u->myPredecessor = v;
				u->myG = gCost;
				u->myF = u->myG + CalculateH(u->myCenterPosition, anEndTriangle->myCenterPosition);
			}
		}
	}

	// Checks if the end index was found
	if (anEndTriangle->myG >= triangles.size()) {
		return std::vector<DirectX::SimpleMath::Vector3>();
	}

	std::vector<DirectX::SimpleMath::Vector3> pathCentroids;
	STriangle* iteratorTriangle = anEndTriangle;
	pathCentroids.emplace_back(anEndTriangle->myCenterPosition);

	while (iteratorTriangle != aStartTriangle && iteratorTriangle != nullptr) {
		iteratorTriangle = iteratorTriangle->myPredecessor;
		auto it = std::find(triangles.begin(), triangles.end(), iteratorTriangle);
		pathCentroids.emplace_back((*it)->myCenterPosition);
	}

	//std::reverse(pathCentroids.begin(), pathCentroids.end());
	return pathCentroids;
}
