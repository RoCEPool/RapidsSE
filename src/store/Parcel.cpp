/*============================================================================*/
/* XmitOperand.cpp -- xmit Operand                                            */
/*============================================================================*/

#include "BASE.h"

#include "ePX.h"

#include "Op.hpp"
#include "OpSpec.hpp"
#include "Parcel.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,XmitOperand,CN_XMITOPERAND)
__aNewFunction(XmitOperand,CN_XMITOPERAND)
__vDeleteFunction(XmitOperand,CN_XMITOPERAND)
__aFooterFunction(XmitOperand)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XmitOperand::XmitOperand(void)
: Object(CN_XMITOPERAND)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XMITOPERAND,_FOOTER_);
  
  m_iPipelineJobId = C_UNKNOWN;
  m_i_Node         = C_UNKNOWN;
  m_tPtnNo         = C_UNKNOWN;
  
  m_eCrossTopo     = PX_NONE;
  m_iNextStage     = C_UNKNOWN;
  m_iNextOp        = C_UNKNOWN;
  
  m_nObjects = 0;
  for (i8 i_Obj = 0; i_Obj < C_MAX_XMIT_OBJ; i_Obj++)
    m_pObjects[i_Obj] = nullptr;
  
  _checkInitialised_(CN_XMITOPERAND);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_XMITOPERAND,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XmitOperand::~XmitOperand(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XMITOPERAND,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* new an operand job                                                         */
/*----------------------------------------------------------------------------*/
XmitOperand* XmitOperand::pNewXmitOperand(Operand* pOperand,
                                         i8 iPipelineJobId,
                                         ePX eCrossTopo, i8 iNextStage, i8 iNextOp,
                                         i8 i_Node, tPN tPtnNo)
{
  XmitOperand* pXmitOperand = new XmitOperand;
  
  pXmitOperand->m_iPipelineJobId = iPipelineJobId;
  pXmitOperand->m_i_Node         = i_Node;
  pXmitOperand->m_tPtnNo         = tPtnNo;
  pXmitOperand->vSetOperand(pOperand);
  
  pXmitOperand->m_eCrossTopo     = eCrossTopo;
  pXmitOperand->m_iNextStage     = iNextStage;
  pXmitOperand->m_iNextOp        = iNextOp;
  
  return pXmitOperand;
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Add object to array                                                        */
/*----------------------------------------------------------------------------*/
void XmitOperand::vAddObject(Object *pObj)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XMITOPERAND,_FOOTER_);

  if (m_nObjects < C_MAX_XMIT_OBJ) {
    m_pObjects[m_nObjects++] = pObj;
  }
}

/*----------------------------------------------------------------------------*/
/* Get an object from array                                                   */
/*----------------------------------------------------------------------------*/
Object* XmitOperand::pGetObject(i8 i_Object)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XMITOPERAND,_FOOTER_);

  if (i_Object < C_MAX_XMIT_OBJ && i_Object < m_nObjects)
    return m_pObjects[i_Object];
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* Get Operand                                                                */
/*----------------------------------------------------------------------------*/
Operand* XmitOperand::pGetOperand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XMITOPERAND,_FOOTER_);

  return (Operand*) m_pObjects[0];
}

/*----------------------------------------------------------------------------*/
/* Set Operand                                                                */
/*----------------------------------------------------------------------------*/
void XmitOperand::vSetOperand(Operand *pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XMITOPERAND,_FOOTER_);

  m_pObjects[0] = pOperand;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
