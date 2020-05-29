/*============================================================================*/
/* SuperCountOp.cpp -- Super Merge                                            */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_SuperCount (
  CN_SUPERCOUNTOP,    // class number
  OK_SINGLE,          // op kind
  "SuperCountOp"_Stz, // first word of prompt command
  "Sct"_Stz,          // prefix
                        
  false,              // partitioned
  false,              // compiler op

  false,              // accepts CrwBuffer?
  false,              // accepts EntBuffer?
  true,               // accepts IrwBuffer?
  false,              // accepts SrtBuffer?
                        
  false,              // accepts CrwBuffer?
  false,              // accepts EntBuffer?
  true,               // accepts IrwBuffer?
  false,              // accepts SrtBuffer?
                        
  CN_NONE,            // what kind of output
                        
  SuperCountOp::pNew, // function which parses the command and new's an op
  SuperCountOp::eCall,// function which calls the op loop
  nullptr,            // function which calls the drain loop
  
  "superCountOp"_Stz
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
SuperCountOp::SuperCountOp(OpSpec* pOpSpec, Pipeline* pPipeline)
: Op(CN_SUPERCOUNTOP,pOpSpec,pPipeline)
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
Op* SuperCountOp::pNew(OpSpec* pOpSpec, Pipeline* pPipeline)
{
  SuperCountOp* pSuperCountOp = new SuperCountOp(pOpSpec,pPipeline);
  
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
  
  RASSERT(0);
  
  for (;;) {
    
    Operand* pOperand = pPopQueue();
    
    //! HACK: TaskLoop_Op
    if (! pOperand) {
      //ASSERT(bIsLocked());
      //return OR_BIDE;
      continue;
    }
    
    if (CN_ENDMARKER == pOperand->eClass()) {
      
      if (--m_nLeftEndMarkers == 0) {
        IrwBuffer* pIrwBuffer = new IrwBuffer();
        
        //! ALKA TO REVIEW : COLUMNV VECTOR
        // create a column vec from each row ref
        ColumnVec* pColumnVec = new ColumnVec;
        m_pStreamDef->vAddColumns(pColumnVec);
        
        Row* pRow = pIrwBuffer->aAllotRow(pColumnVec);
        pRow->m_waColVal[0] = m_nCount;
        
        delete pColumnVec;
        
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
