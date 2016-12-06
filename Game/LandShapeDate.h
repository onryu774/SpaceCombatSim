#pragma once
#include<vector>
#include<memory>
#include<SimpleMath.h>
class LandShapeDate
{
public:
	friend class LandShape;
	struct VERTEX_LANDSHAPE
	{
		DirectX::SimpleMath::Vector3 pos;//座標
		DirectX::SimpleMath::Vector3 Normal; //法線ベクトル

	};

	struct VERTEX_FILE
	{
		DirectX::SimpleMath::Vector3 pos;//座標
		DirectX::SimpleMath::Vector3 Normal; //法線ベクトル
		DirectX::SimpleMath::Vector2 UV;//テクセル
	};
	struct Nodeinfo
	{
		struct Material
		{
			char name[32];
			DirectX::SimpleMath::Vector3 Ambient;
			DirectX::SimpleMath::Vector3 Diffuse; 
			DirectX::SimpleMath::Vector3 Specular;
			DirectX::SimpleMath::Vector3 Emissive;
			float Opacity;
			float Shininess;
			float Reflectivity;
		};
		// 種類 (0:bone 1:mesh)
		int kind;

		// 名前
		char name[32];

		// 兄弟のノード番号
		int sibling;

		// 子供のノード番号
		int child;

		// 基本姿勢
		DirectX::SimpleMath::Vector3 translation;
		DirectX::SimpleMath::Quaternion rotetion;
		//FbxDouble3 scaling;

		// テクスチャ名(Diffuse)
		char textureNameA[32];

		// テクスチャ名(Specular)
		char textureNameB[32];

		// テクスチャ名(Bump)
		char textureNameC[32];

		// マテリアル情報
		Material material;

	};
	std::wstring name;

	std::vector<VERTEX_LANDSHAPE>m_Vertex ;
	std::vector<USHORT> m_Indices;

	static std::unique_ptr<LandShapeDate>CreateFromMDL(const char*FilenName);
	static std::unique_ptr<LandShapeDate>CreateFromMDL(const wchar_t*szFilenName);

	static void UpdateNodeMatrices(int index,const Nodeinfo* nodeinfo_arrey,DirectX::SimpleMath::Matrix*pParentMatrix, std::vector<	std::vector<VERTEX_LANDSHAPE>>&vertex_array);

	
};




