/*============================================================================*/
/* SelfCatalog.cpp -- System-Self Function Catalog                            */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/* find prompt function given command                                         */
/*----------------------------------------------------------------------------*/
SelfCatalog::SelfCatalog(void)
{
  _nonLocalStatic_ // memory is automatically zero'd
}


/*----------------------------------------------------------------------------*/
/* help                                                                       */
/*----------------------------------------------------------------------------*/
void SelfCatalog::vAddFn(SelfFn* aSelfFn)
{
  _nonProper_

  m_paSelfFn[m_nSelf ++] = aSelfFn;
}

/*----------------------------------------------------------------------------*/
/* find prompt function given command                                         */
/*----------------------------------------------------------------------------*/
bool SelfCatalog::bCallFn(Stz* szSelfNm, TokenVec* pTokenVec)
{
  _nonProper_

  SelfFn* pSelfFn = pFindFn(szSelfNm);
  if (! pSelfFn)
    return false;
  
  pSelfFn->m_fpSelfFn(pTokenVec);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* find prompt function given command                                         */
/*----------------------------------------------------------------------------*/
SelfFn* SelfCatalog::pFindFn(Stz* szSelfNm)
{
  _nonProper_

  for (i8 i_Self = 0; i_Self < m_nSelf; i_Self++)
    if (m_paSelfFn[i_Self]->m_szSelfNm->bSame(szSelfNm))
      return m_paSelfFn[i_Self];

  return nullptr;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* find prompt function given command                                         */
/*----------------------------------------------------------------------------*/
void SelfCatalog::vDescribeRecursively(Stz* szSelfNm, i8 iDepth)
{
  _nonProper_

  SelfFn* pSelfFn = pFindFn(szSelfNm);
  if (! pSelfFn)
    return;

  vTrace("%s  %s\n"_Stz,Stz::szTab(iDepth),pSelfFn->m_szSelfNm);

  for (i8 i_SeeAlso = 0; i_SeeAlso < pSelfFn->m_nSeeAlso; i_SeeAlso++) {
    Stz* szSeeAlso = pSelfFn->m_szaSeeAlso[i_SeeAlso];
    vDescribeRecursively(szSeeAlso,iDepth+1);
  }
  
}


/*----------------------------------------------------------------------------*/
/* help                                                                       */
/*----------------------------------------------------------------------------*/
void SelfCatalog::vList(void)
{
  _nonProper_

  for (i8 i_Self = 0; i_Self < m_nSelf; i_Self++)
    vTrace("  %s\n"_Stz,m_paSelfFn[i_Self]->m_szSelfNm);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
