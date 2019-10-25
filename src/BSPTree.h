#pragma once

#include "BSPNode.h"
#include "BoundingBox.h"

namespace {
	inline int MaxDim(Vec3f v)
	{
		return (v.val[0] > v.val[1]) ? ((v.val[0] > v.val[2]) ? 0 : 2) : ((v.val[1] > v.val[2]) ? 1 : 2);
	}
}

class BSPTree
{
public:
	/**
	 * @brief Constructor
	 * @param bounds The scene bounding box
	 * @param vpPrims The vector of pointers to the primitives in the scene
	 */
	BSPTree(CBoundingBox& bounds, const std::vector<std::shared_ptr<CPrim>>& vpPrims)
		: m_bounds(bounds)
		, m_maxDepth(20)
		, m_minTri(3)
		, m_root(nullptr)
	{
		m_root = BuildTree(bounds, vpPrims, 0);
	}
	
	/**
	 * @brief Builds the BSP tree
	 * @details This function builds the BSP tree recursively
	 * @param box The bounding box containing all the scene primitives
	 * @param vpPrims The vector of pointers to the primitives included in the bounding box \b box
	 * @param depth The distance from the root node of the tree
	 */
	std::shared_ptr<CBSPNode> BuildTree(const CBoundingBox& box, const std::vector<std::shared_ptr<CPrim>>& vpPrims, int depth)
	{
		float splitVal;
		int splitDim;
		std::shared_ptr<CBSPNode> pLeft;
		std::shared_ptr<CBSPNode> pRight;
		// --- PUT YOUR CODE HERE ---

		// left and right vectors after splitting
		std::vector<std::shared_ptr<CPrim>> vpPrims_left;
		std::vector<std::shared_ptr<CPrim>> vpPrims_right;

		// new voxels for left and right children
		CBoundingBox voxel_left;
		CBoundingBox voxel_right;

		// get the split dimension and splitVal
		splitDim = depth % 3;
		splitVal = box.m_max[splitDim] - (box.m_max[splitDim] - box.m_min[splitDim]) / 2;

		/********************************
		 * Base Case
		 * ******************************/
		if(depth >=20 || vpPrims.size() <=4) {
			std::shared_ptr<CBSPNode> some_leaf = std::make_shared<CBSPNode>(vpPrims);
			return some_leaf;
		}

		// loop through primitives and sort into left and right voxels
		for(auto prim : vpPrims) {
			if(prim.get()->calcBounds().m_max[splitDim] < splitVal) {
				vpPrims_left.push_back(prim);
			} else {
				vpPrims_right.push_back(prim);
			}
		}

		// initialize new voxels
		for(int i=0; i<3; i++) {
			voxel_left.m_min[i] = box.m_min[i];
			voxel_left.m_max[i] = box.m_max[i];

			voxel_right.m_min[i] = box.m_min[i];
			voxel_right.m_max[i] = box.m_max[i];
		}

		// adjust voxels appropriately
		voxel_left.m_max[splitDim] = splitVal;
		voxel_right.m_min[splitDim] += voxel_right.m_max[splitDim] - splitVal;

		// get the right and left voxels
		pLeft =  BuildTree(voxel_left, vpPrims_left, depth+1);
		pRight = BuildTree(voxel_right, vpPrims_right, depth+1);

		return std::make_shared<CBSPNode>(splitVal, splitDim, pLeft, pRight);
	}

	/**
	 * @brief Checks whether the ray \b ray intersects a primitive.
	 * @details If ray \b ray intersects a primitive, the \b ray.t value will be updated
	 * @param[in,out] ray The ray
	 */
	bool Intersect(Ray& ray)
	{
		// --- PUT YOUR CODE HERE ---
		// std::cout << "FOR RAY: " << ray.dir << std::endl;
		// std::cout << CBSPNode::total << std::endl;
		ray.t = std::numeric_limits<float>::infinity();
		float t0, t1;
		m_bounds.clip(ray, t0, t1);
		return m_root.get()->traverse(ray, t0, t1);
	}

	
private:
	CBoundingBox 				m_bounds;
	const int 	 				m_maxDepth;
	const int 	 				m_minTri;
	std::shared_ptr<CBSPNode> 	m_root;
};
	
