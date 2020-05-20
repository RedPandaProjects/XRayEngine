#include "pch.h"
#include <memory>
#include <unordered_map>
#include <unordered_set>
template <typename u32>
struct InlineMeshlet
{
    struct PackedTriangle
    {
        u32 i0 : 10;
        u32 i1 : 10;
        u32 i2 : 10;
        u32 spare : 2;
    };

    xr_vector<u32>              UniqueVertexIndices;
    xr_vector<PackedTriangle> PrimitiveIndices;
};


namespace
{
    struct EdgeEntry
    {
        uint32_t   i0;
        uint32_t   i1;
        uint32_t   i2;

        uint32_t   Face;
        EdgeEntry* Next;
    };

    size_t CRCHash(const uint32_t* dwords, uint32_t dwordCount)
    {
        size_t h = 0;

        for (uint32_t i = 0; i < dwordCount; ++i)
        {
            uint32_t highOrd = h & 0xf8000000;
            h = h << 5;
            h = h ^ (highOrd >> 27);
            h = h ^ size_t(dwords[i]);
        }

        return h;
    }

    template <typename T>
    inline size_t Hash(const T& val)
    {
        return std::hash<T>()(val);
    }
}

namespace std
{
    template <> struct hash<Fvector> { size_t operator()(const Fvector& v) const { return CRCHash(reinterpret_cast<const uint32_t*>(&v), sizeof(v) / 4); } };
}


inline Fvector operator*(const Fvector& a, float b)
{
    Fvector result;
    result.set(a);
    result.x *= b;
    result.y *= b;
    result.z *= b;
    return a;
}
inline Fvector operator+(const Fvector& a, const Fvector& b)
{
    Fvector result;
    result.set(a);
    result.x += b.x;
    result.y += b.y;
    result.z += b.z;
    return a;
}
inline Fvector operator-(const Fvector& a, const Fvector& b)
{
    Fvector result;
    result.set(a);
    result.x -= b.x;
    result.y -= b.y;
    result.z -= b.z;
    return a;
}

