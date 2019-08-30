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
Calculator::Calculator(void)
: Object(CN_CALCULATOR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_eaCode, sizeof(m_eaCode), CN_CALCULATOR);
  MemLib::vZero(m_iaData,sizeof(m_iaData),CN_CALCULATOR);
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
  
  m_eaCode[0] = EO_PUSH1;
  m_eaCode[1] = EO_PUSH1;
  m_eaCode[2] = EO_ADD;
  m_eaCode[3] = EO_RETURN;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* compute                                                                    */
/*----------------------------------------------------------------------------*/
i8 Calculator::iCompute(Row *pRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
  m_iaData[0] = C_DATUM_NULL;
  
  i8 iArgA;
  i8 iArgB;
  i8 iRes;

  eEO  eCode;
  i8   i_TopVal = 0;
  eEO* peCode = & m_eaCode[0];
  
  for (;;) {
  
    eCode = *peCode++;
    
    switch (eCode) {
        
      case EO_ADD:
        iArgB = m_iaData[i_TopVal--];
        iArgA = m_iaData[i_TopVal--];
        iRes  = iArgA + iArgB;
        m_iaData[++i_TopVal] = iRes;
        break;
        
      case EO_COLUMN:
        break;
        
      case EO_DIVIDE:
        iArgB = m_iaData[i_TopVal--];
        iArgA = m_iaData[i_TopVal--];
        iRes  = iArgA / iArgB;
        m_iaData[++i_TopVal] = iRes;
        break;
        
      case EO_INTEGER:
        break;
        
      case EO_MULTIPLY:
        iArgB = m_iaData[i_TopVal--];
        iArgA = m_iaData[i_TopVal--];
        iRes  = iArgA * iArgB;
        m_iaData[++i_TopVal] = iRes;
        break;
        
      case EO_NEGATE:
        iArgA = m_iaData[i_TopVal--];
        iRes  = - iArgA;
        m_iaData[++i_TopVal] = iRes;
        break;
        
      case EO_PUSH1:
        m_iaData[++i_TopVal] = 1;
        break;
        
      case EO_RETURN:
        return m_iaData[i_TopVal];
        
      case EO_SUBTRACT:
        iArgB = m_iaData[i_TopVal--];
        iArgA = m_iaData[i_TopVal--];
        iRes  = iArgA - iArgB;
        m_iaData[++i_TopVal] = iRes;
        break;

      default:
        vTrace(" unknown opcode in calculator\n"_Stz);
        return C_DATUM_NULL;
    }
  }
  
  return m_iaData[0];
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Calculator);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
