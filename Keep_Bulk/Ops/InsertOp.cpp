/*============================================================================*/
/* InsertOp.cpp -- Limit                                                       */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_Insert (
  CN_INSERTOP,        // class number
  OK_SINGLE,          // op kind
  "Insert"_Stz,       // first word of prompt command
  "Ins"_Stz,          // prefix
                   
  false,              // partitioned
  false,              // compiler op

  true,               // accepts CrwBuffer?
  true,               // accepts EntBuffer?
  true,               // accepts IrwBuffer?
  true,               // accepts SrtBuffer?
                   
  true,               // accepts CrwBuffer?
  false,              // accepts EntBuffer?
  true,               // accepts IrwBuffer?
  true,               // accepts SrtBuffer?

  CN_NONE,            // what kind of output
                   
  InsertOp::pNew,      // function which parses the command and new's an op
  InsertOp::eCall,     // function which calls the op loop
  InsertOp::eDrain,    // function which calls the drain loop

  "limit <nRows>"_Stz
  );


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
InsertOp* DataOperators::_pInsertOp(void* aAddr)
{
  InsertOp* pInsertOp = reinterpret_cast<InsertOp*>(aAddr);
  
  return pInsertOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* InsertOp::operator new(size_t)
{
  InsertOp* pInsertOp = static_cast<InsertOp*>(HeapMem::pAllocate(CN_INSERTOP));
  
  return pInsertOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void InsertOp::operator delete(void* aAddress)
{
  InsertOp* pInsertOp = static_cast<InsertOp*>(aAddress);
  
  HeapMem::vFree(pInsertOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
InsertOp::InsertOp(OpSpec* pOpSpec, Pipeline* pPipeline)
: Op(CN_INSERTOP,pOpSpec,pPipeline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INSERTOP,_FOOTER_);
  
  m_sTableNm  = C_NO_NAME;
  m_pTable    = nullptr;
  m_iSlot     = 0;
  m_pTablePtn = nullptr;

  m_fpCall = InsertOp::eCall;
  
  _checkInitialised_(CN_INSERTOP);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_INSERTOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
InsertOp::~InsertOp(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_INSERTOP);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* InsertOp::pNew(OpSpec* pOpSpec, Pipeline* pPipeline)
{
  InsertOp* pInsertOp = new InsertOp(pOpSpec,pPipeline);
 
  pInsertOp->m_sTableNm = pInsertOp->m_pOpSpec->pEnameSet()->szValue((i8)0);

  return pInsertOp;
}

/*----------------------------------------------------------------------------*/
/* call op loop                                                               */
/*----------------------------------------------------------------------------*/
eOR InsertOp::eCall(void* aThisOp)
{
  eOR eCallAgain = ((InsertOp*)aThisOp)->eOpLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/* call drain loop                                                            */
/*----------------------------------------------------------------------------*/
eOR InsertOp::eDrain(void* aThisOp)
{
  eOR eCallAgain = ((InsertOp*)aThisOp)->eDrainLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* op  loop                                                                   */
/*----------------------------------------------------------------------------*/
eOR InsertOp::eOpLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INSERTOP,_FOOTER_);
  
  vPreliminary();
  
  for (;;) {
    
    Operand* pOperand = pPopQueue();
    
    if (! pOperand) {
      ASSERT(bIsLocked());
      return OR_BIDE;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    switch (pOperand->eClass()) {
  
      // -- -- -- -- -- --
        
      case CN_CRWBUFFER: {
        CrwBuffer* pCrwBuffer = _pCrwBuffer(pOperand);
        vInsertBuffer(pCrwBuffer);
        break;
      }
        
      // -- -- -- -- -- --
        
      case CN_ENDMARKER:
        // pipeout the end marker
        vPipeOut(pOperand);
        goto L_EXIT;
        
        break;

      // -- -- -- -- -- --
        
      case CN_ENTBUFFER:
        NEVER_REACH;
        break;

      // -- -- -- -- -- --
        
      case CN_IRWBUFFER:
        NEVER_REACH;
        break;

      // -- -- -- -- -- --
        
      case CN_PAUSEMARKER:
        NEVER_REACH;
        break;

      // -- -- -- -- -- --
        
      case CN_SRTBUFFER:
        NEVER_REACH;
        break;

      // -- -- -- -- -- --
        
      case CN_STREAMDEF:
        // pipeout the streamdef as is
        vPipeOut(pOperand);
        break;
      
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
/* drain loop                                                                 */
/*----------------------------------------------------------------------------*/
eOR InsertOp::eDrainLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INSERTOP,_FOOTER_);
  
  for (;;) {
    
    Operand* pOperand = pPopQueue();
    
    if (! pOperand) {
      ASSERT(bIsLocked());
      return OR_BIDE;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    switch (pOperand->eClass()) {
        
      case CN_CRWBUFFER:
        break;
        
      case CN_ENDMARKER:
        vPipeOut(pOperand);
        goto L_EXIT;
        
        // -- -- -- -- -- --
        
      case CN_PAUSEMARKER:
        delete (PauseMarker*)pOperand;
        break;
        
      case CN_STREAMDEF: {
        StreamDef* pStreamDef = _pStreamDef(pOperand);
        if (pStreamDef->m_bTemp)
          delete pStreamDef;
        break;
      }
        
      default:
        break;
    }
    
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_EXIT:
  return OR_EXIT;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* insert buffer of rows into table                                           */
/*----------------------------------------------------------------------------*/
void InsertOp::vInsertBuffer(CrwBuffer* pCrwBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INSERTOP,_FOOTER_);
  
  i8 iTxId = 0;

  do {;} while (! m_pTablePtn->bGrabRead());
  
  m_pTable->vInsertEntries(iTxId,m_iSlot,pCrwBuffer);
  
  m_pTablePtn->vDropRead();
}

/*----------------------------------------------------------------------------*/
/* find table preliminary                                                     */
/*----------------------------------------------------------------------------*/
void InsertOp::vPreliminary(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INSERTOP,_FOOTER_);
  
  //! ALKA TO COME BACK TO THIS
  DicStack* pDicStack = new DicStack;
  //AM::vJobDicStack(m_pJobPipeline, pDicStack);
  
  // find the table in the session dictionary stack
  m_pTable = Table::pFindTable(pDicStack, m_sTableNm);
  
  // convert partition to node slot
  m_iSlot = TablePtn::iPartitionToSlot(m_nPartition);

  // get table ptn
  m_pTablePtn = m_pTable->pSlots()->pPartition(m_iSlot);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(InsertOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
