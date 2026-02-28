#include "lab_m1/tema1/my_utils/util.h"

Mesh* MergeMeshes(const std::string& name, std::vector<Mesh*>& meshes)
{
	int cntVertices = 0;
	int cntIndices = 0;
	for (Mesh* mesh : meshes) {
		cntVertices += mesh->vertices.size();
		cntIndices += mesh->indices.size();
	}

	std::vector<VertexFormat> mergedVertices;
	std::vector<unsigned int> mergedIndices;
	mergedVertices.reserve(cntVertices);
	mergedIndices.reserve(cntIndices);

	for (Mesh* mesh : meshes) {
		int vertexOffset = mergedVertices.size();
		mergedVertices.insert(mergedVertices.end(), mesh->vertices.begin(), mesh->vertices.end());
		for (unsigned int index : mesh->indices) {
			mergedIndices.push_back(index + vertexOffset);
		}
	}

	Mesh* mergedMesh = new Mesh(name);
	mergedMesh->InitFromData(mergedVertices, mergedIndices);
	return mergedMesh;
}