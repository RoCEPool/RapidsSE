/*============================================================================*/
/* ColumnVec.hpp - Column Vector                                              */
/*============================================================================*/
#pragma once

namespace DBobjects
{  
  class ColumnVec : public Object
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
  
    void* operator new(size_t);
    void  operator delete(void*);

    ColumnVec(void);
    ~ColumnVec(void);
  
    Footer* pFooter(void);
  };
  
  ColumnVec* _aColumnVec(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
