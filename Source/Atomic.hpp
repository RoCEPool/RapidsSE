/*============================================================================*/
/* Atomic.hpp -- Atomic integer                                               */
/*============================================================================*/
#pragma once

namespace Locking
{
  class Atomic : public Object
  {
  private:
    i8 m_iInt;
    
    // must be last
    Footer m_xFooter;
    
  public:
    Atomic(void);
    ~Atomic(void) noexcept(false);
  
    static u8 uAtomicAdd(i8* pWord, i8 iValue);
    static u8 uAtomicDec(i8* pWord);
    static u8 uAtomicInc(i8* pWord);
  
    static bool bTestAndSet(u8* pWord, u8 uIf, u8 uThen);

    void vCheckInit(void);
    
    i8   iAdd(i8 iValue); // add
    i8   iDec(void);      // decrement
    i8   iInc(void);      // increment
    i8   iVal(void);      // get value

    void operator =(i8 iInt); // assignment
    i8 operator *(void);      // dereference
    i8 operator ++(void);     // increment (prefix form)
    i8 operator --(void);     // decrement (prefix form)
    i8 operator +=(i8 iInt);  // add assign
    i8 operator -=(i8 iInt);  // subtract assign

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
