/*============================================================================*/
/* UT_SkipList.cpp - SkipList Unit Test                                       */
/*============================================================================*/

#include "BASE.h"

#include "UT_SkipList.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,UT_SkipList,CN_UT_SKIPLIST)
__aNewFunction(UT_SkipList,CN_UT_SKIPLIST)
__vDeleteFunction(UT_SkipList,CN_UT_SKIPLIST)
__aFooterFunction(UT_SkipList)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
UT_SkipList::UT_SkipList(void)
: UT(CN_UT_SKIPLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_UT_SKIPLIST,_FOOTER_);

  m_pIntSet = new IntSet;
  
  m_pSkipList = new SkipListNL(true,nullptr);
  
  _postcondition_(_MAGIC_,CN_UT_SKIPLIST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
UT_SkipList::~UT_SkipList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_UT_SKIPLIST,_FOOTER_);

  delete m_pIntSet;
  
  delete m_pSkipList;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* run unit test                                                              */
/*----------------------------------------------------------------------------*/
bool UT_SkipList::bTest(void)
{
  _static_

  UT_SkipList* pUT_SkipList = new UT_SkipList;
  
  bool bPassed = pUT_SkipList->bRunTests();
  
  delete pUT_SkipList;
  
  return bPassed;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* run all tests                                                              */
/*----------------------------------------------------------------------------*/
bool UT_SkipList::bRunTests(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_UT_SKIPLIST,_FOOTER_);
  
  m_pIntSet->vNewAry(10);
  m_pIntSet->vAscending();
  m_pIntSet->vPrint();
  m_pIntSet->vRandomise();
  m_pIntSet->vPrint();

  m_pIntSet->vDeleteAry();

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
