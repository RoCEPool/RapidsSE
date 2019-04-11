/*============================================================================*/
/* Compiler.cpp -- Sen ForthWord Compiler                                          */
/*============================================================================*/

#include "Tasks.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Compiler::operator new(size_t)
{
  Compiler* pCompiler = reinterpret_cast<Compiler*>(HeapMem::pAllocate(CN_COMPILER));
  
  return pCompiler;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Compiler::operator delete(void* pCompilerAddress)
{
  Compiler* pCompiler = reinterpret_cast<Compiler*>(pCompilerAddress);
  
  HeapMem::vFree(pCompiler);
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
  m_nLocals      = 0;

  m_pRetryIP     = nullptr;
  m_pBeginIP     = nullptr;

  m_pTokeniser = new Tokeniser;
  m_pTokeniser->vPush();
  s48K sString(""_Stz);
  m_pTokeniser->vUseString(sString);

  _checkInitialised_(CN_COMPILER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Compiler::~Compiler(void) noexcept(false)
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
