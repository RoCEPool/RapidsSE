/*============================================================================*/
/* SubSysCatalog.hpp -- the subsystem catalog                                 */
/*============================================================================*/
#pragma once

namespace Resources
{
  class SubSysCatalog
  {
  public:

    i8 m_nSubSys;
    SubSysFn* m_paSubSysFn[CN_ENUMS];  // array of subsystem functions

  public:
    SubSysCatalog(void);
  
    void vCallSubSysFn(Stz* szSubSysNm);
  
  SubSysFn* pFindSubSysFn(Stz* szSubSysNm);
  
    void vListSubSystems(void);
  
    void vAddSubSysFn(SubSysFn* aSubSysFn);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
