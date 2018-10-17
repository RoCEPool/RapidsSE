/*============================================================================*/
/* Decider.hpp -- ack decider                                                 */
/*============================================================================*/
#pragma once

namespace System
{
  class Decider : public Object
  {
  private:
    i8 m_iInt;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t, Decider* pDecider);
    void  operator delete(void* aAddress);
    
    Decider(void);
    ~Decider(void);
    
    void vCheckInit(void);
    
    i8   iDec(void);      // decrement
    
    void operator =(i8 iInt); // assignment
    i8 operator *(void);  // dereference
    i8 operator --(void); // decrement (prefix form)

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
