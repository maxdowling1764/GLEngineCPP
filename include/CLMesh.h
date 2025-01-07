#pragma once
#include "CL/opencl.hpp"
#include <iostream>
#include "Util.h"
#include "Mesh.h"

class CLMesh 
{
private:
	const std::string m_srcPath = "kernels/test.cl";
	struct {
		size_t count=2, capacity=32;
	} BinDims;
	
	std::vector<glm::vec3> vp;
	std::vector<unsigned int> binLabels;
	std::vector<std::vector<unsigned int>> bins;

	struct {
		cl::Kernel triangleOccupancy;
		cl::Kernel push_mesh; // CLMesh -> GLMesh
		cl::Kernel pull_mesh; // GLMesh -> CLMesh
		cl::Kernel sortBins;
	} meshOperations;

	struct {
		cl::NDRange offset = cl::NDRange(0);
		cl::NDRange globalSize = cl::NDRange(0);
		cl::NDRange localSize = cl::NDRange(0);
		unsigned int nVerts=0, nTris=0;
		glm::uvec3 objDims = glm::uvec3(0);
	} workDimensions;

	struct {
		cl::Buffer gl_verts;
		cl::Buffer gl_indices;
		
		cl::Buffer obj_indices;
		cl::Buffer obj_primOccupancy;
		
		cl::Buffer obj_vP;
		cl::Buffer obj_vN;
		cl::Buffer obj_vT;
		
		cl::Buffer bins;
		cl::Buffer binCounts;
		cl::Buffer binLabels;
	} buffers;

	void configureKernels();
	void loadMesh(Mesh& m, cl::Context& context);

public:
	enum CLMeshOp {
		PUSH, PULL, OCCUPANCY, BIN
	};
	CLMesh(Mesh& m, cl::Device* selectedDevice, cl::Context* context):vp(std::vector<glm::vec3>(m.OBJDims().x)), binLabels(std::vector<unsigned int>())
	{
		std::cout << "Initializing OpenCL Mesh Module..." << std::endl;

		if (selectedDevice && context)
		{
			std::string src = read_file(m_srcPath);
			cl::Program program(*context, { src });
			auto err = program.build();
			
			size_t log_size;
			clGetProgramBuildInfo(program.get(), selectedDevice->get(), CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
			char* log = (char*)malloc(log_size);
			clGetProgramBuildInfo(program.get(), selectedDevice->get(), CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
			printf("Build Log (srcPath=`%s`): \n%s\n", m_srcPath.c_str(), log);
			free(log);

			meshOperations.triangleOccupancy = cl::Kernel(program, "tri_occupancy");
			meshOperations.pull_mesh = cl::Kernel(program, "pull_mesh");
			meshOperations.sortBins = cl::Kernel(program, "sort_bins");
		}
	};
	void EnqueueMeshOp(Mesh& m, CLMeshOp operation, cl::Device& device, cl::Context& context);
};