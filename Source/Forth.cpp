/*============================================================================*/
/* Forth.cpp -- Forth                                                         */
/*============================================================================*/

#include "SenVM.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Forth* SenVM::_pForth(void* aAddr)
{
  Forth* pForth = reinterpret_cast<Forth*>(aAddr);
  
  RASSERT_TYPE(pForth,CN_FORTH);
  
  return pForth;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Forth::operator new(size_t)
{
  Forth* pForth = (Forth*)(HeapMem::pAllocate(CN_FORTH));
  
  return pForth;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Forth::operator delete(void* aAddress)
{
  Forth* pForth = reinterpret_cast<Forth*>(aAddress);
  
  HeapMem::vFree(pForth);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Forth::Forth(Task* pTask, LogicUnit* pLogicUnit)
: Object(CN_FORTH)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORTH,_FOOTER_);
  
  m_pTask        = pTask;
  m_pLogicUnit      = pLogicUnit;
  
  m_pSeedWord       = nullptr;
  m_pExceptionWord  = nullptr;
  
  m_bSeedInitially  = false;
  m_bDoneFinally    = false;

  // m_oDicStack
  // m_oSaveDicStack

  m_aInxPtr      = nullptr;
  
  m_pAuxStack    = new Stack(KK_AUX_STACK);
  m_pDataStack   = new Stack(KK_DATA_STACK);
  m_pReturnStack = new Stack(KK_RETURN_STACK);

  m_pLocalsFrame = nullptr;

  m_pCompiler    = new Compiler;
  m_pInput       = nullptr; // new Input;

  _checkInitialised_(CN_FORTH);
 
  _postcondition_(_ADDRESS_,_MAGIC_,CN_FORTH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Forth::~Forth(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORTH,_FOOTER_);
  
  RASSERT(m_bSeedInitially);
  RASSERT(m_bDoneFinally);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  delete m_pDataStack;
  delete m_pReturnStack;
  delete m_pAuxStack;
  
  while (m_pLocalsFrame) {
    
    LocalChain* pLocalsFrame = m_pLocalsFrame;
    m_pLocalsFrame = m_pLocalsFrame->pNext();
    
    delete pLocalsFrame;
  }
  
  delete m_pCompiler;
  delete m_pInput;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* check for break point                                                      */
/*----------------------------------------------------------------------------*/
void Forth::vCheckBreakPt(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORTH,_FOOTER_);

  if (! m_aInxPtr)
    return;
  
  Word* pThisWord = Routine::pToWord(m_aInxPtr);
  
  if (! pThisWord->bBreakPt())
    return;
  
  Correctness::vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* get compiler                                                               */
/*----------------------------------------------------------------------------*/
Compiler* Forth::pCompiler(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORTH,_FOOTER_);

  return m_pCompiler;
}

/*----------------------------------------------------------------------------*/
/* set compiler                                                               */
/*----------------------------------------------------------------------------*/
void Forth::vCompiler(Compiler* pCompiler)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORTH,_FOOTER_);
  
  m_pCompiler = pCompiler;
}

/*----------------------------------------------------------------------------*/
/* dump execution context                                                     */
/*----------------------------------------------------------------------------*/
void Forth::vContentDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORTH,_FOOTER_);

  Word* pWord = m_oDicStack.pLocateRoutineWord(m_aInxPtr);

  vTrace("\n"_Stz);
  vTrace("Execution Context\n"_Stz);
  vTrace("-----------------\n"_Stz);

  vTrace("%p:\n"
         "m_aInxPtr=%s %p\n"_Stz,
         
         this,
         
         pWord ? pWord->szWordNm() : C_NO_WORD,
         m_aInxPtr);

  vTrace("m_pSeedWord=%s %p\n"_Stz,
         m_pSeedWord->szWordNm(),
         m_pSeedWord);
  vTrace("m_pExceptionWord=%s %p\n"_Stz,
         m_pExceptionWord ? m_pExceptionWord->szWordNm() : C_NO_NAME,
         m_pExceptionWord);

  m_pDataStack->vDump(&m_oDicStack,  "m_pDataStack:"_Stz);
  m_pReturnStack->vDump(&m_oDicStack,"m_pReturnStack:"_Stz);
  m_pAuxStack->vDump(&m_oDicStack,   "m_pAuxStack:"_Stz);

  m_oDicStack.vContentDump();
  
  if (pWord)
    AM::vWordSee(pWord,&m_oDicStack);
}

/*----------------------------------------------------------------------------*/
/* delete input                                                               */
/*----------------------------------------------------------------------------*/
void Forth::vDeleteInput(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORTH,_FOOTER_);

  delete m_pInput;
  
  m_pInput = nullptr;
}

/*----------------------------------------------------------------------------*/
/* get dictionary stack                                                       */
/*----------------------------------------------------------------------------*/
DicStack* Forth::pDicStack(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORTH,_FOOTER_);

  return & m_oDicStack;
}

/*----------------------------------------------------------------------------*/
/* get input stream                                                           */
/*----------------------------------------------------------------------------*/
Input* Forth::pInput(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORTH,_FOOTER_);

  return m_pInput;
}

/*----------------------------------------------------------------------------*/
/* new input                                                                  */
/*----------------------------------------------------------------------------*/
void Forth::vNewInput(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORTH,_FOOTER_);

  RASSERT(! m_pInput);
    
  m_pInput = new Input;
}

/*----------------------------------------------------------------------------*/
/* reset forth execution context                                              */
/*----------------------------------------------------------------------------*/
void Forth::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORTH,_FOOTER_);

  m_pAuxStack->vReset();
  m_pDataStack->vReset();
  m_pReturnStack->vReset();
  
  while (m_pLocalsFrame) {
    
    LocalChain* pLocalsFrame = m_pLocalsFrame;
    m_pLocalsFrame = m_pLocalsFrame->pNext();
    
    delete pLocalsFrame;
  }
  
  m_aInxPtr = nullptr;
}

/*----------------------------------------------------------------------------*/
/* get save path stack                                                       */
/*----------------------------------------------------------------------------*/
DicStack* Forth::pSaveDicStack(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORTH,_FOOTER_);

  return & m_oSaveDicStack;
}

/*----------------------------------------------------------------------------*/
/* seet forth execution context                                               */
/*----------------------------------------------------------------------------*/
void Forth::vSeedForth(Word* pSeedWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORTH,_FOOTER_);

  m_pSeedWord = pSeedWord;
  m_aInxPtr   = pSeedWord->aFirstCell();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Dic* pRootDic = AM::pRootDic();

  Word* pExceptionWord = pRootDic->pFindWord(pSeedWord->szExceptionHook());
  if (pExceptionWord) {
    m_pExceptionWord = pExceptionWord;
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Word* pExceptionDef = pRootDic->pFindWord("std_exceptionHook"_Stz);
  if (pExceptionDef) {
    m_pExceptionWord = pExceptionDef;
    return;
  }
}

/*----------------------------------------------------------------------------*/
/* get task                                                                   */
/*----------------------------------------------------------------------------*/
Task* Forth::pTask(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORTH,_FOOTER_);

  return m_pTask;
}


/*----------------------------------------------------------------------------*/
/* test for correct layout                                                    */
/*----------------------------------------------------------------------------*/
__bCorrectLayout(Forth,CN_FORTH)

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Forth);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
