/*============================================================================*/
/* SelfCatalog.cpp -- System-Self Function Catalog                            */
/*============================================================================*/

#include "Base.h"

SelfCatalog gxSelfCatalog;

/*----------------------------------------------------------------------------*/
/* find prompt function given command                                         */
/*----------------------------------------------------------------------------*/
SelfCatalog::SelfCatalog(void)
{
}


/*----------------------------------------------------------------------------*/
/* find prompt function given command                                         */
/*----------------------------------------------------------------------------*/
bool SelfCatalog::bCallFn(Stz* szSelfNm, TokenVec* pTokenVec)
{
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
  for (i8 i_Self = 0; i_Self < m_nSelf; i_Self++)
    if (m_paSelfFn[i_Self]->m_szSelfNm->bSame(szSelfNm))
      return m_paSelfFn[i_Self];

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* help                                                                       */
/*----------------------------------------------------------------------------*/
void SelfCatalog::vListFn(void)
{
  for (i8 i_Self = 0; i_Self < m_nSelf; i_Self++)
    vTrace("  %s\n"_Stz,m_paSelfFn[i_Self]->m_szSelfNm);
}

/*----------------------------------------------------------------------------*/
/* help                                                                       */
/*----------------------------------------------------------------------------*/
void SelfCatalog::vAddFn(SelfFn* aSelfFn)
{
  m_paSelfFn[m_nSelf ++] = aSelfFn;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
