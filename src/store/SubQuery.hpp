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

    i8 m_i_SubQuery;
    i8 m_nParams;

    eQA m_eAggregation;
    eQC m_eClause;
    eQO m_eOperator;

    eRO m_eRelationalOp;


    i8 m_nSubQueries;

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

    i8 nParams(void);
    void vParams(i8 nParams);

    eRO eRelationalOp(void);
    void vRelationalOp(eRO eRelationalOp);

    i8 nSubQueries(void);
    void vSubQueries(i8 nSubQueries);

    i8 i_SubQuery(void);
    void vSubQuery(i8 i_SubQuery);

  protected:
    Footer* aFooter(void);
  };

  SubQuery* _pSubQuery(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
