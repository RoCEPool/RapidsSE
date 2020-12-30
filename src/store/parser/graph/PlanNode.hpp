/*============================================================================*/
/* PlanNode.hpp - A base class of a generic node in the plan graph            */
/*============================================================================*/
#pragma once

namespace Parser
{
  class PlanNode;
  typedef ListExo_t<CN_PLANNODELIST, PlanNode> PlanNodeList;

  // A multi-value struct used in every PlanNode
  struct Value {
    union {
      i8 (*m_afFun)(void *aArg);  // function pointer
      b1         m_bBoolean;   // boolean value
      i8         m_iInt;       // signed 8-byte integer value
      u8         m_uInt;       // unsigned 8-byte integer value
      d8         m_dFloat;     // 8-byte floating point value
      Text*      m_pText;      // text
    };
    b1 m_bMustFree; // do we need to delete the object stored in the structure?

    // Default constructor initialisation
    Value()
      : m_uInt(0), m_bMustFree(false)
      {
        /* noop */
      }

    // Constructor (copy stz)
    Value(Stz* szStz)
    {
      if (szStz) {
        m_pText = new Text(szStz);
        m_bMustFree = true;
      } else {
        m_pText = nullptr;
        m_bMustFree = false;
      }
    }

    // Constructor (take ownership of Text*)
    Value(Text* pText)
    {
      if (pText) {
        m_pText = pText;
        m_bMustFree = true;
      } else {
        m_pText = nullptr;
        m_bMustFree = false;
      }
    }
  };

  class PlanNode : public Object
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

    PlanNode(eCN eClassNo, const PlanNode& oNode);

  public:
    Stz* szName();
    Stz* szType();

    // Convenience methods for accessing the value object
    Value getValue();
    b1   getBooleanValue();
    i8   getIntValue();
    u8   getUnsignedIntValue();
    d8   getFloatValue();
    Stz* getStringValue();

    b1 bHasProperties();
    b1 bHasChildren();

    i8 nProperties();
    i8 nChildren();

    PlanNode* pNthProperty(i8 idx);
    PlanNode* pNthChild(i8 idx);
  };
  
  PlanNode* _pPlanNode(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
