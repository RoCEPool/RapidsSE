/*============================================================================*/
/* ToLglPtnNst.hpp -- Convert row buffer to have normalised strings           */
/*============================================================================*/
#pragma once

namespace STORE
{
  class ToLglPtnNst : public Object
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ToLglPtnNst(void);
   ~ToLglPtnNst(void) noexcept(false);

    static void vRepair(Object* pObjectr);

    void vConvert(RowBuffer* pRowBuffer);
  
  private:
  
    Footer* aFooter(void);
  };

  ToLglPtnNst* _pToLglPtnNst(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
