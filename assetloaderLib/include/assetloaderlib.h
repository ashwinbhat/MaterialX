
#ifndef assetloaderlib_EXPORT_H
#define assetloaderlib_EXPORT_H

#ifdef assetloaderlib_STATIC
#  define assetloaderlib_EXPORT
#  define ASSETLOADERLIB_NO_EXPORT
#else
#  ifndef assetloaderlib_EXPORT
#    ifdef assetloader_EXPORTS
        /* We are building this library */
#      define assetloaderlib_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define assetloaderlib_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef ASSETLOADERLIB_NO_EXPORT
#    define ASSETLOADERLIB_NO_EXPORT 
#  endif
#endif

#ifndef ASSETLOADERLIB_DEPRECATED
#  define ASSETLOADERLIB_DEPRECATED __declspec(deprecated)
#endif

#ifndef ASSETLOADERLIB_DEPRECATED_EXPORT
#  define ASSETLOADERLIB_DEPRECATED_EXPORT assetloaderlib_EXPORT ASSETLOADERLIB_DEPRECATED
#endif

#ifndef ASSETLOADERLIB_DEPRECATED_NO_EXPORT
#  define ASSETLOADERLIB_DEPRECATED_NO_EXPORT ASSETLOADERLIB_NO_EXPORT ASSETLOADERLIB_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef ASSETLOADERLIB_NO_DEPRECATED
#    define ASSETLOADERLIB_NO_DEPRECATED
#  endif
#endif

#endif /* assetloaderlib_EXPORT_H */
