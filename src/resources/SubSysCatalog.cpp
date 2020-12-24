/*============================================================================*/
/*SubSysCatalog.cpp -- the subsystem catalog                                  */
/*============================================================================*/

#include "Resources.h"

SubSysCatalog gxSubSysCatalog;

/*----------------------------------------------------------------------------*/
/* find prompt function given command                                         */
/*----------------------------------------------------------------------------*/
SubSysCatalog::SubSysCatalog(void)
{
}


/*----------------------------------------------------------------------------*/
/* find prompt function given command                                         */
/*----------------------------------------------------------------------------*/
void SubSysCatalog::vCallSubSysFn(Stz* szSubSysNm)
{
  SubSysFn* pSubSysFn = pFindSubSysFn(szSubSysNm);
  if (! pSubSysFn)
    return;
  
  pSubSysFn->m_fpSubSysFn(nullptr);
}

/*----------------------------------------------------------------------------*/
/* find prompt function given command                                         */
/*----------------------------------------------------------------------------*/
SubSysFn* SubSysCatalog::pFindSubSysFn(Stz* szSubSysNm)
{
  for (i8 i_SubSys = 0; i_SubSys < m_nSubSys; i_SubSys++)
    if (m_paSubSysFn[i_SubSys]->m_szSubSysNm->bSame(szSubSysNm))
      return m_paSubSysFn[i_SubSys];

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* help                                                                       */
/*----------------------------------------------------------------------------*/
void SubSysCatalog::vListSubSystems(void)
{
  for (i8 i_SubSys = 0; i_SubSys < m_nSubSys; i_SubSys++)
    vTrace("  %s\n"_Stz,m_paSubSysFn[i_SubSys]->m_szSubSysNm);
}

/*----------------------------------------------------------------------------*/
/* help                                                                       */
/*----------------------------------------------------------------------------*/
void SubSysCatalog::vAddSubSysFn(SubSysFn* aSubSysFn)
{
  m_paSubSysFn[m_nSubSys ++] = aSubSysFn;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
