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
		DirectX::SimpleMath::Vector3 pos;//���W
		DirectX::SimpleMath::Vector3 Normal; //�@���x�N�g��

	};

	struct VERTEX_FILE
	{
		DirectX::SimpleMath::Vector3 pos;//���W
		DirectX::SimpleMath::Vector3 Normal; //�@���x�N�g��
		DirectX::SimpleMath::Vector2 UV;//�e�N�Z��
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
		// ��� (0:bone 1:mesh)
		int kind;

		// ���O
		char name[32];

		// �Z��̃m�[�h�ԍ�
		int sibling;

		// �q���̃m�[�h�ԍ�
		int child;

		// ��{�p��
		DirectX::SimpleMath::Vector3 translation;
		DirectX::SimpleMath::Quaternion rotetion;
		//FbxDouble3 scaling;

		// �e�N�X�`����(Diffuse)
		char textureNameA[32];

		// �e�N�X�`����(Specular)
		char textureNameB[32];

		// �e�N�X�`����(Bump)
		char textureNameC[32];

		// �}�e���A�����
		Material material;

	};
	std::wstring name;

	std::vector<VERTEX_LANDSHAPE>m_Vertex ;
	std::vector<USHORT> m_Indices;

	static std::unique_ptr<LandShapeDate>CreateFromMDL(const char*FilenName);
	static std::unique_ptr<LandShapeDate>CreateFromMDL(const wchar_t*szFilenName);

	static void UpdateNodeMatrices(int index,const Nodeinfo* nodeinfo_arrey,DirectX::SimpleMath::Matrix*pParentMatrix, std::vector<	std::vector<VERTEX_LANDSHAPE>>&vertex_array);

	
};




