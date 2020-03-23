#include <iostream>
#include <set>

#include "mesh.h"

const unsigned int HexFaceConf[6][4] = {
    {0, 1, 2, 3},
    {4, 7, 6, 5},
    {1, 5, 6, 2},
    {0, 3, 7, 4},
    {0, 4, 5, 1},
    {2, 6, 7, 3},
};

void Mesh::buildMeshConnectivities() {
    buildVertexConnectivities();
    buildEdgeConnectivities();
    buildFaceConnectivities();
    buildCellConnectivities();
}

void Mesh::buildVertexConnectivities() {
    for (auto& c: C) {
        for (auto vid: c.Vids) {
            V.at(vid).neighboring_Cids.push_back(c.id);
        }
    }
    if (mesh_type == QUAD) {
        for (auto& c: C) {
            for (auto vid: c.Vids) {
                V.at(vid).neighboring_Fids.push_back(c.id);
            }
        }
    }
}

void Mesh::buildEdgeConnectivities() {
    std::vector<Edge> temp_Edges;
    temp_Edges.reserve(C.size() * 4);
    if (mesh_type == HEXAHEDRON) {
        temp_Edges.reserve(C.size() * 12);
    }

    for (auto& c: C) {
        temp_Edges.emplace_back(c.Vids[0], c.Vids[1]);
        temp_Edges.emplace_back(c.Vids[1], c.Vids[2]);
        temp_Edges.emplace_back(c.Vids[2], c.Vids[3]);
        temp_Edges.emplace_back(c.Vids[3], c.Vids[0]);

        if (c.cell_type == HEXAHEDRON) {
            temp_Edges.emplace_back(c.Vids[4], c.Vids[5]);
            temp_Edges.emplace_back(c.Vids[5], c.Vids[6]);
            temp_Edges.emplace_back(c.Vids[6], c.Vids[7]);
            temp_Edges.emplace_back(c.Vids[7], c.Vids[4]);
            temp_Edges.emplace_back(c.Vids[0], c.Vids[4]);
            temp_Edges.emplace_back(c.Vids[1], c.Vids[5]);
            temp_Edges.emplace_back(c.Vids[2], c.Vids[6]);
            temp_Edges.emplace_back(c.Vids[3], c.Vids[7]);
        }
    }
    
    size_t current_id = 0;
    for (auto& e: temp_Edges) {
        bool isDuplicate = false;

        size_t id1 = e.Vids[0];
        size_t id2 = e.Vids[1];

        for (size_t j = 0; j < V[id1].neighboring_Vids.size(); j++) {
            if (V[id1].neighboring_Vids[j] == id2) {
                isDuplicate = true;
                break;
            }
        }
        if (!isDuplicate) {
            e.id = current_id;
            current_id += 1;

            for (auto& Cid1: V[id1].neighboring_Cids) {
                for (auto& Cid2: V[id2].neighboring_Cids) {
                    if (Cid1 == Cid2) {
                        e.neighboring_Cids.push_back(Cid1);
                    }
                }
            }

            E.push_back(e);
            V[id1].neighboring_Eids.push_back(e.id);
            V[id1].neighboring_Vids.push_back(id2);

            V[id2].neighboring_Eids.push_back(e.id);
            V[id2].neighboring_Vids.push_back(id1);
        }
    }

    for (auto& e: E) {
        for (auto& v: e.Vids) {
            for (auto& v1: V.at(v).neighboring_Vids) {
                if (std::find(e.Vids.begin(), e.Vids.end(), v1) == e.Vids.end()) {
                    e.neighboring_Vids.push_back(v1);
                }
            }
        }
    }

    for (auto& v: V) {
        for (auto& e: v.neighboring_Eids) {
            for (auto& e1: v.neighboring_Eids) {
                if (e == e1) {
                    continue;
                }
                E.at(e).neighboring_Eids.push_back(e1);
            }
        }
    }

    for (auto& e: E) {
        std::set<size_t> temp_neighboring_Vids(e.neighboring_Vids.begin(), e.neighboring_Vids.end());
        e.neighboring_Vids.clear();
        for (auto id: temp_neighboring_Vids) {
            e.neighboring_Vids.push_back(id);
        }
    }

    for (auto& e: E) {
        std::set<size_t> temp_neighboring_Eids(e.neighboring_Eids.begin(), e.neighboring_Eids.end());
        e.neighboring_Eids.clear();
        for (auto id: temp_neighboring_Eids) {
            e.neighboring_Eids.push_back(id);
        }
    }

    for (auto& e: E) {
        std::set<size_t> temp_neighboring_Cids(e.neighboring_Cids.begin(), e.neighboring_Cids.end());
        e.neighboring_Cids.clear();
        for (auto id: temp_neighboring_Cids) {
            e.neighboring_Cids.push_back(id);
        }
    }

    for (auto& v: V) {
        std::set<size_t> temp_neighboring_Vids(v.neighboring_Vids.begin(), v.neighboring_Vids.end());
        v.neighboring_Vids.clear();
        for (auto id: temp_neighboring_Vids) {
            v.neighboring_Vids.push_back(id);
        }
    }
}

