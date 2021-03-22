/*============================================================================*/
/* Location.hpp - Logical Location of Object                                  */
/*============================================================================*/
#pragma once

namespace BASE
{
  class Location : public Object
  {
  private:
    
    s32 m_sTenant;
    s32 m_sCatalog;
    s32 m_sSchema;
    s32 m_sTable;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Location(void);
   ~Location(void) noexcept(false);
    
    Footer* aFooter(void);
  };

  Location* _pLocation(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
