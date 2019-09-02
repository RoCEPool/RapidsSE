/*============================================================================*/
/* CountOp.cpp -- Output                                                     */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_Count (
   CN_COUNTOP,         // class number
   OK_SIMPLE,          // op kind
   "Count"_Stz,        // first word of prompt command
   "Cnt"_Stz,          // prefix
   false,              // partitioned
   CountOp::pNew,      // function which parses the command and new's an op
   CountOp::eCall,     // function which calls the op loop
   CountOp::eDrain,    // function which calls the drain loop
   
   "<help>"_Stz
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
CountOp::CountOp(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR)
: Op(CN_COUNTOP,szOpLabel,pDigest,aOpDefn,bAppR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COUNTOP,_FOOTER_);
  
  m_fpCall = CountOp::eCall;
  m_nCount = 0;
  m_pStreamDef = new StreamDef;
  
  // create an integer column
  Column* pColumn = new Column();
  pColumn->vColumnNo(0);
  pColumn->vColumnName("count"_Stz);
  pColumn->vColumnType(CT_INTEGER);
  pColumn->vDatumType(DM_INT);
  
  m_pStreamDef->m_oColVecAry.vItems(1);
  ColumnVec* pColVec = m_pStreamDef->pColumnVec(0);
  pColVec->vAdd(pColumn);
  
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
Op* CountOp::pNew(Stz* szLabel,
                   Digest* pDigest, OpDefn* aOpDefn, bool bAppR,
                   TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot)
{
  i_Tkn_++;
  ASSERT(i_Slot == -1);
  
  CountOp* pCountOp = new CountOp(szLabel,pDigest,aOpDefn,bAppR);
  
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
  
  for (;;) {
    
    Operand* pOperand = pPopQueue();
    
    if (! pOperand) {
      ASSERT(bIsLocked());
      return OR_BIDE;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    switch (pOperand->eClass()) {
        
      case CN_ENDMARKER:
        if (--m_nLeftEndMarkers == 0) {
          IrwBuffer* pIrwBuffer = new IrwBuffer();
          Row* pRow = pIrwBuffer->aAllotRow(m_pStreamDef->pColumnVec(0));
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
        
      case CN_ENTBUFFER: {
        EntBuffer* pEntBuffer = _pEntBuffer(pOperand) ;
        m_nCount += pEntBuffer->nItems();
        break;
      }
        
      case CN_IRWBUFFER: {
        IrwBuffer* pIrwBuffer = _pIrwBuffer(pOperand) ;
        m_nCount += pIrwBuffer->nItems();
        break;
      }
        
      case CN_REFBUFFER: {
        RefBuffer* pRefBuffer = _pRefBuffer(pOperand) ;
        m_nCount += pRefBuffer->nItems();
        delete pRefBuffer;
        break;
      }
        
      case CN_SRTBUFFER: {
        SrtBuffer* pSrtBuffer = _pSrtBuffer(pOperand);
        m_nCount += pSrtBuffer->nItems();
        delete pSrtBuffer;
        break;
      }
        
      case CN_STREAMDEF: {
        delete _pStreamDef(pOperand);
        break;
      }
        
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
