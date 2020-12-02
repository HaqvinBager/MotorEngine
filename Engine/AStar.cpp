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

std::vector<DirectX::SimpleMath::Vector3> CAStar::GetPath(VECTOR3FLOAT aStartPosision, VECTOR3FLOAT aEndPosision, SNavMesh* aNavMesh, STriangle* aStartTriangle, STriangle* anEndTriangle) 
{ 
	std::vector<DirectX::SimpleMath::Vector3> newPath;
	if (aStartTriangle == nullptr || anEndTriangle == nullptr) {
		return newPath;
	}
	if (aStartTriangle->myId == anEndTriangle->myId) {
		newPath.emplace_back(aEndPosision);
		return newPath;
	}
	newPath = StringPull(aStartPosision, aEndPosision, GetPortals(AStar(aNavMesh, aStartTriangle, anEndTriangle), aNavMesh));
	return newPath; 
}

std::vector<int> CAStar::AStar(SNavMesh* aNavmesh, STriangle* aStartTriangle, STriangle* anEndTriangle)
{
	// Create nodes from tiles
	std::vector<int> trianglesID;
	std::vector<STriangle*> triangles = aNavmesh->myTriangles;
	for (int i = 0u; i < triangles.size(); ++i) {
		triangles[i]->myG = FLT_MAX / 2.0f;
		triangles[i]->myIndex = i;
		triangles[i]->myStatus = EStatus::UNVISITED;
	}

	// Setup heap
	CHeap<STriangle*, DrefTriangleLessComparer> openSet;

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
		return std::vector<int>();
	}

	std::vector<DirectX::SimpleMath::Vector3> pathCentroids;
	STriangle* iteratorTriangle = anEndTriangle;
	pathCentroids.emplace_back(anEndTriangle->myCenterPosition);
	trianglesID.push_back(anEndTriangle->myId);
	
	while (iteratorTriangle->myId != aStartTriangle->myId && iteratorTriangle != nullptr) {
		iteratorTriangle = iteratorTriangle->myPredecessor;
		auto it = std::find(aNavmesh->myTriangles.begin(), aNavmesh->myTriangles.end(), iteratorTriangle);
		//pathCentroids.emplace_back((*it)->myCenterPosition);
		trianglesID.push_back(iteratorTriangle->myId);
	}
		trianglesID.push_back(aStartTriangle->myId);
	//std::reverse(pathCentroids.begin(), pathCentroids.end());
	//std::reverse(trianglesID.begin(), trianglesID.end());
	return trianglesID;
}

std::vector<DirectX::SimpleMath::Vector3> CAStar::GetPortals(std::vector<int> nodePath, SNavMesh* nodes ) 
{ 
	std::vector<VECTOR3FLOAT> _portals;
	VECTOR3FLOAT temp;

	if (nodePath.size() <= 0) {
		//sometimes finds triangle outside navmesh??
		return _portals;
	}
	// portals are the 2 vertecis that are connectet between 2 triangles /pontus
	//nodePath is reversed so goes from back to front
	for (size_t i = nodePath.size() - 1; i > 0; --i) {
		for (int x = 0; x < 3; x++) {
			bool none = true;
			for (int y = 0; y < 3; y++) {
				if (nodes->myTriangles[nodePath[i]]->myIndices[x] == nodes->myTriangles[nodePath[i-1]]->myIndices[y]) {
					//adds 2 points to create a portal
					none = false;
				}
			}
			if (none) {
				if (x == 0) {
					_portals.push_back(nodes->myTriangles[nodePath[i]]->myVertexPositions[1]);
					_portals.push_back(nodes->myTriangles[nodePath[i]]->myVertexPositions[2]);
				}
				else if (x == 1) {
					_portals.push_back(nodes->myTriangles[nodePath[i]]->myVertexPositions[2]);
					_portals.push_back(nodes->myTriangles[nodePath[i]]->myVertexPositions[0]);
				}				
				else if (x == 2) {
					_portals.push_back(nodes->myTriangles[nodePath[i]]->myVertexPositions[0]);
					_portals.push_back(nodes->myTriangles[nodePath[i]]->myVertexPositions[1]);
				}
				continue;
			}

		}

	}
	return _portals;}

std::vector<DirectX::SimpleMath::Vector3> CAStar::StringPull(VECTOR3FLOAT aStart, VECTOR3FLOAT aEnd, std::vector<DirectX::SimpleMath::Vector3> somePortals)
{
	if (somePortals.size() <= 0) {
		return std::vector<VECTOR3FLOAT>();
	}


	std::vector<VECTOR3FLOAT> _points;
	if (somePortals.size() <= 2) {
		//_points.push_back(aStart);
		_points.push_back(aEnd);
		return _points;
	}

	somePortals.push_back(aEnd);
	somePortals.push_back(aEnd);

	VECTOR3FLOAT portalApex, portalLeft, portalRight, left, right;
	int apexIndex = 0, leftIndex = 0, rightIndex = 0;
	//set apex to start and gets left and right points in the first portal ask pontus if weard
	portalApex = aStart;
	portalLeft = somePortals[0];
	portalRight = somePortals[1];
	
	//_points.push_back(aStart);
	//int test = 0;
	for (int i = 2; i < somePortals.size(); i += 2) {
		/*
		if (test == 1000) {
			FAILED(L"navigation stringpull");
		}
		test++;
		*/
		left = somePortals[i + 0];
		right = somePortals[i + 1];

		//looks if next point is inside the path aeria
		if (Triarea2d(portalApex, portalRight, right) <= 0.0f) {
			//looks if next point is inside the path aeria of the other side
			if (portalApex == portalRight || Triarea2d(portalApex, portalLeft, right) > 0.0f) {
				portalRight = right;
				rightIndex = i;
			}
			else {
				_points.push_back(portalLeft);


				portalApex = portalLeft;
				apexIndex = leftIndex;

				portalLeft = portalApex;
				portalRight = portalApex;

				leftIndex = apexIndex;
				rightIndex = apexIndex;

				i = leftIndex;
				continue;
			}
		}


		//looks if next point is inside the path aeria
		if (Triarea2d(portalApex, portalLeft, left) >= 0.0f) {
			//looks if next point is inside the path aeria of the other side
			if (portalApex == portalLeft || Triarea2d(portalApex, portalRight, left) < 0.0f) {
				portalLeft = left;
				leftIndex = i;
			}
			else {
				_points.push_back(portalRight);


				portalApex = portalRight;
				apexIndex = rightIndex;

				portalLeft = portalApex;
				portalRight = portalApex;

				leftIndex = apexIndex;
				rightIndex = apexIndex;

				i = rightIndex;
				continue;
			}
		}
	}
	//_points.push_back(aEnd);
	std::reverse(_points.begin(), _points.end());
	return _points;
}

 float CAStar::Triarea2d(const VECTOR3FLOAT a, const VECTOR3FLOAT b, const VECTOR3FLOAT c) {
	const float ax = b.x - a.x;
	const float ay = b.z - a.z;
	const float bx = c.x - a.x;
	const float by = c.z - a.z;
	const float _return = bx* ay - ax * by;
	return _return;
	}
