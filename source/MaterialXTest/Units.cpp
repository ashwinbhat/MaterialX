//
// TM & (c) 2017 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXTest/Catch/catch.hpp>

#include <MaterialXFormat/Environ.h>
#include <MaterialXCore/Document.h>
#include <MaterialXCore/Units.h>
#include <MaterialXFormat/File.h>
#include <MaterialXFormat/XmlIo.h>
#include <MaterialXGenShader/Util.h>
#include <MaterialXGenShader/TypeDesc.h>
#include <MaterialXGenShader/Units.h>

#include <iostream>
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

    mx::UnitTypeDefPtr lengthTypeDef = doc->getUnitTypeDef("length");
    REQUIRE(lengthTypeDef);

    mx::UnitConverterPtr converter = mx::LengthUnitConverter::create(lengthTypeDef);
    REQUIRE(converter);
    doc->addUnitConverter(lengthTypeDef, converter);
    converter = doc->getUnitConverter(lengthTypeDef);
    REQUIRE(converter);

    float result = converter->convert(0.1f, "kilometer", "millimeter");
    REQUIRE((result - 10000.0f) < EPSILON);
    result = converter->convert(1.0f, "meter", "meter");
    REQUIRE((result - 1.0f) < EPSILON);
    result = converter->convert(1.0f, "mile", "meter");
    REQUIRE((result - 0.000621f) < EPSILON);
    result = converter->convert(1.0f, "meter", "mile");
    REQUIRE((result - (1.0 / 0.000621f)) < EPSILON);
}

TEST_CASE("Units Test", "[unitstesting]")
{
    mx::Units::DistanceUnit sceneUnit = mx::Units::UNITTYPE_FOOT;

    bool _needsValidation = false;
    mx::FilePath libraryPath("libraries/stdlib");
    mx::FilePath examplesPath("resources/Materials/Examples/Units");
    std::string searchPath = libraryPath.asString() +
        mx::PATH_LIST_SEPARATOR +
        examplesPath.asString();

    // Read the standard library.
    std::vector<mx::DocumentPtr> libs;
    for (std::string filename : libraryPath.getFilesInDirectory(mx::MTLX_EXTENSION))
    {
        mx::DocumentPtr lib = mx::createDocument();
        mx::readFromXmlFile(lib, filename, searchPath);
        libs.push_back(lib);
    }

    // Read and validate each example document.
    for (std::string filename : examplesPath.getFilesInDirectory(mx::MTLX_EXTENSION))
    {
        mx::DocumentPtr doc = mx::createDocument();
        mx::readFromXmlFile(doc, filename, searchPath);

        if (_needsValidation)
        {
            for (mx::DocumentPtr lib : libs)
            {
                doc->importLibrary(lib);
            }
            std::string message;
            bool docValid = doc->validate(&message);
            if (!docValid)
            {
                WARN("[" + filename + "] " + message);
            }
            REQUIRE(docValid);
        }

        // Traverse the document tree
        int valueElementCount = 0;
        for (mx::ElementPtr elem : doc->traverseTree())
        {
            // If we have nodes with inputs
            mx::NodePtr pNode = elem->asA<mx::Node>();
            if (pNode) 
            {
                std::string category = pNode->getCategory();
                std::cout << "category:" << category << std::endl;

                if (pNode->getInputCount()) {
                    for (mx::InputPtr input : pNode->getInputs()) {
                        const mx::TypeDesc* type = mx::TypeDesc::get(input->getType());
                        const mx::ValuePtr value = input->getValue();
                        std::string value_string = value ? value->getValueString() : "No value ";

                        std::cout << "input_name: " << input->getName() << std::endl
                                  << "input_type: " << type->getName() << std::endl
                                  << "input_value:" << value_string << std::endl;

                        if (input->hasUnit()) {
                            std::cout << "input_unit_type: " << input->getUnit() << std::endl;

                            if (type->isScalar() && value)
                            {
                                float val = value->asA<float>();
                                float cval = (float) mx::Units::convertUnit(val, mx::Units::UNITTYPE_M, sceneUnit);
                                std::cout << "converted_value:" << cval << std::endl;
                            }
                        }
                    }
                }
            
                if (pNode->getParameterCount()) {
                    for (mx::ParameterPtr param: pNode->getParameters()) {
                        const mx::TypeDesc* type = mx::TypeDesc::get(param->getType());
                        const mx::ValuePtr value = param->getValue();
                        std::string value_string = value ? value->getValueString() : "No value ";

                        std::cout << "param_name: " << param->getName() << std::endl
                            << "param_type: " << type->getName() << std::endl
                            << "param_value: " << param->getValueString() << std::endl;

                        if (param->hasUnit()) {
                            std::cout << "param_unit_type: " << param->getUnit() << std::endl;

                            mx::Units::DistanceUnit valUnit = mx::Units::toUnit(param->getUnit());

                            if (type->isScalar() && value)
                            {
                                float val = value->asA<float>();
                                float cval = (float)mx::Units::convertUnit(val, mx::Units::UNITTYPE_M, sceneUnit);
                                std::cout << "From: " << mx::Units::unitName(valUnit) << std::endl
                                    << "To: " << mx::Units::unitName(sceneUnit) << std::endl
                                    << "converted_value: " << cval << std::endl;
                            }
                        }
                    }
                }
            }
        }
        REQUIRE(valueElementCount > 0);

        // Traverse the dataflow graph from each shader parameter and input
        // to its source nodes.
        for (mx::MaterialPtr material : doc->getMaterials())
        {
            REQUIRE(material->getPrimaryShaderNodeDef());
            int edgeCount = 0;
            for (mx::ParameterPtr param : material->getPrimaryShaderParameters())
            {
                REQUIRE(param->getBoundValue(material));
                for (mx::Edge edge : param->traverseGraph(material))
                {
                    edgeCount++;
                }
            }
            for (mx::InputPtr input : material->getPrimaryShaderInputs())
            {
                REQUIRE((input->getBoundValue(material) || input->getUpstreamElement(material)));
                for (mx::Edge edge : input->traverseGraph(material))
                {
                    edgeCount++;
                }
            }
            REQUIRE(edgeCount > 0);
        }
    }
}
