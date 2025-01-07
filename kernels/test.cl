
inline uint matLoc(uint i, uint j, uint dim)
{
	return i * dim + j;
}


float norm2(local float* a, local float* b, uint dim)
{
	float accum = 0.0f;
	for (int i = 0; i < dim; i++)
	{
		float dx_i = a[i] - b[i];
		accum += dx_i * dx_i;
	}
	return accum;
}

float norm(local float* a, local float* b, uint dim)
{
	return sqrt(norm2(a, b, dim));
}

void copyVec(local float* src, local float* dest, uint dim)
{
	for (int i = 0; i < dim; i++)
	{
		dest[i] = src[i];
	}
}

typedef struct {
	float3 p, n;
	float2 t;
} Vertex;

typedef struct {
	uint indices[3];
} Index;

#define FLOATS_PER_VERT 8
void load_vertex(unsigned int i, Vertex* dest, global float* glVertices)
{
	dest->p.x = glVertices[i * FLOATS_PER_VERT];
	dest->p.y = glVertices[i * FLOATS_PER_VERT + 1];
	dest->p.z = glVertices[i * FLOATS_PER_VERT + 2];

	dest->n.x = glVertices[i * FLOATS_PER_VERT + 3];
	dest->n.y = glVertices[i * FLOATS_PER_VERT + 4];
	dest->n.z = glVertices[i * FLOATS_PER_VERT + 5];

	dest->t.x = glVertices[i * FLOATS_PER_VERT + 6];
	dest->t.y = glVertices[i * FLOATS_PER_VERT + 7];
}

void write_vec3f(float3 src, unsigned int i, global float* dest)
{
	dest[i * 3] = src.x;
	dest[i * 3 + 1] = src.y;
	dest[i * 3 + 2] = src.z;
}

void write_vec2f(float2 src, unsigned int i, global float* dest)
{
	dest[i * 2] = src.x;
	dest[i * 2 + 1] = src.y;
}

void load_vec3f(float3* dest, unsigned int i, global float* src)
{
	dest->x = src[i * 3];
	dest->y = src[i * 3 + 1];
	dest->z = src[i * 3 + 2];
}

void write_vertex(Vertex src, Index idx, global float* positions, global float* normals, global float* uvs)
{
	write_vec3f(src.p, idx.indices[0], positions);
	write_vec3f(src.n, idx.indices[1], normals);
	write_vec2f(src.t, idx.indices[2], uvs);
}

void load_index(unsigned int i, Index* dest, global uint* objIndices)
{
	dest->indices[0] = objIndices[i * 3];
	dest->indices[1] = objIndices[i * 3 + 1];
	dest->indices[2] = objIndices[i * 3 + 2];
}

kernel void pull_mesh(global float* glVertices, global uint* objIndices, 
					 global float* positions, global float* normals, global float* uvs, uint nVerts)
{
	uint glIndex = get_global_id(0);
	Vertex tmp;
	
	load_vertex(glIndex, &tmp, glVertices);
	if (glIndex < nVerts)
	{
		Index idx;
		load_index(glIndex, &idx, objIndices);
		write_vertex(tmp, idx, positions, normals, uvs);
	}
	
}

kernel void push_mesh(global Vertex* glVertices, global Index* objIndices,
	global float3* positions, global float3* normals, global float2* uvs, uint nVerts)
{
	uint glIndex = get_global_id(0);

	if (glIndex < nVerts)
	{
		Index idx = objIndices[glIndex];
		glVertices[glIndex].p = (float3)(1.0f, 1.0f, 1.0f);
		//glVertices[glIndex].p = positions[idx.indices[0]];
		glVertices[glIndex].n =	normals[idx.indices[1]];
		glVertices[glIndex].t =	uvs[idx.indices[2]];
	}
}

typedef struct {
	uint a, b;
} Edge;

typedef struct {
	uint a, b;
} DualEdge;

void load_triangle_edges(Index* src, Edge edges[3])
{
	for (int i = 0; i < 3; i++)
	{
		edges[i].a = src->indices[i];
		edges[i].b = src->indices[(i + 1) % 3];
	}
}

bool compare_edges(Edge e1, Edge e2)
{
	return (e1.a == e2.a && e1.b == e2.b) || (e1.a == e2.b && e2.b == e1.a);
}

