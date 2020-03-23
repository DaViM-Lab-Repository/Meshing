#include <iostream>
#include <vector>

#include <vtkCellType.h>
#include <vtkGenericDataObjectReader.h>
#include<vtkPolyData.h>
#include <vtkSmartPointer.h>
#include<vtkUnstructuredGrid.h>

#include "MeshFileReader.h"

MeshFileReader::MeshFileReader(const char* fileName) {
    strFileName = fileName;
    
    if (strFileName.find(".vtk") != strFileName.npos) {
        ReadVtkFile();
    } else {
        std::cout << "Please specify an input file with valid format (.vtk)" << std::endl;
    }
}

MeshFileReader::~MeshFileReader() {}

void MeshFileReader::ReadVtkFile() {
    vtkSmartPointer<vtkGenericDataObjectReader> reader = vtkSmartPointer<vtkGenericDataObjectReader>::New();
    reader->SetFileName(strFileName.c_str());
    reader->Update();

    if (reader->IsFilePolyData()) {
        vtkPolyData* output = reader->GetPolyDataOutput();
        const vtkIdType vnum = output->GetNumberOfPoints();
        const vtkIdType cnum = output->GetNumberOfPolys();
        std::cout << strFileName << " PolyData: " << vnum << " points " << cnum << " polys" << std::endl;

        std::vector<Vertex>& V = mesh.V;
        V.resize(vnum);
        double p[3];
        for (vtkIdType i = 0; i < vnum; i++) {
            output->GetPoint(i, p);
            V.at(i).x = p[0];
            V.at(i).y = p[1];
            V.at(i).z = p[2];
            V.at(i).id = i;
        }

        mesh.mesh_type = POLYGON;
        std::vector<Cell>& C = mesh.C;
        for (vtkIdType i = 0; i < cnum; i++) {
            vtkSmartPointer<vtkIdList> idList = vtkSmartPointer<vtkIdList>::New();
            output->GetPolys()->GetNextCell(idList);
            const vtkIdType csize = idList->GetNumberOfIds();
            Cell c;
            for (vtkIdType j = 0; j < csize; j++) {
                c.Vids.push_back(idList->GetId(j));
            }
            c.id = i;
            if (csize == 4) c.cell_type = QUAD;
            else c.cell_type = POLYGON;
            C.push_back(c);
        }
    } else if (reader->IsFileUnstructuredGrid()) {
        vtkUnstructuredGrid* output = reader->GetUnstructuredGridOutput();
        const vtkIdType vnum = output->GetNumberOfPoints();
        const vtkIdType cnum = output->GetNumberOfCells();
        std::cout << strFileName << " UnstructuredGrid: " << vnum << " points " << cnum << " cells" << std::endl;

        std::vector<Vertex>& V = mesh.V;
        V.resize(vnum);
        double p[3];
        for (vtkIdType i = 0; i < vnum; i++) {
            output->GetPoint(i, p);
            V.at(i).x = p[0];
            V.at(i).y = p[1];
            V.at(i).z = p[2];
            V.at(i).id = i;
        }

        const vtkIdType cellType = output->GetCellType(0);
        if (cellType == VTK_QUAD) mesh.mesh_type = QUAD;
        else if (cellType == VTK_HEXAHEDRON) mesh.mesh_type = HEXAHEDRON;
        std::vector<Cell>& C = mesh.C;
        for (vtkIdType i = 0; i < cnum; i++) {
            vtkSmartPointer<vtkIdList> idList = vtkSmartPointer<vtkIdList>::New();
            output->GetCellPoints(i, idList);
            const vtkIdType csize = idList->GetNumberOfIds();
            Cell c;
            c.id = i;
            const vtkIdType cellType = output->GetCellType(i);
            if (cellType == VTK_QUAD) c.cell_type = QUAD;
            else if (cellType == VTK_HEXAHEDRON) c.cell_type = HEXAHEDRON;
            for (vtkIdType j = 0; j < csize; j++) {
                c.Vids.push_back(idList->GetId(j));
            }
            C.push_back(c);
        }
    }
    mesh.buildMeshConnectivities();
}

Mesh& MeshFileReader::getMesh() {
    return mesh;
}