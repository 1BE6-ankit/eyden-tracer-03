#include "Scene.h"

#include "ShaderEyelight.h"
#include "ShaderPhong.h"
#include "PrimTriangle.h"

#include <stdio.h>


void CScene::ParseOBJ(const std::string& fileName)
{
	std::cout << "Parsing OBJFile : " << fileName << std::endl;

	std::shared_ptr<IShader> pShader = std::make_shared<CShaderEyelight>(RGB(1, 1, 1));
	// --- PUT YOUR CODE HERE ---

	FILE* fs = fopen(fileName.c_str(), "r");
	if(fs == NULL) {
		perror("Cannot open data file\n");
		exit(1);
	}

	char id;
	float v1, v2, v3; // x,y,z vertex points
	int i1, i2, i3; // indices	
	std::vector<std::vector<float>> vertices;
	Vec3f vertex1, vertex2, vertex3;

	while(fscanf(fs, "%c %f %f %f\n", &id, &v1, &v2, &v3) != EOF) {
		if(id == 'v') {
			// add vertices
			vertices.push_back({v1*99.99f, v2*99.99f, v3*99.99f});
		} else if(id == 'f') {
			// Get the indices for vertices vector that represent the vertices
			// of a triangle
			i1 = (int) v1 - 1; i2 = (int) v2 - 1; i3 = (int) v3 - 1; 
			vertex1 = Vec3f(vertices[i1][0], vertices[i1][1], vertices[i1][2]);
			vertex2 = Vec3f(vertices[i2][0], vertices[i2][1], vertices[i2][2]);
			vertex3 = Vec3f(vertices[i3][0], vertices[i3][1], vertices[i3][2]);

			Add(std::make_shared<CPrimTriangle>(vertex1, vertex2, vertex3, pShader));
		}
	}

	fclose(fs);
	std::cout << "Finished Parsing" << std::endl;
}

