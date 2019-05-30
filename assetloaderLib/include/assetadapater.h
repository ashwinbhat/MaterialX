//**************************************************************************/
// Copyright 2019 Autodesk, Inc.
// All rights reserved.
// 
// Based on OGS\SDK\Include\internal\OGS\fbx\AssetAdapter.h
#ifndef _ASSETADAPTER_H_
#define _ASSETADAPTER_H_

#if 0
namespace autodesk {
  namespace platform {
      namespace interop {
          /// <description>
          /// An interface for wrapping an asset, allowing access to its properties and other features.
          /// </description>
          class IAssetAdapter
          {
          public:
              /// <description>
              /// Destructor.
              /// </description>
              virtual ~IAssetAdapter() {}

              /// <description>
              /// Returns whether a property with the specified name exists on the asset.
              /// </description>
              virtual bool HasProperty(const char* name) const = 0;

              /// <description>
              /// Gets the value of a bool property from the asset.
              /// </description>
              virtual bool GetPropBool(const char* name, bool& value) const = 0;

              /// <description>
              /// Gets the value of an int property from the asset.
              /// </description>
              virtual bool GetPropInt(const char* name, int& value) const = 0;

              /// <description>
              /// Gets the value of an int array property from the asset.
              /// </description>
              virtual bool GetPropIntArray(const char* name, Objects::_ArrayList<int>& value) const = 0;

              /// <description>
              /// Gets the value of a float property from the asset.
              /// </description>
              virtual bool GetPropFloat(const char* name, float& value) const = 0;

              /// <description>
              /// Gets the value of a float array property from the asset.
              /// </description>
              virtual bool GetPropFloatArray(const char* name, Objects::_ArrayList<float>& value) const = 0;

              /// <description>
              /// Gets the value of a float3 property from the asset.
              /// </description>
              virtual bool GetPropFloat3(const char* name, float3& value) const = 0;

              /// <description>
              /// Gets the value of a color property from the asset.
              /// </description>
              virtual bool GetPropColor(const char* name, float4& value, bool linearizeSRGB = false) const = 0;

              /// <description>
              /// Gets the value of a color array property from the asset.
              /// </description>
              virtual bool GetPropColorArray(
                  const char* name, Objects::_ArrayList<float4>& value, bool linearizeSRGB = false) const = 0;

              /// <description>
              /// Gets the value of a distance property from the asset.
              /// </description>
              virtual bool GetPropDistance(const char* name, float& value,
                  autodesk::platform::data::units::tUnit& unit) const = 0;

              /// <description>
              /// Gets the resolved value of a path property from the asset.
              /// </description>
              virtual bool GetPropURI(const char* name, Objects::KString& value) const = 0;

              /// <description>
              /// Gets the resolved path of the bitmap asset at the specified resolution.
              /// </description>
              /// <remarks>
              // This function only works with bitmaps, which have the "unifiedbitmap_Bitmap" property.
              // Unlike the more generic GetPropURI() function above, this gets a URI for a specific texture
              // resolution, e.g. "very high" resolution.
              /// </remarks>
              virtual bool GetBitmapURI(FBX::ETextureResolution res, Objects::KString& value) const = 0;

              /// <description>
              /// Get a nested asset from the asset.
              /// </description>
              virtual bool GetSubAsset(
                  const char* name, Objects::AutoPtr<IAssetAdapter>& value, const char* type = NULL) const = 0;

              /// <description>
              /// Get a nested asset from the asset by id.
              /// </description>
              virtual bool GetSubAsset(
                  const char* name, Objects::AutoPtr<IAssetAdapter>& value, size_t id) const = 0;

              /// <description>
              /// Get a nested asset count from the asset.
              /// </description>
              virtual bool GetSubAssetCount(const char* name, size_t& count) const = 0;

              /// <description>
              /// Gets the asset's UI name.
              /// </description>
              virtual bool GetUIName(Objects::KStringA& value) const = 0;

              /// <description>
              /// Gets the asset's implementation.
              /// </description>
              virtual bool GetDefinition(Objects::KStringA& value) const = 0;

          };
      }
    } // na
} // namespace autodesk::platform::interop
#endif
#endif // _ASSETADAPTER_H_
