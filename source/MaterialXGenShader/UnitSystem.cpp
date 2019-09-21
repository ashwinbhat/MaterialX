//
// TM & (c) 2017 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXGenShader/UnitSystem.h>

#include <MaterialXCore/Units.h>
#include <MaterialXGenShader/GenContext.h>
#include <MaterialXGenShader/ShaderGenerator.h>
#include <MaterialXGenShader/Nodes/SourceCodeNode.h>

namespace MaterialX
{
//
// Unit transform methods
//

UnitTransform::UnitTransform(const string& ss, const string& ts, const TypeDesc* t) :
    sourceUnit(ss),
    targetUnit(ts),
    type(t)
{
    if (type != Type::FLOAT && type != Type::VECTOR2 && type != Type::VECTOR3 && type != Type::VECTOR4)
    {
        throw ExceptionShaderGenError("Unit space transform can only be a float or vectors");
    }
}


UnitSystem::UnitSystem()
{
}

void UnitSystem::loadLibrary(DocumentPtr document)
{
    _document = document;
}

bool UnitSystem::supportsTransform(const UnitTransform& transform) const
{
    UnitTypeDefPtr lengthTypeDef = _document->getUnitTypeDef("length");
    const string implName = getImplementationName(transform);
    ImplementationPtr impl = _document->getImplementation(implName);
    return impl != nullptr;
}

ShaderNodePtr UnitSystem::createNode(const ShaderGraph* parent, const UnitTransform& transform, const string& name,
    GenContext& context) const
{
    const string implName = getImplementationName(transform);
    ImplementationPtr impl = _document->getImplementation(implName);
    if (!impl)
    {
        throw ExceptionShaderGenError("No implementation found for transform: ('" + transform.sourceUnit + "', '" + transform.targetUnit + "').");
    }

    // Check if it's created and cached already,
    // otherwise create and cache it.
    ShaderNodeImplPtr nodeImpl = context.findNodeImplementation(implName);
    if (!nodeImpl)
    {
        nodeImpl = SourceCodeNode::create();
        nodeImpl->initialize(*impl, context);
        context.addNodeImplementation(implName, nodeImpl);
    }

    // Create the node.
    ShaderNodePtr shaderNode = ShaderNode::create(parent, name, nodeImpl, ShaderNode::Classification::TEXTURE);

    // Create ports on the node.
    ShaderInputPtr input(shaderNode->addInput("in", transform.type));
    if (transform.type == Type::FLOAT)
    {
        input->setValue(Value::createValue(1.0));
    }
    else if (transform.type == Type::VECTOR2)
    {
        input->setValue(Value::createValue(Vector2(1.0f, 1.0)));
    }
    else if (transform.type == Type::VECTOR3)
    {
        input->setValue(Value::createValue(Vector3(1.0f, 1.0, 1.0)));
    }
    else if (transform.type == Type::VECTOR4)
    {
        input->setValue(Value::createValue(Vector4(1.0f, 1.0, 1.0, 1.0)));
    }
    else
    {
        throw ExceptionShaderGenError("Invalid type specified to createColorTransform: '" + transform.type->getName() + "'");
    }

    // Add the conversion code
    ShaderInputPtr convertFrom(shaderNode->addInput("unit_from", Type::INTEGER));
    int from = transform.encodeSourceUnit();
    convertFrom->setValue(Value::createValue(from));

    ShaderInputPtr convertTo(shaderNode->addInput("unit_to", Type::INTEGER));
    int to = transform.encodeTargetUnit();
    convertTo->setValue(Value::createValue(to));

    shaderNode->addOutput("out", transform.type);

    return shaderNode;
}

} // namespace MaterialX
