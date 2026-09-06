#include <gtest/gtest.h>

#include <sstream>
#include <string>
using namespace std;

#include <wavefront/Wavefront.hpp>
using namespace wavefront;

#include "TestUtils.hpp"
using wavefront_test::TempDir;

TEST(MaterialTest, Init) {
    Material mat;
    EXPECT_FLOAT_EQ(1.0, mat.specExp);
    EXPECT_FLOAT_EQ(1.0, mat.alpha);
}

TEST(MaterialTest, Empty) {
    stringstream ss("");
    auto mats = Material::fromStream(ss);
    EXPECT_TRUE(mats.empty());
}

TEST(MaterialTest, NoMaterial) {
    stringstream ss("Ka 1.0 2.0 3.0");
    EXPECT_THROW(Material::fromStream(ss), MaterialLoadException);
}

TEST(MaterialTest, MultiMaterial) {
    stringstream ss("newmtl One\nnewmtl Two");
    auto mats = Material::fromStream(ss);
    ASSERT_EQ(2, mats.size());
    EXPECT_EQ("One", mats[0]->name);
    EXPECT_EQ("Two", mats[1]->name);
}

TEST(MaterialTest, FromFileMissing) {
    TempDir dir;
    EXPECT_THROW(Material::fromFile(dir.path / "missing.mtl"), MaterialLoadException);
}

TEST(MaterialTest, FromFileSuccess) {
    TempDir dir;
    auto path = dir.writeFile("mat.mtl", "newmtl FileMat\n");
    auto mats = Material::fromFile(path);
    ASSERT_EQ(1, mats.size());
    EXPECT_EQ("FileMat", mats[0]->name);
}

TEST(MaterialTest, AttrErrorNoMaterial) {
    vector<string> attrs {
        "Ka",
        "Kd",
        "Ks",
        "Ni",
        "d",
        "map_Kd",
        "map_Ks",
        "map_bump",
    };
    for (auto & attr : attrs) {
        stringstream ss(attr);
        EXPECT_THROW(Material::fromStream(ss), MaterialLoadException) << attr;
    }
}

TEST(MaterialTest, AttrErrorTooSmall) {
    vector<string> attrs {
        "Ka",
        "Kd",
        "Ks",
    };
    for (auto & attr : attrs) {
        stringstream ss("newmtl a\n" + attr);
        EXPECT_THROW(Material::fromStream(ss), MaterialLoadException) << attr;
    }
}

TEST(MaterialTest, AllAtributes) {
    stringstream ss(
        "# Comment on it's own line\n"
        "newmtl mtlName  # Comment after token\n"
        "K # K too short\n"
        "Kx # K second wrong\n"
        "N # N too short\n"
        "Nx # N second wrong\n"
        "Ka 1.0 2.0 3.0\n"
        "Kd 4.0 5.0 6.0\n"
        "Ks 7.0 8.0 9.0\n"
        "Ni 10.0\n"
        "d 11.0\n"
        "m # m to short\n"
        "mapxx # m not map\n"
        "map_x # map wrong\n"
        "map_Kx # map_K neither Kd nor Ks\n"
        "map_Kd tex_albedo.png\n"
        "map_Ks tex_spec.png\n"
        "map_bump tex_bump.png\n"
        "");
    auto mats = Material::fromStream(ss);
    ASSERT_EQ(1, mats.size());
    auto mat = mats[0];
    EXPECT_EQ("mtlName", mat->name);
    EXPECT_EQ(vec3(1.0, 2.0, 3.0), mat->colAmbient);
    EXPECT_EQ(vec3(4.0, 5.0, 6.0), mat->colDiffuse);
    EXPECT_EQ(vec3(7.0, 8.0, 9.0), mat->colSpecular);
    EXPECT_FLOAT_EQ(10.0, mat->specExp);
    EXPECT_FLOAT_EQ(11.0, mat->alpha);
    EXPECT_EQ("tex_albedo.png", mat->texAlbedo);
    EXPECT_EQ("tex_spec.png", mat->texSpecular);
    EXPECT_EQ("tex_bump.png", mat->texNormal);
}