float LeghtVectorSq(const Fvector& a, const Fvector& b)
{
    return a.dotproduct(b);
}
float LeghtVectorSq(Fvector& a)
{
    return LeghtVectorSq(a, a);
}
float LeghtVector(const Fvector& a, const Fvector& b)
{
    return sqrtf(a.dotproduct(b));
}
float LeghtVector(Fvector& a)
{
    return LeghtVector(a, a);
}
 Fvector3 GetPosition(const void* vertex_buffer, size_t stride,size_t id)
{
    u8* p = (u8*)vertex_buffer;
    p += (stride * id);
    return *(Fvector*)p;
}
void BuildAdjacencyList( const u32* indices, uint32_t indexCount,  const void* vertex_buffer, size_t stride, uint32_t vertexCount,uint32_t* adjacency)
{
    const uint32_t triCount = indexCount / 3;
    // Find point reps (unique positions) in the position stream
    // Create a mapping of non-unique vertex indices to point reps
    xr_vector<u32> pointRep;
    pointRep.resize(vertexCount);

    std::unordered_map<size_t, u32> uniquePositionMap;
    uniquePositionMap.reserve(vertexCount);

    for (uint32_t i = 0; i < vertexCount; ++i)
    {
        Fvector position = GetPosition(vertex_buffer, stride, i);
        size_t hash = Hash(position);

        auto it = uniquePositionMap.find(hash);
        if (it != uniquePositionMap.end())
        {
            // Position already encountered - reference previous index
            pointRep[i] = it->second;
        }
        else
        {
            // New position found - add to hash table and LUT
            uniquePositionMap.insert(std::make_pair(hash, static_cast<u32>(i)));
            pointRep[i] = static_cast<u32>(i);
        }
    }

    // Create a linked list of edges for each vertex to determine adjacency
    const uint32_t hashSize = vertexCount / 3;

    std::unique_ptr<EdgeEntry* []> hashTable(new EdgeEntry * [hashSize]);
    std::unique_ptr<EdgeEntry[]> entries(new EdgeEntry[triCount * 3]);

    std::memset(hashTable.get(), 0, sizeof(EdgeEntry*) * hashSize);
    uint32_t entryIndex = 0;

    for (uint32_t iFace = 0; iFace < triCount; ++iFace)
    {
        uint32_t index = iFace * 3;

        // Create a hash entry in the hash table for each each.
        for (uint32_t iEdge = 0; iEdge < 3; ++iEdge)
        {
            u32 i0 = pointRep[indices[index + (iEdge % 3)]];
            u32 i1 = pointRep[indices[index + ((iEdge + 1) % 3)]];
            u32 i2 = pointRep[indices[index + ((iEdge + 2) % 3)]];

            auto& entry = entries[entryIndex++];
            entry.i0 = i0;
            entry.i1 = i1;
            entry.i2 = i2;

            uint32_t key = entry.i0 % hashSize;

            entry.Next = hashTable[key];
            entry.Face = iFace;

            hashTable[key] = &entry;
        }
    }


    // Initialize the adjacency list
    std::memset(adjacency, uint32_t(-1), indexCount * sizeof(uint32_t));

    for (uint32_t iFace = 0; iFace < triCount; ++iFace)
    {
        uint32_t index = iFace * 3;

        for (uint32_t point = 0; point < 3; ++point)
        {
            if (adjacency[iFace * 3 + point] != uint32_t(-1))
                continue;

            // Look for edges directed in the opposite direction.
            u32 i0 = pointRep[indices[index + ((point + 1) % 3)]];
            u32 i1 = pointRep[indices[index + (point % 3)]];
            u32 i2 = pointRep[indices[index + ((point + 2) % 3)]];

            // Find a face sharing this edge
            uint32_t key = i0 % hashSize;

            EdgeEntry* found = nullptr;
            EdgeEntry* foundPrev = nullptr;

            for (EdgeEntry* current = hashTable[key], *prev = nullptr; current != nullptr; prev = current, current = current->Next)
            {
                if (current->i1 == i1 && current->i0 == i0)
                {
                    found = current;
                    foundPrev = prev;
                    break;
                }
            }

            // Cache this face's normal
            Fvector3 n0;
            {
                Fvector3 p0 = GetPosition(vertex_buffer, stride,i1);
                Fvector3 p1 = GetPosition(vertex_buffer, stride, i0);
                Fvector3 p2 = GetPosition(vertex_buffer, stride, i2);

                Fvector3 e0 = p0 - p1;
                Fvector3 e1 = p1 - p2;

                n0.crossproduct(e0, e1).normalize();
            }

            // Use face normal dot product to determine best edge-sharing candidate.
            float bestDot = -2.0f;
            for (EdgeEntry* current = found, *prev = foundPrev; current != nullptr; prev = current, current = current->Next)
            {
                if (bestDot == -2.0f || (current->i1 == i1 && current->i0 == i0))
                {

                    Fvector3 p0 = GetPosition(vertex_buffer, stride, current->i0);
                    Fvector3 p1 = GetPosition(vertex_buffer, stride, current->i1);
                    Fvector3 p2 = GetPosition(vertex_buffer, stride, current->i2);

                    Fvector3 e0 = p0 - p1;
                    Fvector3 e1 = p1 - p2;

                    Fvector3  n1;
                    n1.crossproduct(e0, e1).normalize();

  
                    float dot = n0.dotproduct(n1);

                    if (dot > bestDot)
                    {
                        found = current;
                        foundPrev = prev;
                        bestDot = dot;
                    }
                }
            }

            // Update hash table and adjacency list
            if (found && found->Face != uint32_t(-1))
            {
                // Erase the found from the hash table linked list.
                if (foundPrev != nullptr)
                {
                    foundPrev->Next = found->Next;
                }
                else
                {
                    hashTable[key] = found->Next;
                }

                // Update adjacency information
                adjacency[iFace * 3 + point] = found->Face;

                // Search & remove this face from the table linked list
                uint32_t key2 = i1 % hashSize;

                for (EdgeEntry* current = hashTable[key2], *prev = nullptr; current != nullptr; prev = current, current = current->Next)
                {
                    if (current->Face == iFace && current->i1 == i0 && current->i0 == i1)
                    {
                        if (prev != nullptr)
                        {
                            prev->Next = current->Next;
                        }
                        else
                        {
                            hashTable[key2] = current->Next;
                        }

                        break;
                    }
                }

                bool linked = false;
                for (uint32_t point2 = 0; point2 < point; ++point2)
                {
                    if (found->Face == adjacency[iFace * 3 + point2])
                    {
                        linked = true;
                        adjacency[iFace * 3 + point] = uint32_t(-1);
                        break;
                    }
                }

                if (!linked)
                {
                    uint32_t edge2 = 0;
                    for (; edge2 < 3; ++edge2)
                    {
                        u32 k = indices[found->Face * 3 + edge2];
                        if (k == uint32_t(-1))
                            continue;

                        if (pointRep[k] == i0)
                            break;
                    }

                    if (edge2 < 3)
                    {
                        adjacency[found->Face * 3 + edge2] = iFace;
                    }
                }
            }
        }
    }
}

