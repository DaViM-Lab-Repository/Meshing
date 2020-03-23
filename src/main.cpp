#include <iostream>
#include <string>

#include "Mesh.h"
#include "MeshFileReader.h"
#include "MeshFileWriter.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "USAGE: MeshApp <file_name.vtk>" << std::endl;
        return -1;
    }
    std::string inputFileName = argv[1];
    MeshFileReader meshFileReader(inputFileName.c_str());

    Mesh& mesh = meshFileReader.getMesh();

    MeshFileWriter meshFileWriter(mesh, "output.vtk");
    meshFileWriter.writeFile();

    return 0;
}