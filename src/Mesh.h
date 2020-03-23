#ifndef __MESH_H__
#define __MESH_H__

#include <vector>
#include <vtkCellType.h>

enum ElementType {
    POLYGON = VTK_POLYGON,
    QUAD = VTK_QUAD,
    HEXAHEDRON = VTK_HEXAHEDRON
};

class Vertex {
    public:
        size_t id;
        double x, y, z;

        std::vector<size_t> neighboring_Vids;
        std::vector<size_t> neighboring_Eids;
        std::vector<size_t> neighboring_Fids;
        std::vector<size_t> neighboring_Cids;
};

class Edge {
    public:
        size_t id;
        std::vector<size_t> Vids;

        std::vector<size_t> neighboring_Vids;
        std::vector<size_t> neighboring_Eids;
        std::vector<size_t> neighboring_Fids;
        std::vector<size_t> neighboring_Cids;

        Edge() {}
        Edge(size_t vid1, size_t vid2) {
            Vids.push_back(vid1);
            Vids.push_back(vid2);
        }
        ~Edge() {}
};

class Face {
    public:
        size_t id;
        std::vector<size_t> Vids;
        std::vector<size_t> Eids;

        std::vector<size_t> neighboring_Vids;
        std::vector<size_t> neighboring_Eids;
        std::vector<size_t> neighboring_Fids;
        std::vector<size_t> neighboring_Cids;

        Face() {}
        Face(std::vector<size_t> ids) {
            for (int i = 0; i < ids.size(); i++) {
                Vids.push_back(ids[i]);
            }
        }
        ~Face() {}
};

class Cell {
    public:
        size_t id;
        ElementType cell_type;
        std::vector<size_t> Vids;
        std::vector<size_t> Eids;
        std::vector<size_t> Fids;
        
        std::vector<size_t> neighboring_Vids;
        std::vector<size_t> neighboring_Eids;
        std::vector<size_t> neighboring_Fids;
        std::vector<size_t> neighboring_Cids;
};

class Mesh {
    public:
        std::vector<Vertex> V;
        std::vector<Edge> E;
        std::vector<Face> F;
        std::vector<Cell> C;
        ElementType mesh_type;

        void buildMeshConnectivities();
        void buildVertexConnectivities();
        void buildEdgeConnectivities();
        void buildFaceConnectivities();
        void buildCellConnectivities();
}; 

#endif