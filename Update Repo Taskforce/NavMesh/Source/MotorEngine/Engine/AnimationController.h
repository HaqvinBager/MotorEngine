#include <string>
#include <map>
#include <vector>

// assimp include files. These three are usually needed.
#include "assimp/Importer.hpp"	//OO version Header!
#include "assimp/postprocess.h"
#include "assimp/LogStream.hpp"
#include <fstream>

#include "MathFunc.h"

#define NUM_BONES_PER_VEREX 4

struct VertexBoneDataAnim
{
	uint IDs[NUM_BONES_PER_VEREX];
	float Weights[NUM_BONES_PER_VEREX];

	VertexBoneDataAnim()
	{
		Reset();
	};

	void Reset()
	{
		memset(IDs, 0, sizeof(IDs));
		memset(Weights, 0, sizeof(Weights));
	}


	void AddBoneData(uint BoneID, float Weight)
	{
		for (uint i = 0; i < NUM_BONES_PER_VEREX; i++)
		{
			if (Weights[i] == 0.0)
			{
				IDs[i] = BoneID;
				Weights[i] = Weight;
				return;
			}
		}

		// should never get here - more bones than we have space for
		assert(false);
	}
};

struct BoneInfoAnim
{
	aiMatrix4x4 BoneOffset;
	aiMatrix4x4 FinalTransformation;
};

#define INVALID_MATERIAL 0xFFFFFFFF;

struct MeshEntry
{
	MeshEntry()
	{
		NumIndices = 0;
		BaseVertex = 0;
		BaseIndex = 0;
		MaterialIndex = INVALID_MATERIAL;
	}

	unsigned int NumIndices;
	unsigned int BaseVertex;
	unsigned int BaseIndex;
	unsigned int MaterialIndex;
};

class AnimationController
{
private:

	std::string m_ModelPath;

	aiVector3D _rotation;
	long long m_lastTime = 0;
	float _animationTime0 = 0;
	float _animationTime1 = 0;
	float _blendingTime = 0;
	float _blendingTimeMul = 0;
	int _prevAnimIndex = 0;
	bool _updateBoth = false;
	bool _temporary = false;
	float _playTime = 0;


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

	void SetRotation(const aiVector3D& r) { _rotation = r; }
	aiVector3D GetRotation() { return _rotation; }

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

	AnimationController(const char* modelpath)
		: m_NumBones(0)
		, _curScene(NULL)
		, m_ModelPath(modelpath)
		, _rotation(0.f)
		, _prevAnimIndex(-1)
		, _blendingTime(0.f)
		, _blendingTimeMul(1.f)
		, _updateBoth(true)
		, _temporary(false)
		, _playTime(0.f)
	{
	}

	void ReadNodeHeirarchy(const aiScene* scene, float AnimationTime, const aiNode* pNode, const aiMatrix4x4& ParentTransform, int stopAnimLevel)
	{
		float time(AnimationTime);

		std::string NodeName(pNode->mName.data);

		const aiAnimation* pAnimation = scene->mAnimations[0];

		aiMatrix4x4 NodeTransformation(pNode->mTransformation);

		const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

		if (pNodeAnim)
		{
			// Interpolate scaling and generate scaling transformation matrix
			aiVector3D Scaling;
			CalcInterpolatedScaling(Scaling, time, pNodeAnim);
			aiMatrix4x4 ScalingM;
			aiMatrix4x4::Scaling(Scaling, ScalingM);

			// Interpolate rotation and generate rotation transformation matrix
			aiQuaternion RotationQ;
			CalcInterpolatedRotation(RotationQ, time, pNodeAnim);
			aiMatrix4x4 RotationM;
			InitM4FromM3(RotationM, RotationQ.GetMatrix());

			// Interpolate translation and generate translation transformation matrix
			aiVector3D Translation;
			{
				float timeStop(stopAnimLevel <= 0 ? AnimationTime : 0.f);
				CalcInterpolatedPosition(Translation, timeStop, pNodeAnim);
			}
			aiMatrix4x4 TranslationM;
			aiMatrix4x4::Translation(Translation, TranslationM);

			// Combine the above transformations
			NodeTransformation = TranslationM * RotationM * ScalingM;
		}
		stopAnimLevel--;

		aiMatrix4x4 GlobalTransformation = ParentTransform * NodeTransformation;

		if (m_BoneMapping.find(NodeName) != m_BoneMapping.end())
		{
			uint BoneIndex = m_BoneMapping[NodeName];
			m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;
		}

		for (uint i = 0; i < pNode->mNumChildren; i++)
		{
			ReadNodeHeirarchy(scene, AnimationTime, pNode->mChildren[i], GlobalTransformation, stopAnimLevel);
		}
	}

