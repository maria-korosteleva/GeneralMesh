#include <iostream>
#include "../GeneralMesh.h"


void main()
{
    try {
        GeneralMesh mesh("C:/Users/Maria/MyDocs/GigaKorea/Seoungbae projects/cloth_data/isaac/isaac_scan.obj",
            GeneralMesh::MALE,
            "C:/Users/Maria/MyDocs/GigaKorea/Seoungbae projects/cloth_data/isaac/isaac_scan_scalar.txt");

        std::cout << "Mesh " << mesh.getName() << " loaded" << std::endl;
        std::cout << "Segmented: " << mesh.isClothSegmented() << std::endl;
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}