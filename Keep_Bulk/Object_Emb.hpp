/*============================================================================*/
/* Obj.hpp -- Base Class for Proper Objects (short form)                      */
/*============================================================================*/
#pragma once

namespace CorrectnessAlgebra
{
  class Object_Emb : public Magic_Emb,
                     public Class_Emb
  {
  private:
  
  public:
    Object_Emb(eCN eClass);
  
    Footer* aFooter(void);
  };
}

/*--------------------------------------------------------------------------*/
/* pre, post condition and check initialised                                */
/*--------------------------------------------------------------------------*/
#ifdef __DEBUG_CHECKS

  #define _checkCondition_emb_(checkValue)   \
  if (typeid(checkValue) == typeid(int)) {   \
    if (checkValue == _MAGIC_) {             \
      _magic_emb_();                         \
    }                                        \
    else if (checkValue == _ADDRESS_)        \
    {                                        \
      _poolAddress_emb_(this);               \
    }                                        \
    else if (checkValue == _FOOTER_)         \
    {                                        \
      _footer_emb_();                        \
    }                                        \
    else {                                   \
      ASSERT("Should never be reached"_Stz); \
    }                                        \
  }                                          \
  else {                                     \
    if (checkValue != CN_NONE)               \
    {                                        \
    _class_emb_(checkValue);                 \
    }                                        \
  }

#endif
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
