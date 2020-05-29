/*============================================================================*/
/* GenVar.hpp - Generic Variable  (which contains a nested value)             */
/*============================================================================*/
#pragma once

namespace Datums
{
  class GenVar : public Datum
  {
  private:
  
    public:
    void* operator new(size_t);
  
    GenVar(void);
    GenVar(AnyVal*);
   ~GenVar(void) noexcept(false);
  
 
    void operator =(AnyVal*);
  
    void operator =(Cell* aFip);
    void operator =(i8 iInt);
    void operator =(Object* pObj);
    void operator =(Stz* szStz);


  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
