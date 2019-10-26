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
  
    CLASS* m_pReferencedObject; // pointer to the referenced object
  
  public:
  
    /*------------------------------------------------------------------------*/
    /* constructor                                                            */
    /*------------------------------------------------------------------------*/
    REF<CLASS>(void)
    {
      m_pReferencedObject = nullptr;
    }
  
    /*------------------------------------------------------------------------*/
    /* parameterized constructor                        (nullptr not allowed) */
    /*------------------------------------------------------------------------*/
//    REF<CLASS>(CRM<CLASS>* pReferenceTo)
//    {
//      if ( ! pReferenceTo || ! AM::bInBufferPool(pReferenceTo) )
//        THROW("REF::REF(pReferenceTo) : invalid pReferenceTo parameter"_Stz);
//
//      m_pReferencedObject = nullptr;
//
//      pReferenceTo->vBorrowRefBy(this);
//    }
  
    /*------------------------------------------------------------------------*/
    /* destructor                                                             */
    /*------------------------------------------------------------------------*/
    ~REF<CLASS>(void)
    {
      RASSERT(! m_pReferencedObject);
    }

    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
  
    /*------------------------------------------------------------------------*/
    /* member operator ->                               (nullptr not allowed) */
    /*------------------------------------------------------------------------*/
    CLASS* operator -> (void)
    {
      if (! m_pReferencedObject)
        THROW("REF::operator ->(void) : m_pObject member is null"_Stz);
   
      return m_pReferencedObject;
    }
  
    /*------------------------------------------------------------------------*/
    /* is there a reference?                                                  */
    /*------------------------------------------------------------------------*/
    bool bHasReference(void)
    {
      if (m_pReferencedObject)
        return true;
      else
        return false;
    }
  
    /*------------------------------------------------------------------------*/
    /* is pointer valid?                                (nullptr not allowed) */
    /*------------------------------------------------------------------------*/
    bool bIsValid(void)
    {
      if ( ! m_pReferencedObject ||
           (m_pReferencedObject && AM::bInBufferPool(m_pReferencedObject)) )
        return true;
      else
        return false;
    }
  
    /*------------------------------------------------------------------------*/
    /* return reference                                                       */
    /*------------------------------------------------------------------------*/
    void vReturnRef(void)
    {
      RASSERT(m_pReferencedObject);
      m_pReferencedObject->vReturnRefBy(this);
    }
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
