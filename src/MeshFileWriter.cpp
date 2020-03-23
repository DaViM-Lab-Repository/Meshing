#include <iostream>
#include <vector>
#include <fstream>

#include "MeshFileWriter.h"

MeshFileWriter::MeshFileWriter(Mesh& mesh_in, std::string fileName) {
    mesh = mesh_in;
    strFileName = fileName;
}

MeshFileWriter::~MeshFileWriter() {}

void MeshFileWriter::writeFile(int index) {
    if (strFileName.find(".vtk") != strFileName.npos) {
        writeVTKFile(index);
    } else {
        std::cout << "Please specify an output file with valid format (.vtk)" << std::endl;
    }
}

void MeshFileWriter::writeVTKFile(int index) {
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

    std::cout << mesh.C.size() << std::endl;

    Cell& c = mesh.C.at(index);
    outputFile << "CELLS " << c.neighboring_Cids.size() + 1 << " " << 5 + 5 * c.neighboring_Cids.size() << " \n";
    // outputFile << "1 " << v.id << "\n";
    // outputFile << "2 " << e.Vids[0] << " " << e.Vids[1] << "\n";
    outputFile << "4";
    for (auto& v: c.Vids) {
        outputFile << " " << v;
    }
    outputFile << "\n";
    // outputFile << "8";
    // for (auto& v: c.Vids) {
    //     outputFile << " " << v;
    // }
    // outputFile << "\n";

    for (auto& f1: c.neighboring_Cids) {
        // outputFile << "1 " << v << "\n";
        // outputFile << "2 " << mesh.E.at(e).Vids[0] << " " << mesh.E.at(e).Vids[1] << "\n";
        outputFile << "4";
        for (auto& v: mesh.C.at(f1).Vids) {
            outputFile << " " << v; 
        }
        outputFile << "\n";
        // outputFile << "8";
        // for (auto& v: mesh.C.at(c1).Vids) {
        //     outputFile << " " << v;
        // }
        // outputFile << "\n";
    }

    outputFile << "CELL_TYPES " << c.neighboring_Cids.size() + 1 << "\n";
    outputFile << "9\n";
    for (auto& id: c.neighboring_Cids) {
        outputFile << "9\n";
    }

    outputFile << "CELL_DATA " << c.neighboring_Cids.size() + 1 << "\n";
    outputFile << "SCALARS fixed int\n";
    outputFile << "LOOKUP_TABLE default\n";
    outputFile << "0\n";
    for (auto& id: c.neighboring_Cids) {
        outputFile << "1\n";
    }
}