/*============================================================================*/
/* Compiler.cpp -- Sen Word Compiler                                          */
/*============================================================================*/

#include "Tasks.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Compiler::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Compiler* pCompiler = reinterpret_cast<Compiler*>(Heaps::pAllocate(CN_COMPILER));
  
  return pCompiler;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Compiler::operator delete(void* pCompilerAddress)
{
  Compiler* pCompiler = reinterpret_cast<Compiler*>(pCompilerAddress);
  
  Heaps::vFree(pCompiler);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Compiler::Compiler(void)
: Object(CN_COMPILER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  m_bCompiling   = false;
  m_pCurrentWord = nullptr;
  m_nSubDic      = 0;
  m_nVariables   = 0;

  m_pTokeniser = new Tokeniser;
  m_pTokeniser->vPush();
  s32 sString(""_Stz);
  m_pTokeniser->vUseString(sString);

  _checkInitialised_(CN_COMPILER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Compiler::~Compiler(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  delete m_pTokeniser;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Compiler::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
