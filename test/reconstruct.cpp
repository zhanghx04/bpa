#include <gtest/gtest.h>
#include <glm/glm.hpp>

#include <chrono>
#include <iostream>
#include <numbers>

#include <iostream>
#include <filesystem>

#include "../src/bpa.h"
#include "../src/IO.h"

using std::system;
namespace fs = std::filesystem;

using namespace bpa;

namespace {
	auto createSphericalCloud(int slices, int stacks) -> std::vector<Point> {
		std::vector<Point> points;
		points.emplace_back(Point{{0, 0, -1}, {0, 0, -1}});
		for (auto slice = 0; slice < slices; slice++) {
			for (auto stack = 1; stack < stacks; stack++) {
				const auto yaw = (static_cast<double>(slice) / slices) * 2 * std::numbers::pi;
				const auto z = std::sin((static_cast<double>(stack) / stacks - 0.5) * std::numbers::pi);
				const auto r = std::sqrt(1 - z * z);

				glm::vec3 v;
				v.x = static_cast<float>(r * std::sin(yaw));
				v.y = static_cast<float>(r * std::cos(yaw));
				v.z = static_cast<float>(z);
				points.push_back({v, glm::normalize(v - glm::vec3{})});
			}
		}
		points.emplace_back(Point{{0, 0, 1}, {0, 0, 1}});
		return points;
	}

	auto measuredReconstruct(const std::vector<Point>& points, float radius) -> std::vector<Triangle> {
		const auto start = std::chrono::high_resolution_clock::now();
		auto result = reconstruct(points, radius);
		const auto end = std::chrono::high_resolution_clock::now();
		const auto seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();
		std::cerr << "[          ] Point: " << points.size() << " Triangles: " << result.size() << " T/s: " << result.size() / seconds << '\n';
		return result;
	}
}

/*
TEST(reconstruct, sphere_36_18) {
	const auto cloud = createSphericalCloud(36, 18);
	savePoints("sphere_36_18_cloud.ply", cloud);
	const auto mesh = measuredReconstruct(cloud, 0.3f);
	saveTriangles("sphere_36_18_mesh.stl", mesh);
}

TEST(reconstruct, sphere_100_50) {
	const auto cloud = createSphericalCloud(100, 50);
	savePoints("sphere_100_50_cloud.ply", cloud);
	const auto mesh = measuredReconstruct(cloud, 0.1f);
	saveTriangles("sphere_100_50_mesh.stl", mesh);
}

TEST(reconstruct, sphere_200_100) {
	const auto cloud = createSphericalCloud(200, 100);
	savePoints("sphere_200_100_cloud.ply", cloud);
	const auto mesh = measuredReconstruct(cloud, 0.04f);
	saveTriangles("sphere_200_100_mesh.stl", mesh);
}

TEST(reconstruct, tetrahedron) {
	const auto cloud = std::vector<Point>{
		{{0, 0, 0}, glm::normalize(glm::vec3{-1, -1, -1})},
		{{0, 1, 0}, glm::normalize(glm::vec3{0, 1, 0})},
		{{1, 0, 0}, glm::normalize(glm::vec3{1, 0, 0})},
		{{0, 0, 1}, glm::normalize(glm::vec3{0, 0, 1})}
	};
	savePoints("tetrahedron_cloud.ply", cloud);
	const auto mesh = measuredReconstruct(cloud, 2);
	saveTriangles("tetrahedron_mesh.stl", mesh);
}

TEST(reconstruct, cube) {
	const auto cloud = std::vector<Point>{
		{{-1, -1, -1}, glm::normalize(glm::vec3{-1, -1, -1})},
		{{-1, +1, -1}, glm::normalize(glm::vec3{-1, +1, -1})},
		{{+1, +1, -1}, glm::normalize(glm::vec3{+1, +1, -1})},
		{{+1, -1, -1}, glm::normalize(glm::vec3{+1, -1, -1})},
		{{-1, -1, +1}, glm::normalize(glm::vec3{-1, -1, +1})},
		{{-1, +1, +1}, glm::normalize(glm::vec3{-1, +1, +1})},
		{{+1, +1, +1}, glm::normalize(glm::vec3{+1, +1, +1})},
		{{+1, -1, +1}, glm::normalize(glm::vec3{+1, -1, +1})},
	};
	savePoints("cube_cloud.ply", cloud);
	const auto mesh = measuredReconstruct(cloud, 2);
	saveTriangles("cube_mesh.stl", mesh);
}

TEST(reconstruct, bunny) {
	const auto cloud = loadXYZ("../test/data/bunny.xyz");
	const auto mesh = measuredReconstruct(cloud, 0.002f);
	saveTriangles("bunny_mesh.stl", mesh);
}
*/

TEST(reconstruct, ABQ) {
	// double radius = 2.0f;
	std::string filename = "abq_blocks_outside_normals_ascii";
	std::string output_directory = "../outputs/" + filename + "/";

	// check if the directory is exist
	auto ret = fs::create_directories(output_directory);
	if (ret) {
        std::cout << "directory " << filename << " created!!!" << std::endl;
    } else {
        std::cout << "folder:" << filename << " exist" << std::endl;
    }

	const auto cloud = loadXYZ("../test/data/" + filename + ".xyz");

	for (double radius = 1.5f; radius <= 5.5; radius += 0.5f) {
		const auto mesh = measuredReconstruct(cloud, radius);

		// get the ouptut file name
		std::string r = std::to_string(radius);
		r.erase( r.find_first_of('.') + 3);
		std::string filepath = output_directory + r + "_abq_blocks_outside_normals_ascii.stl";

		saveTriangles(filepath, mesh);
	}
}