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
  
  m_pIntSet->vNewAry(32);
  m_pIntSet->vAscending();
  //m_pIntSet->vPrint();
  
  m_pIntSet->vRandomise();
  //m_pIntSet->vPrint();

  bool bPassed = bInsertUnitInts();
  if (! bPassed) {
    return false;
  }
  
  vPrintSkipList();

  bPassed = bRemoveUnitInts();
  if (! bPassed) {
    return false;
  }

  m_pIntSet->vDeleteAry();

  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Static -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* if the key is greater than object on the right, then move right            */
/*----------------------------------------------------------------------------*/
bool UT_SkipList::bCompareLR_int(UnitInt* pUnitIntLeft, UnitInt* pUnitIntRight)
{
  _static_

  RASSERT(pUnitIntLeft);
  RASSERT(pUnitIntRight);

  return true;
}

/*----------------------------------------------------------------------------*/
/* if the key is greater than object on the right, then move right            */
/*----------------------------------------------------------------------------*/
bool UT_SkipList::bMoveRight_int(i8 iLookForKey,
                                 UnitInt* pUnitInt,
                                 bool& bFoundKey)
{
  _static_

  RASSERT(pUnitInt);

  i8 iRightVal = pUnitInt->iInt();
  
  if (iLookForKey == iRightVal) {
    bFoundKey = true;
    return false;
  }

  if (iLookForKey > iRightVal)
    return TRUE(move_right);
  else
    return FALSE(shift_down);
}

/*----------------------------------------------------------------------------*/
/* if the key is greater than object on the right, then move right            */
/*----------------------------------------------------------------------------*/
void UT_SkipList::vPrintUnitInt(SkipNode* pNode, int i_Level)
{
  _static_

  RASSERT(pNode);
  
  UnitInt* pUnitInt = _pUnitInt(pNode->m_pObject);

  SkipNode* pRightNode = pNode->m_pRight[i_Level];
  
  i8 iInt = pUnitInt->iInt();
  vTrace("@%lld %p : %lld -> %p "_Stz
         ,i_Level
         ,pNode
         ,iInt
         ,pNode->m_pRight[i_Level]
         );
  
  if (pRightNode) {
    UnitInt* pRightUnitInt = _pUnitInt(pRightNode->m_pObject);
    i8 iRightInt = pRightUnitInt->iInt();
    vTrace("(%lld)\n"_Stz
           ,iRightInt
           );
  }
  else {
    vTrace("\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* insert the int set as unitInt's into the skip list                         */
/*----------------------------------------------------------------------------*/
bool UT_SkipList::bInsertUnitInts(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_UT_SKIPLIST,_FOOTER_);
  
  auto vfPrintNode = [](SkipNode* pNode, int i_Level) {
    
    vPrintUnitInt(pNode,i_Level);
  };

  i8 nInts = m_pIntSet->nInts();
  for (i8 i_Int = 0; i_Int < nInts; i_Int++) {
    
    i8 iInt = m_pIntSet->iInt(i_Int);
    UnitInt* pUnitInt = new UnitInt(iInt);
    
    auto bfMoveRightTest = [iInt](void* aRightUnit, bool& bFoundKey) {
      
      bool bRight = bMoveRight_int(iInt,_pUnitInt(aRightUnit),bFoundKey);
      
      return bRight;
    };
    
    bool bInserted = m_pSkipList->bInsertByKey(pUnitInt,
                                               bfMoveRightTest,
                                               vfPrintNode);
    RASSERT(bInserted);
    
    bool bPassed = bValidateSkipList();
    if (! bPassed)
      return false;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* insert the int set as unitInt's into the skip list                         */
/*----------------------------------------------------------------------------*/
bool UT_SkipList::bRemoveUnitInts(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_UT_SKIPLIST,_FOOTER_);
  
  i8 nInts = m_pIntSet->nInts();
  for (i8 i_Int = 0; i_Int < nInts; i_Int++) {
    
    i8 iInt = m_pIntSet->iInt(i_Int);
    
    auto bfMoveRightTest = [iInt](void* aRightUnit, bool& bFoundKey) {
      
      bool bRight = bMoveRight_int(iInt,_pUnitInt(aRightUnit),bFoundKey);
      
      return bRight;
    };
    
    UnitInt* pUnitInt = _pUnitInt(m_pSkipList->pFindByKey(bfMoveRightTest));
    RASSERT(pUnitInt);

    bool bRemoved = m_pSkipList->bRemoveByKey(pUnitInt,bfMoveRightTest);
    
    RASSERT(bRemoved);
    
    delete pUnitInt;

    bool bPassed = bValidateSkipList();
    if (! bPassed)
      return false;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* print the skip list                                                        */
/*----------------------------------------------------------------------------*/
void UT_SkipList::vPrintSkipList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_UT_SKIPLIST,_FOOTER_);
  
  auto vfPrintNode = [](SkipNode* pNode, int i_Level) {
    
    vPrintUnitInt(pNode,i_Level);
  };

  m_pSkipList->vPrintList(vfPrintNode);
}

/*----------------------------------------------------------------------------*/
/* validate the skip list                                                     */
/*----------------------------------------------------------------------------*/
bool UT_SkipList::bValidateSkipList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_UT_SKIPLIST,_FOOTER_);
  
  auto vfCompare = [](SkipNode* pLeftNode, SkipNode* pRightNode) {
    
    bool b = bCompareLR_int(_pUnitInt(pLeftNode->m_pObject),
                            _pUnitInt(pRightNode->m_pObject));
    
    return b;
  };

  bool bPassed = m_pSkipList->bValidate(vfCompare,vPrintUnitInt);
  
  return bPassed;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
