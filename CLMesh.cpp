#include "CLMesh.h"

void CLMesh::loadMesh(Mesh& m, cl::Context& context)
{
	cl_mem_flags roFlags = CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR;
	cl_mem_flags woFlags = CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY;
	cl_mem_flags rwFlags = CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY;
	unsigned int nVerts = m.Vertices().size();
	buffers.gl_verts = cl::Buffer(context, roFlags, sizeof(Vertex) * nVerts, m.Vertices().data());
	buffers.gl_indices = cl::Buffer(context, roFlags, sizeof(unsigned int) * nVerts, m.Indices().data());
	buffers.obj_indices = cl::Buffer(context, roFlags, sizeof(Index) * m.OBJIndices().size(), m.OBJIndices().data());
	unsigned int nTris = m.Indices().size() / 3;
	buffers.obj_primOccupancy = cl::Buffer(context, woFlags, sizeof(int) * nVerts * nTris);
	
	glm::uvec3 objDims = m.OBJDims();
	buffers.obj_vP = cl::Buffer(context, woFlags, sizeof(glm::fvec3) * objDims.x);
	buffers.obj_vN = cl::Buffer(context, woFlags, sizeof(glm::fvec3) * objDims.y);
	buffers.obj_vT = cl::Buffer(context, woFlags, sizeof(glm::fvec2) * objDims.z); 
	workDimensions.objDims = objDims;
	workDimensions.nVerts = nVerts;
	workDimensions.nTris = nTris;

	for (size_t i = 0; i < nTris; i++)
	{
		binLabels.push_back(i%2);
	}
	buffers.binLabels = cl::Buffer(context, roFlags, sizeof(unsigned int) * nTris, binLabels.data());
	buffers.bins = cl::Buffer(context, rwFlags, (sizeof(unsigned int) * 3) * BinDims.count * BinDims.capacity);
	buffers.binCounts = cl::Buffer(context, rwFlags, sizeof(unsigned int) * BinDims.count);

	configureKernels();
}

unsigned int round_worksize(unsigned int actual)
{
	return (actual / 32u + 1) * 32u;
}

void CLMesh::EnqueueMeshOp(Mesh& m, CLMeshOp operation, cl::Device& device, cl::Context& context) 
{
	loadMesh(m, context);
	cl::CommandQueue queue(context, device);
	cl_int err;
	std::vector<int> occupancy = std::vector<int>(workDimensions.nTris * workDimensions.nVerts);
	std::vector<std::vector<int>> occupancyMatrix = std::vector<std::vector<int>>(workDimensions.nTris, std::vector<int>(workDimensions.nVerts));


	switch (operation)
	{
		case PULL:
			workDimensions.globalSize = cl::NDRange(round_worksize(workDimensions.nVerts));
			workDimensions.localSize = cl::NDRange(std::min(workDimensions.globalSize[0], meshOperations.pull_mesh.getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(device)));
			err = queue.enqueueNDRangeKernel(meshOperations.pull_mesh, workDimensions.offset, workDimensions.globalSize, workDimensions.localSize);
			err = queue.enqueueReadBuffer(buffers.obj_vP, CL_TRUE, 0, sizeof(glm::fvec3) * workDimensions.objDims.x, vp.data());
			break;

		case OCCUPANCY:
			workDimensions.globalSize = cl::NDRange((unsigned int)(workDimensions.nTris - workDimensions.nTris % 32u + 32u), (unsigned int)(workDimensions.nVerts - workDimensions.nVerts % 32u + 32u));
			workDimensions.localSize = cl::NDRange(16,16);
			err = queue.enqueueNDRangeKernel(meshOperations.triangleOccupancy, workDimensions.offset, workDimensions.globalSize, workDimensions.localSize);
			
			err = queue.enqueueReadBuffer(buffers.obj_primOccupancy, CL_TRUE, 0, sizeof(int) * workDimensions.nTris * workDimensions.nVerts, occupancy.data());
			
			for (unsigned int tri = 0; tri < workDimensions.nTris; tri++)
			{
				for (unsigned int vert = 0; vert < workDimensions.nVerts; vert++)
				{
					occupancyMatrix[tri][vert] = occupancy[tri * workDimensions.nTris + vert];
				}
			}
			break;
		case BIN:
			workDimensions.globalSize = cl::NDRange(round_worksize(workDimensions.nTris));
			workDimensions.localSize = cl::NDRange(std::min((unsigned int)workDimensions.globalSize[0], 32u));
			configureKernels();
			err = queue.enqueueNDRangeKernel(meshOperations.sortBins, workDimensions.offset, workDimensions.globalSize, workDimensions.localSize);
			std::vector<unsigned int> tmpBins = std::vector<unsigned int>(BinDims.capacity * BinDims.count * 3);
			err = queue.enqueueReadBuffer(
				buffers.bins, CL_TRUE, 0, 
				sizeof(unsigned int) * BinDims.capacity * BinDims.count * 3, 
				tmpBins.data());
			std::vector<unsigned int> binCounts = std::vector<unsigned int>(BinDims.count);
			err = queue.enqueueReadBuffer(buffers.binCounts, CL_TRUE, 0, sizeof(unsigned int) * BinDims.count, binCounts.data());
			
			bins = std::vector<std::vector<unsigned int>>();
			for (unsigned int binIdx = 0; binIdx < BinDims.count; binIdx++)
			{
				bins.push_back(std::vector<unsigned int>());
				unsigned int linBinIdx = binIdx * BinDims.capacity * 3;
				for (unsigned int i = 0; i < binCounts[binIdx]; i++)
				{
					unsigned int offset = linBinIdx + i*3;
					bins[binIdx].push_back(tmpBins[offset]);
					bins[binIdx].push_back(tmpBins[offset + 1]);
					bins[binIdx].push_back(tmpBins[offset + 2]);
				}
			}
			break;
	}

	std::cout << err << std::endl;

	for (glm::fvec3 p : vp)
	{
		std::cout << "<" << p.x << " " << p.y << " " << p.z << "> ";
	}
	std::cout << std::endl;
}

