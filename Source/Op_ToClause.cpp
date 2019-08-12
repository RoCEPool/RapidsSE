/*============================================================================*/
/* Op_ToClause.cpp -- To Clause Operator                                      */
/*============================================================================*/

#include "CompilerOperators.h"

OpDesc __Op_ToClause (
   CN_OP_TOCLAUSE,         // class number
   OK_SIMPLE,           // op kind
   "ToClause"_Stz,      // first word of prompt command
   "Clu"_Stz,           // prefix
   false,               // not partitioned
   Op_ToClause::pNew,    // function which parses the command and new's an op
   Op_ToClause::eCall,   // function which calls the op loop
   Op_ToClause::eDrain,  // function which calls the drain loop
   
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
Op_ToClause::Op_ToClause(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR)
: Op(CN_OP_TOCLAUSE,szOpLabel,pDigest,aOpDefn,bAppR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP_TOCLAUSE,_FOOTER_);
  
  m_fpCall = Op_ToClause::eCall;
  
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
Op* Op_ToClause::pNew(Stz* szLabel,
                   Digest* pDigest, OpDefn* aOpDefn, bool bAppR,
                   TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot)
{
  i_Tkn_++;
  ASSERT(i_Slot == -1);
  
  Op_ToClause* pToClauseOp = new Op_ToClause(szLabel,pDigest,aOpDefn,bAppR);
  
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
        vPipeOut(pOperand);
        goto L_EXIT;
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
/* drain loop                                                                 */
/*----------------------------------------------------------------------------*/
void Op_ToClause::vAsClauses(Stmt_Select* pStmt_Select)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP_TOCLAUSE,_FOOTER_);
  
  TokenVec* pStmtTknVec = pStmt_Select->pTokenVec();
  i8 nStmtTkn = pStmtTknVec->nTokens();
  
  for (i8 i_StmtTkn = 0; i_StmtTkn < nStmtTkn; ) {
    
    Token* pStmtTkn = pStmtTknVec->pGet(i_StmtTkn++);
    
    if (pStmtTkn->bSame("from"_Stz)) {
      Clause_From* pClause_From = new Clause_From(pStmtTknVec,i_StmtTkn);
      vPipeOut(pClause_From);
    }
    else
    if (pStmtTkn->bSame("group"_Stz)) {
      Clause_GroupBy* pClause_GroupBy = new Clause_GroupBy(pStmtTknVec,i_StmtTkn);
      vPipeOut(pClause_GroupBy);
    }
    else
    if (pStmtTkn->bSame("join"_Stz)) {
      Clause_Join* pClause_Join = new Clause_Join(pStmtTknVec,i_StmtTkn);
      vPipeOut(pClause_Join);
    }
    else
    if (pStmtTkn->bSame("limit"_Stz)) {
      Clause_Limit* pClause_Limit = new Clause_Limit(pStmtTknVec,i_StmtTkn);
      vPipeOut(pClause_Limit);
    }
    else
    if (pStmtTkn->bSame("order"_Stz)) {
      Clause_OrderBy* pClause_OrderBy = new Clause_OrderBy(pStmtTknVec,i_StmtTkn);
      vPipeOut(pClause_OrderBy);
    }
    else
    if (pStmtTkn->bSame("select"_Stz)) {
      Clause_Select* pClause_Select = new Clause_Select(pStmtTknVec,i_StmtTkn);
      vPipeOut(pClause_Select);
    }
    else
    if (pStmtTkn->bSame("where"_Stz)) {
      Clause_Where* pClause_Where = new Clause_Where(pStmtTknVec,i_StmtTkn);
      vPipeOut(pClause_Where);
    }
    else {
      i_StmtTkn++;
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Op_ToClause);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