Fvector4 MinimumBoundingSphere(Fvector3* points, uint32_t count)
{
    VERIFY(points != nullptr && count != 0);

    // Find the min & max points indices along each axis.
    uint32_t minAxis[3] = { 0, 0, 0 };
    uint32_t maxAxis[3] = { 0, 0, 0 };

    for (uint32_t i = 1; i < count; ++i)
    {
        float* point = (float*)(points + i);

        for (uint32_t j = 0; j < 3; ++j)
        {
            float* min = (float*)(&points[minAxis[j]]);
            float* max = (float*)(&points[maxAxis[j]]);

            minAxis[j] = point[j] < min[j] ? i : minAxis[j];
            maxAxis[j] = point[j] > max[j] ? i : maxAxis[j];
        }
    }

    // Find axis with maximum span.
    float distSqMax = 0;
    uint32_t axis = 0;

    for (uint32_t i = 0; i < 3u; ++i)
    {
        Fvector min = points[minAxis[i]];
        Fvector max = points[maxAxis[i]];

        float  distSq = LeghtVectorSq(max - min);
        if (distSq > distSqMax)
        {
            distSqMax = distSq;
            axis = i;
        }
    }

    // Calculate an initial starting center point & radius.
    Fvector3 p1 = points[minAxis[axis]];
    Fvector3 p2 = points[maxAxis[axis]];

    Fvector3 center = (p1 + p2) * 0.5f;
    float radius = LeghtVector(p2 - p1) * 0.5f;
    float radiusSq = radius * radius;

    // Add all our points to bounding sphere expanding radius & recalculating center point as necessary.
    for (uint32_t i = 0; i < count; ++i)
    {
        Fvector3 point = points[i];
        float distSq = LeghtVectorSq(point - center);

        if (distSq > radiusSq)
        {
            float dist = sqrtf(distSq);
            float k = (radius / dist) * 0.5f + 0.5f;

            center = center * k + point * (1 - k);
            radius = (radius + dist) * 0.5f;
        }
    }

    // Populate a single XMVECTOR with center & radius data.
    return Fvector4().set(center.x, center.y, center.z, radius);
}




///
// Helpers

// Sort in reverse order to use vector as a queue with pop_back.
bool CompareScores(const std::pair<u32, float>& a, const std::pair<u32, float>& b)
{
    return a.second > b.second;
}
/*a1.x a1.y a1.z
  a2.x a2.y a2.z
  
  */
Fvector ComputeNormal(Fvector* tri)
{
    Fvector p0 = tri[0];
    Fvector p1 = tri[1];
    Fvector p2 = tri[2];

    Fvector v01 = p0 - p1;
    Fvector v02 = p0 - p2;
    Fvector Out;
    return Out.crossproduct(v01, v02).normalize();
}



// Compute number of triangle vertices already exist in the meshlet
template <typename u32>
u32 ComputeReuse(const InlineMeshlet<u32>& meshlet, u32(&triIndices)[3])
{
    u32 count = 0;

    for (u32 i = 0; i < static_cast<u32>(meshlet.UniqueVertexIndices.size()); ++i)
    {
        for (u32 j = 0; j < 3u; ++j)
        {
            if (meshlet.UniqueVertexIndices[i] == triIndices[j])
            {
                ++count;
            }
        }
    }

    return count;
}

