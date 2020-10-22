#pragma once
#include <SimpleMath.h>

class CModel;
struct aiMesh;
struct STriangle;

enum class EStatus {
	UNVISITED,
	OPEN,
	CLOSED
};

struct STriangle {
	DirectX::SimpleMath::Vector3 myCenterPosition;
	DirectX::SimpleMath::Vector3 myVertexPositions[3];
	UINT myIndices[3];

	float myG = 0;
	float myF = 0;
	int myIndex = -1;
	EStatus myStatus = EStatus::UNVISITED;

	STriangle* myPredecessor = nullptr;
	std::vector<STriangle*> myNeighbors;

	bool operator<(STriangle& other) {
		return this->myG < other.myG;
	}

	bool operator==(STriangle& other) {
		return !(this->myG < other.myG) && !(other.myG < this->myG);
	}

	friend bool operator==(const STriangle& lhs, const STriangle& rhs) {
		return !(lhs.myG < rhs.myG) && !(rhs.myG < lhs.myG);
	}
};

class CNavmeshLoader
{
public:
	struct SNavMesh {
		std::vector<STriangle*> myTriangles;
	};

	CNavmeshLoader();
	~CNavmeshLoader();
	bool Init();

	SNavMesh* LoadNavmesh(std::string aFilepath);
private:
	void MakeTriangles(aiMesh* aMesh, SNavMesh* aNavMesh);
	DirectX::SimpleMath::Vector3 GetCentroid(DirectX::SimpleMath::Vector3& aVectorOne, DirectX::SimpleMath::Vector3& aVectorTwo, DirectX::SimpleMath::Vector3& aVectorThree);
	bool AreNeighbors(UINT* someIndices, UINT* someOtherIndices);
};

