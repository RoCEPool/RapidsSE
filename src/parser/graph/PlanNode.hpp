/*============================================================================*/
/* PlanNode.hpp - A base class of a generic node in the plan graph            */
/*============================================================================*/
#pragma once

namespace Parser
{
  struct Value {
    union {
      i8 (*m_afFun)(void *aArg);  // function pointer
      b1         m_bBoolean;   // boolean value
      i8         m_iInt;       // signed 8-byte integer value
      u8         m_uInt;       // unsigned 8-byte integer value
      d8         m_dFloat;     // 8-byte floating point value
      Text*      m_pText;      // text
      //  //    PlanNode*  m_pXref;      // cross-ref to another PlanNode
    };
    b1 bMustFree; // do we need to delete the object stored in the structure?

    // constructor initialisation
    Value() : m_uInt(0), bMustFree(false) {/*noop*/}
  };

  class PlanNode : public Object,
                   public ListItem_t<PlanNode>
  {
  protected:
    s256  m_sName;
    s256  m_sType;
    Value m_xValue;
    PlanNodeList* m_pProperties;
    PlanNodeList* m_pChildren;
    
  private:
    Footer m_xFooter;
    
  public:
    virtual ~PlanNode(void) = 0;

    Footer* aFooter(void);

  protected:
    // Protected constructor to force inheritence of this base class.
    PlanNode(eCN eClassNo, Stz* szName, Stz* szType, Value xValue,
             PlanNodeList* pProperties, PlanNodeList* pChildren);

  public:
    Stz* getName();
    Stz* getType();

    // Convenience methods for accessing the value object
    Value getValue();
    b1   getBooleanValue();
    i8   getIntValue();
    u8   getUnsignedIntValue();
    d8   getFloatValue();
    Stz* getStringValue();
  };
  
  PlanNode* _pPlanNode(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