// Computes a candidacy score based on spatial locality, orientational coherence, and vertex re-use within a meshlet.

float ComputeScore(const InlineMeshlet<u32>& meshlet, Fvector4 sphere, Fvector normal, u32(&triIndices)[3], Fvector* triVerts)
{
    const float reuseWeight = 0.334f;
    const float locWeight = 0.333f;
    const float oriWeight = 0.333f;

    // Vertex reuse
    u32 reuse = ComputeReuse(meshlet, triIndices);
    float reuseScore = 1 - (float(reuse) / 3.0f);

    // Distance from center point
    float maxSq = 0;
    for (u32 i = 0; i < 3u; ++i)
    {
        Fvector v = Fvector().set(sphere.x, sphere.y, sphere.z)- triVerts[i];
        maxSq = std::max(maxSq, v.dotproduct( v));
    }
    float r = sphere.w;
    float r2 = r * r;
    float locScore = logf(maxSq / r2 + 1);

    // Angle between normal and meshlet cone axis
    Fvector n = ComputeNormal(triVerts);
    float d = n.dotproduct( normal);
    float oriScore = (-d + 1) / 2.0f;

    float b = reuseWeight * reuseScore + locWeight * locScore + oriWeight * oriScore;

    return b;
}

// Determines whether a candidate triangle can be added to a specific meshlet; if it can, does so.
template <typename u32>
bool AddToMeshlet(u32 maxVerts, u32 maxPrims, InlineMeshlet<u32>& meshlet, u32(&tri)[3])
{
    // Are we already full of vertices?
    if (meshlet.UniqueVertexIndices.size() == maxVerts)
        return false;

    // Are we full, or can we store an additional primitive?
    if (meshlet.PrimitiveIndices.size() == maxPrims)
        return false;

    static const u32 Undef = u32(-1);
    u32 indices[3] = { Undef, Undef, Undef };
    u32 newCount = 3;

    for (u32 i = 0; i < meshlet.UniqueVertexIndices.size(); ++i)
    {
        for (u32 j = 0; j < 3; ++j)
        {
            if (meshlet.UniqueVertexIndices[i] == tri[j])
            {
                indices[j] = i;
                --newCount;
            }
        }
    }

    // Will this triangle fit?
    if (meshlet.UniqueVertexIndices.size() + newCount > maxVerts)
        return false;

    // Add unique vertex indices to unique vertex index list
    for (u32 j = 0; j < 3; ++j)
    {
        if (indices[j] == Undef)
        {
            indices[j] = static_cast<u32>(meshlet.UniqueVertexIndices.size());
            meshlet.UniqueVertexIndices.push_back(tri[j]);
        }
    }

    // Add the new primitive 
    typename InlineMeshlet<u32>::PackedTriangle prim = {};
    prim.i0 = indices[0];
    prim.i1 = indices[1];
    prim.i2 = indices[2];

    meshlet.PrimitiveIndices.push_back(prim);

    return true;
}

template <typename u32>
bool IsMeshletFull(u32 maxVerts, u32 maxPrims, const InlineMeshlet<u32>& meshlet)
{
    VERIFY(meshlet.UniqueVertexIndices.size() <= maxVerts);
    VERIFY(meshlet.PrimitiveIndices.size() <= maxPrims);

    return meshlet.UniqueVertexIndices.size() == maxVerts
        || meshlet.PrimitiveIndices.size() == maxPrims;
}


///
// Implementation 

