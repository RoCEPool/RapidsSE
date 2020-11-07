/*============================================================================*/
/* XmitOperand.cpp -- xmit Operand                                            */
/*============================================================================*/

#include "Jobs.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
XmitOperand* Jobs::_pXmitOperand(void* aAddress)
{
  XmitOperand* pXmitOperand= reinterpret_cast<XmitOperand*>(aAddress);
  
  RASSERT_TYPE(pXmitOperand,CN_XMITOPERAND);
  
  return pXmitOperand;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* XmitOperand::operator new(size_t)
{
  XmitOperand* pXmitOperand =
    static_cast<XmitOperand*>(HeapMem::pAllocate(CN_XMITOPERAND));
  
  return pXmitOperand;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* XmitOperand::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void XmitOperand::operator delete(void* aAddress)
{
  XmitOperand* pXmitOperand = static_cast<XmitOperand*>(aAddress);
  HeapMem::vFree(pXmitOperand);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XmitOperand::XmitOperand(void)
: Object(CN_XMITOPERAND)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XMITOPERAND,_FOOTER_);
  
  m_pOperand       = nullptr;
  m_iPipelineJobId = C_UNKNOWN;
  m_i_Node          = C_UNKNOWN;
  m_tPtnNo         = C_UNKNOWN;
  
  m_eTopologyX     = PX_NONE;
  m_iNextStage     = C_UNKNOWN;
  m_iNextOp        = C_UNKNOWN;
  
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
                                         Op* pOp, i8 i_Node, tPN tPtnNo)
{
  XmitOperand* pXmitOperand = new XmitOperand;
  
  pXmitOperand->m_iPipelineJobId = iPipelineJobId;
  pXmitOperand->m_i_Node         = i_Node;
  pXmitOperand->m_tPtnNo         = tPtnNo;
  pXmitOperand->m_pOperand       = pOperand;
  
  pXmitOperand->m_eTopologyX     = pOp->eTopologyX();
  pXmitOperand->m_iNextStage     = pOp->iNextStage();
  pXmitOperand->m_iNextOp        = pOp->iNextOp();
  
  return pXmitOperand;
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* XmitOperand::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XMITOPERAND);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