	void ReadNodeHeirarchy(const aiScene* scene0, const aiScene* scene1
		, float AnimationTime0, float AnimationTime1
		, const aiNode* pNode0, const aiNode* pNode1
		, const aiMatrix4x4& ParentTransform, int stopAnimLevel)
	{
		float time0(AnimationTime0);
		float time1(AnimationTime1);

		std::string NodeName0(pNode0->mName.data);
		std::string NodeName1(pNode1->mName.data);
		assert(NodeName0 == NodeName1);

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
			aiMatrix4x4::Scaling(Scaling0 * _blendingTime + Scaling1 * (1.f - _blendingTime), ScalingM);

			// Interpolate rotation and generate rotation transformation matrix
			aiQuaternion RotationQ0;
			CalcInterpolatedRotation(RotationQ0, time0, pNodeAnim0);
			aiQuaternion RotationQ1;
			CalcInterpolatedRotation(RotationQ1, time1, pNodeAnim1);
			aiMatrix4x4 RotationM;
			aiQuaternion RotationQ;
			aiQuaternion::Interpolate(RotationQ, RotationQ1, RotationQ0, _blendingTime);
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
			aiMatrix4x4::Translation(Translation0 * _blendingTime + Translation1 * (1.f - _blendingTime), TranslationM);

			// Combine the above transformations
			NodeTransformation0 = TranslationM * RotationM * ScalingM;
		}

		stopAnimLevel--;

		aiMatrix4x4 GlobalTransformation = ParentTransform * NodeTransformation0;

		if (m_BoneMapping.find(NodeName0) != m_BoneMapping.end())
		{
			uint BoneIndex = m_BoneMapping[NodeName0];
			m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation *
				m_BoneInfo[BoneIndex].BoneOffset;
		}

