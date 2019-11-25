/*============================================================================*/
/* XmitOp.cpp -- Transmit                                                     */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_XmitOp (
  CN_XMITOP,          // class number
  OK_SINGLE,          // op kind
  "XmitOp"_Stz,       // first word of prompt command
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
  
  m_bCancelMarker = false;
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

  // add arg-0 as the cross topology for the xmit
  pXmitOp->m_eTopologyX = (ePX)pXmitOp->m_pOpSpec->pEnameSet()->iValue((i8)0);
  
  // add arg-1 as the next stage of the pipeline where the xmit needs to xmit
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
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* the sort operator loop function                                            */
/*----------------------------------------------------------------------------*/
eOR XmitOp::eXmitLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XMITOP,_FOOTER_);
  
  RASSERT(0);
  
  i8 iOperands = 0;
  
  for (;;) {
    
    Operand* pOperand = pPopQueue();
  
    //! HACK: TaskLoop_Op
    if (! pOperand) {
      //ASSERT(bIsLocked());
      //return OR_BIDE;
      continue;
    }
    
    switch (pOperand->eClass()) {
        
      // -- -- -- -- -- --
      case CN_ENDMARKER:  {
        EndMarker* pEndMarker = _pEndMarker(pOperand);
        pEndMarker->m_iTotalOperands = iOperands;
        AM::vSendJobOperand(m_pTask,
                            pOperand,
                            m_pPipeline->pJob(),
                            this,
                            m_iXmitNode);
        AM::vDeleteOperand(pOperand);
        goto L_EXIT;
      }
      
      // -- -- -- -- -- --
      case CN_STREAMDEF:  {
        m_pStreamDef = _pStreamDef(pOperand);
        AM::vSendJobOperand(m_pTask,
                            pOperand,
                            m_pPipeline->pJob(),
                            this,
                            m_iXmitNode);
        break;
      }
     
      // -- -- -- -- -- --
        
      case CN_CRWBUFFER:
      case CN_IRWBUFFER:
      case CN_CSVBUFFER: {
        
        // check if query being cancelled
        if (m_pPipeline->bCancelling()) {
          
          // has the CANCELMARKER already been transmitted ?
          if ( ! m_bCancelMarker ) {
            CancelMarker* pCancelMarker = new CancelMarker;
            pCancelMarker->m_iTotalOperands = iOperands;
            AM::vSendJobOperand(m_pTask,
                                pOperand,
                                m_pPipeline->pJob(),
                                this,
                                m_iXmitNode);
          }
          
          // delete non-data operands
          if (pOperand->eClass() == CN_IRWBUFFER)
            delete _pIrwBuffer(pOperand);
          if (pOperand->eClass() == CN_CSVBUFFER)
            delete _pIrwBuffer(pOperand);
            
          break;
        }
        iOperands++;
        
        if (AM::bVerbose())
          vTrace("%s::XmitOut :    %s %p (N%lld)\n"_Stz,
                 m_sName.szStz(),
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
        break;
      }
      
      // -- -- -- -- -- --
      default:
        RASSERT(0);
        break;
      }
    
    }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

L_EXIT:
  return OR_EXIT;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(XmitOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
