/*============================================================================*/
/* REG_t.hpp -- register of logic unit                                        */
/*============================================================================*/
#pragma once

#include "eRF.h"

namespace STORE
{
  template <class COMPONENT, eCN CLASSNO>
  class REG
  {
  public:

    COMPONENT* m_pComponent; // pointer to the component object
    eCN        m_eClassNo;   // class no of the component

    LogicUnit* m_p_LogicUnit; // which logic unit
    Stz*       m_szRegNm;     // register name
    eRF        m_eRepair;     // how to repair
  
    void     (*m_fpRepair)(Object*, eRF eRepair); // the component repair function


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
    REG<COMPONENT,CLASSNO>(LogicUnit* pLogicUnit,
                   eRF eRepair,
                   void (*fpRepair)(Object* pObject, eRF eRepair),
                   Stz* szRegNm)
    {
      m_p_LogicUnit = pLogicUnit;
      m_szRegNm    = szRegNm;
      m_eRepair    = eRepair;
      m_fpRepair   = fpRepair;
    
      m_pComponent = nullptr;
      m_eClassNo   = CLASSNO;

      m_bUseAllowed      = true;
      m_bInitialised     = true;
      m_bFinalised       = true;
    
      m_bNullOnDelete    = false;
      m_bDeleteOnFailure = false;
    
      m_uSpare = 0;
    
      AM::LOGICUNIT::vAddReg(pLogicUnit,this);
    }

    /*------------------------------------------------------------------------*/
    /* constructor                                                            */
    /*------------------------------------------------------------------------*/
    REG<COMPONENT,CLASSNO>(LogicUnit* pLogicUnit,
                   eRF eRepair,
                   void (*fpRepair)(Object* pObject, eRF eRepair),
                   bool bNullOnDelete,
                   bool bDeleteOnFailure,
                   Stz* szRegNm)
    {
      m_p_LogicUnit = pLogicUnit;
      m_szRegNm    = szRegNm;
      m_eRepair    = eRepair;
      m_fpRepair   = fpRepair;

      m_pComponent = nullptr;
      m_eClassNo   = CLASSNO;
    
      m_bUseAllowed      = true;
      m_bInitialised     = true;
      m_bFinalised       = true;

      m_bNullOnDelete    = bNullOnDelete;
      m_bDeleteOnFailure = bDeleteOnFailure;

      m_uSpare = 0;

      AM::LOGICUNIT::vAddReg(pLogicUnit,this);
    }

    /*------------------------------------------------------------------------*/
    /* destructor                                                             */
    /*------------------------------------------------------------------------*/
    ~REG<COMPONENT,CLASSNO>(void)
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
      if (pComponent && ! AM::bInBufferPool(pComponent)) {
        vTrace("%20s: %p \n"_Stz,m_szRegNm,m_pComponent);
        THROW("REG::operator =(pComponent) : invalid pComponent address"_Stz);
      }
    
      if (pComponent && CN_NONE != m_eClassNo) {
        eCN eClassNo = pComponent->eGetClassNo();
        vTrace("Attempting to assign %p %lld\n"_Stz
               ,pComponent
               ,eClassNo
               );
        RASSERT(eClassNo == m_eClassNo);
      }
    
      m_pComponent = pComponent;
    }
  
    /*------------------------------------------------------------------------*/
    /* member operator ->                               (nullptr not allowed) */
    /*------------------------------------------------------------------------*/
    COMPONENT* operator -> (void)
    {
      if (! m_pComponent) {
        vTrace("%20s: %p \n"_Stz,m_szRegNm,m_pComponent);
        THROW("REG::operator ->(void) : m_pComponent member is null"_Stz);
      }
    
      return m_pComponent;
    }
  
    /*------------------------------------------------------------------------*/
    /* member operator *                               (nullptr not allowed) */
    /*------------------------------------------------------------------------*/
    COMPONENT* operator * (void)
    {
      if (! m_pComponent) {
        vTrace("%20s: %p \n"_Stz,m_szRegNm,m_pComponent);
        THROW("REG::operator *() : m_pComponent member is null"_Stz);
      }
    
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
    /* dump the register                                                      */
    /*------------------------------------------------------------------------*/
    void vDump(void)
    {
      if (m_pComponent) {
        vTrace("%20s : %p ; \n"_Stz
               ,m_szRegNm
               ,m_pComponent
        );
      }
    }

    /*------------------------------------------------------------------------*/
    /* gurded delete object                                                   */
    /*------------------------------------------------------------------------*/
    bool vGuardedDeleteComponent(Object* pDeleter, Stz* szFunc)
    {
      m_pComponent->vGuardedDelete(m_pComponent,pDeleter,szFunc);
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
    /* call m_pComponent repair function to repair itself                     */
    /*------------------------------------------------------------------------*/
    void vRepair(void)
    {

      // m_pComponent already NULL ?
      if ( !m_pComponent ) {
        // nothing to do
        return;
      }

      // RF_REPAIR_NEVER ?
      // Should never reach here for a RF_REPAIR_NEVER ?
      RASSERT(m_eRepair != RF_REPAIR_NEVER);

      // RF_REPAIR_NONE ?
      if (m_eRepair == RF_REPAIR_NONE) {
        // nothing to do
        return;
      }

      // RF_SET_NULL ?
      if (m_eRepair == RF_SET_NULL) {
        // null the component
        m_pComponent = nullptr;
        RASSERT(!m_fpRepair);
        return;
      }

      // Following code to be uncommented once all repairs in place
      /*
      RASSERT(m_fpRepair);
      m_fpRepair((Object *) m_pComponent, m_eRepair);

      // null the component
      m_pComponent = nullptr;
      */

      // execute the repair function ?
      if (m_fpRepair) {
        m_fpRepair((Object *) m_pComponent, m_eRepair);

        // null the component
        m_pComponent = nullptr;
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
      m_p_LogicUnit = pLogicUnit;
    }

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
