/*============================================================================*/
/* REF.hpp -- counted reference pointer                                       */
/*============================================================================*/
#pragma once

#include "AM.hpp"

namespace Gadgets {
  template <class CLASS> class CRM;
}

namespace Correctness
{
  template <class CLASS>
  class REF
  {
    friend class CRM<CLASS>;
  
  private:
  
    CLASS* m_pObject; // pointer to the object
  
  public:
  
    /*------------------------------------------------------------------------*/
    /* constructor                                                            */
    /*------------------------------------------------------------------------*/
    REF<CLASS>(void)
    {
      m_pObject = nullptr;
    }
  
    /*------------------------------------------------------------------------*/
    /* parameterized constructor                        (nullptr not allowed) */
    /*------------------------------------------------------------------------*/
//    REF<CLASS>(CRM<CLASS>* pReferenceTo)
//    {
//      if ( ! pReferenceTo || ! AM::bInBufferPool(pReferenceTo) )
//        THROW("REF::REF(pReferenceTo) : invalid pReferenceTo parameter"_Stz);
//
//      m_pObject = nullptr;
//
//      pReferenceTo->vBorrowRefBy(this);
//    }
  
    /*------------------------------------------------------------------------*/
    /* destructor                                                             */
    /*------------------------------------------------------------------------*/
    ~REF<CLASS>(void)
    {
      RASSERT(! m_pObject);
    }

    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
  
    /*------------------------------------------------------------------------*/
    /* member operator ->                               (nullptr not allowed) */
    /*------------------------------------------------------------------------*/
    CLASS* operator -> (void)
    {
      if (! m_pObject)
        THROW("REF::operator ->(void) : m_pObject member is null"_Stz);
   
      return m_pObject;
    }
  
    /*------------------------------------------------------------------------*/
    /* is pointer valid?                                (nullptr not allowed) */
    /*------------------------------------------------------------------------*/
    bool bIsValid(void)
    {
      if ( ! m_pObject || (m_pObject && AM::bInBufferPool(m_pObject)) )
        return true;
      else
        return false;
    }
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
