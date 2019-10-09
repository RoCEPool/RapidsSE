/*============================================================================*/
/* SuperAggOp.cpp -- Super Merge                                            */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_SuperAgg (
  CN_SUPERAGGOP,    // class number
  OK_SINGLE,        // op kind
  "SuperAgg"_Stz,   // first word of prompt command
  "Sag"_Stz,        // prefix
                      
  false,            // partitioned
  false,            // compiler op

  false,              // accepts CrwBuffer?
  false,              // accepts EntBuffer?
  true,               // accepts IrwBuffer?
  false,              // accepts SrtBuffer?
  
  false,              // accepts CrwBuffer?
  false,              // accepts EntBuffer?
  true,               // accepts IrwBuffer?
  false,              // accepts SrtBuffer?
                      
  CN_NONE,          // what kind of output
                      
  SuperAggOp::pNew, // function which parses the command and new's an op
  SuperAggOp::eCall,// function which calls the op loop
  nullptr,          // function which calls the drain loop
  
  "superAgg"_Stz
  );

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SuperAggOp* DataOperators::_pSuperAggOp(void* aAddr)
{
  SuperAggOp* pSuperAggOp = reinterpret_cast<SuperAggOp*>(aAddr);
  
  return pSuperAggOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* SuperAggOp::operator new(size_t)
{
  SuperAggOp* pSuperAggOp =
  static_cast<SuperAggOp*>(HeapMem::pAllocate(CN_SORTOP));
  
  return pSuperAggOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void SuperAggOp::operator delete(void* aAddress)
{
  SuperAggOp* pSuperAggOp = static_cast<SuperAggOp*>(aAddress);
  
  HeapMem::vFree(pSuperAggOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SuperAggOp::SuperAggOp(OpSpec* pOpSpec, Pipeline* pPipeline)
: Op(CN_SUPERAGGOP,pOpSpec,pPipeline)
{
  _precondition_(_MAGIC_,CN_SUPERAGGOP);
  
  m_fpCall = SuperAggOp::eCall;
  m_nSum   = 0;
  
  m_fpCall = SuperAggOp::eCall;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SuperAggOp::~SuperAggOp(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SUPERAGGOP);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* SuperAggOp::pNew(OpSpec* pOpSpec, Pipeline* pPipeline)
{  
  SuperAggOp* pSuperAggOp = new SuperAggOp(pOpSpec,pPipeline);
  
  return pSuperAggOp;
}

/*----------------------------------------------------------------------------*/
/* call the merge loop function                                               */
/*----------------------------------------------------------------------------*/
eOR SuperAggOp::eCall(void* aThis)
{
  eOR eCallAgain = ((SuperAggOp*)aThis)->eSuperAggLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* the super aggregate operator loop function                                 */
/*----------------------------------------------------------------------------*/
eOR SuperAggOp::eSuperAggLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUPERAGGOP,_FOOTER_);
  
  // create a column vec from each row ref
  ColumnVec* pColumnVec = new ColumnVec;
  m_pStreamDef->vAddColumns(pColumnVec);
  
  for (;;) {
    
    Operand* pOperand = pPopQueue();
    
    if (! pOperand) {
      ASSERT(bIsLocked());
      return OR_BIDE;
    }
    
    if (CN_ENDMARKER == pOperand->eClass()) {
      
      if (--m_nLeftEndMarkers == 0) {
        IrwBuffer* pIrwBuffer = new IrwBuffer();
        Row* pRow = pIrwBuffer->aAllotRow(pColumnVec);
        pRow->m_waColVal[0] = m_nSum;
        
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
    m_nSum += aRow->m_waColVal[0].m_iI8;
    
    // now delete the buffer
    delete pIrwBuffer;
    
  }
  
  return OR_EXIT;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(SuperAggOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
