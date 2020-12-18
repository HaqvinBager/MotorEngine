#include <string>
#include <map>
#include <vector>

// assimp include files. These three are usually needed.
#include "assimp/Importer.hpp"	//OO version Header!
#include "assimp/postprocess.h"
#include "assimp/LogStream.hpp"
#include <fstream>

#include "AnimMathFunc.h"
#include "Timer.h"

#include "../ModelLoader/modelExceptionTools.h"

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
/*
	Ex: can every enemy share the same controller?
	animation data like currentScene, playtime etc is held by CAnimation and passed to the controller. Which becomes some form of handler, made static.
*/


class AnimationController
{
private:

	std::string myModelPath;

	aiVector3D myRotation;
	
	float myAnimationTimePrev		= 0;
	float myAnimationTimeCurrent	= 0;
	float myAnimationTimePercent	= 0.f;
	float myBlendingTime			= 0;
	float myBlendingTimeMul			= 0;
	int myPrevAnimIndex				= 0;
	bool myUpdateBoth				= false;
	bool myTemporary				= false;
	float myPlayTime				= 0;


	int myCurSceneIndex		= 0;// we use this
	int myPrevSceneIndex	= 0;
	int myLoopingSceneIndex	= 0;// we also use this
	float myAnimationTimeLooping = 0.0f;
	std::vector<Assimp::Importer*>		myImporters;// This is huge
	std::vector<const aiScene*>			myScenes;// This is huge, entire scene for model. 
	aiMatrix4x4							myGlobalInverseTransform;
	std::map<std::string, uint>			myBoneMapping;
	std::vector<MeshEntry>				myEntries;
	std::vector<BoneInfoAnim>			myBoneInfo;
	uint								myNumOfBones;
	std::vector<VertexBoneDataAnim>		myMass;

public:

	void SetRotation(const aiVector3D& aRotation) { myRotation = aRotation; }
	aiVector3D GetRotation() { return myRotation; }

	void SetCurSceneIndex(int aCurSceneIndex) { myCurSceneIndex = aCurSceneIndex; }
	void SetLoopingSceneIndex(int aSceneIndex) { myLoopingSceneIndex = aSceneIndex; }
	const int GetCurSceneIndex() { return myCurSceneIndex; }
	const int GetLoopingSceneIndex() { return myLoopingSceneIndex; }
	const size_t GetNrOfAnimations() const { return myScenes.size(); }
	void ResetAnimationTimeCurrent() { myAnimationTimeCurrent = 0.0f; }
	const float CurrentAnimationDuration() { return static_cast<float>(myScenes[myCurSceneIndex]->mAnimations[0]->mDuration); }
	const float CurrentAnimationTicksPerSecond() { return static_cast<float>(myScenes[myCurSceneIndex]->mAnimations[0]->mTicksPerSecond); }
	const float CurrentAnimationTimePercent() { return myAnimationTimePercent; }

	~AnimationController()
	{
		Release();
		//for (uint i = 0; i < myScenes.size(); ++i)
		//{
		//	delete myScenes[i];
		//}
		//myScenes.clear();
	}

	void Release()
	{
		// *** cleanup ***
		for (uint i = 0; i < myImporters.size(); ++i)
		{
			delete myImporters[i];
		}
		myImporters.clear();
	}

	AnimationController(const char* aModelPath)
		: myNumOfBones(0)
		, myCurSceneIndex(NULL)
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

	void ReadNodeHeirarchy(	const aiScene* aScene
							, float anAnimationTime
							, const aiNode* aNode
							, const aiMatrix4x4& aParentTransform
							, int aStopAnimAtLevel)
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

		if (myBoneMapping.find(nodeName) != myBoneMapping.end())
		{
			uint BoneIndex = myBoneMapping[nodeName];
			myBoneInfo[BoneIndex].myFinalTransformation = myGlobalInverseTransform * GlobalTransformation * myBoneInfo[BoneIndex].myBoneOffset;
		}

