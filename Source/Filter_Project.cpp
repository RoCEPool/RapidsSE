/*============================================================================*/
/* Filter_Project.cpp -- Project LogicalOp operand                               */
/*============================================================================*/

#include "LogicalPlans.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Filter_Project* LogicalPlans::_pFilter_Project(void* aAddr)
{
  Filter_Project* pOp = reinterpret_cast<Filter_Project*>(aAddr);
  
  return pOp;
}

/*----------------------------------------------------------------------------*/
/* allocate from heap                                                         */
/*----------------------------------------------------------------------------*/
void* Filter_Project::operator new(size_t)
{
  Filter_Project* pFilter_Project =
    static_cast<Filter_Project*>(HeapMem::pAllocate(CN_FILTER_PROJECT));
  
  return pFilter_Project;
}

/*----------------------------------------------------------------------------*/
/* free back to the heap                                                      */
/*----------------------------------------------------------------------------*/
void Filter_Project::operator delete(void* aAddress)
{
  Object* pObject = static_cast<Object*>(aAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Filter_Project::Filter_Project(TokenVec* pTokenVec)
: LogicalOp(CN_FILTER_PROJECT,pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTER_PROJECT,_FOOTER_);

  m_sName = "project"_Stz;
  
  _checkInitialised_(CN_FILTER_PROJECT);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_FILTER_PROJECT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Filter_Project::~Filter_Project(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTER_PROJECT,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Filter_Project);


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
