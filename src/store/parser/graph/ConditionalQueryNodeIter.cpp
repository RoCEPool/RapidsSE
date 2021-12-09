/*============================================================================*/
/* ConditionalQueryNodeIter.hpp - a conditional iterator of query node lists  */
/*============================================================================*/
#include "BASE.h"
#include "ConditionalQueryNodeIter.hpp"
#include "QueryNode.hpp"


using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(Parser,ConditionalQueryNodeIter,CN_CONDITIONALQUERYNODEITER)
__aNewFunction(ConditionalQueryNodeIter,CN_CONDITIONALQUERYNODEITER)
__vDeleteFunction(ConditionalQueryNodeIter,CN_CONDITIONALQUERYNODEITER)
__aFooterFunction(ConditionalQueryNodeIter)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ConditionalQueryNodeIter::ConditionalQueryNodeIter(PlanNodeIter* pIter, b1(*pfCond)(PlanNode*))
    : Object(CN_CONDITIONALQUERYNODEITER),
      m_pPlanNodeIter(pIter),
      m_pfCondition(pfCond)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CONDITIONALQUERYNODEITER,_FOOTER_);

  _checkInitialised_(CN_CONDITIONALQUERYNODEITER);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CONDITIONALQUERYNODEITER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ConditionalQueryNodeIter::~ConditionalQueryNodeIter(void) noexcept
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CONDITIONALQUERYNODEITER,_FOOTER_);

  delete m_pPlanNodeIter;
}

/*----------------------------------------------------------------------------*/
/* position after last item for a backwards traversal                         */
/*----------------------------------------------------------------------------*/
void ConditionalQueryNodeIter::vAfterLast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CONDITIONALQUERYNODEITER,_FOOTER_);

  m_pPlanNodeIter->vAfterLast();
}

/*----------------------------------------------------------------------------*/
/* position before first item                                                 */
/*----------------------------------------------------------------------------*/
void ConditionalQueryNodeIter::vBeforeFirst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CONDITIONALQUERYNODEITER,_FOOTER_);

  m_pPlanNodeIter->vBeforeFirst();
}

/*----------------------------------------------------------------------------*/
/* get first item of list                                                     */
/*----------------------------------------------------------------------------*/
QueryNode* ConditionalQueryNodeIter::pFirst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CONDITIONALQUERYNODEITER,_FOOTER_);

  m_pPlanNodeIter->vBeforeFirst();

  PlanNode* pItem;
  while ((pItem = m_pPlanNodeIter->pNext()) != nullptr) {
    if ((*m_pfCondition)(pItem)) {
      return _pQueryNode(pItem);
    }
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* return true if the iterator is after the last element in the list          */
/*----------------------------------------------------------------------------*/
bool ConditionalQueryNodeIter::bIsAfterLast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CONDITIONALQUERYNODEITER,_FOOTER_);

  return m_pPlanNodeIter->bIsAfterLast();
}

/*----------------------------------------------------------------------------*/
/* return true if the iterator is before the first element in the list        */
/*----------------------------------------------------------------------------*/
bool ConditionalQueryNodeIter::bIsBeforeFirst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CONDITIONALQUERYNODEITER,_FOOTER_);

  return m_pPlanNodeIter->bIsBeforeFirst();
}

/*----------------------------------------------------------------------------*/
/* Return true if there is another node after this one                        */
/*----------------------------------------------------------------------------*/
bool ConditionalQueryNodeIter::bHasNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CONDITIONALQUERYNODEITER,_FOOTER_);

  ListExoNode_t<PlanNode>* pNextCursor = m_pPlanNodeIter->m_pNextCursor;
  while (pNextCursor) {
    PlanNode* pItem = pNextCursor->pItem();

    if ((*m_pfCondition)(pItem)) {
      return true;
    }

    pNextCursor = pNextCursor->pNextNode();
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* Return true if there is another node before this one                       */
/*----------------------------------------------------------------------------*/
bool ConditionalQueryNodeIter::bHasPrev(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CONDITIONALQUERYNODEITER,_FOOTER_);

  ListExoNode_t<PlanNode>* pPrevCursor = m_pPlanNodeIter->m_pPrevCursor;
  while (pPrevCursor) {
    PlanNode* pItem = pPrevCursor->pItem();

    if ((*m_pfCondition)(pItem)) {
      return true;
    }

    pPrevCursor = pPrevCursor->pPrevNode();
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* get next item in list moving towards the tail                              */
/*----------------------------------------------------------------------------*/
QueryNode* ConditionalQueryNodeIter::pNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CONDITIONALQUERYNODEITER,_FOOTER_);

  PlanNode* pItem;

  while ((pItem = m_pPlanNodeIter->pNext()) != nullptr) {
    if ((*m_pfCondition)(pItem)) {
      return _pQueryNode(pItem);
    }
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get previous item in list moving towards the head                          */
/*----------------------------------------------------------------------------*/
QueryNode* ConditionalQueryNodeIter::pPrev(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CONDITIONALQUERYNODEITER,_FOOTER_);

  PlanNode* pItem;

  while ((pItem = m_pPlanNodeIter->pPrev()) != nullptr) {
    if ((*m_pfCondition)(pItem)) {
      return _pQueryNode(pItem);
    }
  }

  return nullptr;
}
