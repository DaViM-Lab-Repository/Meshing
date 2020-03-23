#ifndef __Mesh_File_Reader_H__
#define __Mesh_File_Reader_H__

#include "Mesh.h"

class MeshFileReader
{
public:
	MeshFileReader(const char* fileName);
	~MeshFileReader();
	Mesh& getMesh();

private:
	std::string strFileName;
    Mesh mesh;

	void ReadVtkFile();
};

#endif