bool shared_edge(uint idxA, uint idxB, global uint* indices, DualEdge* result)
{
	Index triA;
	Index triB;
	load_index(idxA, &triA, indices);
	load_index(idxB, &triB, indices);
	
	Edge edges_A[3];
	Edge edges_B[3];
	load_triangle_edges(&triA, edges_A);
	load_triangle_edges(&triB, edges_B);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j <= i; j++)
		{
			if (compare_edges(edges_A[i], edges_B[j]))
			{
				result->a = i;
				result->b = j;
				return true;
			}
		}
	}
	return false;
}


kernel void triangle_occupancy(global uint* indices, global int* occupancy, uint nTriangles, uint nVerts)
{
	uint triIndex = get_global_id(0);
	uint vertIndex = get_global_id(1);
	occupancy[triIndex * nVerts + vertIndex] = -1;
	if (triIndex < nTriangles && vertIndex < nVerts)
	{
		for (uint i = 0; i < 3; i++)
		{
			if (indices[triIndex * 3 + i] == vertIndex)
				occupancy[triIndex * nVerts + vertIndex] = i;
		}
	}
}

kernel void tri_occupancy(global uint* objIndices, global int* occupancy, uint nTriangles, uint nVerts)
{
	uint triIndex = get_global_id(0);
	uint vertIndex = get_global_id(1);
	occupancy[triIndex * nVerts + vertIndex] = -1;
	if (triIndex < nTriangles && vertIndex < nVerts)
	{
		// check if any vertex of this triangle references the vp index of the current vertex
		for (int i = 0; i < 3; i++)
		{
			if (objIndices[vertIndex * 3] == objIndices[triIndex * 9 + 3 * i])
			{
				occupancy[triIndex * nVerts + vertIndex] = i;
			}
		}
	}
}


/* matA.length = dim ** 2 */

void gauss_seidel(local float* matA, local float* x, local float* b, uint dim, float epsilon, uint maxiter)
{
	float error;
	uint iter = 0;
	do
	{
		error = 0.0f;
		for (uint i = 0; i < dim; i++)
		{
			float tmpX = x[i]; // store previous iteration value for error accumulation
			x[i] = b[i];
			for (uint j = 0; j < dim; j++)
			{
				if (j != i)
					x[i] += matA[matLoc(i, j, dim)] * x[j];
			}
			x[i] /= matA[matLoc(i, i, dim)];
			float delta = x[i] - tmpX;
			error += delta * delta;
		}
	} while (error > epsilon && (++iter) < maxiter);
}

kernel void sort_bins(global uint* indices, global uint* binLabels, 
	global uint* bins, global uint* binCounts, uint bin_capacity, 
	uint n_bins, uint n_tris,
	local uint* localIndices, local uint* localBinLabels,
	local uint* localBins, local uint* localBinCounts, uint localBinCapacity)
{
	uint idx = get_global_id(0);
	uint gid = get_group_id(0);
	uint lid = get_local_id(0);
	uint localSize = get_local_size(0);
	
	if (lid < n_bins)
	{
		localBinCounts[lid] = 0;
	}
	if (lid == 0)
	{
		for (int i = localSize; i < n_bins; i++)
		{
			localBinCounts[i] = 0;
		}
	}

	barrier(CLK_LOCAL_MEM_FENCE);
	
	if (idx < n_tris)
	{
		localIndices[lid * 3] = indices[idx * 3];
		localIndices[lid * 3 + 1] = indices[idx * 3 + 1];
		localIndices[lid * 3 + 2] = indices[idx * 3 + 2];

		localBinLabels[lid] = binLabels[idx];

		barrier(CLK_LOCAL_MEM_FENCE);
		
		uint assignedBin = localBinLabels[lid];

		uint binPos = atomic_add(&localBinCounts[assignedBin], 1);
		uint destBinIdx = assignedBin * localBinCapacity * 3 + binPos * 3;
		uint srcIdx = lid * 3;
		
		localBins[destBinIdx] = localIndices[srcIdx];
		localBins[destBinIdx + 1] = localIndices[srcIdx + 1];
		localBins[destBinIdx + 2] = localIndices[srcIdx + 2];
	}
	barrier(CLK_LOCAL_MEM_FENCE);

	if (gid * localSize < n_tris)
	{
		if (lid < n_bins)
		{
			uint destIdx = atomic_add(&binCounts[lid], localBinCounts[lid]);
			for (int i = 0; i < localBinCounts[lid]; i++)
			{
				uint globalOffset = 3 * (lid * bin_capacity + destIdx + i);
				uint localOffset = 3 * (lid * localBinCapacity * n_bins + i);
				bins[globalOffset] = localBins[localOffset];
				bins[globalOffset + 1] = localBins[localOffset + 1];
				bins[globalOffset + 2] = localBins[localOffset + 2];
			}
		}

		if (lid == 0)
		{
			for (int j = localSize; j < n_bins; j++)
			{
				uint destIdx = atomic_add(&binCounts[j], localBinCounts[j]);
				for (int i = 0; i < localBinCounts[j]; i++)
				{
					uint globalOffset = 3 * (j * bin_capacity + destIdx + i);
					uint localOffset = 3 * (j * localBinCapacity * n_bins + i);
					bins[globalOffset] = localBins[localOffset];
					bins[globalOffset + 1] = localBins[localOffset + 1];
					bins[globalOffset + 2] = localBins[localOffset + 2];
				}
			}
		}
	}
	
}

