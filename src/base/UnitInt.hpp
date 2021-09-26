/*============================================================================*/
/* UnitInt.hpp -- Int Object for Unit Tests                                   */
/*============================================================================*/
#pragma once

namespace BASE
{
  class UnitInt : public Object
  {
  private:
    i8  m_iVal;
   
    // must be last
    Footer m_xFooter;

  public:
  void* operator new(size_t);
  void operator delete(void* aUnitInt);

    UnitInt(void);
    UnitInt(i8 iVal);
   ~UnitInt(void) noexcept(false);
    
    i8 iVal(void);
    void vVal(i8 iVal);
 
  private:
    Footer* aFooter(void);
  };
  
  UnitInt* _pUnitInt(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
