﻿//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXTest/Catch/catch.hpp>

#include <MaterialXCore/Document.h>
#include <MaterialXCore/UnitConverter.h>
#include <MaterialXFormat/File.h>
#include <MaterialXGenShader/TypeDesc.h>
#include <MaterialXGenShader/Util.h>

#include <cmath>

namespace mx = MaterialX;

const float EPSILON = 1e-4f;

TEST_CASE("UnitAttribute", "[units]")
{
    mx::DocumentPtr doc = mx::createDocument();
    mx::loadLibrary(mx::FilePath::getCurrentPath() / mx::FilePath("libraries/stdlib/stdlib_defs.mtlx"), doc);
    std::vector<mx::UnitTypeDefPtr> unitTypeDefs = doc->getUnitTypeDefs();
    REQUIRE(!unitTypeDefs.empty());

    mx::NodeGraphPtr nodeGraph = doc->addNodeGraph();

    mx::NodePtr constant = nodeGraph->addNode("constant");
    constant->setParameterValue("value", mx::Color3(0.5f));
    mx::ParameterPtr param = constant->getParameter("value");
    param->setUnitString("meter");
    REQUIRE(param->hasUnitString());
    REQUIRE(!param->getUnitString().empty());

    mx::OutputPtr output = nodeGraph->addOutput();
    output->setConnectedNode(constant);
    output->setUnitString("bad unit");
    REQUIRE(!output->validate());
    output->setUnitString("foot");
    REQUIRE(output->hasUnitString());
    REQUIRE(!output->getUnitString().empty());

    REQUIRE(doc->validate());
}

TEST_CASE("Length", "[units]")
{
    mx::DocumentPtr doc = mx::createDocument();
    mx::loadLibrary(mx::FilePath::getCurrentPath() / mx::FilePath("libraries/stdlib/stdlib_defs.mtlx"), doc);

    mx::UnitTypeDefPtr lengthTypeDef = doc->getUnitTypeDef(mx::LengthUnitConverter::LENGTH_UNIT);
    REQUIRE(lengthTypeDef);

    mx::UnitConverterPtr uconverter = mx::LengthUnitConverter::create(lengthTypeDef);
    REQUIRE(uconverter);
    doc->addUnitConverter(lengthTypeDef, uconverter);
    uconverter = doc->getUnitConverter(lengthTypeDef);
    REQUIRE(uconverter);

    mx::LengthUnitConverterPtr converter = std::dynamic_pointer_cast<mx::LengthUnitConverter>(uconverter);

    // Use converter to convert
    float result = converter->convert(0.1f, "kilometer", "millimeter");
    REQUIRE((result - 10000.0f) < EPSILON);
    result = converter->convert(1.0f, "meter", "meter");
    REQUIRE((result - 1.0f) < EPSILON);
    result = converter->convert(1.0f, "mile", "meter");
    REQUIRE((result - 0.000621f) < EPSILON);
    result = converter->convert(1.0f, "meter", "mile");
    REQUIRE((result - (1.0 / 0.000621f)) < EPSILON);

    // Use explicit converter values
    const std::unordered_map<std::string, float>& unitScale = converter->getUnitScale();
    result = 0.1f * unitScale.find("kilometer")->second / unitScale.find("millimeter")->second;
    REQUIRE((result - 10000.0f) < EPSILON);
    const std::string& defaultUnit = converter->getGefaultUnit();
    REQUIRE(defaultUnit == lengthTypeDef->getDefault());
}