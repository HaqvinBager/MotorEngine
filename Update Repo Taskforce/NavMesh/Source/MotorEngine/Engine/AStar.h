#include <vector>
#include "NavmeshLoader.h"
#include "SimpleMath.h"

class CAStar {

public:
	std::vector<DirectX::SimpleMath::Vector3> AStar(
		CNavmeshLoader::SNavMesh* aNavmesh, STriangle* aStartTriangle, STriangle* anEndTriangle
	);

private:

};
