/*============================================================================*/
/* CRM.hpp -- counted reference manager                                       */
/*============================================================================*/
#pragma once



namespace Gadgets
{
  template <class CLASSNM>
  class CRM
  {
  public:
  
    CLASSNM* m_pThis; // pointer to the object
  
    Atomic m_nxReferences; // number of references
  
  public:
  
    /*------------------------------------------------------------------------*/
    /* parameterized constructor                        (nullptr not allowed) */
    /*------------------------------------------------------------------------*/
    CRM<CLASSNM>(CLASSNM* pThis)
    {
      if ( ! pThis || ! AM::bInBufferPool(pThis) )
        THROW("CRM::CRM(pThis) : invalid pThis parameter"_Stz);
    
      m_pThis = pThis;
    }
    
    /*------------------------------------------------------------------------*/
    /* destructor                                                             */
    /*------------------------------------------------------------------------*/
    ~CRM<CLASSNM>(void)
    {
      //! this assert may fail if a job completes before its references
      //! are completely returned
      if ( m_nxReferences.iVal() )
        vDebugPoint();
      
      RASSERT(! m_nxReferences.iVal());
    }
  
    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
  
  
    /*------------------------------------------------------------------------*/
    /* borrow a reference to this object                                      */
    /*------------------------------------------------------------------------*/
    void vBorrowRefBy(REF<CLASSNM>* pBorrowingRef)
    {
       RASSERT(pBorrowingRef);
       RASSERT(! pBorrowingRef->m_pReferencedObject);

       ++ m_nxReferences;
    
       pBorrowingRef->m_pReferencedObject = (CLASSNM*)this;
    }
  
    /*------------------------------------------------------------------------*/
    /* return a reference to this object                                      */
    /*------------------------------------------------------------------------*/
    i8 nReferences(void)
    {
      return m_nxReferences.iVal();
    }
  
    /*------------------------------------------------------------------------*/
    /* return a reference to this object                                      */
    /*------------------------------------------------------------------------*/
    void vReturnRefBy(REF<CLASSNM>* pReturningRef)
    {
      RASSERT(pReturningRef);
      RASSERT(pReturningRef->m_pReferencedObject);

      pReturningRef->m_pReferencedObject = nullptr;
    
      -- m_nxReferences;
    }
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
