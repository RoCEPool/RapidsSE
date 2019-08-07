/*============================================================================*/
/* OpDesc.cpp -- the class which populates the op catalog                     */
/*============================================================================*/

#include "PushDown.h"

extern OpCatalog gxOpCatalog;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
OpDesc::OpDesc(eCN  eClass,     // class number
               eOK  eKind,      // op kind
               Stz* szCommand,  // first word of prompt command
               Stz* szPrefix,   // prefix
               Op*  (*fpNew)(Stz* szLabel,
                             Digest* pDigest, OpDefn* aOpDefn, bool bOutB,
                             TokenVec* pTknVec, i8& i_Tkn),
               eOR  (*fpCall) (void* aThisOp),  // fun which calls the op loop
               eOR  (*fpDrain)(void* aThisOp),  // fun which calls the drain loop
               Stz* szHelp      // help text
              )
{
  i8 i_OpDefn = gxOpCatalog.m_nOpDefns++;
  
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_eClass    = eClass;
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_eKind     = eKind;
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_szCommand = szCommand;
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_szPrefix  = szPrefix;
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_fpNew     = fpNew;
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_fpCall    = fpCall;
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_fpDrain   = fpDrain;
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_szHelp    = szHelp;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