kernel void test(global int* data, local int* localData, global int* outData)
{
	uint gid = get_global_id(0);
	uint localSize = get_local_size(0);
	uint lid = get_local_id(0);
	
	localData[lid] = data[gid];
	barrier(CLK_LOCAL_MEM_FENCE); // synchronized local after loading from global
	for (uint i = localSize >> 1; i > 0; i >>= 1)
	{
		if (lid < i)
		{
			localData[lid] += localData[lid + i];
		}
		barrier(CLK_LOCAL_MEM_FENCE); // synchronize local after each reduction
		// Invariant : localData[k]_{j+1} = localData[k]_{j} + localData[k + i]_{j} for k in range(0, i)
	}

	if (lid == 0)
	{
		outData[get_group_id(0)] = localData[0];
	}
}

kernel void convolve1D(global int* data, local int* localData, global int* filter, uint filter_width, global int* result)
{
	int position = get_global_id(0);
	uint filter_half = filter_width >> 1;

	// Offset local_id by filter_half to make room for boundary in localData
	int localPos = get_local_id(0) + filter_half;

	uint localSize = get_local_size(0);
	uint globalSize = get_global_size(0);
	// localData[0: filter_half] = data[globalGroupPos - filter_half: globalGroupPos - 1]
	localData[localPos] = data[position];
	
	int globalGroupPos = get_group_id(0) * localSize;

	// Fill in boundary of work group only if this is the first thread in the group
	if (get_local_id(0) == 0)
	{
		for (int i = 0; i < filter_half; i++)
		{
			// Position <i> cells from left and right boundaries
			int lPos = globalGroupPos - i - 1;
			int rPos = globalGroupPos + localSize + i;
		
			// Copy global boundaries to local if work group boundary is within global domain
			localData[filter_half - i - 1] = (lPos >= 0) ? data[lPos] : 0;
			localData[filter_half + localSize + i] = (rPos < globalSize) ? data[rPos] : 0;
		}
	}
	// Boundary values are loaded for work group; synchronize local data and compute convolution for this group
	barrier(CLK_LOCAL_MEM_FENCE);

	for (int i = 0; i < filter_width; i++)
	{
		int samplePos = localPos - filter_half + i;
		result[position] += localData[samplePos] * filter[i];
	}
}


kernel void testLocalId(global int* data, local int* localData, global int* outData)
{
	uint gid = get_global_id(0);
	uint lid = get_local_id(0);
	localData[lid] = data[gid];
	if (lid < 3)
	{
		outData[gid] = localData[lid];
	}
	
}

/*
kernel void convolve2D(global int* data, local int* localData, global const int* filter, global const size_t filter_width, global const size_t filter_height, global int* result)
{
	uint2 position = (uint2)(get_global_id(0), get_global_id(1));
	uint2 globalSize = (uint2)(get_global_size(0), get_global_size(1));
	uint2 filter_half = (uint2)(filter_width >> 1, filter_height >> 1);
	uint2 localSize = (uint2)(get_local_size(0), get_local_size(1));
	int res = 0;
	uint2 localPos = (uint2)(get_local_id(0), get_local_id(1));
	localData[localPos.x * (localSize.y - 1) + localPos.y] = data[position.x * (globalSize.y - 1) + position.y];
	barrier(CLK_LOCAL_MEM_FENCE);
	for (int i = localPos.x - filter_half.x; i < localPos.x + filter_half.x; i++)
	{
		for (int j = localPos.y - filter_half.y; j < localPos.y + filter_half.y; j++)
		{
			if (i >= 0 && i < localSize.x && j >= 0 && j < localSize.y)
			{
				res += filter[i - localPos.x + filter_half.x][j - localPos.y + filter_half.y] * localData[i][j];
			}
		}
	}
	result[position.x][position.y] = res;
}
*/
