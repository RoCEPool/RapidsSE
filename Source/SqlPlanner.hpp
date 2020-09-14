/*============================================================================*/
/* SqlPlanner.hpp -                                                           */
/*============================================================================*/
#pragma once

#include "sqlBaseVisitor.h"

namespace Parser
{  
  class SqlPlanner : public Object, public sqlBaseVisitor
  {
  private:

  
    Footer m_xFooter;
    
   public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    SqlPlanner(void);

    ~SqlPlanner(void);
  
    Footer* aFooter(void);
  };
  
  SqlPlanner* _pSqlPlanner(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
