/*============================================================================*/
/* Stmt_SubQry.cpp -- Sub-Query Clause operand                              */
/*============================================================================*/

#include "CompilerStream.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(CompilerStream,Stmt_SubQry,CN_STMT_SUBQRY)
__aNewFunction(Stmt_SubQry,CN_STMT_SUBQRY)
__vDeleteFunction(Stmt_SubQry,CN_STMT_SUBQRY)
__aFooterFunction(Stmt_SubQry)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Stmt_SubQry::Stmt_SubQry(void)
: Stmt(CN_STMT_SUBQRY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STMT_SUBQRY,_FOOTER_);
  
  m_iSubSeqDgt = C_UNKNOWN;
  m_sSubQryNbr = ""_Stz;
  
  m_iAtDepth   = C_UNKNOWN;
  m_iNxtKidDgt = 0;

  m_pDadSubQry = nullptr;
  
  m_pProject   = nullptr;
  m_pFrom      = nullptr;
  m_pWhere     = nullptr;
  m_pGroupBy   = nullptr;
  m_pOrderBy   = nullptr;
  m_pLimit     = nullptr;
  
  _checkInitialised_(CN_STMT_SUBQRY);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_STMT_SUBQRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Stmt_SubQry::~Stmt_SubQry(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STMT_SUBQRY,_FOOTER_);
  
  delete m_pProject;
  delete m_pFrom;
  delete m_pWhere;
  delete m_pGroupBy;
  delete m_pOrderBy;
  delete m_pLimit;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* descend                                                                    */
/*----------------------------------------------------------------------------*/
bool Stmt_SubQry::bParse(Descend* pDescend, Stmt_SubQry* pDadSubQry)
{
  _static_
  
  // begin new sub-query
  Stmt_SubQry* pSubQry = new Stmt_SubQry;
  
  // nested sub-query?
  if (pDadSubQry) {
    // this new sub-query is nested
    Stz* szQryNbr = pDadSubQry->szSubQryNbr();
    pSubQry->vDadSubQry(pDadSubQry);
    i8 iSubDepth = pDadSubQry->iAtDepth() + 1;
    pSubQry->vAtDepth(iSubDepth);
  }
  else {
    // this sub-query is the root
    pSubQry->vAtDepth(0);
    pSubQry->vSubSeqDgt(0);
  }
  
  // does the statement have a root sub-query?
  Stmt_Select* pSelectStmt = _pStmt_Select(pDescend->pStmt());
  if (! pSelectStmt->pRootSubQry()) {
    pSelectStmt->vRootSubQry(pSubQry);
  }
  
  Token* pCurTkn = pDescend->pCurrToken();
  
  vTrace("%s) SELECT\n"_Stz,pSubQry->szSubQryNbr());
  pDescend->vNextToken();
  
  Clause_Project::bParse(pDescend,pSubQry);

  bool bSubQry;
  while (pDescend->bMoreTokens() && pDescend->bIsDescendParse(bSubQry))
    if (! pDescend->bCallFunc(pSubQry))
      break;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get dad sub query                                                          */
/*----------------------------------------------------------------------------*/
Stmt_SubQry* Stmt_SubQry::pDadSubQry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STMT_SUBQRY,_FOOTER_);

  return m_pDadSubQry;
}

/*----------------------------------------------------------------------------*/
/* set dad sub query                                                          */
/*----------------------------------------------------------------------------*/
void Stmt_SubQry::vDadSubQry(Stmt_SubQry* pDadSubQry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STMT_SUBQRY,_FOOTER_);

  m_pDadSubQry = pDadSubQry;
  
  m_sSubQryNbr = m_pDadSubQry->szSubQryNbr();
}

/*----------------------------------------------------------------------------*/
/* get depth of this sub query                                                */
/*----------------------------------------------------------------------------*/
i8 Stmt_SubQry::iAtDepth(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STMT_SUBQRY,_FOOTER_);

  return m_iAtDepth;
}

/*----------------------------------------------------------------------------*/
/* set depth of this sub query                                                */
/*----------------------------------------------------------------------------*/
void Stmt_SubQry::vAtDepth(i8 iAtDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STMT_SUBQRY,_FOOTER_);

  m_iAtDepth = iAtDepth;
}

