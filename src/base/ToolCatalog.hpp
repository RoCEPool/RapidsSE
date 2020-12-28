/*============================================================================*/
/* ToolCatalog.hpp -- Tool Command Catalog                                    */
/*============================================================================*/
#pragma once

namespace BASE
{
  class ToolCatalog : public SelfCatalog
  {
  public:

  public:
    ToolCatalog(void);
  
    static void vTools(SelfFn* aSelfFn, TokenVec* pTokenVec);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
