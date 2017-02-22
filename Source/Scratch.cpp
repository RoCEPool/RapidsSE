/*============================================================================*/
/* Scratch.cpp -- scratch space                                               */
/*============================================================================*/

#include "Timer.hpp"

#include "Pool.h"

extern BufferPool* gpBufferPool;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Scratch* Common::_pScratch(void* pAddress)
{
  Scratch* pScratch = reinterpret_cast<Scratch*>(pAddress);
  
  RASSERT( !pScratch || CN_SCRATCH == pScratch->eClass() );
  
  return pScratch;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Scratch::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pObject = Heap::pAllocate(CN_SCRATCH);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Scratch::operator new(size_t iSize, Scratch* pScratch)
{
  RASSERT(iSize);
  return pScratch;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void Scratch::operator delete(void* pAddress)
{
  ASSERT(pAddress);
  
  Object* pObject = reinterpret_cast<Object*>(pAddress);
  
  Heap::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Scratch::Scratch(void)
: Object(CN_SCRATCH)
{
  _precondition_(_MAGIC_,CN_SCRATCH,_FOOTER_);

  Memory::vZero(m_wWorkarea,C_WORKAREA);
  
  _checkInitialised_(CN_SCRATCH);

  _postcondition_(_MAGIC_,CN_SCRATCH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Scratch::~Scratch(void)
{
  _precondition_(_MAGIC_,CN_SCRATCH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get work area                                                              */
/*----------------------------------------------------------------------------*/
char* Scratch::pWorkArea(void)
{
  _precondition_(_MAGIC_,CN_SCRATCH,_FOOTER_);
  
  return m_wWorkarea;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Scratch::pFooter(void)
{
  _precondition_(_MAGIC_,CN_SCRATCH);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
