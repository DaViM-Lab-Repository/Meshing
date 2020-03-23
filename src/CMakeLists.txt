# CMakeList.txt : CMake project for HexMeshing, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 2.8)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
include_directories(
	${VTK_INCLUDE_DIRS}
)
# Add source to this project's executable.
set(EXT_LIBS ${VTK_LIBRARIES})
set(SRC_MESHING
	main.cpp
	mesh.cpp
	mesh.h
	meshFileReader.cpp
	meshFileReader.h
	meshFileWriter.cpp
	meshFileWriter.h
)
add_executable (MeshApp ${SRC_MESHING} "main.cpp")

target_link_libraries(MeshApp ${EXT_LIBS})

# TODO: Add tests and install targets if needed.
