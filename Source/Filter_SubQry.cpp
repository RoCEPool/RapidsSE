/*============================================================================*/
/* Filter_SubQry.cpp -- SubQry Filter operand                                 */
/*============================================================================*/

#include "LogicalPlans.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Filter_SubQry* LogicalPlans::_pFilter_SubQry(void* aAddr)
{
  Filter_SubQry* pOp = reinterpret_cast<Filter_SubQry*>(aAddr);
  
  return pOp;
}

/*----------------------------------------------------------------------------*/
/* allocate from heap                                                         */
/*----------------------------------------------------------------------------*/
void* Filter_SubQry::operator new(size_t)
{
  Filter_SubQry* pFilter_SubQry =
    static_cast<Filter_SubQry*>(HeapMem::pAllocate(CN_FILTER_SUBQRY));
  
  return pFilter_SubQry;
}

/*----------------------------------------------------------------------------*/
/* free back to the heap                                                      */
/*----------------------------------------------------------------------------*/
void Filter_SubQry::operator delete(void* aAddress)
{
  Object* pObject = static_cast<Object*>(aAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Filter_SubQry::Filter_SubQry(TokenVec* pTokenVec)
: Filter(CN_FILTER_SUBQRY,pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTER_SUBQRY,_FOOTER_);

  m_sName = "subqry"_Stz;
  
  _checkInitialised_(CN_FILTER_SUBQRY);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_FILTER_SUBQRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Filter_SubQry::~Filter_SubQry(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTER_SUBQRY,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Filter_SubQry);


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
