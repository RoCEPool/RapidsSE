/*============================================================================*/
/* ConditionalQueryNodeIter.hpp - a conditional iterator of query node lists  */
/*============================================================================*/
#pragma once

#include <functional> // for std::function
#include "ListExo_t.hpp"
#include "PlanNode.hpp"

namespace Parser
{

class QueryNode;

class ConditionalQueryNodeIter : public Object
{
protected:
  PlanNodeIter* m_pPlanNodeIter;
  b1(*m_pfCondition)(PlanNode*);  // pointer to a function that accepts a PlanNode* and returns a b1

private:
  Footer m_xFooter;

public:
  void* operator new(size_t);
  void  operator delete(void*);

  ConditionalQueryNodeIter(PlanNodeIter* pIter, b1(*pfCond)(PlanNode*));
  ~ConditionalQueryNodeIter(void) noexcept;

  Footer* aFooter(void);

  void       vAfterLast(void);
  void       vBeforeFirst(void);
  QueryNode* pFirst(void);
  bool       bIsAfterLast(void);
  bool       bIsBeforeFirst(void);
  bool       bHasNext(void);
  bool       bHasPrev(void);
  QueryNode* pNext(void);
  QueryNode* pPrev(void);
};

ConditionalQueryNodeIter* _pConditionalQueryNodeIter(void* aAddr);

} //namespace Parser