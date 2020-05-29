/*============================================================================*/
/* MP.hpp -- mandatory (not nullable) pointers                                */
/*============================================================================*/
#pragma once


namespace Gadgets
{
  template <class OBJECT, eCN eClassNo>
  class MP
  {
  public:
    OBJECT* m_pPointer;
    
    /*------------------------------------------------------------------------*/
    /* constructor                                                            */
    /*------------------------------------------------------------------------*/
    MP<OBJECT,eClassNo>(OBJECT* pObject)
    {
      if (!pObject || (pObject && pObject->eClass() != eClassNo)) {
        THROW("MP::MP invalid pObject parameter"_Stz);
      }
      m_pPointer = pObject;
    }
    
    /*------------------------------------------------------------------------*/
    /* assignment                                                             */
    /*------------------------------------------------------------------------*/
    void operator = (OBJECT* pObject)
    {
      if (!pObject || (pObject && pObject->eClass() != eClassNo)) {
        THROW("MP::operator =() invalid pObject parameter"_Stz);
      }
      m_pPointer = pObject;
    }
    
    /*------------------------------------------------------------------------*/
    /* member                                                                 */
    /*------------------------------------------------------------------------*/
    OBJECT* operator -> (void)
    {
      if (m_pPointer == nullptr) {
        THROW("MP::operator ->() m_pPointer member is null"_Stz);
      }
      return m_pPointer;
    }
    
    /*------------------------------------------------------------------------*/
    /* dereference                                                            */
    /*------------------------------------------------------------------------*/
    OBJECT* operator * (void)
    {
      if (m_pPointer == nullptr) {
        THROW("MP::operator *() m_pPointer member is null"_Stz);
      }
      return m_pPointer;
    }
    
    /*------------------------------------------------------------------------*/
    /* == operator                                                            */
    /*------------------------------------------------------------------------*/
    bool operator == (OBJECT* object)
    {
      return m_pPointer == object;
    }
    
    /*------------------------------------------------------------------------*/
    /* != operator                                                            */
    /*------------------------------------------------------------------------*/
    bool operator != (OBJECT* object)
    {
      return m_pPointer != object;
    }
    
  };
}

namespace Tasks {
  
  typedef MP<Task,CN_TASK> MP_Task;
  
  //typedef void (*StepFp)(Forth* pForth);
  typedef void (Step::*StepFp)(void);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
