/*============================================================================*/
/* REF_t.hpp -- counted reference pointer                                     */
/*============================================================================*/
#pragma once



namespace Gadgets {
  template <class CLASSNM> class CRM;
}

namespace Correctness
{
  template <class CLASSNM>
  class REF
  {
    friend class CRM<CLASSNM>;
  
  private:
  
    CLASSNM* m_pReferencedObject; // pointer to the referenced object
  
  public:
  
    /*------------------------------------------------------------------------*/
    /* constructor                                                            */
    /*------------------------------------------------------------------------*/
    REF<CLASSNM>(void)
    {
      m_pReferencedObject = nullptr;
    }
  
    /*------------------------------------------------------------------------*/
    /* parameterized constructor                        (nullptr not allowed) */
    /*------------------------------------------------------------------------*/
//    REF<CLASSNM>(CRM<CLASSNM>* pReferenceTo)
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
    ~REF<CLASSNM>(void)
    {
      RASSERT(! m_pReferencedObject);
    }

    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
  
    /*------------------------------------------------------------------------*/
    /* member operator ->                               (nullptr not allowed) */
    /*------------------------------------------------------------------------*/
    CLASSNM* operator -> (void)
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
    /* get the referenced object                                              */
    /*------------------------------------------------------------------------*/
    CLASSNM* pReferencedObject(void)
    {
      return m_pReferencedObject;
    }
  
    /*------------------------------------------------------------------------*/
    /* get the number of references                                           */
    /*------------------------------------------------------------------------*/
    i8 nReferences(void)
    {
      return m_pReferencedObject->nReferences();
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
