/*============================================================================*/
/* Calculator.cpp -- Calculator                                               */
/*============================================================================*/

#include "Operators.h"


/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Calculator* Operators::_pCalculator(void* aAddr)
{
  Calculator* pCalculator = reinterpret_cast<Calculator*>(aAddr);
  
  RASSERT_TYPE(pCalculator,CN_CALCULATOR);
  
  return pCalculator;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* Calculator::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_CALCULATOR);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void Calculator::operator delete(void* aAddr)
{
  Calculator* pCalculator = reinterpret_cast<Calculator*>(aAddr);
  
  HeapMem::vFree(pCalculator);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Calculator::Calculator(Job* pJob)
: Object(CN_CALCULATOR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_baExpr,sizeof(m_baExpr),CN_CALCULATOR);
#endif
  
  _checkInitialised_(CN_CALCULATOR);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Calculator::~Calculator(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compile                                                                    */
/*----------------------------------------------------------------------------*/
bool Calculator::bCompile(TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
  m_baExpr[0] = EO_RETURN;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* compute                                                                    */
/*----------------------------------------------------------------------------*/
i8 Calculator::iCompute(Row *pRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
  m_iaStack[0] = C_UNKNOWN;
  
  return m_iaStack[0];
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Calculator);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
