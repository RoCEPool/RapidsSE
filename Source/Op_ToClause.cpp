/*============================================================================*/
/* Op_ToClause.cpp -- To Clause Operator                                      */
/*============================================================================*/

#include "CompilerOperators.h"

OpDesc __Op_ToClause (
  CN_OP_TOCLAUSE,      // class number
  OK_SINGLE,           // op kind
  "ToClause"_Stz,      // first word of prompt command
  "ToClause"_Stz,      // prefix
                    
  false,               // not partitioned
  true,                // compiler op

  false,              // accepts CrwBuffer?
  false,              // accepts EntBuffer?
  false,              // accepts IrwBuffer?
  false,              // accepts SrtBuffer?
                      
  false,              // accepts CrwBuffer?
  false,              // accepts EntBuffer?
  false,              // accepts IrwBuffer?
  false,              // accepts SrtBuffer?

  CN_NONE,            // what kind of output
                    
  Op_ToClause::pNew,   // function which parses the command and new's an op
  Op_ToClause::eCall,  // function which calls the op loop
  Op_ToClause::eDrain, // function which calls the drain loop

  "<help>"_Stz
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Op_ToClause* CompilerOperators::_pOp_ToClause(void* aAddr)
{
  Op_ToClause* pToClauseOp = reinterpret_cast<Op_ToClause*>(aAddr);
  
  return pToClauseOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* Op_ToClause::operator new(size_t)
{
  Op_ToClause* pToClauseOp =
    static_cast<Op_ToClause*>(HeapMem::pAllocate(CN_OP_TOCLAUSE));
  
  return pToClauseOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void Op_ToClause::operator delete(void* aAddress)
{
  Op_ToClause* pToClauseOp = static_cast<Op_ToClause*>(aAddress);
  
  HeapMem::vFree(pToClauseOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Op_ToClause::Op_ToClause(OpSpec* pOpSpec, Pipeline* pPipeline)
: Op(CN_OP_TOCLAUSE,pOpSpec,pPipeline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP_TOCLAUSE,_FOOTER_);
  
  m_fpCall = Op_ToClause::eCall;
  
  m_pClause_From    = nullptr;
  m_pClause_GroupBy = nullptr;
  m_pClause_Join    = nullptr;
  m_pClause_Limit   = nullptr;
  m_pClause_OrderBy = nullptr;
  m_pClause_Select  = nullptr;
  m_pClause_Where   = nullptr;
  
  _checkInitialised_(CN_OP_TOCLAUSE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_OP_TOCLAUSE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Op_ToClause::~Op_ToClause(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP_TOCLAUSE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* Op_ToClause::pNew(OpSpec* pOpSpec, Pipeline* pPipeline)
{
  Op_ToClause* pToClauseOp = new Op_ToClause(pOpSpec,pPipeline);
  
  return pToClauseOp;
}

/*----------------------------------------------------------------------------*/
/* call loop                                                                  */
/*----------------------------------------------------------------------------*/
eOR Op_ToClause::eCall(void* aThisOp)
{
  eOR eCallAgain = ((Op_ToClause*)aThisOp)->eOpLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/* call drain loop                                                            */
/*----------------------------------------------------------------------------*/
eOR Op_ToClause::eDrain(void* aThisOp)
{
  eOR eCallAgain = ((Op_ToClause*)aThisOp)->eDrainLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* op loop                                                                    */
/*----------------------------------------------------------------------------*/
eOR Op_ToClause::eOpLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP_TOCLAUSE,_FOOTER_);
  
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
          vPipeClauses();
          vPipeOut(pOperand);
          goto L_EXIT;
        }
        else
          delete (EndMarker*)pOperand;
        break;
        
      // -- -- -- -- -- --
       
      case CN_STMT_SELECT:
        vAsClauses(_pStmt_Select(pOperand));
        //vPipeOut(pOperand);
        break;
        
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
eOR Op_ToClause::eDrainLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP_TOCLAUSE,_FOOTER_);
  
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
        break;
        
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
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert select statement into clauses                                      */
/*----------------------------------------------------------------------------*/
void Op_ToClause::vAsClauses(Stmt_Select* pStmt_Select)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP_TOCLAUSE,_FOOTER_);
  
  TokenVec* pStmtTknVec = pStmt_Select->pTokenVec();
  i8 nStmtTkn = pStmtTknVec->nTokens();
  
  for (i8 i_StmtTkn = 0; i_StmtTkn < nStmtTkn; ) {
    
    Token* pStmtTkn = pStmtTknVec->pGet(i_StmtTkn++);
    
    if (pStmtTkn->bSame("from"_Stz))
      m_pClause_From = new Clause_From(pStmtTknVec,i_StmtTkn);
    else
    if (pStmtTkn->bSame("group"_Stz))
      m_pClause_GroupBy = new Clause_GroupBy(pStmtTknVec,i_StmtTkn);
    else
    if (pStmtTkn->bSame("join"_Stz))
      m_pClause_Join = new Clause_Join(pStmtTknVec,i_StmtTkn);
    else
    if (pStmtTkn->bSame("limit"_Stz))
      m_pClause_Limit = new Clause_Limit(pStmtTknVec,i_StmtTkn);
    else
    if (pStmtTkn->bSame("order"_Stz))
      m_pClause_OrderBy = new Clause_OrderBy(pStmtTknVec,i_StmtTkn);
    else
    if (pStmtTkn->bSame("select"_Stz))
      m_pClause_Select = new Clause_Select(pStmtTknVec,i_StmtTkn);
    else
    if (pStmtTkn->bSame("where"_Stz))
      m_pClause_Where = new Clause_Where(pStmtTknVec,i_StmtTkn);
    else
      i_StmtTkn++;    
  }
}

/*----------------------------------------------------------------------------*/
/* pipe out the clauses                                                       */
/*----------------------------------------------------------------------------*/
void Op_ToClause::vPipeClauses(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP_TOCLAUSE,_FOOTER_);
  
  if (m_pClause_From)    vPipeOut(m_pClause_From);
  if (m_pClause_Join)    vPipeOut(m_pClause_Join);
  if (m_pClause_Select)  vPipeOut(m_pClause_Select);
  if (m_pClause_GroupBy) vPipeOut(m_pClause_GroupBy);
  if (m_pClause_Where)   vPipeOut(m_pClause_Where);
  if (m_pClause_OrderBy) vPipeOut(m_pClause_OrderBy);
  if (m_pClause_Limit)   vPipeOut(m_pClause_Limit);
  
  m_pClause_From    = nullptr;
  m_pClause_GroupBy = nullptr;
  m_pClause_Join    = nullptr;
  m_pClause_Limit   = nullptr;
  m_pClause_OrderBy = nullptr;
  m_pClause_Select  = nullptr;
  m_pClause_Where   = nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Op_ToClause);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
