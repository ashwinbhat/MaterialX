//**************************************************************************/
// Copyright 2019 Autodesk, Inc.
// All rights reserved.
// 
// Based on OGS Devel\Protein\ProteinAssetTranslator\Source\NativeAssetAdapter.h/.cpp 

#ifndef _MATERIALX_ASSETADAPTER_H_
#define _MATERIALX_ASSETADAPTER_H_

#include <MaterialXCore/Types.h>

// Forward declarations.
namespace autodesk { namespace platform { namespace assets
{
    class IAssetInstance;
} } }

namespace autodesk {
    namespace platform {
        namespace interop
        {
            /// <description>
            /// Implementation of IAssetAdapter for Protein library assets.
            /// </description>
            class MaterialXAssetAdapter //: public IAssetAdapter
            {
            public:
                /// <description>
                /// Constructor, accepting the underlying Protein asset instance and an optional flag indicating
                /// whether the adapter should manage the lifetime of the asset instance.
                /// </description>
                MaterialXAssetAdapter(autodesk::platform::assets::IAssetInstance& asset, bool manage = false);

                /// <description>
                /// Destructor.
                /// </description>
                virtual ~MaterialXAssetAdapter();

                /// <description>
                /// Implements IAssetAdapter.HasProperty().
                /// </description>
                virtual bool HasProperty(const char* name) const;

                /// <description>
                /// Implements IAssetAdapter.GetPropBool().
                /// </description>
                virtual bool GetPropBool(const char* name, bool& value) const;

                /// <description>
                /// Implements IAssetAdapter.GetPropInt().
                /// </description>
                virtual bool GetPropInt(const char* name, int& value) const;

                /// <description>
                /// Implements IAssetAdapter.GetPropIntArray().
                /// </description>
                //virtual bool GetPropIntArray(const char* name, Objects::_ArrayList<int>& value) const;

                /// <description>
                /// Implements IAssetAdapter.GetPropFloat().
                /// </description>
                virtual bool GetPropFloat(const char* name, float& value) const;

                /// <description>
                /// Implements IAssetAdapter.GetPropFloatArray().
                /// </description>
                //virtual bool GetPropFloatArray(const char* name, Objects::_ArrayList<float>& value) const;

                /// <description>
                /// Implements IAssetAdapter.GetPropFloat3().
                /// </description>
                //virtual bool GetPropFloat3(const char* name, MaterialX::Vector4& value) const;

                /// <description>
                /// Implements IAssetAdapter.GetPropColor().
                /// </description>
                virtual bool GetPropColor(const char* name, MaterialX::Color4& value, bool linearizeSRGB = false) const;

                /// <description>
                /// Implements IAssetAdapter.GetPropColorArray().
                /// </description>
                //virtual bool GetPropColorArray(
                //    const char* name, std::vector<MaterialX::Vector4>& value, bool linearizeSRGB = false) const;

                /// <description>
                /// Implements IAssetAdapter.GetPropDistance().
                /// </description>
                //virtual bool GetPropDistance(const char* name, float& value,
                //    autodesk::platform::data::units::tUnit& unit) const;

                /// <description>
                /// Implements IAssetAdapter.GetPropURI().
                /// </description>
                //virtual bool GetPropURI(const char* name, MaterialX::string& value) const;

                /// <description>
                /// Implements IAssetAdapter.GetBitmapURI().
                /// </description>
                //virtual bool GetBitmapURI(FBX::ETextureResolution res, MaterialX::string& value) const;

                /// <description>
                /// Implements IAssetAdapter.GetSubAsset().
                /// </description>
                //virtual bool GetSubAsset(
                //    const char* name, std::shared_ptr<IAssetAdapter>& value, const char* type = NULL) const;

                /// <description>
                /// Get a nested asset from the asset by id.
                /// </description>
                //virtual bool GetSubAsset(
                //    const char* name, Objects::AutoPtr<IAssetAdapter>& value, size_t id) const;

                /// <description>
                /// Get a nested asset count from the asset.
                /// </description>
                virtual bool GetSubAssetCount(const char* name, size_t& count) const;

                /// <description>
                /// Implements IAssetAdapter.GetUIName().
                /// </description>
                //virtual bool GetUIName(MaterialX::string& value) const;

                /// <description>
                /// Implements IAssetAdapter.GetImplementation().
                /// </description>
                //virtual bool GetDefinition(MaterialX::string& value) const;

            private:
                autodesk::platform::assets::IAssetInstance* mpAsset;
                bool mManaged;
            };

        }
    }
} // namespace autodesk::platform::interop

#endif // _MATERIALX_ASSETADAPTER_H_
