/*============================================================================*/
/* CountOp.cpp -- Output                                                     */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_Count (
  CN_COUNTOP,         // class number
  OK_SINGLE,          // op kind
  "CountOp"_Stz,      // first word of prompt command
  "Cnt"_Stz,          // prefix
                 
  true,               // partitioned
  false,              // compiler op
                 
  true,               // accepts CrwBuffer?
  true,               // accepts EntBuffer?
  true,               // accepts IrwBuffer?
  true,               // accepts SrtBuffer?
                   
  false,              // accepts CrwBuffer?
  false,              // accepts EntBuffer?
  true,               // accepts IrwBuffer?
  false,              // accepts SrtBuffer?
                   
  CN_NONE,            // what kind of output

  CountOp::pNew,      // function which parses the command and new's an op
  CountOp::eCall,     // function which calls the op loop
  CountOp::eDrain,    // function which calls the drain loop

  "countOp"_Stz
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
CountOp* DataOperators::_pCountOp(void* aAddr)
{
  CountOp* pCountOp = reinterpret_cast<CountOp*>(aAddr);
  
  return pCountOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* CountOp::operator new(size_t)
{
  CountOp* pCountOp =
    static_cast<CountOp*>(HeapMem::pAllocate(CN_COUNTOP));
  
  return pCountOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void CountOp::operator delete(void* aAddress)
{
  CountOp* pCountOp = static_cast<CountOp*>(aAddress);
  
  HeapMem::vFree(pCountOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CountOp::CountOp(OpSpec* pOpSpec, Pipeline* pPipeline)
: Op(CN_COUNTOP,pOpSpec,pPipeline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COUNTOP,_FOOTER_);
  
  m_fpCall = CountOp::eCall;
  m_nCount = 0;
//  m_pStreamDef = new StreamDef;
//
//  // create an integer column
//  m_pStreamDef->m_xaColumn[0].vColumnNo(0);
//  m_pStreamDef->m_xaColumn[0].vColumnNm("count"_Stz);
//  m_pStreamDef->m_xaColumn[0].vColumnType(CT_INTEGER);
//  m_pStreamDef->m_xaColumn[0].vDatumType(DM_INT);
//
//  m_pStreamDef->m_nDefSets++;
//  m_pStreamDef->m_nDefColumns[0] = 1;
//  m_pStreamDef->m_i_DefStart[0] = 0;
  
  _checkInitialised_(CN_COUNTOP);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_COUNTOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CountOp::~CountOp(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COUNTOP,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* CountOp::pNew(OpSpec* pOpSpec, Pipeline* pPipeline)
{
  CountOp* pCountOp = new CountOp(pOpSpec,pPipeline);
  
  return pCountOp;
}

/*----------------------------------------------------------------------------*/
/* call loop                                                                  */
/*----------------------------------------------------------------------------*/
eOR CountOp::eCall(void* aThisOp)
{
  eOR eCallAgain = ((CountOp*)aThisOp)->eOpLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/* call drain loop                                                            */
/*----------------------------------------------------------------------------*/
eOR CountOp::eDrain(void* aThisOp)
{
  eOR eCallAgain = ((CountOp*)aThisOp)->eDrainLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* op loop                                                                    */
/*----------------------------------------------------------------------------*/
eOR CountOp::eOpLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COUNTOP,_FOOTER_);
  
  RASSERT(0);
  
  for (;;) {
    
    Operand* pOperand = pPopQueue();
    
    //! HACK: TaskLoop_Op
    if (! pOperand) {
      //ASSERT(bIsLocked());
      //return OR_BIDE;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    switch (pOperand->eClass()) {
        
      case CN_ENDMARKER:
        if (--m_nLeftEndMarkers == 0) {
          IrwBuffer* pIrwBuffer = new IrwBuffer();
          
          // get the columnVec defining the rowref
          ColumnVec* pColumnVec = new ColumnVec;
          m_pStreamDef->vAddColumns(pColumnVec);
          
          Row* pRow = pIrwBuffer->aAllotRow(pColumnVec);
          pRow->m_waColVal[0] = m_nCount;
          
          // pipe out the streamdef
          vPipeOut(new StreamDef(m_pStreamDef));
          
          // pipe out the buffer with the count
          vPipeOut(pIrwBuffer);
          
          // now pipe out the end marker
          vPipeOut(pOperand);
          goto L_EXIT;
        }
        else
          delete (EndMarker*)pOperand;
        
        break;
        
      // -- -- -- -- -- --
        
      case CN_CRWBUFFER: {
        CrwBuffer* pCrwBuffer = _pCrwBuffer(pOperand) ;
        m_nCount += pCrwBuffer->nRowCount();
        break;
      }
      
      // -- -- -- -- -- --
        
      case CN_ENTBUFFER: {
        EntBuffer* pEntBuffer = _pEntBuffer(pOperand) ;
        m_nCount += pEntBuffer->nItems();
        break;
      }
      
      // -- -- -- -- -- --
        
      case CN_IRWBUFFER: {
        IrwBuffer* pIrwBuffer = _pIrwBuffer(pOperand) ;
        m_nCount += pIrwBuffer->nItems();
        delete pIrwBuffer;
        break;
      }
       
      // -- -- -- -- -- --
        
      case CN_SRTBUFFER: {
        SrtBuffer* pSrtBuffer = _pSrtBuffer(pOperand);
        m_nCount += pSrtBuffer->nItems();
        delete pSrtBuffer;
        break;
      }
      
      // -- -- -- -- -- --
        
      case CN_STREAMDEF: {
        delete _pStreamDef(pOperand);
        break;
      }
      
      // -- -- -- -- -- --
        
      default:
        vPipeOut(pOperand);
        break;
    }
    
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_EXIT:
  return OR_EXIT;
}

/*----------------------------------------------------------------------------*/
/* drain loop                                                                 */
/*----------------------------------------------------------------------------*/
eOR CountOp::eDrainLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COUNTOP,_FOOTER_);
  
  for (;;) {
    
    Operand* pOperand = pPopQueue();
    
    if (! pOperand) {
      ASSERT(bIsLocked());
      return OR_BIDE;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    switch (pOperand->eClass()) {
        
      case CN_ENDMARKER:
        vPipeOut(pOperand);
        goto L_EXIT;
        
      // -- -- -- -- -- --
        
      case CN_PAUSEMARKER:
        vPipeOut(pOperand);
        break;
        
      // -- -- -- -- -- --
        
      case CN_INTBUFFER:
        //vPipeOut(pOperand);
        break;
        
      default:
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

__aFooterFunction(CountOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
