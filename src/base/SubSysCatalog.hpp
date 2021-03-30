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
  
    static void vDescribe(TokenVec* pTokenVec);
    static void vSubSystems(TokenVec* pTokenVec);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
