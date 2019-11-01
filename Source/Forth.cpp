/*============================================================================*/
/* Forth.hpp -- Forth                                                         */
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
Forth::Forth(Task* pTask, RegSet* pRegSet)
: Object(CN_FORTH)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORTH,_FOOTER_);
  
  m_pTask        = pTask;
  m_pRegSet      = pRegSet;
  
  m_pSeedWord       = nullptr;
  m_pExceptionWord  = nullptr;
  
  m_bSeedInitially  = true;
  m_bDoneFinally    = true;

  // m_oDicStack
  
  m_aInxPtr      = nullptr;
  
  m_pAuxStack    = new Stack(KK_AUX_STACK);
  m_pDataStack   = new Stack(KK_DATA_STACK);
  m_pReturnStack = new Stack(KK_RETURN_STACK);

  m_pLocalsFrame = nullptr;

  m_pCompiler    = nullptr;
  
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
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump execution context                                                     */
/*----------------------------------------------------------------------------*/
void Forth::vContentDump(void)
{
  Word* pWord = m_oDicStack.pLocateWord(m_aInxPtr);

  vTrace("\n"_Stz);
  vTrace("Execution Context\n"_Stz);
  vTrace("-----------------\n"_Stz);

  vTrace("%p:\n"
         "m_aInxPtr=%s %p\n"_Stz,
         
         this,
         
         pWord ? pWord->szWordNm() : "<none>"_Stz,
         m_aInxPtr);

  vTrace("m_pSeedWord=%s %p\n"_Stz,
         m_pSeedWord->szWordNm(),
         m_pSeedWord);
  vTrace("m_pExceptionWord=%s %p\n"_Stz,
         m_pExceptionWord->szWordNm(),
         m_pExceptionWord);

  m_pAuxStack->vContentDump(&m_oDicStack,   "m_pAuxStack:"_Stz);
  m_pDataStack->vContentDump(&m_oDicStack,  "m_pDataStack:"_Stz);
  m_pReturnStack->vContentDump(&m_oDicStack,"m_pReturnStack:"_Stz);
  
  m_oDicStack.vContentDump();
}

/*----------------------------------------------------------------------------*/
/* reset forth execution context                                              */
/*----------------------------------------------------------------------------*/
void Forth::vReset(void)
{
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
/* seet forth execution context                                               */
/*----------------------------------------------------------------------------*/
void Forth::vSeedForth(Word* pSeedWord)
{
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
