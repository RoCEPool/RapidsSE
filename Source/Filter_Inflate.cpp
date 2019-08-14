/*============================================================================*/
/* Filter_Inflate.cpp -- Inflate Filter operand                               */
/*============================================================================*/

#include "CompilerStream.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Filter_Inflate* CompilerStream::_pFilter_Inflate(void* aAddr)
{
  Filter_Inflate* pOp = reinterpret_cast<Filter_Inflate*>(aAddr);
  
  return pOp;
}

/*----------------------------------------------------------------------------*/
/* allocate from heap                                                         */
/*----------------------------------------------------------------------------*/
void* Filter_Inflate::operator new(size_t)
{
  Filter_Inflate* pFilter_Inflate =
    static_cast<Filter_Inflate*>(HeapMem::pAllocate(CN_FILTER_FROM));
  
  return pFilter_Inflate;
}

/*----------------------------------------------------------------------------*/
/* free back to the heap                                                      */
/*----------------------------------------------------------------------------*/
void Filter_Inflate::operator delete(void* aAddress)
{
  Object* pObject = static_cast<Object*>(aAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Filter_Inflate::Filter_Inflate(TokenVec* pTokenVec)
: Filter(CN_FILTER_FROM,pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTER_FROM,_FOOTER_);

  m_sName = "inflate"_Stz;
  
  _checkInitialised_(CN_FILTER_FROM);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_FILTER_FROM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Filter_Inflate::~Filter_Inflate(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTER_FROM,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Filter_Inflate);


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
