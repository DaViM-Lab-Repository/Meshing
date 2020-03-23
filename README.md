# Meshing
# Introduction
This repository contains a C++ program to open mesh files (Quad or HExahedral meshes in VTK file format) and build a data structure for the mesh. The code can be build on Linux or Windows OS using CMAKE GUI, VTK and Visual Studio (in case of Windows OS).
# Building
The code requires VTK libraries to build a meshing data structure from input VTK files, therefore, VTK (version >= 8.0) is required. The VTK source code can be downloaded from https://vtk.org/download/ and instruction to build VTK on different platforms can be found at https://vtk.org/Wiki/VTK/Configure_and_Build. To build and run the code:
- Download the repository to local machine.
- Create a new directory named build inside the downloaded repository.
- In CMAKE GUI, set source as source directory (nameOfComputer/Drive/Meshing) and set build as build directory (nameOfComputer/Drive/Meshing/build).
- Configure and Generate Make files.
- Once the Make files are generated: For Linux use make command to build executibles. For Windows open the project in Visual Studio and build.
# Data Structure
The mesh data structure is organized as follows:
## Vertices
A set of vertices which contain information of individual 3D points in the mesh. The vertices further have information about neighboring Vertices, Edges, Faces and Cells.
### Neighboring Vertices Examples
## Edges
A set of edges where each edge is defined by two vertices in the mesh. The edges also have information about neighboring Vertices, Edges, Faces and Cells.
## Faces
A set of faces defined by certain edges (4 edges for Quad and Hexahedral meshes). Each face contains information about neighboring Vertices, Edges, Faces and Cells.
## Cells
A set of cells which are essentially polygons (Quad or Hexahedra). In case of Quad Mesh each cell has only one face while for Hexahedral mesh, cells are built using 6 faces. Each cell contains information about neighboring Vertices, Edges, Faces and Cells.
