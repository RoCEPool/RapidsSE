/*============================================================================*/
/* SubQuery.hpp -- SubQuery analysis                                                */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "eQA.h"
#include "eQC.h"
#include "eQO.h"
#include "eRO.h"

namespace STORE
{
  class SubQuery : public Object
  {
  protected:

    i8  m_iForm;

    eQA m_eAggregation;
    eQC m_eClause;
    eQO m_eOperator;

    eRO m_eRelationalOp;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    SubQuery(void);
   ~SubQuery(void) noexcept(false);

    i8 iForm(void);
    void vForm(i8 iForm);

    eQA eAggregation(void);
    void vAggregation(eQA eAggregation);

    eQC eClause(void);
    void vClause(eQC eClause);

    eQO eOperator(void);
    void vOperator(eQO eOperator);

    eRO eRelationalOp(void);
    void vRelationalOp(eRO eRelationalOp);

  protected:
    Footer* aFooter(void);
  };

  SubQuery* _pQuery(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