/*----------------------------------------------------------------------------*/
/* get next kid digit                                                         */
/*----------------------------------------------------------------------------*/
i8 Stmt_SubQry::iNextKidDgt(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STMT_SUBQRY,_FOOTER_);

  return m_iNxtKidDgt++;
}

/*----------------------------------------------------------------------------*/
/* get depth of this sub query                                                */
/*----------------------------------------------------------------------------*/
Stz* Stmt_SubQry::szSubQryNbr(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STMT_SUBQRY,_FOOTER_);

  return m_sSubQryNbr;
}

/*----------------------------------------------------------------------------*/
/* get depth of this sub query                                                */
/*----------------------------------------------------------------------------*/
i8 Stmt_SubQry::iSubSeqDgt(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STMT_SUBQRY,_FOOTER_);

  return m_iSubSeqDgt;
}

/*----------------------------------------------------------------------------*/
/* set depth of this sub query                                                */
/*----------------------------------------------------------------------------*/
void Stmt_SubQry::vSubSeqDgt(i8 iSubSeqDgt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STMT_SUBQRY,_FOOTER_);

  m_iSubSeqDgt = iSubSeqDgt;
  
  if (m_iAtDepth)
    StrLib::vConcat(m_sSubQryNbr.azChar(),'.');
  
  StrLib::vConcat(m_sSubQryNbr.azChar(),m_iSubSeqDgt);
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get from                                                                   */
/*----------------------------------------------------------------------------*/
Clause_From* Stmt_SubQry::pFrom(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STMT_SUBQRY,_FOOTER_);

  return m_pFrom;
}

/*----------------------------------------------------------------------------*/
/* put from                                                                   */
/*----------------------------------------------------------------------------*/
void Stmt_SubQry::vFrom(Clause_From* pFrom)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STMT_SUBQRY,_FOOTER_);

  m_pFrom = pFrom;
}

/*----------------------------------------------------------------------------*/
/* get where                                                                  */
/*----------------------------------------------------------------------------*/
Clause_Where* Stmt_SubQry::pWhere(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STMT_SUBQRY,_FOOTER_);

  return m_pWhere;
}

/*----------------------------------------------------------------------------*/
/* put where                                                                  */
/*----------------------------------------------------------------------------*/
void Stmt_SubQry::vWhere(Clause_Where* pWhere)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STMT_SUBQRY,_FOOTER_);

  m_pWhere = pWhere;
}

/*----------------------------------------------------------------------------*/
/* get groupBy                                                                */
/*----------------------------------------------------------------------------*/
Clause_GroupBy* Stmt_SubQry::pGroupBy(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STMT_SUBQRY,_FOOTER_);

  return m_pGroupBy;
}

/*----------------------------------------------------------------------------*/
/* put groupBy                                                                */
/*----------------------------------------------------------------------------*/
void Stmt_SubQry::vGroupBy(Clause_GroupBy* pGroupBy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STMT_SUBQRY,_FOOTER_);

  m_pGroupBy = pGroupBy;
}

/*----------------------------------------------------------------------------*/
/* get orderBy                                                                */
/*----------------------------------------------------------------------------*/
Clause_OrderBy* Stmt_SubQry::pOrderBy(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STMT_SUBQRY,_FOOTER_);

  return m_pOrderBy;
}

/*----------------------------------------------------------------------------*/
/* put orderBy                                                                */
/*----------------------------------------------------------------------------*/
void Stmt_SubQry::vOrderBy(Clause_OrderBy* pOrderBy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STMT_SUBQRY,_FOOTER_);

  m_pOrderBy = pOrderBy;
}

/*----------------------------------------------------------------------------*/
/* get limit                                                                  */
/*----------------------------------------------------------------------------*/
Clause_Limit* Stmt_SubQry::pLimit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STMT_SUBQRY,_FOOTER_);

  return m_pLimit;
}

/*----------------------------------------------------------------------------*/
/* put limit                                                                  */
/*----------------------------------------------------------------------------*/
void Stmt_SubQry::vOrderBy(Clause_Limit* pLimit)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STMT_SUBQRY,_FOOTER_);

  m_pLimit = pLimit;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