void Meshletize(u32 maxVerts, u32 maxPrims,const u32* indices, u32 indexCount,  const void* vertex_buffer, size_t stride, u32 vertexCount, xr_vector<InlineMeshlet<u32>>& output)
{
    const u32 triCount = indexCount / 3;

    // Build a primitive adjacency list
    xr_vector<u32> adjacency;
    adjacency.resize(indexCount);

    BuildAdjacencyList(indices, indexCount, vertex_buffer,stride, vertexCount, adjacency.data());

    // Rest our outputs
    output.clear();
    output.emplace_back();
    auto* curr = &output.back();

    // Bitmask of all triangles in mesh to determine whether a specific one has been added.
    xr_vector<bool> checklist;
    checklist.resize(triCount);

    xr_vector<Fvector> m_positions;
    xr_vector<Fvector> normals;
    xr_vector<std::pair<u32, float>> candidates;
    std::unordered_set<u32> candidateCheck;

    Fvector4 psphere;
    Fvector normal;

    // Arbitrarily start at triangle zero.
    u32 triIndex = 0;
    candidates.push_back(std::make_pair(triIndex, 0.0f));
    candidateCheck.insert(triIndex);

    // Continue adding triangles until 
    while (!candidates.empty())
    {
        u32 index = candidates.back().first;
        candidates.pop_back();

        u32 tri[3] =
        {
            indices[index * 3],
            indices[index * 3 + 1],
            indices[index * 3 + 2],
        };

        VERIFY(tri[0] < vertexCount);
        VERIFY(tri[1] < vertexCount);
        VERIFY(tri[2] < vertexCount);

        // Try to add triangle to meshlet
        if (AddToMeshlet(maxVerts, maxPrims, *curr, tri))
        {
            // Success! Mark as added.
            checklist[index] = true;

            // Add m_positions & normal to list
            Fvector points[3] =
            {
                GetPosition(vertex_buffer,stride,tri[0]),
                GetPosition(vertex_buffer, stride, tri[1]),
                GetPosition(vertex_buffer, stride, tri[2]),
            };

            m_positions.push_back(points[0]);
            m_positions.push_back(points[1]);
            m_positions.push_back(points[2]);

            Fvector Normal = ComputeNormal(points);
            normals.push_back(Normal);

            // Compute new bounding sphere & normal axis
            psphere = MinimumBoundingSphere(m_positions.data(), static_cast<u32>(m_positions.size()));

            Fvector4 nsphere4 = MinimumBoundingSphere(normals.data(), static_cast<u32>(normals.size()));
            Fvector3 nsphere;
            nsphere.set(nsphere4.x, nsphere4.y, nsphere4.z);
            normal = nsphere.normalize();

            // Find and add all applicable adjacent triangles to candidate list
            const u32 adjIndex = index * 3;

            u32 adj[3] =
            {
                adjacency[adjIndex],
                adjacency[adjIndex + 1],
                adjacency[adjIndex + 2],
            };

            for (u32 i = 0; i < 3u; ++i)
            {
                // Invalid triangle in adjacency slot
                if (adj[i] == -1)
                    continue;

                // Already processed triangle
                if (checklist[adj[i]])
                    continue;

                // Triangle already in the candidate list
                if (candidateCheck.count(adj[i]))
                    continue;

                candidates.push_back(std::make_pair(adj[i], FLT_MAX));
                candidateCheck.insert(adj[i]);
            }

            // Re-score remaining candidate triangles
            for (u32 i = 0; i < static_cast<u32>(candidates.size()); ++i)
            {
                u32 candidate = candidates[i].first;

                u32 triIndices[3] =
                {
                    indices[candidate * 3],
                    indices[candidate * 3 + 1],
                    indices[candidate * 3 + 2],
                };

                VERIFY(triIndices[0] < vertexCount);
                VERIFY(triIndices[1] < vertexCount);
                VERIFY(triIndices[2] < vertexCount);

                Fvector triVerts[3] =
                { 
                    GetPosition(vertex_buffer,stride,triIndices[0]),
                    GetPosition(vertex_buffer, stride, triIndices[1]),
                    GetPosition(vertex_buffer, stride, triIndices[2]),
                };

                candidates[i].second = ComputeScore(*curr, psphere, normal, triIndices, triVerts);
            }

            // Determine whether we need to move to the next meshlet.
            if (IsMeshletFull(maxVerts, maxPrims, *curr))
            {
                m_positions.clear();
                normals.clear();
                candidateCheck.clear();

                // Use one of our existing candidates as the next meshlet seed.
                if (!candidates.empty())
                {
                    candidates[0] = candidates.back();
                    candidates.resize(1);
                    candidateCheck.insert(candidates[0].first);
                }

                output.emplace_back();
                curr = &output.back();
            }
            else
            {
                std::sort(candidates.begin(), candidates.end(), &CompareScores);
            }
        }
        else
        {
            if (candidates.empty())
            {
                m_positions.clear();
                normals.clear();
                candidateCheck.clear();

                output.emplace_back();
                curr = &output.back();
            }
        }

        // Ran out of candidates; add a new seed candidate to start the next meshlet.
        if (candidates.empty())
        {
            while (triIndex < triCount && checklist[triIndex])
                ++triIndex;

            if (triIndex == triCount)
                break;

            candidates.push_back(std::make_pair(triIndex, 0.0f));
            candidateCheck.insert(triIndex);
        }
    }

    // The last meshlet may have never had any primitives added to it - in which case we want to remove it.
    if (output.back().PrimitiveIndices.empty())
    {
        output.pop_back();
    }
}


