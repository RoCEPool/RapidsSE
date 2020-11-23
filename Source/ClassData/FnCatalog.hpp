/*============================================================================*/
/* FnCatalog.hpp -- catalog of named function definitions                     */
/*============================================================================*/
#pragma once

namespace ClassData
{
  class FnCatalog
  {
  public:
    i8      m_nFunctions;   // number of named functions
    FnDefn  m_xFnDefn[128]; // array of named function definitions
  
  public:
    FnDefn* aFind(Stz* szFnName);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
