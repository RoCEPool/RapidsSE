/*============================================================================*/
/* Forth.cpp -- Forth                                                         */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"


/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Forth* STORE::_pForth(void* aAddr)
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

  _checkInitialised_(CN_FORTH);
 
  _postcondition_(_ADDRESS_,_MAGIC_,CN_FORTH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Forth::~Forth(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORTH,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* new input                                                                  */
/*----------------------------------------------------------------------------*/
void Forth::vNewInput(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORTH,_FOOTER_);

  RASSERT(! m_pTask->m_pInput);
    
  m_pTask->m_pInput = new Input;
}

/*----------------------------------------------------------------------------*/
/* reset forth execution context                                              */
/*----------------------------------------------------------------------------*/
void Forth::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORTH,_FOOTER_);

   m_pTask->m_pAuxStack->vReset();
  m_pTask->m_pDataStack->vReset();
  m_pTask->m_pReturnStack->vReset();
  
  while (m_pTask->m_pLocalsFrame) {
    
    LocalChain* pLocalsFrame = m_pTask->m_pLocalsFrame;
    m_pTask->m_pLocalsFrame = m_pTask->m_pLocalsFrame->pNext();
    
    delete pLocalsFrame;
  }
  
  m_pTask->m_aInxPtr = nullptr;
}

/*----------------------------------------------------------------------------*/
/* seed forth execution context                                               */
/*----------------------------------------------------------------------------*/
void Forth::vSeedForth(Word* pSeedWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORTH,_FOOTER_);

  m_pTask->m_pSeedWord = pSeedWord;
  m_pTask->m_aInxPtr   = pSeedWord->aFirstCell();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Dic* pRootDic = AM::pRootDic();

  Word* pExceptionWord = pRootDic->pFindWord(pSeedWord->szExceptionHook());
  if (pExceptionWord) {
    m_pTask->m_pExceptionWord = pExceptionWord;
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Word* pExceptionDef = pRootDic->pFindWord("std_exceptionHook"_Stz);
  if (pExceptionDef) {
    m_pTask->m_pExceptionWord = pExceptionDef;
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
