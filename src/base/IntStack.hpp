/*============================================================================*/
/* IntStack.hpp -- Int Stack                                                  */
/*============================================================================*/
#pragma once

namespace BASE
{
  class IntStack : public Object
  {
  private:
     ArrayNL_t<CN_INTSTACK,i8,C_64KB>*  m_pIntAry;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    IntStack(void);
   ~IntStack(void) noexcept(false);

    static void vRepair(Object* pObject);

    i8 nItems(void);
    i8 iPeek(i8 i_Item);
    i8 iPop(void);
    void vPrint(bool bNewLine = true);
    void vPush(i8 iInt);

    Footer* aFooter(void);
  };

  IntStack* _pIntStack(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
