#include <iostream>
#include "../GeneralMesh.h"


void main()
{
    GeneralMesh mesh("D:/Data/smpl_outs/pose_A.obj", "C:/Users/Maria/MyDocs/my_modules/GeneralMesh/key_vertices_example.txt");

    std::cout << "Mesh " << mesh.getName() << " loaded" << std::endl;
}