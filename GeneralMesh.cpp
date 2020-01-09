#pragma once
#include "GeneralMesh.h"


GeneralMesh::GeneralMesh(const std::string& input_filename, Gender gender, 
    const std::string& cloth_segmentation_file)
    :gender_(gender), is_cloth_segmented_(false)
{   
    // check for existance -- fast fail
    if (!checkFileExist_(input_filename))
    {
        throw std::invalid_argument("General Mesh: input file doesn't exist");
    }
    if (cloth_segmentation_file != "" && !checkFileExist_(input_filename))
    {
        throw std::invalid_argument("General Mesh: secified segmentation file doesn't exist");
    }

    cutName_(input_filename);

    readFile_(input_filename);
    normalizeVertices_();
    glFriendlyMesh_();  

    if (cloth_segmentation_file != "")
    {
        is_cloth_segmented_ = true;
        readClothProbabilitesFile_(cloth_segmentation_file);
    }
}

GeneralMesh::~GeneralMesh()
{
}

void GeneralMesh::saveNormalizedMesh(std::string path) const
{
    igl::writeOBJ(path + name_with_group_ + "_normalized.obj", verts_normalized_, faces_);
}

void GeneralMesh::readFile_(const std::string & filename)
{
    std::size_t point_pos = filename.find_last_of('.');
    std::string extention = filename.substr(point_pos + 1);
    bool sucess;
    if (extention == "obj")
    {
        sucess = igl::readOBJ(filename, verts_, faces_);
    }
    else if (extention == "ply")
    {
        sucess = igl::readPLY(filename, verts_, faces_);
    }
    else
    {
        throw std::invalid_argument("GeneralMesh::Unsupported type of input mesh. Supported types: .obj, .ply");
    }

    if (!sucess || verts_.rows() == 0 || faces_.rows() == 0)
    {
        throw std::runtime_error("GeneralMesh::reading mesh file failed: vertices/faces are not filled");
    }

    if (faces_.cols() != 3)
    {
        std::cout << "GeneralMesh::Warning::mesh " << name_ 
            << " is a quad mesh. The tools were developed assuming triangle mesh" 
            << "Unexpected behavior might occur." << std::endl;
    }

    // normals are invariant to verts normalization - translation and scaling
    igl::per_vertex_normals(verts_, faces_, verts_normals_);
    igl::per_face_normals(verts_, faces_, faces_normals_);
}

void GeneralMesh::readClothProbabilitesFile_(const std::string & filename)
{
    std::ifstream inFile(filename);
    int file_verts_n;
    inFile >> file_verts_n;

    verts_cloth_probability_.resize(verts_.rows());
    int vert_id = 0;
    float vert_raw_probability;
    while (inFile >> vert_raw_probability)
    {
        if (vert_id >= verts_.rows()) // prevent crashes
            throw std::invalid_argument("GeneralMesh: length of segmentation list doesn't match the number of vertices in the mesh.");

        // need normalization as it's [-1, 1] in the file
        verts_cloth_probability_[vert_id] = (vert_raw_probability + 1.) / 2.;
        vert_id++;
    }

    // final sanity check
    if (vert_id != verts_.rows())
        throw std::invalid_argument("GeneralMesh: length of segmentation list doesn't match the number of vertices in the mesh.");
}

void GeneralMesh::normalizeVertices_()
{
    mean_point_ = verts_.colwise().mean();

    verts_normalized_ = verts_.rowwise() - mean_point_.transpose();

    // convert to meters heuristically
    // check for sm througth height (Y axis)
    if (verts_normalized_.col(1).maxCoeff() - verts_normalized_.col(1).minCoeff() > 100)
    {
        verts_normalized_ *= 0.01;
        std::cout << "Warning: Mesh " << name_ << " is found to use sm/mm units. Scaled down by 0.01" << std::endl;
    }
    // check for mm/dm
    if (verts_normalized_.col(1).maxCoeff() - verts_normalized_.col(1).minCoeff() > 10)
    {
        verts_normalized_ *= 0.1;
        std::cout << "Warning: Mesh is found to use mm/dm units. Scaled down by 0.1" << std::endl;
    }
}

void GeneralMesh::glFriendlyMesh_()
{
    // vertices
    for (int i = 0; i < verts_normalized_.rows(); ++i)
    {
        GLMVertex gl_vert;
        gl_vert.position = glm::vec3(verts_normalized_(i, 0), verts_normalized_(i, 1), verts_normalized_(i, 2));
        gl_vert.normal = glm::vec3(verts_normals_(i, 0), verts_normals_(i, 1), verts_normals_(i, 2));

        gl_vertices_normalized_.push_back(gl_vert);
    }

    // faces
    for (int i = 0; i < faces_.rows(); ++i)
    {
        for (int j = 0; j < faces_.cols(); ++j)
        {
            // shift indices to start from zero
            gl_faces_.push_back(faces_(i, j));
        }
    }
}

void GeneralMesh::cutName_(const std::string & filename)
{
    std::size_t point_pos = filename.find_last_of('.');
    std::size_t slash_pos = filename.find_last_of("/\\");
    
    std::string path = filename.substr(0, slash_pos);
    std::string data_group = path.substr(path.find_last_of("/\\") + 1);
    std::string object_name = filename.substr(slash_pos + 1, (point_pos - slash_pos - 1));

    name_with_group_ = data_group + "-" + object_name;
    name_ = object_name;
    path_ = path;
}

bool GeneralMesh::checkFileExist_(const std::string& filename)
{
    std::ifstream infile(filename);
    return infile.good();
}