void Mesh::buildFaceConnectivities() {
    
    if (mesh_type == QUAD) {
        F.resize(C.size());
        for (size_t i = 0; i < C.size(); i++) {
            F[i].Vids = C[i].Vids;
            F[i].id = C[i].id;
            C[i].Fids.push_back(F[i].id);
            for (int j = 0; j < F[i].Vids.size(); j++) {
                V.at(F[i].Vids[j]).neighboring_Fids.push_back(F[i].id);
                size_t id1 = F[i].Vids[j];
                size_t id2 = F[i].Vids[(j + 1) % F[i].Vids.size()];
                for (auto& e1: V.at(id1).neighboring_Eids) {
                    for (auto& e2: V.at(id2).neighboring_Eids) {
                        if (e1 == e2) {
                            F[i].Eids.push_back(e1);
                            C[i].Eids.push_back(e1);
                            E.at(e1).neighboring_Fids.push_back(F[i].id);
                        }
                    } 
                }
            }
        }
    } else if (mesh_type == HEXAHEDRON) {
        size_t current_Fid = 0;
        for (auto& c: C) {
            for (int i = 0; i < 6; i++) {
                std::vector<size_t> ids = {
                    c.Vids[HexFaceConf[i][0]], 
                    c.Vids[HexFaceConf[i][1]], 
                    c.Vids[HexFaceConf[i][2]],
                    c.Vids[HexFaceConf[i][3]]
                };
                Face f(ids);

                bool isDuplicate = false;
                for (int j = 0; j < F.size(); j++) {
                    Face& f2 = F.at(j);
                    for (int k = 0; k < f.Vids.size(); k++) {
                        if (std::find(f2.Vids.begin(), f2.Vids.end(), f.Vids.at(k)) == f2.Vids.end()) {
                            isDuplicate = false;
                            break;
                        }
                        isDuplicate = true;
                    }
                    if (isDuplicate) {
                        for (int j = 0; j < f2.Vids.size(); j++) {
                            size_t id1 = f2.Vids[j];
                            size_t id2 = f2.Vids[(j + 1) % f2.Vids.size()];
                            for (auto& e1: V.at(id1).neighboring_Eids) {
                                for (auto& e2: V.at(id2).neighboring_Eids) {
                                    if (e1 == e2) {
                                        c.Eids.push_back(e1);
                                    }
                                } 
                            }
                        }
                        c.Fids.push_back(f2.id);
                        break;
                    }
                }

                if (!isDuplicate) {
                    f.id = current_Fid;
                    for (int j = 0; j < f.Vids.size(); j++) {
                        V.at(f.Vids[j]).neighboring_Fids.push_back(f.id);
                        size_t id1 = f.Vids[j];
                        size_t id2 = f.Vids[(j + 1) % f.Vids.size()];
                        for (auto& e1: V.at(id1).neighboring_Eids) {
                            for (auto& e2: V.at(id2).neighboring_Eids) {
                                if (e1 == e2) {
                                    f.Eids.push_back(e1);
                                    c.Eids.push_back(e1);
                                    E.at(e1).neighboring_Fids.push_back(f.id);
                                }
                            } 
                        }
                    }
                    c.Fids.push_back(f.id);
                    F.push_back(f);
                    current_Fid += 1;
                }
            }
            std::set<size_t> temp_Eids(c.Eids.begin(), c.Eids.end());
            c.Eids.clear();
            for (auto id: temp_Eids) {
                c.Eids.push_back(id);
            }
        }
    }

    for (auto& f: F) {
        for (auto& v: f.Vids) {
            for (auto& v1: V.at(v).neighboring_Vids) {
                if (std::find(f.Vids.begin(), f.Vids.end(), v1) == f.Vids.end()) {
                    f.neighboring_Vids.push_back(v1);
                }
            }
        }

        for (auto& e: f.Eids) {
            for (auto& e1: E.at(e).neighboring_Eids) {
                if (std::find(f.Eids.begin(), f.Eids.end(), e1) == f.Eids.end()) {
                    f.neighboring_Eids.push_back(e1);
                }
            }

            for (auto& f1: E.at(e).neighboring_Fids) {
                if (f.id != f1) {
                    f.neighboring_Fids.push_back(f1);
                }
            }

            for (auto& c: E.at(e).neighboring_Cids) {
                if (mesh_type == QUAD && f.id != c) {
                    f.neighboring_Cids.push_back(c);
                    C.at(f.id).neighboring_Cids.push_back(c);
                } else if (mesh_type == HEXAHEDRON) {
                    f.neighboring_Cids.push_back(c);
                }
            }
        }
        std::set<size_t> temp_Cids(f.neighboring_Cids.begin(), f.neighboring_Cids.end());
        f.neighboring_Cids.clear();
        for (auto id: temp_Cids) {
            f.neighboring_Cids.push_back(id);
        }
    }

}

