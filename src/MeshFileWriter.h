#ifndef __MESH_FILE_WRITER_H__
#define __MESH_FILE_WRITER_H__

#include "Mesh.h"

class MeshFileWriter {
public:
    MeshFileWriter(Mesh& mesh_in, std::string fileName);
    ~MeshFileWriter();
    void writeFile();

private:
    std::string strFileName;
    Mesh mesh;

    void writeVTKFile();
};

#endif