#include <iostream>
#include "GeneralMesh.h"


void main()
{
    GeneralMesh mesh("D:/Data/smpl_outs/pose_A.obj");

    std::cout << "Mesh " << mesh.getName() << " loaded" << std::endl;
}