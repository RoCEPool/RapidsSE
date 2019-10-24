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
Forth::Forth(Task* pTask)
: Object(CN_FORTH)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORTH,_FOOTER_);
  
  m_pTask        = pTask;
  
  m_pSeedWord       = nullptr;
  m_pExceptionWord  = nullptr;

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
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Forth);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
