/*============================================================================*/
/* SenCompiler.cpp -- Sen Word Compiler                                       */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,SenCompiler,CN_SENCOMPILER)
__aNewFunction(SenCompiler,CN_SENCOMPILER)
__vDeleteFunction(SenCompiler,CN_SENCOMPILER)
__aFooterFunction(SenCompiler)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SenCompiler::SenCompiler(void)
: Object(CN_SENCOMPILER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENCOMPILER,_FOOTER_);
  
  m_pCurrent   = nullptr;
  m_eMode      = CM_INTERPRETING;

  m_nLocals    = 0;
  m_aiLocals   = nullptr;
  
  m_bEnterExitTrace = false;
  
  for (i8 i_Nested = 0; i_Nested < C_MAX_NESTED_WORDS; i_Nested++)
    m_paWord[i_Nested] = nullptr;

  _checkInitialised_(CN_SENCOMPILER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SENCOMPILER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SenCompiler::~SenCompiler(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENCOMPILER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is the compiler in compiling mode?                                         */
/*----------------------------------------------------------------------------*/
bool SenCompiler::bCompiling(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENCOMPILER,_FOOTER_);
  
  bool bIsCompiling = CM_COMPILING == m_eMode;
  
  return bIsCompiling;
}

/*----------------------------------------------------------------------------*/
/* get current word                                                           */
/*----------------------------------------------------------------------------*/
Word* SenCompiler::pCurrentWord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENCOMPILER,_FOOTER_);
  
  return m_pCurrent;
}

/*----------------------------------------------------------------------------*/
/* set current word                                                           */
/*----------------------------------------------------------------------------*/
void SenCompiler::vCurrentWord(Word* pWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENCOMPILER,_FOOTER_);
  
  m_pCurrent = pWord;
}

/*----------------------------------------------------------------------------*/
/* encode entry to sen function                                               */
/*----------------------------------------------------------------------------*/
void SenCompiler::vEncodeEnter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENCOMPILER,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/* encode exit of sen function                                                */
/*----------------------------------------------------------------------------*/
void SenCompiler::vEncodeExit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENCOMPILER,_FOOTER_);
 
  m_pCurrent->vEncodeCall("exit_RT"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get enter/exit trace of word                                               */
/*----------------------------------------------------------------------------*/
bool SenCompiler::bEnterExitTrace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENCOMPILER,_FOOTER_);
  
  return m_bEnterExitTrace;
}

/*----------------------------------------------------------------------------*/
/* set enter/exit trace of word                                               */
/*----------------------------------------------------------------------------*/
void SenCompiler::vEnterExitTrace(bool bExterExitTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENCOMPILER,_FOOTER_);
  
  m_bEnterExitTrace = bExterExitTrace;
}

/*----------------------------------------------------------------------------*/
/* is the compiler in interpreting mode?                                      */
/*----------------------------------------------------------------------------*/
bool SenCompiler::bInterpreting(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENCOMPILER,_FOOTER_);
  
  bool bIsInterpreting = CM_INTERPRETING == m_eMode;
  
  return bIsInterpreting;
}

/*----------------------------------------------------------------------------*/
/* get number of locals                                                       */
/*----------------------------------------------------------------------------*/
i8 SenCompiler::nLocals(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENCOMPILER,_FOOTER_);
  
  return m_nLocals;
}

/*----------------------------------------------------------------------------*/
/* set number of locals                                                       */
/*----------------------------------------------------------------------------*/
void SenCompiler::vLocals(i8 nLocals)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENCOMPILER,_FOOTER_);
  
  m_nLocals = nLocals;
  
  static i8 nMaxLocals = 0;
  
  if (nLocals > nMaxLocals) {
    nMaxLocals = nLocals;
//    vTrace("                                                      MaxLocals %lld\n"_Stz,nMaxLocals);
  }
}

/*----------------------------------------------------------------------------*/
/* mark where nlocals                                                         */
/*----------------------------------------------------------------------------*/
void SenCompiler::vLocalsMark(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENCOMPILER,_FOOTER_);
  
  byte* ayLatter = m_pCurrent->pCodeBlock()->ayWspLatter();
  
  m_aiLocals = (i8*) (ayLatter - 8);
  
  RASSERT(12345678 == * m_aiLocals);
}

/*----------------------------------------------------------------------------*/
/* update nLocals                                                             */
/*----------------------------------------------------------------------------*/
void SenCompiler::vLocalsUpdate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENCOMPILER,_FOOTER_);
  
  //RASSERT(12345678 == * m_aiLocals);
  
  RASSERT(m_nLocals > 0 && m_nLocals <= 24);
  
  * m_aiLocals = m_nLocals;
}

/*----------------------------------------------------------------------------*/
/* get mode                                                                   */
/*----------------------------------------------------------------------------*/
eCM SenCompiler::eMode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENCOMPILER,_FOOTER_);
  
  return m_eMode;
}

/*----------------------------------------------------------------------------*/
/* set mode                                                                   */
/*----------------------------------------------------------------------------*/
void SenCompiler::vMode(eCM eMode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENCOMPILER,_FOOTER_);
  
  m_eMode = eMode;
}

/*----------------------------------------------------------------------------*/
/* get nested word                                                            */
/*----------------------------------------------------------------------------*/
Word* SenCompiler::pNestedWord(i8 i_Nested)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENCOMPILER,_FOOTER_);
  
  Word* pNested = m_paWord[i_Nested];
  
  return pNested;
}

/*----------------------------------------------------------------------------*/
/* set nested word                                                            */
/*----------------------------------------------------------------------------*/
void SenCompiler::vNestedWord(i8 i_Nested, Word* pNested)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENCOMPILER,_FOOTER_);
  
  m_paWord[i_Nested] = pNested;
}

/*----------------------------------------------------------------------------*/
/* is the compiler in postponing mode?                                        */
/*----------------------------------------------------------------------------*/
bool SenCompiler::bPostponing(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENCOMPILER,_FOOTER_);
  
  bool bIsPostponing = CM_POSTPONING == m_eMode;
  
  return bIsPostponing;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