		uint n = min(pNode0->mNumChildren, pNode1->mNumChildren);
		for (uint i = 0; i < n; i++)
		{
			ReadNodeHeirarchy(scene0, scene1, AnimationTime0, AnimationTime1, pNode0->mChildren[i], pNode1->mChildren[i], GlobalTransformation, stopAnimLevel);
		}
	}

	void BoneTransform(std::vector<aiMatrix4x4>& Transforms)
	{
		aiMatrix4x4 Identity;
		InitIdentityM4(Identity);

		if (_blendingTime > 0.f)
		{
			float TicksPerSecond = static_cast<float>(_scenes[_prevAnimIndex]->mAnimations[0]->mTicksPerSecond) != 0 ?
				static_cast<float>(_scenes[_prevAnimIndex]->mAnimations[0]->mTicksPerSecond) : 25.0f;
			float TimeInTicks = _animationTime0 * TicksPerSecond;
			float AnimationTime0 = fmodf(TimeInTicks, static_cast<float>(_scenes[_prevAnimIndex]->mAnimations[0]->mDuration));

			TicksPerSecond = static_cast<float>(_scenes[_curScene]->mAnimations[0]->mTicksPerSecond) != 0 ?
				static_cast<float>(_scenes[_curScene]->mAnimations[0]->mTicksPerSecond) : 25.0f;
			TimeInTicks = _animationTime1 * TicksPerSecond;
			float AnimationTime1 = fmodf(TimeInTicks, static_cast<float>(_scenes[_curScene]->mAnimations[0]->mDuration));

			ReadNodeHeirarchy(_scenes[_prevAnimIndex], _scenes[_curScene]
				, AnimationTime0, AnimationTime1
				, _scenes[_prevAnimIndex]->mRootNode, _scenes[_curScene]->mRootNode
				, Identity, 2);
		}
		else
		{
			float TicksPerSecond = static_cast<float>(_scenes[_curScene]->mAnimations[0]->mTicksPerSecond) != 0 ? static_cast<float>(_scenes[_curScene]->mAnimations[0]->mTicksPerSecond) : 25.0f;
			float TimeInTicks = _animationTime0 * TicksPerSecond;
			float AnimationTime = fmodf(TimeInTicks, static_cast<float>(_scenes[_curScene]->mAnimations[0]->mDuration));

			ReadNodeHeirarchy(_scenes[_curScene], AnimationTime, _scenes[_curScene]->mRootNode, Identity, 2);
		}

		Transforms.resize(m_NumBones);

		for (uint i = 0; i < m_NumBones; i++)
		{
			Transforms[i] = m_BoneInfo[i].FinalTransformation;
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
			m_BoneInfo[BoneIndex].BoneOffset = pMesh->mBones[i]->mOffsetMatrix;

			for (uint j = 0; j < pMesh->mBones[i]->mNumWeights; j++)
			{
				uint VertexID = m_Entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
				float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
				m_Mass[VertexID].AddBoneData(BoneIndex, Weight);
			}
		}

	}

	bool InitFromScene(const aiScene* pScene)
	{
		m_lastTime = -1;
		_animationTime0 = 0.f;

		m_Entries.resize(pScene->mNumMeshes);

		uint NumVertices = 0;
		uint NumIndices = 0;

		// Count the number of vertices and indices
		for (uint i = 0; i < m_Entries.size(); i++)
		{
			m_Entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
			m_Entries[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
			m_Entries[i].BaseVertex = NumVertices;
			m_Entries[i].BaseIndex = NumIndices;

			NumVertices += pScene->mMeshes[i]->mNumVertices;
			NumIndices += m_Entries[i].NumIndices;
		}

		m_Mass.resize(NumVertices);

		for (uint i = 0; i < pScene->mNumMeshes; ++i)
		{
			LoadBones(i, pScene->mMeshes[i]);

		}
		return true;
	}

	bool Import3DFromFile(const std::string& pFile = "")
	{
		if (pFile.length())
			m_ModelPath = pFile;

		// Check if file exists
		std::ifstream fin(m_ModelPath.c_str());
		if (!fin.fail())
		{
			fin.close();
		}
		else
		{
			MessageBoxA(NULL, ("Couldn't open file: " + m_ModelPath).c_str(), "ERROR", MB_OK | MB_ICONEXCLAMATION);
			return false;
		}

		_curScene = static_cast<int>(_importers.size());
		_importers.push_back(new Assimp::Importer);
		_scenes.push_back(_importers[_curScene]->ReadFile(m_ModelPath, aiProcessPreset_TargetRealtime_Quality | aiProcess_ConvertToLeftHanded));
		//_curScene = importer.ReadFile( m_ModelPath, aiProcess_Triangulate | aiProcess_GenSmoothNormals );

		bool ret = false;
		// If the import failed, report it
		if (_scenes[_curScene])
		{
			m_GlobalInverseTransform = _scenes[_curScene]->mRootNode->mTransformation;
			m_GlobalInverseTransform.Inverse();
			ret = InitFromScene(_scenes[_curScene]);
			// Now we can access the file's contents.
			logInfo("Import of _curScene " + m_ModelPath + " succeeded.");
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
		if (m_lastTime == -1)
		{
			m_lastTime = GetCurrentTimeMillis();
		}
		long long newTime = GetCurrentTimeMillis();
		float dt = (float)((double)newTime - (double)m_lastTime) / 1000.0f;
		m_lastTime = newTime;


		_animationTime0 += dt;
		if (_blendingTime > 0.f)
		{
			_blendingTime -= dt * _blendingTimeMul;
			if (_blendingTime <= 0.f)
			{
				_animationTime0 = _animationTime1;
			}
			if (_updateBoth)
			{
				_animationTime1 += dt;
			}
		}
		else
		{
			_animationTime1 += dt;
		}

		if (_temporary)
		{
			_playTime -= dt;
			if (_playTime <= 0.f)
			{
				_temporary = false;
				SetAnimIndex(_prevAnimIndex);
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
		_prevAnimIndex = _curScene;
		_curScene = index;
		_blendingTime = 1.f;
		_blendingTimeMul = 1.f / blendDuration;
		_animationTime1 = 0.f;
		_updateBoth = updateBoth;
		_temporary = temporary;
		_playTime = time;
		return true;
	}

	bool SetBlendTime(float aTime)
	{
		_blendingTime = aTime;
		return true;
	}

	uint GetMaxIndex()
	{
		return static_cast<uint>(_scenes.size());
	}

	bool IsDoneBlending()
	{
		return _blendingTime <= 0.0f;
	}
};
