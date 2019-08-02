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
               eOR  (*fpOpFun)(void* aThis),  // function which does the op
               void (*fpDrain)(void* aThis),  // function which drains the operand queue
               Stz* szHelp      // help text
              )
{
  i8 i_OpDefn = gxOpCatalog.m_nOpDefns++;
  
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_eClass    = eClass;
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_eKind     = eKind;
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_szCommand = szCommand;
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_fpOpFun   = fpOpFun;
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_fpDrain   = fpDrain;
  gxOpCatalog.m_xOpDefn[i_OpDefn].m_szHelp    = szHelp;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