		for (uint i = 0; i < aNode->mNumChildren; i++)
		{
			ReadNodeHeirarchy(aScene, animationTime, aNode->mChildren[i], GlobalTransformation, aStopAnimAtLevel);
		}
	}

	void ReadNodeHeirarchy(	  const aiScene* aFromScene, const aiScene* aToScene
							, float anAnimationTimeFrom, float anAnimationTimeTo
							, const aiNode* aStartNodeFrom, const aiNode* aStartNodeTo
							, const aiMatrix4x4& aParentTransform, int aStopAnimAtLevel)
	{
		float time0(anAnimationTimeFrom);// Why is AnimationTime0 made into a local variable?
		float time1(anAnimationTimeTo);// Why is AnimationTime1 made into a local variable?

		std::string NodeName0(aStartNodeFrom->mName.data);// rename pNode0/1 is the previous animations data. pNode0 is pointerToPreviousAnimNode
		std::string NodeName1(aStartNodeTo->mName.data);// pointerToCurrentAnimNode
		assert(NodeName0 == NodeName1);// their first node should be the same, roots must be equal

		const aiAnimation* pAnimation0 = aFromScene->mAnimations[0];
		const aiAnimation* pAnimation1 = aToScene->mAnimations[0];

		aiMatrix4x4 NodeTransformation0(aStartNodeFrom->mTransformation);
		aiMatrix4x4 NodeTransformation1(aStartNodeTo->mTransformation);

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
				float time(aStopAnimAtLevel <= 0 ? anAnimationTimeFrom : 0.f);
				CalcInterpolatedPosition(Translation0, time, pNodeAnim0);
			}
			aiVector3D Translation1;
			{
				float time(aStopAnimAtLevel <= 0 ? anAnimationTimeTo : 0.f);
				CalcInterpolatedPosition(Translation1, time, pNodeAnim1);
			}
			aiMatrix4x4 TranslationM;
			aiMatrix4x4::Translation(Translation0 * myBlendingTime + Translation1 * (1.f - myBlendingTime), TranslationM);

			// Combine the above transformations
			NodeTransformation0 = TranslationM * RotationM * ScalingM;
		}

		aStopAnimAtLevel--;

		aiMatrix4x4 GlobalTransformation = aParentTransform * NodeTransformation0;

		if (myBoneMapping.find(NodeName0) != myBoneMapping.end())
		{
			uint BoneIndex = myBoneMapping[NodeName0];
			myBoneInfo[BoneIndex].myFinalTransformation = 
				  myGlobalInverseTransform 
				* GlobalTransformation 
				* myBoneInfo[BoneIndex].myBoneOffset;
		}

		uint n = min(aStartNodeFrom->mNumChildren, aStartNodeTo->mNumChildren); // Does one movement for all the children
		for (uint i = 0; i < n; i++)
		{
			ReadNodeHeirarchy(aFromScene, aToScene, anAnimationTimeFrom, anAnimationTimeTo, aStartNodeFrom->mChildren[i], aStartNodeTo->mChildren[i], GlobalTransformation, aStopAnimAtLevel);
		}
	}

	void BoneTransformWithBlend(std::vector<aiMatrix4x4>& aTransformsVector)
	{
		aiMatrix4x4 Identity;// Used for ReadNodeHierarchy
		InitIdentityM4(Identity);

		if (myBlendingTime > 0.f)// There are 2 animations that we are blending between
		{
			// Animation time for the first anim ( 0 ) (Previous?)
			// Ticks == Frames
			float TicksPerSecond = 
				static_cast<float>(myScenes[myPrevAnimIndex]->mAnimations[0]->mTicksPerSecond) != 0 
				? 
				static_cast<float>(myScenes[myPrevAnimIndex]->mAnimations[0]->mTicksPerSecond) : 25.0f;

			// How many frames are we into the animation?
			float TimeInTicks = myAnimationTimePrev * TicksPerSecond;
			// Where are we in the animation, on which frame?
			float AnimationTime0 = fmodf(TimeInTicks, static_cast<float>(myScenes[myPrevAnimIndex]->mAnimations[0]->mDuration));
			// !  Animation time for the first anim ( 0 )

			// Animation time for the second anim ( 1 ) (Current?)
			TicksPerSecond = 
				static_cast<float>(myScenes[myCurSceneIndex]->mAnimations[0]->mTicksPerSecond) != 0 
				?
				static_cast<float>(myScenes[myCurSceneIndex]->mAnimations[0]->mTicksPerSecond) : 25.0f;

			TimeInTicks = myAnimationTimeCurrent * TicksPerSecond;
			float AnimationTime1 = fmodf(TimeInTicks, static_cast<float>(myScenes[myCurSceneIndex]->mAnimations[0]->mDuration));
			// ! Animation time for the second anim ( 1 )

			ReadNodeHeirarchy(	myScenes[myPrevAnimIndex], myScenes[myCurSceneIndex]
							  , AnimationTime0, AnimationTime1
							  , myScenes[myPrevAnimIndex]->mRootNode, myScenes[myCurSceneIndex]->mRootNode
							  , Identity, /*stopAnimLevel=*/2);
			// Using identity matrix since there is no parent. Means there is no transformation being made on the matrix for the animation.
		}
		else// There is only one animation to play. No blending.
		{
			float TicksPerSecond = 
				static_cast<float>(myScenes[myCurSceneIndex]->mAnimations[0]->mTicksPerSecond) != 0 
				? 
				static_cast<float>(myScenes[myCurSceneIndex]->mAnimations[0]->mTicksPerSecond) : 25.0f;
			float TimeInTicks = myAnimationTimePrev * TicksPerSecond;
			float AnimationTime = fmodf(TimeInTicks, static_cast<float>(myScenes[myCurSceneIndex]->mAnimations[0]->mDuration));

			ReadNodeHeirarchy(myScenes[myCurSceneIndex], AnimationTime, myScenes[myCurSceneIndex]->mRootNode, Identity, 2);
		}

		aTransformsVector.resize(myNumOfBones);

		for (uint i = 0; i < myNumOfBones; i++)
		{
			aTransformsVector[i] = myBoneInfo[i].myFinalTransformation;
		}
	}

	/// PlayAnim(animIndex, anIsLoop)
	/// 
	/// AnimationTime is the current frame of the animation between 0.0f to mDuration
	/// Once anim is almost mDuration the animation resets.
	/// (ceil(AnimationTime) >= static_cast<float>(myScenes[myCurSceneIndex]->mAnimations[0]->mDuration))
	/// returns 1 if the animation is complete.
	/// if 1 set myAnimationTimeCurrent = 0.0f
	/// set myCurSceneIndex to myLoopingSceneIndex

	void  BoneTransform(std::vector<aiMatrix4x4>& aTransformsVector, const float /*anAnimSpeedMultiplier*/)
	{
		aiMatrix4x4 identity;// Used for ReadNodeHierarchy
		InitIdentityM4(identity);

		bool switchBackToLooping = false;
		float AnimationTime = 0.0f;
		if (myScenes[myCurSceneIndex]->mAnimations != nullptr)
		{
			float animTimeCurrentModified = myAnimationTimeCurrent;//myAnimationTimeCurrent + 0.000001f) * anAnimSpeedMultiplier;

			float TicksPerSecond = 
				static_cast<float>(myScenes[myCurSceneIndex]->mAnimations[0]->mTicksPerSecond) != 0 
				? 
				static_cast<float>(myScenes[myCurSceneIndex]->mAnimations[0]->mTicksPerSecond) : 25.0f;
			float TimeInTicks = animTimeCurrentModified/*myAnimationTimeCurrent*/ * TicksPerSecond;
			AnimationTime = fmodf(TimeInTicks, static_cast<float>(myScenes[myCurSceneIndex]->mAnimations[0]->mDuration));
			
			//std::cout << "animTime " << AnimationTime << std::endl;
			//std::cout << "TimeInTicks " << TimeInTicks << std::endl;
			//std::cout << "animTime " << AnimationTime << std::endl;

			ReadNodeHeirarchy(myScenes[myCurSceneIndex], AnimationTime, myScenes[myCurSceneIndex]->mRootNode, identity, 2);
			// anAnimSpeedMultiplier = 20.0f =>  ceil(AnimationTime * myAnimationTimeCurrent * 2.0f) stoppar anim perfekt
			/// 
			/// 
			//switchBackToLooping = (ceil(AnimationTime * myAnimationTimeCurrent * (anAnimSpeedMultiplier / myScenes[myCurSceneIndex]->mAnimations[0]->mDuration)) >= static_cast<float>(myScenes[myCurSceneIndex]->mAnimations[0]->mDuration));
			//switchBackToLooping = (ceil(AnimationTime * myScenes[myCurSceneIndex]->mAnimations[0]->mDuration) >= static_cast<float>(myScenes[myCurSceneIndex]->mAnimations[0]->mDuration));
			const float duration = static_cast<float>(myScenes[myCurSceneIndex]->mAnimations[0]->mDuration);
			switchBackToLooping		= (ceil(AnimationTime) >= duration);
			myAnimationTimePercent = AnimationTime / duration;
			 //std::cout << "switchBack " << switchBackToLooping << std::endl;
		}

		aTransformsVector.resize(myNumOfBones);

		for (uint i = 0; i < myNumOfBones; i++)
		{
			aTransformsVector[i] = myBoneInfo[i].myFinalTransformation;
		}

		if (switchBackToLooping)
		{
			myCurSceneIndex = myLoopingSceneIndex;
		}
	}

	void LoadBones(uint aMeshIndex, const aiMesh* aMesh)
	{
		for (uint i = 0; i < aMesh->mNumBones; i++)
		{
			uint BoneIndex = 0;
			std::string BoneName(aMesh->mBones[i]->mName.data);

			if (myBoneMapping.find(BoneName) == myBoneMapping.end())
			{
				BoneIndex = myNumOfBones;
				myNumOfBones++;
				BoneInfoAnim bi;
				myBoneInfo.push_back(bi);
			}
			else
			{
				BoneIndex = myBoneMapping[BoneName];
			}

			myBoneMapping[BoneName] = BoneIndex;
			myBoneInfo[BoneIndex].myBoneOffset = aMesh->mBones[i]->mOffsetMatrix;

			for (uint j = 0; j < aMesh->mBones[i]->mNumWeights; j++)
			{
				uint VertexID = myEntries[aMeshIndex].myBaseVertex + aMesh->mBones[i]->mWeights[j].mVertexId;
				float Weight = aMesh->mBones[i]->mWeights[j].mWeight;
				myMass[VertexID].AddBoneData(BoneIndex, Weight);
			}
		}

	}

	bool InitFromScene(const aiScene* pScene)
	{
		
		myAnimationTimePrev = 0.f;

		myEntries.resize(pScene->mNumMeshes);

		uint NumVertices = 0;
		uint NumIndices = 0;

		// Count the number of vertices and indices
		for (uint i = 0; i < myEntries.size(); i++)
		{
			myEntries[i].myMaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
			myEntries[i].myNumIndices = pScene->mMeshes[i]->mNumFaces * 3;
			myEntries[i].myBaseVertex = NumVertices;
			myEntries[i].myBaseIndex = NumIndices;

			NumVertices += pScene->mMeshes[i]->mNumVertices;
			NumIndices += myEntries[i].myNumIndices;
		}

		myMass.resize(NumVertices);

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

		myCurSceneIndex = static_cast<int>(myImporters.size());
		myImporters.push_back(new Assimp::Importer);

		using namespace ModelExceptionTools;
		if (IsDestructibleModel(anFBXFilePath))
		{
			//myImporters[myCurSceneIndex]->GetOrphanedScene()// Cool but can't be used atm
			myScenes.push_back(myImporters[myCurSceneIndex]->ReadFile(myModelPath, aiProcessPreset_TargetRealtime_Quality_DontJoinIdentical | aiProcess_ConvertToLeftHanded));
		}
		else
		{
			myScenes.push_back(myImporters[myCurSceneIndex]->ReadFile(myModelPath, aiProcessPreset_TargetRealtime_Quality | aiProcess_ConvertToLeftHanded));
		}
		//_curScene = importer.ReadFile( m_ModelPath, aiProcess_Triangulate | aiProcess_GenSmoothNormals );

		bool ret = false;
		// If the import failed, report it
		if (myScenes[myCurSceneIndex])
		{
			myGlobalInverseTransform = myScenes[myCurSceneIndex]->mRootNode->mTransformation;
			myGlobalInverseTransform.Inverse();
			ret = InitFromScene(myScenes[myCurSceneIndex]);
			// Now we can access the file's contents.
			logInfo("Import of _curScene " + myModelPath + " succeeded.");
		}
		else
		{
			logInfo(myImporters[myCurSceneIndex]->GetErrorString());
		}

		// We're done. Everything will be cleaned up by the importer destructor
		return ret;
	}

	void UpdateBlendFrame()
	{
		float dt = CTimer::Dt();

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

	void UpdateFrame()
	{
		float dt = CTimer::Dt();
		
		//myAnimationTimeCurrent += dt;
		//myAnimationTimePrev += dt;
		if (myCurSceneIndex != myPrevSceneIndex)
		{
			myBlendingTime = 0.0f;
		}
		myAnimationTimeCurrent += dt;

	
		//if (myTemporary)
		//{
		//	myPlayTime -= dt;
		//	if (myPlayTime <= 0.f)
		//	{
		//		myTemporary = false;
		//		SetAnimIndex(myPrevAnimIndex);
		//	}
		//}
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

		myCurSceneIndex = static_cast<int>(myImporters.size());
		myImporters.push_back(new Assimp::Importer);
		myScenes.push_back(myImporters[myCurSceneIndex]->ReadFile(fileName, aiProcessPreset_TargetRealtime_Quality | aiProcess_ConvertToLeftHanded));
		
		// aiProcessPreset_TargetRealtime_MaxQuality_DontJoinIndetical affects performance. Create someway to toggle it, so that only destructibles use it!
		//myScenes.push_back(myImporters[myCurSceneIndex]->ReadFile(fileName, aiProcessPreset_TargetRealtime_Quality_DontJoinIdentical | aiProcess_ConvertToLeftHanded));
		
		// If the import failed, report it
		if (!myScenes[myCurSceneIndex])
		{
			logInfo(myImporters[myCurSceneIndex]->GetErrorString());
			return false;
		}
		return true;
	}

	bool SetAnimIndex(uint index, bool updateBoth = true, float blendDuration = 0.3f, bool temporary = false, float time = 0.f)
	{
		if (index == static_cast<uint>(myCurSceneIndex) || index >= static_cast<uint>(myScenes.size()))
		{
			return false;
		}
		myPrevAnimIndex = myCurSceneIndex;
		myCurSceneIndex = index;
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
		return static_cast<uint>(myScenes.size());
	}

	bool IsDoneBlending()
	{
		return myBlendingTime <= 0.0f;
	}
};
