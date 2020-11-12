#include <string>
#include <map>
#include <vector>

// assimp include files. These three are usually needed.
#include "assimp/Importer.hpp"	//OO version Header!
#include "assimp/postprocess.h"
#include "assimp/LogStream.hpp"
#include <fstream>

#include "MathFunc.h"

// Note 2020 11 12 Refactoring to use TGA code standard.

#define NUM_BONES_PER_VERTEX 4

struct VertexBoneDataAnim
{
	uint myIDs[NUM_BONES_PER_VERTEX];
	float myWeights[NUM_BONES_PER_VERTEX];

	VertexBoneDataAnim()
	{
		Reset();
	};

	void Reset()
	{
		memset(myIDs, 0, sizeof(myIDs));
		memset(myWeights, 0, sizeof(myWeights));
	}

	void AddBoneData(uint BoneID, float Weight)
	{
		for (uint i = 0; i < NUM_BONES_PER_VERTEX; i++)
		{
			if (myWeights[i] == 0.0)
			{
				myIDs[i] = BoneID;
				myWeights[i] = Weight;
				return;
			}
		}

		// Should never get here - more bones than we have space for
		assert(false);
	}
};

struct BoneInfoAnim
{
	aiMatrix4x4 myBoneOffset;
	aiMatrix4x4 myFinalTransformation;
};

#define INVALID_MATERIAL 0xFFFFFFFF;

struct MeshEntry
{
	MeshEntry()
	{
		myNumIndices = 0;
		myBaseVertex = 0;
		myBaseIndex = 0;
		myMaterialIndex = INVALID_MATERIAL;
	}

	unsigned int myNumIndices;
	unsigned int myBaseVertex;
	unsigned int myBaseIndex;
	unsigned int myMaterialIndex;
};

class AnimationController
{
private:

	std::string myModelPath;

	aiVector3D myRotation;
	long long myLastTime			= 0;
	float myAnimationTimePrev		= 0;
	float myAnimationTimeCurrent	= 0;
	float myBlendingTime			= 0;
	float myBlendingTimeMul			= 0;
	int myPrevAnimIndex				= 0;
	bool myUpdateBoth				= false;
	bool myTemporary				= false;
	float myPlayTime				= 0;


	std::vector<Assimp::Importer*> _importers;
	std::vector<const aiScene*> _scenes;
	int _curScene;
	aiMatrix4x4 m_GlobalInverseTransform;
	std::map<std::string, uint> m_BoneMapping;
	std::vector<MeshEntry> m_Entries;
	std::vector<BoneInfoAnim> m_BoneInfo;
	uint m_NumBones;
	std::vector<VertexBoneDataAnim> m_Mass;

public:

	void SetRotation(const aiVector3D& aRotation) { myRotation = aRotation; }
	aiVector3D GetRotation() { return myRotation; }

	const size_t GetNrOfAnimations() const { return _scenes.size() - 1; }

	~AnimationController()
	{
		Release();
	}

	void Release()
	{
		// *** cleanup ***
		for (uint i = 0; i < _importers.size(); ++i)
		{
			delete _importers[i];
		}
		_importers.clear();
	}

	AnimationController(const char* aModelPath)
		: m_NumBones(0)
		, _curScene(NULL)
		, myModelPath(aModelPath)
		, myRotation(0.f)
		, myPrevAnimIndex(0)
		, myBlendingTime(0.f)
		, myBlendingTimeMul(1.f)
		, myUpdateBoth(true)
		, myTemporary(false)
		, myPlayTime(0.f)
	{
	}

	void ReadNodeHeirarchy(const aiScene* aScene, float anAnimationTime, const aiNode* aNode, const aiMatrix4x4& aParentTransform, int aStopAnimAtLevel)
	{
		float animationTime(anAnimationTime);

		std::string nodeName(aNode->mName.data);

		const aiAnimation* animation = aScene->mAnimations[0];

		aiMatrix4x4 NodeTransformation(aNode->mTransformation);

		const aiNodeAnim* nodeAnimation = FindNodeAnim(animation, nodeName);

		if (nodeAnimation)
		{
			// Interpolate scaling and generate scaling transformation matrix
			aiVector3D Scaling;
			CalcInterpolatedScaling(Scaling, animationTime, nodeAnimation);
			aiMatrix4x4 ScalingM;
			aiMatrix4x4::Scaling(Scaling, ScalingM);

			// Interpolate rotation and generate rotation transformation matrix
			aiQuaternion RotationQ;
			CalcInterpolatedRotation(RotationQ, animationTime, nodeAnimation);
			aiMatrix4x4 RotationM;
			InitM4FromM3(RotationM, RotationQ.GetMatrix());

			// Interpolate translation and generate translation transformation matrix
			aiVector3D Translation;
			{
				float timeStop(aStopAnimAtLevel <= 0 ? animationTime : 0.f);
				CalcInterpolatedPosition(Translation, timeStop, nodeAnimation);
			}
			aiMatrix4x4 TranslationM;
			aiMatrix4x4::Translation(Translation, TranslationM);

			// Combine the above transformations
			NodeTransformation = TranslationM * RotationM * ScalingM;
		}
		aStopAnimAtLevel--;

		aiMatrix4x4 GlobalTransformation = aParentTransform * NodeTransformation;

		if (m_BoneMapping.find(nodeName) != m_BoneMapping.end())
		{
			uint BoneIndex = m_BoneMapping[nodeName];
			m_BoneInfo[BoneIndex].myFinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].myBoneOffset;
		}

