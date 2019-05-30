
// auto-generated header
#include <assetloaderlib.h>

// MaterialX Includes
#include <MaterialXCore/Document.h>

class assetloaderlib_EXPORT assetloader
{
  public:
    assetloader();
    //virtual void load_asset(std::string libraryPath);
    virtual bool load_asset(std::string libraryPath, std::string preset,
        MaterialX::Color4& surface_albedo, MaterialX::Color4& metal_f0, float& surface_roughness);

    MaterialX::MaterialPtr convertPrismMetalToStandardSurface(MaterialX::DocumentPtr document, std::string materialName,
        MaterialX::Color4 surface_albedo, MaterialX::Color4 metal_f0, float surface_roughness);

};