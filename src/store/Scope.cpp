/*============================================================================*/
/* Scope.cpp --Scope                                                          */
/*============================================================================*/

#include "Scope.hpp"

#include "Symbol.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Scope,CN_SCOPE)
__aNewFunction(Scope,CN_SCOPE)
__vDeleteFunction(Scope,CN_SCOPE)
__aFooterFunction(Scope)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Scope::Scope(void)
: Object(CN_SCOPE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCOPE,_FOOTER_);

  MemLib::vZero(m_baKnown,sizeof(m_baKnown),CN_SCOPE);

  _checkInitialised_(CN_SCOPE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SCOPE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Scope::~Scope(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCOPE,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is table in scope?                                                         */
/*----------------------------------------------------------------------------*/
bool Scope::bInScope(Symbol* pSymbol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCOPE,_FOOTER_);

  char cTblSyn = pSymbol->cExtractTableSynonym();

  i8 i_Table = C_UNKNOWN;
  if (cTblSyn >= 'a' && cTblSyn <= 'j')
    i_Table = cTblSyn - 'a';
  else
  if (cTblSyn >= 'A' && cTblSyn <= 'J')
    i_Table = cTblSyn - 'A';
  else
    NEVER_REACH

  bool bInScope = m_baKnown[i_Table];

  return bInScope;
}

/*----------------------------------------------------------------------------*/
/* set table in scope                                                         */
/*----------------------------------------------------------------------------*/
void Scope::vInScope(Symbol* pSymbol, bool bInScope)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCOPE,_FOOTER_);

  char cTblSyn = pSymbol->cSF_synonym();

  i8 i_Table = C_UNKNOWN;
  if (cTblSyn >= 'a' && cTblSyn <= 'j')
    i_Table = cTblSyn - 'a';
  else
  if (cTblSyn >= 'A' && cTblSyn <= 'J')
    i_Table = cTblSyn - 'A';
  else
    NEVER_REACH

  m_baKnown[i_Table] = bInScope;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
