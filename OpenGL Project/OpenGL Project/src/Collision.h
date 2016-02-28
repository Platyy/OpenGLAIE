#pragma once
#include "glm\glm.hpp"
#include <iostream>
#include <vector>
class AABB
{
public:
	AABB() { Reset();  }
	~AABB() {}

	glm::vec3 min, max;

	void Reset() { min.x = min.y = min.z = 1e37f; max.x = max.y = max.z = -1e37f; }

	void Fit(const std::vector<glm::vec3>& points) {
		for (auto& p : points) {
			if (p.x < min.x) min.x = p.x;
			if (p.y < min.y) min.y = p.y;
			if (p.z < min.z) min.z = p.z;
			if (p.x > max.x) max.x = p.x;
			if (p.y > max.y) max.y = p.y;
			if (p.z > max.z) max.z = p.z;
		}
	}
};

class BoundingSphere
{
public:
	BoundingSphere() : center(0), radius(0) {}
	~BoundingSphere() {}

	void Fit(const std::vector<glm::vec3>& points) {
		glm::vec3 min(1e37f), max(-1e37f);
		for (auto& p : points) {
			if (p.x < min.x) min.x = p.x;
			if (p.y < min.y) min.y = p.y;
			if (p.z < min.z) min.z = p.z;
			if (p.x > max.x) max.x = p.x;
			if (p.y > max.y) max.y = p.y;
			if (p.z > max.z) max.z = p.z;
		}
		center = (min + max) * 0.5f;
		radius = glm::distance(min, center);
	}

	void GetFrustumPlanes(const glm::mat4& transform, glm::vec4* planes);

	glm::vec3 center;
	float radius;
};