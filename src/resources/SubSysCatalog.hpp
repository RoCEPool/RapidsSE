/*============================================================================*/
/* SubSysCatalog.hpp -- the subsystem catalog                                 */
/*============================================================================*/
#pragma once

namespace Resources
{
  class SubSysCatalog : public SelfCatalog
  {
  public:

    i8 m_nSubSys;
    SubSysFn* m_paSubSysFn[CN_ENUMS];  // array of subsystem functions

  public:
    SubSysCatalog(void);
  
    bool bCallSubSysFn(Stz* szSubSysNm, void* aSubSysArg);
  
    SubSysFn* pFindSubSysFn(Stz* szSubSysNm);
  
    void vListSubSystems(void);
  
    void vAddSubSysFn(SubSysFn* aSubSysFn);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
