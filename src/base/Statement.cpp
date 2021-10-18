/*============================================================================*/
/* Statement.cpp -- Medium Text                                              */
/*============================================================================*/

#include "BASE.h"

#include "Statement.hpp"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Statement* BASE::_pStatement(void* aAddr)
{
  Statement* pStatement = reinterpret_cast<Statement*>(aAddr);
  
  RASSERT_TYPE(pStatement,CN_STATEMENT);
  
  return pStatement;
}

/*----------------------------------------------------------------------------*/
/* new a statement from the heap                                              */
/*----------------------------------------------------------------------------*/
void* Statement::operator new(size_t)
{
  void* pObject = Heaps::pAllocate(CN_STATEMENT);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Statement::operator delete(void* pAddress)
{
  Statement* pStatement = reinterpret_cast<Statement*>(pAddress);
  
  Heaps::vFree(pStatement);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Statement::Statement(void)
: Object(CN_STATEMENT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STATEMENT);
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caText,C_SIZE_MEDIUM_TEXT,CN_STATEMENT);
#else
  m_caText[0] = '\0';
#endif

  _checkInitialised_(CN_STATEMENT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STATEMENT);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Statement::Statement(Stz* szText)
: Object(CN_STATEMENT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STATEMENT);
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caText,C_SIZE_MEDIUM_TEXT,CN_STATEMENT);
#else
  m_caText[0] = '\0';
#endif
  
  StrLib::vCopyLen(m_caText,szText->azChar(),C_SIZE_MEDIUM_TEXT-1);

  _checkInitialised_(CN_STATEMENT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STATEMENT);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Statement::~Statement(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STATEMENT);
}

/*----------------------------------------------------------------------------*/
/* get text to given value                                                    */
/*----------------------------------------------------------------------------*/
Stz* Statement::szText(void)
{
  return (Stz*)&m_caText[0];
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Statement::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TEXT);

  return &m_xFooter;
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