		for (uint i = 0; i < aNode->mNumChildren; i++)
		{
			ReadNodeHeirarchy(aScene, animationTime, aNode->mChildren[i], GlobalTransformation, aStopAnimAtLevel);
		}
	}

	void ReadNodeHeirarchy(	  const aiScene* scene0, const aiScene* scene1
							, float AnimationTime0, float AnimationTime1
							, const aiNode* pNode0, const aiNode* pNode1
							, const aiMatrix4x4& ParentTransform, int stopAnimLevel)
	{
		float time0(AnimationTime0);// Why is AnimationTime0 made into a local variable?
		float time1(AnimationTime1);// Why is AnimationTime1 made into a local variable?

		std::string NodeName0(pNode0->mName.data);// rename pNode0/1 is the previous animations data. pNode0 is pointerToPreviousAnimNode
		std::string NodeName1(pNode1->mName.data);// pointerToCurrentAnimNode
		assert(NodeName0 == NodeName1);// their first node should be the same, roots must be equal

		const aiAnimation* pAnimation0 = scene0->mAnimations[0];
		const aiAnimation* pAnimation1 = scene1->mAnimations[0];

		aiMatrix4x4 NodeTransformation0(pNode0->mTransformation);
		aiMatrix4x4 NodeTransformation1(pNode1->mTransformation);

		const aiNodeAnim* pNodeAnim0 = FindNodeAnim(pAnimation0, NodeName0);
		const aiNodeAnim* pNodeAnim1 = FindNodeAnim(pAnimation1, NodeName0);

		if (pNodeAnim0 && pNodeAnim1)
		{
			// Interpolate scaling and generate scaling transformation matrix
			aiVector3D Scaling0;
			CalcInterpolatedScaling(Scaling0, time0, pNodeAnim0);
			aiVector3D Scaling1;
			CalcInterpolatedScaling(Scaling1, time1, pNodeAnim1);
			aiMatrix4x4 ScalingM;
			aiMatrix4x4::Scaling(Scaling0 * myBlendingTime + Scaling1 * (1.f - myBlendingTime), ScalingM);

			// Interpolate rotation and generate rotation transformation matrix
			aiQuaternion RotationQ0;
			CalcInterpolatedRotation(RotationQ0, time0, pNodeAnim0);
			aiQuaternion RotationQ1;
			CalcInterpolatedRotation(RotationQ1, time1, pNodeAnim1);
			aiMatrix4x4 RotationM;
			aiQuaternion RotationQ;
			aiQuaternion::Interpolate(RotationQ, RotationQ1, RotationQ0, myBlendingTime);
			InitM4FromM3(RotationM, RotationQ.GetMatrix());

			// Interpolate translation and generate translation transformation matrix
			aiVector3D Translation0;
			{
				float time(stopAnimLevel <= 0 ? AnimationTime0 : 0.f);
				CalcInterpolatedPosition(Translation0, time, pNodeAnim0);
			}
			aiVector3D Translation1;
			{
				float time(stopAnimLevel <= 0 ? AnimationTime1 : 0.f);
				CalcInterpolatedPosition(Translation1, time, pNodeAnim1);
			}
			aiMatrix4x4 TranslationM;
			aiMatrix4x4::Translation(Translation0 * myBlendingTime + Translation1 * (1.f - myBlendingTime), TranslationM);

			// Combine the above transformations
			NodeTransformation0 = TranslationM * RotationM * ScalingM;
		}

		stopAnimLevel--;

		aiMatrix4x4 GlobalTransformation = ParentTransform * NodeTransformation0;

		if (m_BoneMapping.find(NodeName0) != m_BoneMapping.end())
		{
			uint BoneIndex = m_BoneMapping[NodeName0];
			m_BoneInfo[BoneIndex].myFinalTransformation = 
				  m_GlobalInverseTransform 
				* GlobalTransformation 
				* m_BoneInfo[BoneIndex].myBoneOffset;
		}

		uint n = min(pNode0->mNumChildren, pNode1->mNumChildren); // Does one movement for all the children
		for (uint i = 0; i < n; i++)
		{
			ReadNodeHeirarchy(scene0, scene1, AnimationTime0, AnimationTime1, pNode0->mChildren[i], pNode1->mChildren[i], GlobalTransformation, stopAnimLevel);
		}
	}

	void BoneTransform(std::vector<aiMatrix4x4>& Transforms)
	{
		aiMatrix4x4 Identity;// Used for ReadNodeHierarchy
		InitIdentityM4(Identity);

		if (myBlendingTime > 0.f)// There are 2 animations that we are blending between
		{
			// Animation time for the first anim ( 0 ) (Previous?)
			// Ticks == Frames
			float TicksPerSecond = 
				static_cast<float>(_scenes[myPrevAnimIndex]->mAnimations[0]->mTicksPerSecond) != 0 
				? 
				static_cast<float>(_scenes[myPrevAnimIndex]->mAnimations[0]->mTicksPerSecond) : 25.0f;

			// How many frames are we into the animation?
			float TimeInTicks = myAnimationTimePrev * TicksPerSecond;
			// Where are we in the animation, on which frame?
			float AnimationTime0 = fmodf(TimeInTicks, static_cast<float>(_scenes[myPrevAnimIndex]->mAnimations[0]->mDuration));
			// !  Animation time for the first anim ( 0 )

			// Animation time for the second anim ( 1 ) (Current?)
			TicksPerSecond = 
				static_cast<float>(_scenes[_curScene]->mAnimations[0]->mTicksPerSecond) != 0 
				?
				static_cast<float>(_scenes[_curScene]->mAnimations[0]->mTicksPerSecond) : 25.0f;

			TimeInTicks = myAnimationTimeCurrent * TicksPerSecond;
			float AnimationTime1 = fmodf(TimeInTicks, static_cast<float>(_scenes[_curScene]->mAnimations[0]->mDuration));
			// ! Animation time for the second anim ( 1 )

			ReadNodeHeirarchy(	_scenes[myPrevAnimIndex], _scenes[_curScene]
								, AnimationTime0, AnimationTime1
								, _scenes[myPrevAnimIndex]->mRootNode, _scenes[_curScene]->mRootNode
								, Identity, /*stopAnimLevel=*/2);
			// Using identity matrix since there is no parent. Means there is no transformation being made on the matrix for the animation.
		}
		else// There is only one animation to play. No blending.
		{
			float TicksPerSecond = 
				static_cast<float>(_scenes[_curScene]->mAnimations[0]->mTicksPerSecond) != 0 
				? 
				static_cast<float>(_scenes[_curScene]->mAnimations[0]->mTicksPerSecond) : 25.0f;
			float TimeInTicks = myAnimationTimePrev * TicksPerSecond;
			float AnimationTime = fmodf(TimeInTicks, static_cast<float>(_scenes[_curScene]->mAnimations[0]->mDuration));

			ReadNodeHeirarchy(_scenes[_curScene], AnimationTime, _scenes[_curScene]->mRootNode, Identity, 2);
		}

		Transforms.resize(m_NumBones);

		for (uint i = 0; i < m_NumBones; i++)
		{
			Transforms[i] = m_BoneInfo[i].myFinalTransformation;
		}
	}

	void LoadBones(uint MeshIndex, const aiMesh* pMesh)
	{
		for (uint i = 0; i < pMesh->mNumBones; i++)
		{
			uint BoneIndex = 0;
			std::string BoneName(pMesh->mBones[i]->mName.data);

			if (m_BoneMapping.find(BoneName) == m_BoneMapping.end())
			{
				BoneIndex = m_NumBones;
				m_NumBones++;
				BoneInfoAnim bi;
				m_BoneInfo.push_back(bi);
			}
			else
			{
				BoneIndex = m_BoneMapping[BoneName];
			}

			m_BoneMapping[BoneName] = BoneIndex;
			m_BoneInfo[BoneIndex].myBoneOffset = pMesh->mBones[i]->mOffsetMatrix;

			for (uint j = 0; j < pMesh->mBones[i]->mNumWeights; j++)
			{
				uint VertexID = m_Entries[MeshIndex].myBaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
				float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
				m_Mass[VertexID].AddBoneData(BoneIndex, Weight);
			}
		}

	}

	bool InitFromScene(const aiScene* pScene)
	{
		myLastTime = -1;
		myAnimationTimePrev = 0.f;

		m_Entries.resize(pScene->mNumMeshes);

		uint NumVertices = 0;
		uint NumIndices = 0;

		// Count the number of vertices and indices
		for (uint i = 0; i < m_Entries.size(); i++)
		{
			m_Entries[i].myMaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
			m_Entries[i].myNumIndices = pScene->mMeshes[i]->mNumFaces * 3;
			m_Entries[i].myBaseVertex = NumVertices;
			m_Entries[i].myBaseIndex = NumIndices;

			NumVertices += pScene->mMeshes[i]->mNumVertices;
			NumIndices += m_Entries[i].myNumIndices;
		}

		m_Mass.resize(NumVertices);

		for (uint i = 0; i < pScene->mNumMeshes; ++i)
		{
			LoadBones(i, pScene->mMeshes[i]);

		}
		return true;
	}

	bool Import3DFromFile(const std::string& anFBXFilePath = "")
	{
		if (anFBXFilePath.length())
			myModelPath = anFBXFilePath;

		// Check if file exists
		std::ifstream fIn(myModelPath.c_str());
		if (!fIn.fail())
		{
			fIn.close();
		}
		else
		{
			MessageBoxA(NULL, ("Couldn't open file: " + myModelPath).c_str(), "ERROR", MB_OK | MB_ICONEXCLAMATION);
			return false;
		}

		_curScene = static_cast<int>(_importers.size());
		_importers.push_back(new Assimp::Importer);
		_scenes.push_back(_importers[_curScene]->ReadFile(myModelPath, aiProcessPreset_TargetRealtime_Quality | aiProcess_ConvertToLeftHanded));
		//_curScene = importer.ReadFile( m_ModelPath, aiProcess_Triangulate | aiProcess_GenSmoothNormals );

		bool ret = false;
		// If the import failed, report it
		if (_scenes[_curScene])
		{
			m_GlobalInverseTransform = _scenes[_curScene]->mRootNode->mTransformation;
			m_GlobalInverseTransform.Inverse();
			ret = InitFromScene(_scenes[_curScene]);
			// Now we can access the file's contents.
			logInfo("Import of _curScene " + myModelPath + " succeeded.");
		}
		else
		{
			logInfo(_importers[_curScene]->GetErrorString());
		}

		// We're done. Everything will be cleaned up by the importer destructor
		return ret;
	}

	void Update(/*float dt*/)
	{
		// using engine dt removes this if case
		if (myLastTime == -1)
		{
			myLastTime = GetCurrentTimeMillis();// 0?
		}
		long long newTime = GetCurrentTimeMillis();
		float dt = (float)((double)newTime - (double)myLastTime) / 1000.0f;// replace with engine dt?
		myLastTime = newTime;
		// using engine dt removes this if case

		myAnimationTimePrev += dt;
		if (myBlendingTime > 0.f)
		{
			myBlendingTime -= dt * myBlendingTimeMul;
			if (myBlendingTime <= 0.f)
			{
				myAnimationTimePrev = myAnimationTimeCurrent;
			}
			if (myUpdateBoth)
			{
				myAnimationTimeCurrent += dt;
			}
		}
		else
		{
			myAnimationTimeCurrent += dt;
		}

		if (myTemporary)
		{
			myPlayTime -= dt;
			if (myPlayTime <= 0.f)
			{
				myTemporary = false;
				SetAnimIndex(myPrevAnimIndex);
			}
		}
	}

	bool Add3DAnimFromFile(const std::string& fileName)
	{
		// Check if file exists
		std::ifstream fin(fileName.c_str());
		if (!fin.fail())
		{
			fin.close();
		}
		else
		{
			MessageBoxA(NULL, ("Couldn't open file: " + fileName).c_str(), "ERROR", MB_OK | MB_ICONEXCLAMATION);
			return false;
		}

		_curScene = static_cast<int>(_importers.size());
		_importers.push_back(new Assimp::Importer);
		_scenes.push_back(_importers[_curScene]->ReadFile(fileName, aiProcessPreset_TargetRealtime_Quality | aiProcess_ConvertToLeftHanded));
		// If the import failed, report it
		if (!_scenes[_curScene])
		{
			logInfo(_importers[_curScene]->GetErrorString());
			return false;
		}
		return true;
	}

	bool SetAnimIndex(uint index, bool updateBoth = true, float blendDuration = 0.3f, bool temporary = false, float time = 0.f)
	{
		if (index == static_cast<uint>(_curScene) || index >= static_cast<uint>(_scenes.size()))
		{
			return false;
		}
		myPrevAnimIndex = _curScene;
		_curScene = index;
		myBlendingTime = 1.f;
		myBlendingTimeMul = blendDuration > 0.0f ? 1.0f / blendDuration : 1.0f;
		myAnimationTimeCurrent = 0.f;
		myUpdateBoth = updateBoth;
		myTemporary = temporary;
		myPlayTime = time;
		return true;
	}

	bool SetBlendTime(float aTime)
	{
		myBlendingTime = aTime;
		return true;
	}

	uint GetMaxIndex()
	{
		return static_cast<uint>(_scenes.size());
	}

	bool IsDoneBlending()
	{
		return myBlendingTime <= 0.0f;
	}
};
