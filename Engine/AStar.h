#include <vector>
#include "NavmeshLoader.h"
#include "SimpleMath.h"

class CAStar {

public:
	static std::vector<DirectX::SimpleMath::Vector3> AStar(
		SNavMesh* aNavmesh, STriangle* aStartTriangle, STriangle* anEndTriangle
	);

private:

};