void XRayMeshletGenerator::ComputeMeshlets(u32 maxVerts, u32 maxPrims, const u32* indices, u32 indexCount, const XRaySubset* indexSubsets, u32 subsetCount, const void* vertex_buffer, size_t stride, u32 vertexCount, xr_vector<XRaySubset>& meshletSubsets, xr_vector<XRayMeshlet>& meshlets, xr_vector<uint8_t>& uniqueVertexIndices, xr_vector<XRayPackedTriangle>& primitiveIndices)
{
    for (u32 i = 0; i < subsetCount; ++i)
    {
       XRaySubset s = indexSubsets[i];

        VERIFY(s.Offset + s.Count <= indexCount);

        xr_vector<InlineMeshlet<u32>> builtMeshlets;
        Meshletize(maxVerts, maxPrims, indices + s.Offset, s.Count, vertex_buffer,stride, vertexCount, builtMeshlets);

        XRaySubset meshletSubset;
        meshletSubset.Offset = static_cast<u32>(meshlets.size());
        meshletSubset.Count = static_cast<u32>(builtMeshlets.size());
        meshletSubsets.push_back(meshletSubset);

        // Determine final unique vertex index and primitive index counts & offsets.
        u32 startVertCount = static_cast<u32>(uniqueVertexIndices.size()) / sizeof(u32);
        u32 startPrimCount = static_cast<u32>(primitiveIndices.size());

        u32 uniqueVertexIndexCount = startVertCount;
        u32 primitiveIndexCount = startPrimCount;

        size_t offset = meshlets.size();
        // Resize the meshlet output array to hold the newly formed meshlets.
        meshlets.resize(builtMeshlets.size()+ offset);

        for (u32 i = 0; i < static_cast<u32>(builtMeshlets.size()); ++i)
        {
            meshlets[i + offset].VertOffset = uniqueVertexIndexCount;
            meshlets[i + offset].VertCount = static_cast<u32>(builtMeshlets[i].UniqueVertexIndices.size());
            uniqueVertexIndexCount += static_cast<u32>(builtMeshlets[i].UniqueVertexIndices.size());

            meshlets[i + offset].PrimOffset = primitiveIndexCount;
            meshlets[i + offset].PrimCount = static_cast<u32>(builtMeshlets[i].PrimitiveIndices.size());
            primitiveIndexCount += static_cast<u32>(builtMeshlets[i].PrimitiveIndices.size());
        }

        // Allocate space for the new data.
        uniqueVertexIndices.resize(uniqueVertexIndexCount * sizeof(u32));
        primitiveIndices.resize(primitiveIndexCount);

        // Copy data from the freshly built meshlets into the output buffers.
        auto vertDest = reinterpret_cast<u32*>(uniqueVertexIndices.data()) + startVertCount;
        auto primDest = reinterpret_cast<u32*>(primitiveIndices.data()) + startPrimCount;

        for (u32 j = 0; j < static_cast<u32>(builtMeshlets.size()); ++j)
        {
            std::memcpy(vertDest, builtMeshlets[j].UniqueVertexIndices.data(), builtMeshlets[j].UniqueVertexIndices.size() * sizeof(u32));
            std::memcpy(primDest, builtMeshlets[j].PrimitiveIndices.data(), builtMeshlets[j].PrimitiveIndices.size() * sizeof(u32));

            vertDest += builtMeshlets[j].UniqueVertexIndices.size();
            primDest += builtMeshlets[j].PrimitiveIndices.size();
        }
    }
}
