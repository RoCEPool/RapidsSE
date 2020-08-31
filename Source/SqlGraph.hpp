/*============================================================================*/
/* SqlGraph.hpp - Sql Graph                                                   */
/*============================================================================*/
#pragma once

namespace Facts
{
  class SqlGraph : public Object
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    SqlGraph(void);
   ~SqlGraph(void) noexcept(false);

  private:
    Footer* aFooter(void);
  };

  SqlGraph* _pSqlGraph(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
