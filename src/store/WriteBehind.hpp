/*============================================================================*/
/* WriteBehind.hpp -- Working WriteBehind Buffer                              */
/*============================================================================*/
#pragma once

namespace STORE
{
  class WriteBehind : public Object
  {
  private:
  
    RowBuffer*  m_pRowBuffer;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    WriteBehind(void);
   ~WriteBehind(void) noexcept(false);
  
  public:
    RowBuffer* pRowBuffer(void);
    void vRowBuffer(RowBuffer* pRowBuffer);

    Footer* aFooter(void);
  };

  WriteBehind* _pWriteBehind(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
