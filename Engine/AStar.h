#include <vector>
#include "NavmeshLoader.h"
#include "SimpleMath.h"

#define VECTOR3FLOAT DirectX::SimpleMath::Vector3

class CAStar {

public:
	static CAStar* GetInstance() { static CAStar astar; return &astar; }
	std::vector<DirectX::SimpleMath::Vector3> GetPath(VECTOR3FLOAT aStartPosision, VECTOR3FLOAT aEndPosision, SNavMesh* aNavMesh, STriangle* aStartTriangle, STriangle* anEndTriangle);
private:
	std::vector<int> AStar(
		SNavMesh* aNavmesh, STriangle* aStartTriangle, STriangle* anEndTriangle
	);
	// np from pontus
	std::vector<DirectX::SimpleMath::Vector3> GetPortals(std::vector<int> aListOfInts, SNavMesh* aNavmesh);
	std::vector<DirectX::SimpleMath::Vector3> StringPull(VECTOR3FLOAT aStartTriangle, VECTOR3FLOAT anEndTriangle, std::vector<DirectX::SimpleMath::Vector3> somePortals);
	float Triarea2d(const VECTOR3FLOAT a, const VECTOR3FLOAT b, const VECTOR3FLOAT c);
};
