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
  
  i8* piArg;
  
  i8 i_Code = 0;
  
  m_eaCode[i_Code++] = EO_COLUMN;
  piArg = (i8*) & m_eaCode[i_Code];
  * piArg = 1;
  i_Code += sizeof(i8);

  m_eaCode[i_Code++] = EO_INTEGER;
  piArg = (i8*) & m_eaCode[i_Code];
  * piArg = 41;
  i_Code += sizeof(i8);

  m_eaCode[i_Code++] = EO_ADD;
  m_eaCode[i_Code++] = EO_PUSH1;
  m_eaCode[i_Code++] = EO_ADD;
  m_eaCode[i_Code++] = EO_NEGATE;
  m_eaCode[i_Code++] = EO_RETURN;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* compute                                                                    */
/*----------------------------------------------------------------------------*/
i8 Calculator::iCompute(i8* paColValues)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CALCULATOR,_FOOTER_);
  
  i8 iArgA;
  i8 iArgB;
  i8 iRes;

  eEO  eCode;
  eEO* peCode = & m_eaCode[0];
 
  i8 i_TopVal = 0;
  m_iaData[i_TopVal] = C_DATUM_NULL;

  for (;;) {
  
    eCode = *peCode++;
    
    switch (eCode) {
        
      // -- -- -- -- -- -- -- -- -- -- --
        
      case EO_COLUMN:
        iArgA = * (i8*)peCode;
        peCode += 8;
        iRes  = paColValues[iArgA];
        m_iaData[++i_TopVal] = iRes;
        break;
        
      case EO_INTEGER:
        iRes = * (i8*)peCode;
        peCode += 8;
        m_iaData[++i_TopVal] = iRes;
        break;
        
      case EO_PUSHNULL:
        m_iaData[++i_TopVal] = C_DATUM_NULL;
        break;
        
      case EO_PUSH0:
        m_iaData[++i_TopVal] = 0;
        break;
        
      case EO_PUSH1:
        m_iaData[++i_TopVal] = 1;
        break;
        
      // -- -- -- -- -- -- -- -- -- -- --
        
      case EO_ADD:
        iArgB = m_iaData[i_TopVal--];
        iArgA = m_iaData[i_TopVal--];
        iRes  = iArgA + iArgB;
        m_iaData[++i_TopVal] = iRes;
        break;
        
      case EO_DIVIDE:
        iArgB = m_iaData[i_TopVal--];
        iArgA = m_iaData[i_TopVal--];
        iRes  = iArgA / iArgB;
        m_iaData[++i_TopVal] = iRes;
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
        
      case EO_SUBTRACT:
        iArgB = m_iaData[i_TopVal--];
        iArgA = m_iaData[i_TopVal--];
        iRes  = iArgA - iArgB;
        m_iaData[++i_TopVal] = iRes;
        break;

      // -- -- -- -- -- -- -- -- -- -- --
        
      case EO_LESSTHAN:
        iArgB = m_iaData[i_TopVal--];
        iArgA = m_iaData[i_TopVal--];
        iRes  = iArgA < iArgB;
        m_iaData[++i_TopVal] = iRes;
        break;

      case EO_LESSEQUAL:
        iArgB = m_iaData[i_TopVal--];
        iArgA = m_iaData[i_TopVal--];
        iRes  = iArgA <= iArgB;
        m_iaData[++i_TopVal] = iRes;
        break;
        
      case EO_EQUAL:
        iArgB = m_iaData[i_TopVal--];
        iArgA = m_iaData[i_TopVal--];
        iRes  = iArgA == iArgB;
        m_iaData[++i_TopVal] = iRes;
        break;
        
      case EO_NOTEQUAL:
        iArgB = m_iaData[i_TopVal--];
        iArgA = m_iaData[i_TopVal--];
        iRes  = iArgA != iArgB;
        m_iaData[++i_TopVal] = iRes;
        break;
        
      case EO_GREATEREQUAL:
        iArgB = m_iaData[i_TopVal--];
        iArgA = m_iaData[i_TopVal--];
        iRes  = iArgA >= iArgB;
        m_iaData[++i_TopVal] = iRes;
        break;
        
      case EO_GREATERTHAN:
        iArgB = m_iaData[i_TopVal--];
        iArgA = m_iaData[i_TopVal--];
        iRes  = iArgA > iArgB;
        m_iaData[++i_TopVal] = iRes;
        break;
        
      // -- -- -- -- -- -- -- -- -- -- --
        
      case EO_ISNULL:
        iArgA = m_iaData[i_TopVal--];
        iRes  = (C_DATUM_NULL == iArgA) ? 1 : 0;
        m_iaData[++i_TopVal] = iRes;
        break;

      case EO_ISNOTNULL:
        iArgA = m_iaData[i_TopVal--];
        iRes  = (C_DATUM_NULL != iArgA) ? 1 : 0;
        m_iaData[++i_TopVal] = iRes;
        break;
        
      case EO_NOT:
        iArgA = m_iaData[i_TopVal--];
        iRes  = (! iArgA) ? 1 : 0;
        m_iaData[++i_TopVal] = iRes;
        break;
        
      // -- -- -- -- -- -- -- -- -- -- --
        
      case EO_RETURN:
        iRes = m_iaData[i_TopVal];
        return iRes;
        
      // -- -- -- -- -- -- -- -- -- -- --
        
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
