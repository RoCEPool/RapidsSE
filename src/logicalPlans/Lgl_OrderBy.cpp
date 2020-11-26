/*============================================================================*/
/* Filter_OrderBy.cpp -- OrderBy LogicalOp operand                               */
/*============================================================================*/

#include "LogicalPlans.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Filter_OrderBy* LogicalPlans::_pFilter_OrderBy(void* aAddr)
{
  Filter_OrderBy* pOp = reinterpret_cast<Filter_OrderBy*>(aAddr);
  
  return pOp;
}

/*----------------------------------------------------------------------------*/
/* allocate from heap                                                         */
/*----------------------------------------------------------------------------*/
void* Filter_OrderBy::operator new(size_t)
{
  Filter_OrderBy* pFilter_OrderBy =
    static_cast<Filter_OrderBy*>(HeapMem::pAllocate(CN_FILTER_ORDERBY));
  
  return pFilter_OrderBy;
}

/*----------------------------------------------------------------------------*/
/* free back to the heap                                                      */
/*----------------------------------------------------------------------------*/
void Filter_OrderBy::operator delete(void* aAddress)
{
  Object* pObject = static_cast<Object*>(aAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Filter_OrderBy::Filter_OrderBy(TokenVec* pTokenVec)
: LogicalOp(CN_FILTER_ORDERBY,pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTER_ORDERBY,_FOOTER_);

  m_sName = "orderBy"_Stz;
  
  _checkInitialised_(CN_FILTER_ORDERBY);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_FILTER_ORDERBY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Filter_OrderBy::~Filter_OrderBy(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTER_ORDERBY,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Filter_OrderBy);


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
