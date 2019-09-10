/*============================================================================*/
/* OpCatalog.cpp -- the catalog of op definitions                             */
/*============================================================================*/

#include "Operators.h"
#include "AM.hpp"

/*----------------------------------------------------------------------------*/
/* find op definition given command                                           */
/*----------------------------------------------------------------------------*/
OpDefn* OpCatalog::aFindOp(Stz* szCommand)
{
  for (i8 i_OpDefn = 0; i_OpDefn < m_nOpDefns; i_OpDefn++) {
    
//    vTrace("Find: %s v %s\n"_Stz,
//           m_xOpDefn[i_OpDefn].m_szCommand,
//           szCommand);
    
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

/*----------------------------------------------------------------------------*/
/* print catalog                                                              */
/*----------------------------------------------------------------------------*/
void OpCatalog::vPrintCatalog(void)
{
  
  for (i8 i_OpDefn = 0; i_OpDefn < m_nOpDefns; i_OpDefn++) {
    vPrintDefn(& m_xOpDefn[i_OpDefn]);
        
//    vTrace(sFormat.aStz(),
//
//           m_xOpDefn[i_OpDefn].m_szCommand,
//           aCN[m_xOpDefn[i_OpDefn].m_eClass],
//           aOK[m_xOpDefn[i_OpDefn].m_eKind],
//
//           m_xOpDefn[i_OpDefn].m_bIsPartitioned ? ""_Stz : "not-"_Stz,
//
//           m_xOpDefn[i_OpDefn].m_szHelp,
//
//           C_ENDARGS);
  }
}

/*----------------------------------------------------------------------------*/
/* print catalog                                                              */
/*----------------------------------------------------------------------------*/
void OpCatalog::vPrintDefn(OpDefn* aOpDefn)
{
  vTrace("%12s %12s %6s %spartitioned %s\n"_Stz,
         
         aOpDefn->m_szCommand,
         aCN[aOpDefn->m_eClass],
         aOK[aOpDefn->m_eKind],
         
         aOpDefn->m_bIsPartitioned ? ""_Stz : "not-"_Stz,
         
         aOpDefn->m_szHelp,
         
         C_ENDARGS);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
