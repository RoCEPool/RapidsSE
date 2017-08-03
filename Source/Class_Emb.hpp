/*============================================================================*/
/* Class.hpp -- Class header                                                  */
/*============================================================================*/
#pragma once

namespace CorrectnessAlgebra
{
  class Class_Emb
  {
  protected:
  u1 m_eClass;
  
  public:
    Class_Emb(eCN m_eClass);
  
    Stz* aClass(void);
    eCN  eClass(void);
    void vSetClass(eCN eClassNo);
  };
}

/*--------------------------------------------------------------------------*/
/* check the object is of a known class                                     */
/*--------------------------------------------------------------------------*/
#ifdef __CLASS_CHECK
  #define _class_emb_(arg) ASSERT(eClass() == arg);
  #define _class2_emb_(arg1,arg2) ASSERT(eClass() == arg1 || eClass() == arg2);
  #define _class3_emb_(arg1,arg2,arg3) \
  ASSERT(eClass() == arg1 || eClass() == arg2 || eClass() == arg3);
#else
  #define _class_emb_(arg)
  #define _class2_emb_(arg1,arg2)
  #define _class3_emb_(arg1,arg2,arg3)
#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
