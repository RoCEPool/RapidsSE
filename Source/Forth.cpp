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
  
  m_aInxPtr      = nullptr;
  
  m_pDataStack   = new Stack;
  m_pReturnStack = new Stack;
  m_pAuxStack    = new Stack;

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
    
    LocalSet* pLocalsFrame = m_pLocalsFrame;
    m_pLocalsFrame = m_pLocalsFrame->pNext();
    
    delete pLocalsFrame;
  }
  
  delete m_pCompiler;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Forth);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
