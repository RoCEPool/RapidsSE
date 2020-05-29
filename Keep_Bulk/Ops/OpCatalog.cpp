/*============================================================================*/
/* OpCatalog.cpp -- the catalog of op definitions                             */
/*============================================================================*/

#include "Pipelines.h"
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
  
  for (i8 i_OpDefn = 0; i_OpDefn < m_nOpDefns; i_OpDefn++)
    if (OK_SOURCE == m_xOpDefn[i_OpDefn].m_eKind)
      vPrintDefn(& m_xOpDefn[i_OpDefn]);
  
  vTrace("\n"_Stz);

  for (i8 i_OpDefn = 0; i_OpDefn < m_nOpDefns; i_OpDefn++)
    if (m_xOpDefn[i_OpDefn].m_bIsPartitioned && OK_SINGLE == m_xOpDefn[i_OpDefn].m_eKind)
      vPrintDefn(& m_xOpDefn[i_OpDefn]);
  
  vTrace("\n"_Stz);
  
  for (i8 i_OpDefn = 0; i_OpDefn < m_nOpDefns; i_OpDefn++)
    if (m_xOpDefn[i_OpDefn].m_bIsPartitioned && OK_DUPLEX == m_xOpDefn[i_OpDefn].m_eKind)
      vPrintDefn(& m_xOpDefn[i_OpDefn]);
  
  vTrace("\n"_Stz);
  
  for (i8 i_OpDefn = 0; i_OpDefn < m_nOpDefns; i_OpDefn++)
    if (! m_xOpDefn[i_OpDefn].m_bCompilerOp    &&
        ! m_xOpDefn[i_OpDefn].m_bIsPartitioned && OK_SINGLE == m_xOpDefn[i_OpDefn].m_eKind)
      vPrintDefn(& m_xOpDefn[i_OpDefn]);
  
  vTrace("\n"_Stz);
  
//  for (i8 i_OpDefn = 0; i_OpDefn < m_nOpDefns; i_OpDefn++)
//    if (! m_xOpDefn[i_OpDefn].m_bIsPartitioned && OK_DUPLEX == m_xOpDefn[i_OpDefn].m_eKind)
//      vPrintDefn(& m_xOpDefn[i_OpDefn]);
//  
//  vTrace("\n"_Stz);
  
  for (i8 i_OpDefn = 0; i_OpDefn < m_nOpDefns; i_OpDefn++)
    if (OK_SINK == m_xOpDefn[i_OpDefn].m_eKind)
      vPrintDefn(& m_xOpDefn[i_OpDefn]);
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* print catalog                                                              */
/*----------------------------------------------------------------------------*/
void OpCatalog::vPrintDefn(OpDefn* aOpDefn)
{
  vTrace("%12s %12s %6s %16s :: "
         "%3s %3s %3s %3s -> %-28s -> "_Stz
         "%3s %3s %3s %3s\n"_Stz,

         aOpDefn->m_szCommand,
         aCN[aOpDefn->m_eClass],
         aOK[aOpDefn->m_eKind],
         aOpDefn->m_bIsPartitioned ? "partitioned"_Stz : "non-partitioned"_Stz,
     
         aOpDefn->m_bCrwBuffer_in  ? "Crw"_Stz : ""_Stz,
         aOpDefn->m_bEntBuffer_in  ? "Ent"_Stz : ""_Stz,
         aOpDefn->m_bIrwBuffer_in  ? "Irw"_Stz : ""_Stz,
         aOpDefn->m_bSrtBuffer_in  ? "Srt"_Stz : ""_Stz,
         aOpDefn->m_szHelp,
         
         aOpDefn->m_bCrwBuffer_out ? "Crw"_Stz : ""_Stz,
         aOpDefn->m_bEntBuffer_out ? "Ent"_Stz : ""_Stz,
         aOpDefn->m_bIrwBuffer_out ? "Irw"_Stz : ""_Stz,
         aOpDefn->m_bSrtBuffer_out ? "Srt"_Stz : ""_Stz,

         C_ENDARGS);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
