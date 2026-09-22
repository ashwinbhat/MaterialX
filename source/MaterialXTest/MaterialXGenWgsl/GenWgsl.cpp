//
// Copyright Contributors to the MaterialX Project
// SPDX-License-Identifier: Apache-2.0
//

#include <MaterialXTest/External/Catch/catch.hpp>

#include <MaterialXGenWgsl/WgslShaderGenerator.h>
#include <MaterialXGenWgsl/WgslSyntax.h>

#include <MaterialXGenShader/GenContext.h>
#include <MaterialXGenShader/Shader.h>
#include <MaterialXGenShader/Util.h>

#include <MaterialXFormat/Util.h>
#include <MaterialXFormat/XmlIo.h>

#include <MaterialXCore/Document.h>

#include <fstream>

namespace mx = MaterialX;

TEST_CASE("GenShader: WGSL Syntax Check", "[genwgsl]")
{
    mx::TypeSystemPtr typeSystem = mx::TypeSystem::create();
    mx::SyntaxPtr syntax = mx::WgslSyntax::create(typeSystem);

    REQUIRE(syntax->getTypeName(mx::Type::FLOAT) == "f32");
    REQUIRE(syntax->getTypeName(mx::Type::COLOR3) == "vec3f");
    REQUIRE(syntax->getTypeName(mx::Type::VECTOR4) == "vec4f");
    REQUIRE(syntax->getTypeName(mx::Type::MATRIX44) == "mat4x4f");
    REQUIRE(syntax->getSourceFileExtension() == ".wgsl");
}

TEST_CASE("GenShader: WGSL Target Registration", "[genwgsl]")
{
    mx::ShaderGeneratorPtr generator = mx::WgslShaderGenerator::create();
    REQUIRE(generator->getTarget() == "genwgsl");
}

TEST_CASE("GenShader: WGSL Shader Generation", "[genwgsl]")
{
    mx::FileSearchPath searchPath = mx::getDefaultDataSearchPath();

    mx::DocumentPtr doc = mx::createDocument();
    mx::loadLibraries({ "libraries" }, searchPath, doc);

    // Load a representative lit surface material (standard_surface).
    mx::FilePath examplePath = searchPath.find("resources/Materials/Examples/StandardSurface/standard_surface_default.mtlx");
    REQUIRE(!examplePath.isEmpty());
    mx::readFromXmlFile(doc, examplePath);

    mx::ShaderGeneratorPtr generator = mx::WgslShaderGenerator::create();
    mx::GenContext context(generator);
    context.registerSourceCodeSearchPath(searchPath);
    context.getOptions().hwSpecularEnvironmentMethod = mx::SPECULAR_ENVIRONMENT_FIS;
    context.getOptions().hwMaxActiveLightSources = 1;

    std::vector<mx::TypedElementPtr> renderables = mx::findRenderableElements(doc);
    REQUIRE(!renderables.empty());

    size_t generated = 0;
    for (const mx::TypedElementPtr& element : renderables)
    {
        const std::string name = mx::createValidName(element->getNamePath());

        mx::ShaderPtr shader;
        REQUIRE_NOTHROW(shader = generator->generate(name, element, context));
        REQUIRE(shader != nullptr);

        const std::string& vertexCode = shader->getSourceCode(mx::Stage::VERTEX);
        const std::string& pixelCode = shader->getSourceCode(mx::Stage::PIXEL);

        REQUIRE(!vertexCode.empty());
        REQUIRE(!pixelCode.empty());
        REQUIRE(vertexCode.find("@vertex") != std::string::npos);
        REQUIRE(vertexCode.find("fn vertexMain") != std::string::npos);
        REQUIRE(pixelCode.find("@fragment") != std::string::npos);
        REQUIRE(pixelCode.find("fn fragmentMain") != std::string::npos);

        // Write the generated WGSL to files for manual inspection / external validation.
        std::ofstream(name + ".vertex.wgsl") << vertexCode;
        std::ofstream(name + ".pixel.wgsl") << pixelCode;
        generated++;
    }
    REQUIRE(generated > 0);
}

TEST_CASE("GenShader: WGSL Struct Uniform Layout", "[genwgsl]")
{
    mx::FileSearchPath searchPath = mx::getDefaultDataSearchPath();

    mx::DocumentPtr doc = mx::createDocument();
    mx::loadLibraries({ "libraries" }, searchPath, doc);

    mx::FilePath examplePath = searchPath.find("resources/Materials/Examples/StandardSurface/standard_surface_default.mtlx");
    REQUIRE(!examplePath.isEmpty());
    mx::readFromXmlFile(doc, examplePath);

    mx::ShaderGeneratorPtr generator = mx::WgslShaderGenerator::create();
    mx::GenContext context(generator);
    context.registerSourceCodeSearchPath(searchPath);
    context.getOptions().hwSpecularEnvironmentMethod = mx::SPECULAR_ENVIRONMENT_FIS;
    context.getOptions().hwMaxActiveLightSources = 1;

    // The WGSL generator forces UNIFORM_LAYOUT_STRUCT; verify the output.
    REQUIRE(context.getOptions().hwUniformLayout == mx::UNIFORM_LAYOUT_INDIVIDUAL);

    std::vector<mx::TypedElementPtr> renderables = mx::findRenderableElements(doc);
    REQUIRE(!renderables.empty());

    const std::string name = mx::createValidName(renderables[0]->getNamePath());
    mx::ShaderPtr shader = generator->generate(name, renderables[0], context);
    REQUIRE(shader != nullptr);

    // After generation, layout should have been promoted to STRUCT.
    REQUIRE(context.getOptions().hwUniformLayout == mx::UNIFORM_LAYOUT_STRUCT);

    const std::string& vertexCode = shader->getSourceCode(mx::Stage::VERTEX);
    const std::string& pixelCode = shader->getSourceCode(mx::Stage::PIXEL);

    // Vertex shader: struct definition and struct-qualified access.
    REQUIRE(vertexCode.find("struct PrivateUniforms") != std::string::npos);
    REQUIRE(vertexCode.find("u_prv.u_worldMatrix") != std::string::npos);
    REQUIRE(vertexCode.find("u_prv.u_viewProjectionMatrix") != std::string::npos);

    // Pixel shader: both private and public struct access.
    REQUIRE(pixelCode.find("struct PrivateUniforms") != std::string::npos);
    REQUIRE(pixelCode.find("struct PublicUniforms") != std::string::npos);
    REQUIRE(pixelCode.find("u_prv.u_envMatrix") != std::string::npos);
    REQUIRE(pixelCode.find("u_pub.SR_default_base") != std::string::npos);

    // No module-scope const aliases should be present.
    REQUIRE(vertexCode.find("const u_worldMatrix") == std::string::npos);
    REQUIRE(pixelCode.find("const u_envMatrix") == std::string::npos);
}
