/*============================================================================*/
/* ToCsvOp.cpp -- conver to csv operator                                      */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_ToCsv (
  CN_TOCSVOP,         // class number
  OK_SINGLE,          // op kind
  "ToCsvOp"_Stz,      // first word of prompt command
  "Csv"_Stz,          // prefix
                 
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

  ToCsvOp::pNew,      // function which parses the command and new's an op
  ToCsvOp::eCall,     // function which calls the op loop
  ToCsvOp::eDrain,    // function which calls the drain loop

  "ToCsv"_Stz
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ToCsvOp* DataOperators::_pToCsvOp(void* aAddr)
{
  ToCsvOp* pToCsvOp = reinterpret_cast<ToCsvOp*>(aAddr);
  
  return pToCsvOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* ToCsvOp::operator new(size_t)
{
  ToCsvOp* pToCsvOp =
    static_cast<ToCsvOp*>(HeapMem::pAllocate(CN_TOCSVOP));
  
  return pToCsvOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void ToCsvOp::operator delete(void* aAddress)
{
  ToCsvOp* pToCsvOp = static_cast<ToCsvOp*>(aAddress);
  
  HeapMem::vFree(pToCsvOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ToCsvOp::ToCsvOp(OpSpec* pOpSpec, Pipeline* pPipeline)
: Op(CN_TOCSVOP,pOpSpec,pPipeline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOCSVOP,_FOOTER_);
  
  m_fpCall = ToCsvOp::eCall;
  m_pStreamDef = new StreamDef;
  
  // create an integer column
  Column oColumn = m_pStreamDef->m_xaColumn[0];
  oColumn.vColumnNo(0);
  oColumn.vColumnNm("row"_Stz);
  oColumn.vColumnType(CT_VARCHAR);
  oColumn.vDatumType(DM_VCH);
  
  m_pStreamDef->m_nDefSets++;
  m_pStreamDef->m_nDefColumns[0] = 1;
  m_pStreamDef->m_i_DefStart[0] = 0;
  
  _checkInitialised_(CN_TOCSVOP);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_TOCSVOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ToCsvOp::~ToCsvOp(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOCSVOP,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* ToCsvOp::pNew(OpSpec* pOpSpec, Pipeline* pPipeline)
{  
  ToCsvOp* pToCsvOp = new ToCsvOp(pOpSpec,pPipeline);
  
  return pToCsvOp;
}

/*----------------------------------------------------------------------------*/
/* call loop                                                                  */
/*----------------------------------------------------------------------------*/
eOR ToCsvOp::eCall(void* aThisOp)
{
  eOR eCallAgain = ((ToCsvOp*)aThisOp)->eOpLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/* call drain loop                                                            */
/*----------------------------------------------------------------------------*/
eOR ToCsvOp::eDrain(void* aThisOp)
{
  eOR eCallAgain = ((ToCsvOp*)aThisOp)->eDrainLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* op loop                                                                    */
/*----------------------------------------------------------------------------*/
eOR ToCsvOp::eOpLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOCSVOP,_FOOTER_);
  
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
          
          // pipe out the streamdef
          vPipeOut(new StreamDef(m_pStreamDef));
          
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
        CsvBuffer* pCsvBuffer = pCrwBuffer->pToCsvBuffer();
        
        while (pCsvBuffer) {
          CsvBuffer* pPipeOut = pCsvBuffer;
          pCsvBuffer = pCsvBuffer->pNextBuffer();
          vPipeOut(pPipeOut);
        }
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
eOR ToCsvOp::eDrainLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOCSVOP,_FOOTER_);
  
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

__aFooterFunction(ToCsvOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
