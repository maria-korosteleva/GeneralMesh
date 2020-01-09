// GeneralMesh is a simple class to work with meshes (static, so far)
//
// * loads the files in .obj or .ply
// * performs heuristic normalization 
//      * puts the mesh in the world center 
//      * scales it to the meters units (simple heuristic, may not work for large objects or on arbitrary scales)
//      * but allows to access both normalized and original unnormalized vertices 
// * provides access to geometry in different formats  
//      * Eigen, for compatibility with libigl
//      * glm, for compatibility with OpenGL
// * is able to load the list of the designated vertices with names (optional),
//   see example of the file in the GeneralMeash example project (key_vertices_example.txt)
// 

#pragma once

// Fix compilation errors
#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif

#include <igl/readOBJ.h>
#include <igl/writeOBJ.h>
#include <igl/readPLY.h>
#include <igl/per_vertex_normals.h>
#include <igl/per_face_normals.h>

#include <map>
#include <vector>

// for opengl-friendly conversion
#include <glm/glm.hpp>

class GeneralMesh
{
public:
    enum Gender {
        FEMALE, 
        MALE, 
        UNKNOWN
    };

    struct GLMVertex
    {
        glm::vec3 position;
        glm::vec3 normal;
    };

    GeneralMesh(const std::string& input_filename, Gender gender = UNKNOWN, const std::string& cloth_segmentation_file = "");
    ~GeneralMesh();

    const std::string& getName() const           { return name_; };
    const std::string& getNameWithGroup() const  { return name_with_group_; };
    const std::string& getPath() const           { return path_; };
    Gender getGender() const                     { return gender_; };

    const Eigen::MatrixXi& getFaces() const      { return faces_; };
    const Eigen::MatrixXd& getFaceNormals() const      { return faces_normals_; };
    // with vertex ids staring from zero:
    const std::vector<unsigned int>& getGLMFaces() const      { return gl_faces_; };

    const Eigen::MatrixXd& getVertices() const   { return verts_; };
    const Eigen::MatrixXd& getNormalizedVertices() const   { return verts_normalized_; };
    const std::vector<GLMVertex>& getGLNormalizedVertices() const   { return gl_vertices_normalized_; };
    const Eigen::VectorXd& getMeanPoint() const  { return mean_point_; };

    bool isClothSegmented() const                { return is_cloth_segmented_; };
    // returns probability of being cloth for each vertex
    const Eigen::VectorXd& getClothProbabilities() const { return verts_cloth_probability_; };

    void saveNormalizedMesh(std::string path) const;

private:
    void readFile_(const std::string& filename);
    void readClothProbabilitesFile_(const std::string& filename);
    void normalizeVertices_();
    void glFriendlyMesh_();
    void cutName_(const std::string& filename);
    bool checkFileExist_(const std::string& filename);

    std::string name_;
    std::string name_with_group_;
    std::string path_;
    Gender gender_;

    Eigen::MatrixXd verts_;
    Eigen::MatrixXd verts_normalized_;
    Eigen::MatrixXd verts_normals_;
    std::vector<GLMVertex> gl_vertices_normalized_;

    Eigen::MatrixXi faces_;
    Eigen::MatrixXd faces_normals_;
    std::vector<unsigned int> gl_faces_;

    Eigen::VectorXd mean_point_;

    // segmentation
    bool is_cloth_segmented_;
    Eigen::VectorXd verts_cloth_probability_;   // probability of being cloth
};
