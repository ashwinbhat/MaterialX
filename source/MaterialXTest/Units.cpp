//
// TM & (c) 2017 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXFormat/Environ.h>
#include <MaterialXFormat/File.h>
#include <MaterialXFormat/XmlIo.h>

#include <MaterialXCore/Units.h>
#include <MaterialXGenShader/TypeDesc.h>
#include <MaterialXTest/Catch/catch.hpp>

#include <iostream>

namespace mx = MaterialX;

TEST_CASE("Units Test", "[unitstesting]")
{
    mx::Units::EUnit sceneUnit = mx::Units::E_UNIT_FOOT;

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
                                float cval = mx::Units::convertUnit(val, mx::Units::E_UNIT_M, sceneUnit);
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

                            mx::Units::EUnit valUnit = mx::Units::toUnit(param->getUnit());

                            if (type->isScalar() && value)
                            {
                                float val = value->asA<float>();
                                float cval = mx::Units::convertUnit(val, mx::Units::E_UNIT_M, sceneUnit);
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
