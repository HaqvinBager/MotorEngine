#pragma once

#include <string>

struct ID3D11Device;

class DirectX11Framework;
class CFBXLoaderCustom;

namespace ModelSystem
{
	class Model;
	class ModelBuilder;

	class ModelFactory
	{
	public:
		ModelFactory( const ModelFactory &other )			  = delete;
		ModelFactory &operator =( const ModelFactory &other ) = delete;
		static void			 Create();
		static ModelFactory& GetInstance();
		void				 Init(DirectX11Framework* aFramework);
		void				 Destroy();

		Model* GetExistingModel(const std::string& aModelPath);
		Model* LoadModel(const std::string& aModelPath);
		Model* LoadModelTexturePacked(const std::string& aModelPath);

		Model* GetCube();

	private:
		ModelFactory();
		~ModelFactory();

		static ModelFactory* myInstance;

		ID3D11Device* myDevice;

		//Temp
		Model* myCube;
		// Add unoderedmap to hold loaded models [model][filepath]
		//!Temp
	};
}