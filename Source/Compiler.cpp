/*============================================================================*/
/* Compiler.cpp -- Sen Word Compiler                                          */
/*============================================================================*/

#include "SenVM.h"

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
  
  m_pCurrent   = nullptr;
  m_bCompiling = false;
  m_eMode      = CM_INTERPRETING;

  m_pTokeniser = new Tokeniser;
  m_pTokeniser->vPush();
  m_pTokeniser->vUseString(""_Stz);
  
  m_nLocals    = 0;
  for (i8 i_Nested = 0; i_Nested < C_MAX_NESTED_WORDS; i_Nested++)
    m_paWord[i_Nested] = nullptr;

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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is the compiler compiling?                                                 */
/*----------------------------------------------------------------------------*/
bool Compiler::bCompiling(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  bool bIsCompiling = CM_COMPILING == m_eMode;
  
  return bIsCompiling;
}

/*----------------------------------------------------------------------------*/
/* get current word                                                           */
/*----------------------------------------------------------------------------*/
Word* Compiler::pCurrentWord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  return m_pCurrent;
}

/*----------------------------------------------------------------------------*/
/* set current word                                                           */
/*----------------------------------------------------------------------------*/
void Compiler::vCurrentWord(Word* pWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  m_pCurrent = pWord;
}

/*----------------------------------------------------------------------------*/
/* is the compiler interpreting?                                              */
/*----------------------------------------------------------------------------*/
bool Compiler::bInterpreting(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  bool bIsInterpreting = CM_INTERPRETING == m_eMode;
  
  return bIsInterpreting;
}

/*----------------------------------------------------------------------------*/
/* get number of locals                                                       */
/*----------------------------------------------------------------------------*/
i8 Compiler::nLocals(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  return m_nLocals;
}

/*----------------------------------------------------------------------------*/
/* set number of locals                                                       */
/*----------------------------------------------------------------------------*/
void Compiler::vLocals(i8 nLocals)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  m_nLocals = nLocals;
}

/*----------------------------------------------------------------------------*/
/* get mode                                                                   */
/*----------------------------------------------------------------------------*/
eCM Compiler::eMode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  return m_eMode;
}

/*----------------------------------------------------------------------------*/
/* set mode                                                                   */
/*----------------------------------------------------------------------------*/
void Compiler::vMode(eCM eMode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  m_eMode = eMode;
}

/*----------------------------------------------------------------------------*/
/* get nested word                                                            */
/*----------------------------------------------------------------------------*/
Word* Compiler::pNestedWord(i8 i_Nested)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  Word* pNested = m_paWord[i_Nested];
  
  return pNested;
}

/*----------------------------------------------------------------------------*/
/* set nested word                                                            */
/*----------------------------------------------------------------------------*/
void Compiler::vNestedWord(i8 i_Nested, Word* pNested)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  m_paWord[i_Nested] = pNested;
}

/*----------------------------------------------------------------------------*/
/* advance to next token                                                      */
/*----------------------------------------------------------------------------*/
void Compiler::vNextToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  m_pTokeniser->vNextToken();
}

/*----------------------------------------------------------------------------*/
/* get current token                                                          */
/*----------------------------------------------------------------------------*/
Stz* Compiler::szToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  return m_pTokeniser->szToken();
}

/*----------------------------------------------------------------------------*/
/* get tokeniser                                                              */
/*----------------------------------------------------------------------------*/
Tokeniser* Compiler::pTokeniser(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  return m_pTokeniser;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Compiler)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
