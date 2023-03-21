/*============================================================================*/
/* CompileLib.hpp - Dynamic code compiler and loader                          */
/*============================================================================*/

#include "BASE.h"
#include "CompileLib.hpp"
#include "LibInfo.hpp"

using namespace STORE;

/*----------------------------------------hy------------------------------------*/
/* Initialise the CompileLib and look for all supported compilers.            */
/*----------------------------------------------------------------------------*/
void CompileLib::vInit()
{

}

// TODO: list known compilers


/*----------------------------------------------------------------------------*/
/* Compiler source code to a dynamic library. Returns true on success.        */
/*----------------------------------------------------------------------------*/
bool CompileLib::bCompile(Stz* szSource, Stz* szDynLibNm, LibInfo* pLibInfo)
{

}

/*----------------------------------------------------------------------------*/
/* Dynamically load a shared object library. Returns true on success.         */
/*----------------------------------------------------------------------------*/
bool CompileLib::bDynamicLink(LibInfo* pLibInfo)
{

}

/*----------------------------------------------------------------------------*/
/* Dynamically unload a shared object library. Returns true on success.       */
/*----------------------------------------------------------------------------*/
bool CompileLib::bDynamicUnlink(LibInfo* pLibInfo)
{

}

/*----------------------------------------------------------------------------*/
/* Delete a shared object library. Returns true on success.                   */
/*----------------------------------------------------------------------------*/
bool CompileLib::bDelete(LibInfo* pLibInfo)
{

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/