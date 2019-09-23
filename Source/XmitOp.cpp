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
XmitOp::XmitOp(OpSpec* pOpSpec, Digest* pDigest, OpDefn* aOpDefn)
: Op(CN_XMITOP,pOpSpec,pDigest,aOpDefn)
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
Op* XmitOp::pNew(OpSpec* pOpSpec, Digest* pDigest, OpDefn* aOpDefn,
                 TokenVec* pTknVec, i8& i_Tkn_)
{
  i_Tkn_++;
  XmitOp* pXmitOp = new XmitOp(pOpSpec,pDigest,aOpDefn);

  pXmitOp->m_iXmitNode = pTknVec->pGet(i_Tkn_)->iToI8();
  
  pXmitOp->m_sArgs = pTknVec->pGet(i_Tkn_++)->sText();
  
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
    
    for (;;) {
        
        Operand* pOperand = pPopQueue();
        
        if (! pOperand) {
          ASSERT(bIsLocked());
          return OR_BIDE;
        }
        
        if (CN_ENDMARKER == pOperand->eClass()) {
          EndMarker* pEndMarker = _pEndMarker(pOperand);
          delete pEndMarker;
          break;
        }
        
        if (CN_STREAMDEF == pOperand->eClass()) {
          m_pStreamDef = new StreamDef(_pStreamDef(pOperand));
          vPipeOut(pOperand);
          continue;
        }
        
        RASSERT(CN_CRWBUFFER == pOperand->eClass());
      
      if (AM::bVerbose())
        vTrace("%s::XmitOut :    %s %p (N%lld)\n"_Stz,
               m_sLabel.aStz(),
               aCN[pOperand->eClass()],
               pOperand,
               m_iXmitNode);
      
      AM::vLogOpr(SL_INF,this,OA_OP_APPEND,
                  "XmitOut operand %s to node %lld"_Stz, //!format-LogOpr
                  aCN[pOperand->eClass()],
                  m_iXmitNode);
      
      AM::vSpawnJobXmit(m_pDigest->m_pSession,
                              pOperand,
                              -1,
                              m_iXmitNode);
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    vPipeOut_endMarker();
    
    return OR_EXIT;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(XmitOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
