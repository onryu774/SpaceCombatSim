#include<fstream>
#include<algorithm>
#include "LandShapeDate.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

std::unique_ptr<LandShapeDate> LandShapeDate::CreateFromMDL(const char*meshDate)
{
	const void*head = meshDate;
	std::unique_ptr<LandShapeDate>landshape(new LandShapeDate());

	const UINT*p_nNode = static_cast<const UINT*>(head);

	head = p_nNode + 1;
	UINT nNode = *p_nNode;

	const Nodeinfo*nodeinfo_arrey = static_cast<const Nodeinfo*>(head);
	head = nodeinfo_arrey + nNode;

	int mesh = 0;

	landshape->m_Indices.clear();
	vector<vector<VERTEX_LANDSHAPE>> vertex_array;
	vector<vector<USHORT>> index_array;

	for (UINT i = 0; i < nNode; i++)
	{

		const Nodeinfo*nodeInfo = &nodeinfo_arrey[i];
		vector<VERTEX_LANDSHAPE>va;
		vector<USHORT>ia;
		vertex_array.push_back(va);
		index_array.push_back(ia);

		if (nodeInfo->kind == 1)
		{
			const UINT*p_nVertex= static_cast<const UINT*>(head);
			head = p_nVertex + 1;

			UINT nVertex = *p_nVertex;
			vertex_array[i].reserve(nVertex);
			size_t vbBytes = sizeof(VERTEX_FILE)*nVertex;
			const VERTEX_FILE*node_Vertex_arrey = static_cast<const VERTEX_FILE*>(head);
			head = node_Vertex_arrey + nVertex;

			for (UINT j = 0; j < nVertex; j++)
			{
				const VERTEX_FILE*vertex = &node_Vertex_arrey[j];
				VERTEX_LANDSHAPE vertex_landshape;
				vertex_landshape.pos = vertex->pos;
				vertex_landshape.Normal = vertex->Normal;
				vertex_array[i].push_back(vertex_landshape);
			}

			const UINT*p_nIndices= static_cast<const UINT*>(head);
			head = p_nIndices + 1;
			UINT nIndices = *p_nIndices;
			index_array[i].reserve(nIndices);


			size_t inBytes = sizeof(USHORT)*nIndices;
			const USHORT*node_index_arrey= static_cast<const USHORT*>(head);
			head = node_index_arrey + nIndices;

			for (UINT j = 0; j < nIndices; j++)
			{

				const USHORT* index = &node_index_arrey[j];
				index_array[i].push_back(*index);
			}
		}
	}

	UpdateNodeMatrices(0, nodeinfo_arrey, nullptr, vertex_array);

	landshape->m_Vertex.clear();
	landshape->m_Indices.clear();

	int Vertex_count = 0;
	for (UINT i = 0; i < nNode; i++)
	{
		vector<USHORT>::iterator it;

		for (it  = index_array[i].begin(); it != index_array[i].end(); it++)
		{
			*it += Vertex_count;
		}


		Vertex_count += vertex_array[i].size();

		landshape->m_Vertex.insert(landshape->m_Vertex.end(), vertex_array[i].begin(), vertex_array[i].end());
		landshape->m_Indices.insert(landshape->m_Indices.end(), index_array[i].begin(), index_array[i].end());

	}
	return landshape;

}

std::unique_ptr<LandShapeDate> LandShapeDate::CreateFromMDL(const wchar_t*szFilenName)
{
	ifstream ifs;
	std::unique_ptr<char[]>date;
	ifs.open(szFilenName, ios::in | ios::binary);
	assert(ifs&&"CreateFromMDL Error:Failed to open MDL");
	ifs.seekg(0, fstream::end);

	streamoff eofPos = ifs.tellg();
	ifs.clear();
	ifs.seekg(0, fstream::beg);
	streamoff begPos = ifs.tellg();
	streamoff size = eofPos- begPos;

	date.reset(new char[(UINT)size]);
	ifs.read(date.get(),size);
	ifs.close();

	OutputDebugStringW(szFilenName);
	OutputDebugStringW(L"\n");

	auto model = CreateFromMDL(date.get());
	model->name = szFilenName;
	return model;


}

void LandShapeDate::UpdateNodeMatrices(int index, const Nodeinfo * nodeinfo_arrey, DirectX::SimpleMath::Matrix * pParentMatrix, std::vector<std::vector<VERTEX_LANDSHAPE>>& vertex_array)
{
	const Nodeinfo*nodeinfo = &nodeinfo_arrey[index];
	Matrix modelm;
	Matrix rotm = Matrix::CreateFromQuaternion(nodeinfo->rotetion);
	Matrix transm = Matrix::CreateTranslation(nodeinfo->translation);

	modelm *= rotm;
	modelm *= transm;
	if (pParentMatrix != nullptr)
	{
		modelm = modelm*(*pParentMatrix);
	}

	if (nodeinfo->kind==1)
	{
		vector<VERTEX_LANDSHAPE>::iterator it;
		for (it = vertex_array[index].begin(); it != vertex_array[index].end(); it++)
		{
			it->pos = Vector3::Transform(it->pos, modelm);
			it->Normal = Vector3::TransformNormal(it->Normal, modelm);
			char str[256];
			sprintf_s(str, "(%.3f,%.3f,%.3f)\n", it->pos.x, it->pos.y, it->pos.z);
			OutputDebugStringA(str);
		}
	}
	if (nodeinfo->sibling >0)
	{
		UpdateNodeMatrices(nodeinfo->sibling, nodeinfo_arrey, pParentMatrix, vertex_array);
	}

	if (nodeinfo->child >0)
	{
		UpdateNodeMatrices(nodeinfo->child, nodeinfo_arrey, &modelm, vertex_array);
	}

}


