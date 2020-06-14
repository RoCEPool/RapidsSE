/*============================================================================*/
/* Filter_Inflate.cpp -- Inflate Filter operand                               */
/*============================================================================*/

#include "LogicalPlans.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Filter_Inflate* LogicalPlans::_pFilter_Inflate(void* aAddr)
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
    static_cast<Filter_Inflate*>(HeapMem::pAllocate(CN_FILTER_INFLATE));
  
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
: Filter(CN_FILTER_INFLATE,pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTER_INFLATE,_FOOTER_);

  m_sName = "inflate"_Stz;
  
  _checkInitialised_(CN_FILTER_INFLATE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_FILTER_INFLATE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Filter_Inflate::~Filter_Inflate(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTER_INFLATE,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Filter_Inflate);


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
