# GeneralMesh

GeneralMesh is a simple class to work with meshes (static, so far)

## Dependencies:
* libigl (reading files and some computations)
* glm

## How to use (VisualStudio):
* Add the project directory (or parent of it) to the include directories 
         (Configuration Properties -> C/C++ -> General -> Additional Include Directories)
* Add the GeneralMesh.cpp file to the project through Adding and existing file
* Or [Dirty trick] add #include <GeneralMesh/GeneralMesh.cpp> to one (!!!!) of your own files. 

## Functionality 
* loads the files in .obj or .ply
* performs heuristic normalization 
     * puts the mesh in the world center 
     * scales it to the meters units (simple heuristic, may not work for large objects or on arbitrary scales)
     * but allows to access both normalized and original unnormalized vertices 
* provides access to geometry in different formats  
     * Eigen, for compatibility with libigl
     * glm, for compatibility with OpenGL
* is able to load the list of the designated vertices with names (optional),
  see example of the file in the GeneralMeash example project (key_vertices_example.txt)

## Limitations:
* Works only with geometry (no materials/rig info supported)

## Further development: 
You can extend the class with the fuctionality you need
     * For backward compatibility, do not change existing interfaces
     * Kindly ask to follow the existing code style and to keep the new finctions small and readible. 