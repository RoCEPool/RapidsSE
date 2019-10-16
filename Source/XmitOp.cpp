/*============================================================================*/
/* XmitOp.cpp -- Transmit                                                     */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_XmitOp (
  CN_XMITOP,          // class number
  OK_SINGLE,          // op kind
  "Xmit"_Stz,         // first word of prompt command
  "Xmt"_Stz,          // prefix
  
  true,               // partitioned
  false,              // compiler op
  
  true,               // accepts CrwBuffer?
  false,              // accepts EntBuffer?
  false,              // accepts IrwBuffer?
  false,              // accepts SrtBuffer?
  
  true,               // accepts CrwBuffer?
  false,              // accepts EntBuffer?
  false,              // accepts IrwBuffer?
  false,              // accepts SrtBuffer?
  
  CN_NONE,            // what kind of output
  
  XmitOp::pNew,       // function which parses the command and new's an op
  XmitOp::eCall,      // function which calls the op loop
  nullptr,            // function which calls the drain loop
  
  "xmit"_Stz
  );


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
XmitOp* DataOperators::_pXmitOp(void* aAddr)
{
  XmitOp* pXmitOp = reinterpret_cast<XmitOp*>(aAddr);
  
  return pXmitOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* XmitOp::operator new(size_t)
{
    XmitOp* pXmitOp =
    static_cast<XmitOp*>(HeapMem::pAllocate(CN_XMITOP));
    
    return pXmitOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void XmitOp::operator delete(void* aAddress)
{
    XmitOp* pXmitOp = static_cast<XmitOp*>(aAddress);
    
    HeapMem::vFree(pXmitOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XmitOp::XmitOp(OpSpec* pOpSpec, Pipeline* pPipeline)
: Op(CN_XMITOP,pOpSpec,pPipeline)
{
  _precondition_(_MAGIC_,CN_XMITOP);
  
  m_iXmitNode = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XmitOp::~XmitOp(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_XMITOP);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* XmitOp::pNew(OpSpec* pOpSpec, Pipeline* pPipeline)
{
  XmitOp* pXmitOp = new XmitOp(pOpSpec,pPipeline);

  pXmitOp->m_eTopologyX = (ePX)pXmitOp->m_pOpSpec->pEnameSet()->iValue((i8)0);
  pXmitOp->m_iNextStage   = pXmitOp->m_pOpSpec->pEnameSet()->iValue((i8)1);
  
  return pXmitOp;
}

/*----------------------------------------------------------------------------*/
/* call the xmot loop function                                                */
/*----------------------------------------------------------------------------*/
eOR XmitOp::eCall(void* aThis)
{
    eOR eCallAgain = ((XmitOp*)aThis)->eXmitLoop();
    
    return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/* the sort operator loop function                                            */
/*----------------------------------------------------------------------------*/
void XmitOp::vXmitNode(i8 i_XmitNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XMITOP,_FOOTER_);
  
  m_iXmitNode = i_XmitNode;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* the sort operator loop function                                            */
/*----------------------------------------------------------------------------*/
eOR XmitOp::eXmitLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XMITOP,_FOOTER_);
  
  i8 iOperands = 0;
  
  for (;;) {
    
    Operand* pOperand = pPopQueue();
  
    if (! pOperand) {
      ASSERT(bIsLocked());
      return OR_BIDE;
    }
    
    if (CN_ENDMARKER == pOperand->eClass()) {
      EndMarker* pEndMarker = _pEndMarker(pOperand);
      //delete pEndMarker;
      pEndMarker->m_iTotalOperands = iOperands;
      AM::vSendJobOperand(m_pTask,
                          pOperand,
                          m_pPipeline->pJob(),
                          this,
                          m_iXmitNode);
      
      break;
    }
  
    if (CN_STREAMDEF == pOperand->eClass()) {
      m_pStreamDef = new StreamDef(_pStreamDef(pOperand));
      //vPipeOut(pOperand);
      AM::vSendJobOperand(m_pTask,
                          pOperand,
                          m_pPipeline->pJob(),
                          this,
                          m_iXmitNode);
      continue;
    }
  
    RASSERT(CN_CRWBUFFER == pOperand->eClass() ||
            CN_IRWBUFFER == pOperand->eClass());
  
    iOperands++;
    
    if (AM::bVerbose())
      vTrace("%s::XmitOut :    %s %p (N%lld)\n"_Stz,
             m_sLabel.szStz(),
             aCN[pOperand->eClass()],
             pOperand,
             m_iXmitNode);
  
    AM::vLogOpr(SL_INF,this,OA_OP_APPEND,
                "XmitOut operand %s to node %lld"_Stz, //!format-LogOpr
                aCN[pOperand->eClass()],
                m_iXmitNode);
  
    AM::vSendJobOperand(m_pTask,
                        pOperand,
                        m_pPipeline->pJob(),
                        this,
                        m_iXmitNode);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  return OR_EXIT;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(XmitOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
