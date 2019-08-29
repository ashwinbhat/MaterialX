//
// TM & (c) 2017 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXFormat/Environ.h>
#include <MaterialXFormat/File.h>
#include <MaterialXFormat/XmlIo.h>

#include <MaterialXTest/Catch/catch.hpp>

#include <iostream>

namespace mx = MaterialX;

TEST_CASE("Units", "[units]")
{
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
            if (elem->isA<mx::Input>())
            {
                std::cout << "Input: " << elem->getName() << std::endl;
                for (const std::string& attrName : elem->getAttributeNames()) {
                    std::cout << "\t Attributes: " << attrName << std::endl;
                }
                //std::cout << "\t Attributes: " << elem->getAttribute() << std::endl;
                valueElementCount++;
            }

            if (elem->isA<mx::Parameter>())
            {
                std::cout << "Input: " << elem->getName() << std::endl;
                for (const std::string& attrName : elem->getAttributeNames()) {
                    std::cout << "\t Attributes: " << attrName << std::endl;
                }
                //std::cout << "\t Attributes: " << elem->getAttribute() << std::endl;
                valueElementCount++;
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

