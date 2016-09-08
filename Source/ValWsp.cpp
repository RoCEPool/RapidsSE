/*============================================================================*/
/* ValWsp.cpp - workspace for moving values to/from thread and task stacks    */
/*============================================================================*/

#include "Values.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ValWsp::operator new(size_t iSize)
{
  vFatal("fatal: ValWsp class cannot be new'd\n");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ValWsp::ValWsp(void)
: Value(CN_VALWSP)
{
//  Memory::vZero(m_mArray,C_VALWSP_ARRAY);
//  
//  Memory::vCheckInitialised(this,sizeof(ValWsp));
}

/*----------------------------------------------------------------------------*/
/* assign/copy a pointer into workspace                                       */
/*----------------------------------------------------------------------------*/
void ValWsp::operator =(i8 iInt)
{
  vClass(CN_INTVAL);
  
  *pAsIntVal() = iInt;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a pointer into workspace                                       */
/*----------------------------------------------------------------------------*/
void ValWsp::operator =(void* pPtr)
{
  vClass(CN_PTRVAL);
  
  *pAsPtrVal() = pPtr;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a string into workspace                                        */
/*----------------------------------------------------------------------------*/
void ValWsp::operator =(char* pStr)
{
  vClass(CN_STRVAL);

  *pAsStrVal() = pStr;
}

/*----------------------------------------------------------------------------*/
/* assign/copy a string into workspace                                        */
/*----------------------------------------------------------------------------*/
void ValWsp::operator =(const char* pStr)
{
  vClass(CN_STRVAL);

  *pAsStrVal() = pStr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/