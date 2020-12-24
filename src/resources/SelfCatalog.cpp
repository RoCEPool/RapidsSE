/*============================================================================*/
/* SelfCatalog.cpp -- System-Self Function Catalog                            */
/*============================================================================*/

#include "Resources.h"

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
bool SelfCatalog::bCallSelfFn(Stz* szSelfNm, void* aSelfArg)
{
  SelfFn* pSelfFn = pFindSelfFn(szSelfNm);
  if (! pSelfFn)
    return false;
  
  pSelfFn->m_fpSelfFn(aSelfArg);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* find prompt function given command                                         */
/*----------------------------------------------------------------------------*/
SelfFn* SelfCatalog::pFindSelfFn(Stz* szSelfNm)
{
  for (i8 i_Self = 0; i_Self < m_nSelf; i_Self++)
    if (m_paSelfFn[i_Self]->m_szSelfNm->bSame(szSelfNm))
      return m_paSelfFn[i_Self];

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* help                                                                       */
/*----------------------------------------------------------------------------*/
void SelfCatalog::vListSelftems(void)
{
  for (i8 i_Self = 0; i_Self < m_nSelf; i_Self++)
    vTrace("  %s\n"_Stz,m_paSelfFn[i_Self]->m_szSelfNm);
}

/*----------------------------------------------------------------------------*/
/* help                                                                       */
/*----------------------------------------------------------------------------*/
void SelfCatalog::vAddSelfFn(SelfFn* aSelfFn)
{
  m_paSelfFn[m_nSelf ++] = aSelfFn;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
