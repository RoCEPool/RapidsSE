/*============================================================================*/
/* REG_t.hpp -- register of logic unit                                        */
/*============================================================================*/
#pragma once

#include "eRF.h"

namespace STORE
{
  template <class COMPONENT>
  class REG
  {
  public:
  
    LogicUnit* m_pLogicUnit; // which logic unit
    Stz*       m_szRegNm;    // register name
    eRF        m_eRepair;    // how to repair

    COMPONENT* m_pComponent; // pointer to the object

    struct {
      
      u8 m_bUseAllowed      :  1; // this register will be used
      u8 m_bInitialised     :  1; // this register has been initialised
      u8 m_bFinalised       :  1; // this register has been finalised
      
      u8 m_bNullOnDelete    :  1; // m_pComponent must be null on delete
      u8 m_bDeleteOnFailure :  1; // m_pComponent to be deleted on failure
      
      u8 m_uSpare           : 59; // spare bits
      //                      64b
    };
  
  public:
  
    /*------------------------------------------------------------------------*/
    /* constructor                                                            */
    /*------------------------------------------------------------------------*/
    REG<COMPONENT>(LogicUnit* pLogicUnit, eRF eRepair, void*, Stz* szRegNm)
    {
      m_pLogicUnit = pLogicUnit;
      m_szRegNm    = szRegNm;
      m_eRepair    = eRepair;
    
      m_pComponent = nullptr;
      
      m_bUseAllowed      = true;
      m_bInitialised     = true;
      m_bFinalised       = true;
    
      m_bNullOnDelete    = false;
      m_bDeleteOnFailure = false;
    
      m_uSpare = 0;
    
      AM::LOGICUNIT::vAddReg(pLogicUnit,this);
    }
  
    /*------------------------------------------------------------------------*/
    /* destructor                                                             */
    /*------------------------------------------------------------------------*/
    ~REG<COMPONENT>(void)
    {
      RASSERT(m_bInitialised);
      RASSERT(m_bFinalised);
    
      RASSERT(! m_bNullOnDelete || ! m_pComponent);
    }


    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
  
    /*------------------------------------------------------------------------*/
    /* assignment operator =                                (nullptr allowed) */
    /*------------------------------------------------------------------------*/
    void operator = (COMPONENT* pComponent)
    {
      if (pComponent && ! AM::bInBufferPool(pComponent))
        THROW("REG::operator =(pComponent) : invalid pComponent parameter"_Stz);
    
      m_pComponent = pComponent;
    }
  
    /*------------------------------------------------------------------------*/
    /* member operator ->                               (nullptr not allowed) */
    /*------------------------------------------------------------------------*/
    COMPONENT* operator -> (void)
    {
      if (! m_pComponent)
        THROW("REG::operator ->(void) : m_pComponent member is null"_Stz);
   
      return m_pComponent;
    }
  
    /*------------------------------------------------------------------------*/
    /* member operator *                               (nullptr not allowed) */
    /*------------------------------------------------------------------------*/
    COMPONENT* operator * (void)
    {
      if (! m_pComponent)
        THROW("REG::operator *() : m_pComponent member is null"_Stz);
      
      return m_pComponent;
    }
    
    /*------------------------------------------------------------------------*/
    /* delete object                                                          */
    /*------------------------------------------------------------------------*/
    bool vDeleteComponent(void)
    {
      delete m_pComponent;
      m_pComponent = nullptr;
    }
  
    /*------------------------------------------------------------------------*/
    /* is there a pointer?                                                    */
    /*------------------------------------------------------------------------*/
    bool bIsNotNull(void)
    {
      if (m_pComponent)
        return true;
      else
        return false;
    }
    
    /*------------------------------------------------------------------------*/
    /* is null pointer?                                                       */
    /*------------------------------------------------------------------------*/
    bool bIsNull(void)
    {
      if ( ! m_pComponent )
        return true;
      else
        return false;
    }
    
    /*------------------------------------------------------------------------*/
    /* is pointer valid?                                (nullptr not allowed) */
    /*------------------------------------------------------------------------*/
    bool bIsValid(void)
    {
      if ( ! m_pComponent || (m_pComponent && AM::bInBufferPool(m_pComponent)) )
        return true;
      else
        return false;
    }
  
  
  
    /*------------------------------------------------------------------------*/
    /* set null on delete                                                     */
    /*------------------------------------------------------------------------*/
    bool vNullOnDelete(bool bNullOnDelete)
    {
      m_bNullOnDelete = bNullOnDelete;
    }
  
    /*------------------------------------------------------------------------*/
    /* call m_pComponent to recover from failure                              */
    /*------------------------------------------------------------------------*/
    bool vRepair(void)
    {
      //m_pComponent->vRecoverFromFailure();
    
      switch (m_eRepair) {
          
        case RF_NONE:
          break;
          
        case RF_REPAIR_AND_DELETE:
          break;
          
        case RF_REPAIR_ONLY:
          break;
          
        case RF_DELETE_ONLY:
          break;
          
        default:
          break;
      }

      if (m_bDeleteOnFailure) {
        vDeleteComponent();
      }
    }

    /*------------------------------------------------------------------------*/
    /* use is allowed                                                         */
    /*------------------------------------------------------------------------*/
    bool vUseAllowed(void)
    {
      m_bNullOnDelete = true;
    }

    /*------------------------------------------------------------------------*/
    /* initialised                                                            */
    /*------------------------------------------------------------------------*/
    bool vInitialised(void)
    {
      m_bNullOnDelete = true;
    }

    /*------------------------------------------------------------------------*/
    /* finalised                                                              */
    /*------------------------------------------------------------------------*/
    bool vFinalised(void)
    {
      m_bFinalised = true;
    }
  
    /*------------------------------------------------------------------------*/
    /* set register name                                                      */
    /*------------------------------------------------------------------------*/
    void vRegNm(Stz* szRegNm)
    {
      m_szRegNm = C_NO_NAME;
    }

    /*------------------------------------------------------------------------*/
    /* set logic unit                                                         */
    /*------------------------------------------------------------------------*/
    void vLogicUnit(LogicUnit* pLogicUnit)
    {
      m_pLogicUnit = pLogicUnit;
    }

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