void CLMesh::configureKernels()
{
	meshOperations.triangleOccupancy.setArg(0, buffers.obj_indices);
	meshOperations.triangleOccupancy.setArg(1, buffers.obj_primOccupancy);
	meshOperations.triangleOccupancy.setArg(2, workDimensions.nTris);
	meshOperations.triangleOccupancy.setArg(3, workDimensions.nVerts);

	meshOperations.push_mesh.setArg(0, buffers.gl_verts);
	meshOperations.push_mesh.setArg(1, buffers.obj_indices);
	meshOperations.push_mesh.setArg(2, buffers.obj_vP);
	meshOperations.push_mesh.setArg(3, buffers.obj_vN);
	meshOperations.push_mesh.setArg(4, buffers.obj_vT);
	meshOperations.push_mesh.setArg(5, workDimensions.nVerts);

	meshOperations.pull_mesh.setArg(0, buffers.gl_verts);
	meshOperations.pull_mesh.setArg(1, buffers.obj_indices);
	meshOperations.pull_mesh.setArg(2, buffers.obj_vP); 
	meshOperations.pull_mesh.setArg(3, buffers.obj_vN);
	meshOperations.pull_mesh.setArg(4, buffers.obj_vT);
	meshOperations.pull_mesh.setArg(5, workDimensions.nVerts);

	cl_int err = meshOperations.sortBins.setArg(0, buffers.gl_indices);
	err = meshOperations.sortBins.setArg(1, buffers.binLabels);
	err = meshOperations.sortBins.setArg(2, buffers.bins);
	err = meshOperations.sortBins.setArg(3, buffers.binCounts);
	err = meshOperations.sortBins.setArg(4, (unsigned int) BinDims.capacity);
	err = meshOperations.sortBins.setArg(5, (unsigned int) BinDims.count);
	err = meshOperations.sortBins.setArg(6, (unsigned int) workDimensions.nTris);


	// Allocate uint buffer containing local indices
	// sizeof(localIndices) = 3 * sizeof(unsigned int) * triangles_per_workgroup
	unsigned int localIndexSize = sizeof(unsigned int) * (unsigned int) workDimensions.localSize[0] * 3u;
	err = meshOperations.sortBins.setArg(7, localIndexSize, nullptr);
	
	unsigned int localBinCap = 8u; // max triangles per bin in each workgroup
	// sizeof(localBins) = 3 * sizeof(unsigned int) * localBinCap * n_bins
	unsigned int localBinSize =  (unsigned int) localBinCap * sizeof(unsigned int) * (unsigned int)BinDims.count * 3u;
	err = meshOperations.sortBins.setArg(8, sizeof(unsigned int) * workDimensions.nTris, nullptr);
	err = meshOperations.sortBins.setArg(9, localBinSize, nullptr);

	unsigned int localBinCountSize = sizeof(unsigned int) * (unsigned int)BinDims.count;
	err = meshOperations.sortBins.setArg(10, localBinCountSize, nullptr);
	err = meshOperations.sortBins.setArg(11, localBinCap);
}