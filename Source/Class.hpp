/*============================================================================*/
/* Class.hpp -- Class header                                                  */
/*============================================================================*/
#pragma once

namespace CorrectnessAlgebra
{
  class Class
  {
  protected:
    eCN  m_eClass;
  
  public:
    Class(void);
    ~Class(void);
  
    Str* aClass(void);
    eCN  eClass(void);
    void vSetClass(eCN eClassNo);
  };
  
/*--------------------------------------------------------------------------*/
/* get class                                                                */
/*--------------------------------------------------------------------------*/
inline eCN Class::eClass(void)
{
  return m_eClass;
}
}

/*--------------------------------------------------------------------------*/
/* check the object is of a known class                                     */
/*--------------------------------------------------------------------------*/
#ifdef __CLASS_CHECK
  #define _class_(arg) ASSERT(eClass() == arg);
  #define _class2_(arg1,arg2) ASSERT(eClass() == arg1 || eClass() == arg2);
  #define _class3_(arg1,arg2,arg3) \
  ASSERT(eClass() == arg1 || eClass() == arg2 || eClass() == arg3);
#else
  #define _class_(arg)
  #define _class2_(arg1,arg2)
  #define _class3_(arg1,arg2,arg3)
#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