void Mesh::buildCellConnectivities() {
    for (auto& c: C) {
        for (auto& v: c.Vids) {
            for (auto& v1: V.at(v).neighboring_Vids) {
                if (std::find(c.Vids.begin(), c.Vids.end(), v1) == c.Vids.end()) {
                    c.neighboring_Vids.push_back(v1);
                }
            }
        }

        std::set<size_t> temp_Vids(c.neighboring_Vids.begin(), c.neighboring_Vids.end());
        c.neighboring_Vids.clear();
        for (auto id: temp_Vids) {
            c.neighboring_Vids.push_back(id);
        }

        for (auto& e: c.Eids) {
            for (auto& e1: E.at(e).neighboring_Eids) {
                if (std::find(c.Eids.begin(), c.Eids.end(), e1) == c.Eids.end()) {
                    c.neighboring_Eids.push_back(e1);
                }
            }
        }

        std::set<size_t> temp_Eids(c.neighboring_Eids.begin(), c.neighboring_Eids.end());
        c.neighboring_Eids.clear();
        for (auto id: temp_Eids) {
            c.neighboring_Eids.push_back(id);
        }

        for (auto& f: c.Fids) {
            for (auto& f1: F.at(f).neighboring_Fids) {
                if (std::find(c.Fids.begin(), c.Fids.end(), f1) == c.Fids.end()) {
                    c.neighboring_Fids.push_back(f1);
                }
            }
        }

        std::set<size_t> temp_Fids(c.neighboring_Fids.begin(), c.neighboring_Fids.end());
        c.neighboring_Fids.clear();
        for (auto id: temp_Fids) {
            c.neighboring_Fids.push_back(id);
        }

        for (auto& f: c.Fids) {
            for (auto& c1: F.at(f).neighboring_Cids) {
                if (c.id == c1) {
                    continue;
                }
                for (auto& f1: C.at(c1).Fids) {
                    if (f == f1) {
                        c.neighboring_Cids.push_back(c1);
                        break;
                    }
                }
            }
        }

        std::set<size_t> temp_Cids(c.neighboring_Cids.begin(), c.neighboring_Cids.end());
        c.neighboring_Cids.clear();
        for (auto id: temp_Cids) {
            c.neighboring_Cids.push_back(id);
        }
    }
}