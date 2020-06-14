/*============================================================================*/
/* Clause_SubQry.cpp -- Sub-Query Clause operand                              */
/*============================================================================*/

#include "CompilerStream.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(CompilerStream,Clause_SubQry,CN_CLAUSE_SUBQRY)
__aNewFunction(Clause_SubQry,CN_CLAUSE_SUBQRY)
__vDeleteFunction(Clause_SubQry,CN_CLAUSE_SUBQRY)
__aFooterFunction(Clause_SubQry)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Clause_SubQry::Clause_SubQry(void)
: Clause(CN_CLAUSE_SUBQRY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_SUBQRY,_FOOTER_);

  m_sName = "subqry"_Stz;
  
  m_iAtDepth = C_UNKNOWN;
  m_sSubQryNbr = ""_Stz;
  m_pParentSubQry = nullptr;
  
  m_pSelect  = nullptr;
  m_pFrom    = nullptr;
  m_pWhere   = nullptr;
  m_pGroupBy = nullptr;
  m_pOrderBy = nullptr;
  m_pLimit   = nullptr;
  
  _checkInitialised_(CN_CLAUSE_SUBQRY);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_SUBQRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Clause_SubQry::~Clause_SubQry(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_SUBQRY,_FOOTER_);
  
  delete m_pSelect;
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
bool Clause_SubQry::bParse(Descend* pDescend, Clause_SubQry* pSubQry)
{
  _static_
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get parent sub query                                                       */
/*----------------------------------------------------------------------------*/
Clause_SubQry* Clause_SubQry::pParentSubQry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_SUBQRY,_FOOTER_);

  return m_pParentSubQry;
}

/*----------------------------------------------------------------------------*/
/* set parent sub query                                                       */
/*----------------------------------------------------------------------------*/
void Clause_SubQry::vParentSubQry(Clause_SubQry* pParentSubQry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_SUBQRY,_FOOTER_);

  m_pParentSubQry = pParentSubQry;
  
  m_sSubQryNbr = m_pParentSubQry->szSubQryNbr();
}

/*----------------------------------------------------------------------------*/
/* get depth of this sub query                                                */
/*----------------------------------------------------------------------------*/
i8 Clause_SubQry::iAtDepth(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_SUBQRY,_FOOTER_);

  return m_iAtDepth;
}

/*----------------------------------------------------------------------------*/
/* set depth of this sub query                                                */
/*----------------------------------------------------------------------------*/
void Clause_SubQry::vAtDepth(i8 iAtDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_SUBQRY,_FOOTER_);

  m_iAtDepth = iAtDepth;
  
  if (m_iAtDepth)
    StrLib::vConcat(m_sSubQryNbr.azChar(),'.');
  
  StrLib::vConcat(m_sSubQryNbr.azChar(),iAtDepth);
}

/*----------------------------------------------------------------------------*/
/* get depth of this sub query                                                */
/*----------------------------------------------------------------------------*/
Stz* Clause_SubQry::szSubQryNbr(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_SUBQRY,_FOOTER_);

  return m_sSubQryNbr;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get select                                                                 */
/*----------------------------------------------------------------------------*/
Clause_Select* Clause_SubQry::pSelect(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_SUBQRY,_FOOTER_);

  return m_pSelect;
}

/*----------------------------------------------------------------------------*/
/* put select                                                                 */
/*----------------------------------------------------------------------------*/
void Clause_SubQry::vSelect(Clause_Select* pSelect)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_SUBQRY,_FOOTER_);

  m_pSelect = pSelect;
}

/*----------------------------------------------------------------------------*/
/* get from                                                                   */
/*----------------------------------------------------------------------------*/
Clause_From* Clause_SubQry::pFrom(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_SUBQRY,_FOOTER_);

  return m_pFrom;
}

/*----------------------------------------------------------------------------*/
/* put from                                                                   */
/*----------------------------------------------------------------------------*/
void Clause_SubQry::vFrom(Clause_From* pFrom)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_SUBQRY,_FOOTER_);

  m_pFrom = pFrom;
}

/*----------------------------------------------------------------------------*/
/* get where                                                                  */
/*----------------------------------------------------------------------------*/
Clause_Where* Clause_SubQry::pWhere(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_SUBQRY,_FOOTER_);

  return m_pWhere;
}

/*----------------------------------------------------------------------------*/
/* put where                                                                  */
/*----------------------------------------------------------------------------*/
void Clause_SubQry::vWhere(Clause_Where* pWhere)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_SUBQRY,_FOOTER_);

  m_pWhere = pWhere;
}

/*----------------------------------------------------------------------------*/
/* get groupBy                                                                */
/*----------------------------------------------------------------------------*/
Clause_GroupBy* Clause_SubQry::pGroupBy(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_SUBQRY,_FOOTER_);

  return m_pGroupBy;
}

/*----------------------------------------------------------------------------*/
/* put groupBy                                                                */
/*----------------------------------------------------------------------------*/
void Clause_SubQry::vGroupBy(Clause_GroupBy* pGroupBy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_SUBQRY,_FOOTER_);

  m_pGroupBy = pGroupBy;
}

/*----------------------------------------------------------------------------*/
/* get orderBy                                                                */
/*----------------------------------------------------------------------------*/
Clause_OrderBy* Clause_SubQry::pOrderBy(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_SUBQRY,_FOOTER_);

  return m_pOrderBy;
}

/*----------------------------------------------------------------------------*/
/* put orderBy                                                                */
/*----------------------------------------------------------------------------*/
void Clause_SubQry::vOrderBy(Clause_OrderBy* pOrderBy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_SUBQRY,_FOOTER_);

  m_pOrderBy = pOrderBy;
}

/*----------------------------------------------------------------------------*/
/* get limit                                                                  */
/*----------------------------------------------------------------------------*/
Clause_Limit* Clause_SubQry::pLimit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_SUBQRY,_FOOTER_);

  return m_pLimit;
}

/*----------------------------------------------------------------------------*/
/* put limit                                                                  */
/*----------------------------------------------------------------------------*/
void Clause_SubQry::vOrderBy(Clause_Limit* pLimit)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_SUBQRY,_FOOTER_);

  m_pLimit = pLimit;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
