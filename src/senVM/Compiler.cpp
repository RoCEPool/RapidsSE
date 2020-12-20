/*============================================================================*/
/* Compiler.cpp -- Sen Word Compiler                                          */
/*============================================================================*/

#include "SenVM.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(SenVM,Compiler,CN_COMPILER)
__aNewFunction(Compiler,CN_COMPILER)
__vDeleteFunction(Compiler,CN_COMPILER)
__aFooterFunction(Compiler)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Compiler::Compiler(void)
: Object(CN_COMPILER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  m_pCurrent   = nullptr;
  m_eMode      = CM_INTERPRETING;

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
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is the compiler in compiling mode?                                         */
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
/* is the compiler in interpreting mode?                                      */
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
  
  static i8 nMaxLocals = 0;
  
  if (nLocals > nMaxLocals) {
    nMaxLocals = nLocals;
//    vTrace("                                                      MaxLocals %lld\n"_Stz,nMaxLocals);
  }
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
/* is the compiler in postponing mode?                                        */
/*----------------------------------------------------------------------------*/
bool Compiler::bPostponing(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMPILER,_FOOTER_);
  
  bool bIsPostponing = CM_POSTPONING == m_eMode;
  
  return bIsPostponing;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
