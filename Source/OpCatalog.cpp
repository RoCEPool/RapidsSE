/*============================================================================*/
/* OpCatalog.cpp -- the catalog of op definitions                             */
/*============================================================================*/

#include "PushDown.h"
#include "AM.hpp"

/*----------------------------------------------------------------------------*/
/* find op definition given command                                           */
/*----------------------------------------------------------------------------*/
OpDefn* OpCatalog::pFindOp(Stz* szCommand)
{
  for (i8 i_OpDefn = 0; i_OpDefn < m_nOpDefns; i_OpDefn++) {
    
    vTrace("Find: %s v %s\n"_Stz,
           m_xOpDefn[i_OpDefn].m_szCommand,
           szCommand);
    
    if (! m_xOpDefn[i_OpDefn].m_szCommand->bSame(szCommand))
      continue;
    
    return &m_xOpDefn[i_OpDefn];
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* help                                                                       */
/*----------------------------------------------------------------------------*/
void OpCatalog::vHelp(eCN eClass)
{
  for (i8 i_OpDefn = 0; i_OpDefn < m_nOpDefns; i_OpDefn++) {
    
    if (m_xOpDefn[i_OpDefn].m_eClass != eClass)
      continue;
    
    if (m_xOpDefn[i_OpDefn].m_szHelp)
      vTrace("%8s %s\n"_Stz, m_xOpDefn[i_OpDefn].m_szCommand,
                            m_xOpDefn[i_OpDefn].m_szHelp);
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
