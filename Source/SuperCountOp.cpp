/*============================================================================*/
/* SuperCountOp.cpp -- Super Merge                                            */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_SuperCount (
  CN_SUPERCOUNTOP,    // class number
  OK_SIMPLE,          // op kind
  "SuperCount"_Stz,   // first word of prompt command
  "Sct"_Stz,          // prefix
  false,              // partitioned
  SuperCountOp::pNew, // function which parses the command and new's an op
  SuperCountOp::eCall,// function which calls the op loop
  nullptr,            // function which calls the drain loop
  
  "<help>"_Stz
  );

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SuperCountOp* DataOperators::_pSuperCountOp(void* aAddr)
{
  SuperCountOp* pSuperCountOp = reinterpret_cast<SuperCountOp*>(aAddr);
  
  return pSuperCountOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* SuperCountOp::operator new(size_t)
{
  SuperCountOp* pSuperCountOp =
  static_cast<SuperCountOp*>(HeapMem::pAllocate(CN_SORTOP));
  
  return pSuperCountOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void SuperCountOp::operator delete(void* aAddress)
{
  SuperCountOp* pSuperCountOp = static_cast<SuperCountOp*>(aAddress);
  
  HeapMem::vFree(pSuperCountOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SuperCountOp::SuperCountOp(OpSpec* pOpSpec, Digest* pDigest, OpDefn* aOpDefn)
: Op(CN_SUPERCOUNTOP,pOpSpec,pDigest,aOpDefn)
{
  _precondition_(_MAGIC_,CN_SUPERCOUNTOP);
  
  m_fpCall = SuperCountOp::eCall;
  m_nCount = 0;
  
  m_fpCall = SuperCountOp::eCall;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SuperCountOp::~SuperCountOp(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SUPERCOUNTOP);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* SuperCountOp::pNew(OpSpec* pOpSpec, Digest* pDigest, OpDefn* aOpDefn,
                       TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot)
{
  i_Tkn_++;
  ASSERT(i_Slot == -1);
  
  SuperCountOp* pSuperCountOp = new SuperCountOp(pOpSpec,pDigest,aOpDefn);
  
  return pSuperCountOp;
}

/*----------------------------------------------------------------------------*/
/* call the merge loop function                                               */
/*----------------------------------------------------------------------------*/
eOR SuperCountOp::eCall(void* aThis)
{
  eOR eCallAgain = ((SuperCountOp*)aThis)->eSuperCountLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* the super count operator loop function                                     */
/*----------------------------------------------------------------------------*/
eOR SuperCountOp::eSuperCountLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUPERCOUNTOP,_FOOTER_);
  
  for (;;) {
    
    Operand* pOperand = pPopQueue();
    
    if (! pOperand) {
      ASSERT(bIsLocked());
      return OR_BIDE;
    }
    
    if (CN_ENDMARKER == pOperand->eClass()) {
      
      if (--m_nLeftEndMarkers == 0) {
        IrwBuffer* pIrwBuffer = new IrwBuffer();
        Row* pRow = pIrwBuffer->aAllotRow(m_pStreamDef->pColumnVec(0));
        pRow->m_waColVal[0] = m_nCount;
        
        // pipe out the buffer with the count
        vPipeOut(pIrwBuffer);
        
        // now pipe out the end marker
        vPipeOut(pOperand);

        break;
      }
      else
        delete (EndMarker*)pOperand;
      
      continue;
    }
    
    if (CN_STREAMDEF == pOperand->eClass()) {
      if ( ! m_pStreamDef ) {
        m_pStreamDef = new StreamDef(_pStreamDef(pOperand));
        vPipeOut(pOperand);
      }
      else
        delete _pStreamDef(pOperand);
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // gather the sum from the buffers of each partition
    
    IrwBuffer* pIrwBuffer = _pIrwBuffer(pOperand) ;
    RASSERT(pIrwBuffer->nItems() == 1);
    Row* aRow = pIrwBuffer->aFirstRow();
    
    // col;ect the count
    m_nCount += aRow->m_waColVal[0].m_iI8;
    
    // now delete the buffer
    delete pIrwBuffer;
    
  }
  
  return OR_EXIT;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(SuperCountOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
