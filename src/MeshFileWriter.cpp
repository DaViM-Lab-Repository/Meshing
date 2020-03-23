#include <iostream>
#include <vector>
#include <fstream>

#include "MeshFileWriter.h"

MeshFileWriter::MeshFileWriter(Mesh& mesh_in, std::string fileName) {
    mesh = mesh_in;
    strFileName = fileName;
}

MeshFileWriter::~MeshFileWriter() {}

void MeshFileWriter::writeFile() {
    if (strFileName.find(".vtk") != strFileName.npos) {
        writeVTKFile();
    } else {
        std::cout << "Please specify an output file with valid format (.vtk)" << std::endl;
    }
}

void MeshFileWriter::writeVTKFile() {
    std::ofstream outputFile;
    outputFile.open(strFileName);

    outputFile << "# vtk DataFile Version 1.0\n";
    outputFile << strFileName << "\n";
    outputFile << "ASCII\n";
    outputFile << "\nDATASET UNSTRUCTURED_GRID\n";

    outputFile << "POINTS " << mesh.V.size() << " double\n";
    for (auto& v: mesh.V) {
        outputFile << v.x << " " << v.y << " " << v.z << "\n";
    }

    outputFile << "CELLS " <<  mesh.C.size() << " " <<  (mesh.C.at(0).Vids.size() + 1) *  mesh.C.size()  << " \n";
    for (auto&c: mesh.C) {
        outputFile << mesh.C.at(0).Vids.size();
        for (auto& v: c.Vids) {
            outputFile << " " << v;
        }    
        outputFile << "\n";
    } 

    outputFile << "CELL_TYPES " << mesh.C.size() << "\n";
    for (auto& c: mesh.C) {
        outputFile << mesh.mesh_type << "\n";
    }
}