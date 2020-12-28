/*============================================================================*/
/* SubSysCatalog.hpp -- the subsystem catalog                                 */
/*============================================================================*/
#pragma once

namespace BASE
{
  class SubSysCatalog : public SelfCatalog
  {
  public:

  public:
    SubSysCatalog(void);
  
    static void vDescribe(SelfFn* aSelfFn, TokenVec* pTokenVec);
    static void vSubSystems(SelfFn* aSelfFn, TokenVec* pTokenVec);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
