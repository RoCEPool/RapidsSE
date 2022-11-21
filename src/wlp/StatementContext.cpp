/*============================================================================*/
/* StatementContext.hpp - State of the current statement for the AdaptorWL    */
/*============================================================================*/

#include "BASE.h"
#include "StatementContext.hpp"

using namespace WLP;
/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(WLP,StatementContext,CN_STATEMENTCONTEXT)
__aNewFunction(StatementContext,CN_STATEMENTCONTEXT)
__vDeleteFunction(StatementContext,CN_STATEMENTCONTEXT)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StatementContext::StatementContext(Stz* szStmtStart, i8 iLen, Stz* szId, i8 iKey)
: Object(CN_STATEMENTCONTEXT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STATEMENTCONTEXT,_FOOTER_);

  m_sStmtBuf.vCopy(szStmtStart, iLen);
  m_sStmtId  = szId;
  m_iStmtKey = iKey;
  m_bGotStmtError = false;
  m_iAffectedRows = 0;
  m_eVerb = NONE;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STATEMENTCONTEXT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StatementContext::~StatementContext(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STATEMENTCONTEXT,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Return the statement text                                                  */
/*----------------------------------------------------------------------------*/
Stz* StatementContext::szStmt()
{
  return m_sStmtBuf.szStz();
}

/*----------------------------------------------------------------------------*/
/* Return the statement identifier                                            */
/*----------------------------------------------------------------------------*/
Stz* StatementContext::szStmtId()
{
  return m_sStmtId.szStz();
}

/*----------------------------------------------------------------------------*/
/* Return the statement key (secret token)                                    */
/*----------------------------------------------------------------------------*/
i8 StatementContext::iStmtId()
{
  return m_iStmtKey;
}

/*----------------------------------------------------------------------------*/
/* Return whether an error has been seen in the execution of this statement   */
/*----------------------------------------------------------------------------*/
bool StatementContext::bGotError()
{
  return m_bGotStmtError;
}

/*----------------------------------------------------------------------------*/
/* Return whether an error has been seen in the execution of this statement   */
/*----------------------------------------------------------------------------*/
void StatementContext::vGotError(bool bError)
{
  m_bGotStmtError = bError;
}

/*----------------------------------------------------------------------------*/
/* Return the count of rows affected by this query                            */
/*----------------------------------------------------------------------------*/
i8 StatementContext::nAffectedRows()
{
  return m_iAffectedRows;
}

/*----------------------------------------------------------------------------*/
/* Set the count of rows affected by this query                               */
/*----------------------------------------------------------------------------*/
void StatementContext::vAffectedRows(i8 nRows)
{
  m_iAffectedRows = nRows;
}

/*----------------------------------------------------------------------------*/
/* Return the affected row verb                                               */
/*----------------------------------------------------------------------------*/
StatementContext::eAffectedVerb StatementContext::eVerb()
{
  return m_eVerb;
}

/*----------------------------------------------------------------------------*/
/* Set the affected row verb                                                  */
/*----------------------------------------------------------------------------*/
void StatementContext::vVerb(StatementContext::eAffectedVerb eVerb)
{
  m_eVerb = eVerb;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(StatementContext);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
