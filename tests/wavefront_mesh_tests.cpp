#include <gtest/gtest.h>

#include <sstream>
#include <string>
using namespace std;

#include <wavefront/Wavefront.hpp>
using namespace wavefront;

TEST(MeshTest, Init) {
    Mesh mesh;
    EXPECT_TRUE(mesh.name.empty());
    EXPECT_EQ(-1, mesh.matId);
    EXPECT_TRUE(mesh.vertices.empty());
    EXPECT_TRUE(mesh.texcoords.empty());
    EXPECT_TRUE(mesh.normals.empty());
}

TEST(MeshTest, Size) {
    Mesh mesh;
    mesh.vertices.emplace_back();
    mesh.vertices.emplace_back();
    EXPECT_EQ(2, mesh.size());
}
