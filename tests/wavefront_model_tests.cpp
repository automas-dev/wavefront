#include <gtest/gtest.h>

#include <sstream>
#include <string>
using namespace std;

#include <wavefront/Wavefront.hpp>
using namespace wavefront;

#include "TestUtils.hpp"
using wavefront_test::TempDir;

TEST(ModelTest, Empty) {
    stringstream ss("");
    auto model = Model::fromStream(ss);
    ASSERT_NE(nullptr, model);
    EXPECT_TRUE(model->materials.empty());
    EXPECT_TRUE(model->objects.empty());
}

TEST(ModelTest, NoModel) {
    stringstream ss("v 1.0 2.0 3.0");
    EXPECT_THROW(Model::fromStream(ss), ModelLoadException);
}

TEST(ModelTest, MultiModel) {
    stringstream ss("o One\no Two");
    auto model = Model::fromStream(ss);
    ASSERT_NE(nullptr, model);
    ASSERT_EQ(2, model->objects.size());
    EXPECT_EQ("One", model->objects[0]->name);
    EXPECT_EQ("Two", model->objects[1]->name);
}

TEST(ModelTest, FromFileMissing) {
    TempDir dir;
    EXPECT_THROW(Model::fromFile(dir.path / "missing.obj"), ModelLoadException);
}

TEST(ModelTest, FromFileSuccess) {
    TempDir dir;
    dir.writeFile("model.mtl", "newmtl OtherMat\nnewmtl FileMat\n");
    auto objPath = dir.writeFile(
        "model.obj",
        "o objName\n"
        "mtllib model.mtl\n"
        "usemtl FileMat\n"
        "");
    auto model = Model::fromFile(objPath);
    ASSERT_NE(nullptr, model);
    ASSERT_EQ(1, model->objects.size());
    ASSERT_EQ(2, model->materials.size());
    EXPECT_EQ("OtherMat", model->materials[0]->name);
    EXPECT_EQ("FileMat", model->materials[1]->name);
    EXPECT_EQ(1, model->objects[0]->matId);
}

TEST(ModelTest, AttrErrorTooSmall) {
    vector<string> attrs {
        "v",
        "vt",
        "vn",
        "f",
    };
    for (auto & attr : attrs) {
        stringstream ss("o a\n" + attr);
        EXPECT_THROW(Model::fromStream(ss), ModelLoadException) << attr;
    }
}

TEST(ModelTest, AllAtributes) {
    stringstream ss(
        "# Comment on it's own line\n"
        "o objName  # Comment after token\n"
        "vx # v second wrong\n"
        "v 0.0 1.0 2.0\n"
        "v 3.0 4.0 5.0\n"
        "v 6.0 7.0 8.0\n"
        "vt 9.0 10.0\n"
        "vt 11.0 12.0\n"
        "vt 13.0 14.0\n"
        "vn 15.0 16.0 17.0\n"
        "vn 18.0 19.0 20.0\n"
        "vn 21.0 22.0 23.0\n"
        "f 1/1/1 2/2/2 3/3/3\n"
        "");
    auto model = Model::fromStream(ss);
    ASSERT_NE(nullptr, model);
    ASSERT_EQ(1, model->objects.size());
    EXPECT_EQ(0, model->materials.size());
    auto obj = model->objects[0];
    EXPECT_EQ("objName", obj->name);
    EXPECT_EQ(-1, obj->matId);
    EXPECT_EQ(3, obj->size());
    EXPECT_EQ(vec3(0.0, 1.0, 2.0), obj->vertices[0]);
    EXPECT_EQ(vec3(3.0, 4.0, 5.0), obj->vertices[1]);
    EXPECT_EQ(vec3(6.0, 7.0, 8.0), obj->vertices[2]);
    EXPECT_EQ(vec2(9.0, 10.0), obj->texcoords[0]);
    EXPECT_EQ(vec2(11.0, 12.0), obj->texcoords[1]);
    EXPECT_EQ(vec2(13.0, 14.0), obj->texcoords[2]);
    EXPECT_EQ(vec3(15.0, 16.0, 17.0), obj->normals[0]);
    EXPECT_EQ(vec3(18.0, 19.0, 20.0), obj->normals[1]);
    EXPECT_EQ(vec3(21.0, 22.0, 23.0), obj->normals[2]);
}

TEST(ModelTest, FWrongArgCount) {
    stringstream ss(
        "o objName\n"
        "f 1/1/1/1 2/2/2 3/3/3\n"
        "");
    EXPECT_THROW(Model::fromStream(ss), ModelLoadException);
}

TEST(ModelTest, VIndexOutOfRange) {
    stringstream ss(
        "o objName\n"
        "f 1/1/1 2/2/2 3/3/3\n"
        "");
    EXPECT_THROW(Model::fromStream(ss), ModelLoadException);
}

TEST(ModelTest, TIndexOutOfRange) {
    stringstream ss(
        "o objName\n"
        "v 0.0 1.0 2.0\n"
        "f 1/1/1 2/2/2 3/3/3\n"
        "");
    EXPECT_THROW(Model::fromStream(ss), ModelLoadException);
}

TEST(ModelTest, NIndexOutOfRange) {
    stringstream ss(
        "o objName\n"
        "v 0.0 1.0 2.0\n"
        "vt 9.0 10.0\n"
        "f 1/1/1 2/2/2 3/3/3\n"
        "");
    EXPECT_THROW(Model::fromStream(ss), ModelLoadException);
}

TEST(ModelTest, MissingMaterial) {
    stringstream ss(
        "o objName\n"
        "usemtl missing\n"
        "");
    EXPECT_THROW(Model::fromStream(ss), ModelLoadException);
}

TEST(ModelTest, UNotUseMtl) {
    stringstream ss(
        "o objName\n"
        "ux\n"
        "");
    auto model = Model::fromStream(ss);
    ASSERT_NE(nullptr, model);
    EXPECT_EQ(1, model->objects.size());
    EXPECT_TRUE(model->materials.empty());
}

TEST(ModelTest, MNotMtllib) {
    stringstream ss(
        "o objName\n"
        "mx\n"
        "");
    auto model = Model::fromStream(ss);
    ASSERT_NE(nullptr, model);
    EXPECT_EQ(1, model->objects.size());
    EXPECT_TRUE(model->materials.empty());
}
