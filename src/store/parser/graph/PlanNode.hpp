/*============================================================================*/
/* PlanNode.hpp - A base class of a generic node in the plan graph            */
/*============================================================================*/
#pragma once

namespace Parser
{
  // Forward declarations and typedefs
  class PlanNode;
  typedef ListExo_t<CN_PLANNODELIST, PlanNode> PlanNodeList;
  typedef ListExoIter_t<PlanNode> PlanNodeIter;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  // First we need to define a multi-value struct used in every PlanNode
  struct Value
  {

    // Enum describing the type of value contained in the union
    enum Type
    {
      VT_NONE = 0,
      VT_UNKNOWN, // NB: this is not the same as the value being null
      VT_BOOLEAN,
      VT_INT,
      VT_UINT,
      VT_FLOAT,
      VT_STRING,
      VT_DATE,
      VT_TIME,
      VT_TIMESTAMP,
      VT_INTERVAL,
      VT_NENUMS
    };

    union
    {
      b1 m_bBoolean;
      i8 m_iInt;        // signed 8-byte integer value
      u8 m_uInt;        // unsigned 8-byte integer value
      d8 m_dFloat;      // 8-byte floating point value
      Text *m_pText;    // string data
      Dte m_xDate;      // date
      Tme m_xTime;      // time
      Tsp m_xTsp;       // timestamp
      Ivl m_xIvl;       // interval
    };

    Type m_eType;     // The type of the value stored in the union.
    b1 m_bMustFree; // Do we need to delete the pointer stored in the structure?
    b1 m_bIsNull;   // True if the value is null. Note that the value may be non-null
    // but the type is UNKNOWN.

    // Default constructor initialisation
    Value()
        : m_uInt(0), m_eType(VT_UNKNOWN), m_bMustFree(false), m_bIsNull(true)
    { /* noop */ }

    // Constructor (type only, null value)
    Value(Value::Type eType)
        : m_uInt(0), m_eType(eType), m_bMustFree(false), m_bIsNull(true)
    { /* noop */ }

    // Constructor (boolean)
    Value(b1 bVal)
        : m_bBoolean(bVal), m_eType(VT_BOOLEAN), m_bMustFree(false), m_bIsNull(false)
    { /* noop */ }

    // Constructor (integer)
    Value(i8 iVal)
        : m_iInt(iVal), m_eType(VT_INT), m_bMustFree(false), m_bIsNull(false)
    { /* noop */ }

    // Constructor (unsigned integer)
    Value(u8 uVal)
        : m_uInt(uVal), m_eType(VT_UINT), m_bMustFree(false), m_bIsNull(false)
    { /* noop */ }

    // Constructor (float)
    Value(d8 dVal)
        : m_dFloat(dVal), m_eType(VT_FLOAT), m_bMustFree(false), m_bIsNull(false)
    { /* noop */ }

    // Constructor (copy stz)
    Value(Stz *szStz)
    {
      m_eType = VT_STRING;

      if (szStz) {
        m_pText = new Text(szStz);
        m_bMustFree = true;
        m_bIsNull = false;
      } else {
        m_pText = nullptr;
        m_bMustFree = false;
        m_bIsNull = true;
      }
    }

    // Constructor (take ownership of Text*)
    Value(Text *pText)
    {
      m_eType = VT_STRING;

      if (pText) {
        m_pText = pText;
        m_bMustFree = true;
        m_bIsNull = false;
      } else {
        m_pText = nullptr;
        m_bMustFree = false;
        m_bIsNull = true;
      }
    }

    // Constructor (date)
    Value(const Dte xDate)
        : m_xDate(xDate), m_eType(VT_DATE), m_bMustFree(false), m_bIsNull(false)
    { /* noop */ }

    // Constructor (time)
    Value(const Tme xTime)
        : m_xTime(xTime), m_eType(VT_TIME), m_bMustFree(false), m_bIsNull(false)
    { /* noop */ }

    // Constructor (timestamp)
    Value(const Tsp xTsp)
        : m_xTsp(xTsp), m_eType(VT_TIMESTAMP), m_bMustFree(false), m_bIsNull(false)
    { /* noop */ }

    // Constructor (interval)
    Value(const Ivl xIvl)
        : m_xIvl(xIvl), m_eType(VT_INTERVAL), m_bMustFree(false), m_bIsNull(false)
    { /* noop */ }

    // Copy constructor
    Value(const Value &xVal)
    {
      m_eType = xVal.m_eType;
      m_bMustFree = xVal.m_bMustFree;
      m_bIsNull = xVal.m_bIsNull;

      switch (m_eType) {
        case VT_STRING:
          if (xVal.m_pText) {
            m_pText = new Text(xVal.m_pText->szGet());
            m_bMustFree = true;

          } else {
            m_pText = nullptr;
            m_bIsNull = true;
          }
          break;

        default:
          // Assumption, the union is only 8 bytes wide
          m_iInt = xVal.m_iInt;
          break;
      }
    }

    // Move Constructor (takes ownership of pointers)
    Value(Value &&xOtherVal)
    {
      m_eType = xOtherVal.m_eType;
      m_bMustFree = xOtherVal.m_bMustFree;
      m_bIsNull = xOtherVal.m_bIsNull;

      switch (m_eType) {
        case VT_STRING:
          if (xOtherVal.m_pText) {
            // Steal the string
            m_pText = xOtherVal.m_pText;
            xOtherVal.m_pText = nullptr;
            xOtherVal.m_bMustFree = false;

          } else {
            m_pText = nullptr;
            m_bIsNull = true;
          }
          break;

        default:
          // Assumption, the union is only 8 bytes wide
          m_iInt = xOtherVal.m_iInt;
          break;
      }
    }

    // Move assignment (takes ownership of pointers)
    Value &operator=(Value &&xOtherVal)
    {
      if (this == &xOtherVal) {
        return *this;
      }

      ASSERT(!m_bMustFree); // must already have been deallocated
      //vDealloc();

      m_eType = xOtherVal.m_eType;
      m_bMustFree = xOtherVal.m_bMustFree;
      m_bIsNull = xOtherVal.m_bIsNull;

      switch (m_eType) {
        case VT_STRING:
          if (xOtherVal.m_pText) {
            // Steal the string
            m_pText = xOtherVal.m_pText;
            xOtherVal.m_pText = nullptr;
            xOtherVal.m_bMustFree = false;

          } else {
            m_pText = nullptr;
            m_bIsNull = true;
          }
          break;

        default:
          // Assumption, the union is only 8 bytes wide
          m_iInt = xOtherVal.m_iInt;
          break;
      }

      return *this;
    }

    // Equality operator
    b1 operator==(const Value& xOther)
    {
      if (this == &xOther)  return true;

      if (    m_eType != xOther.m_eType)      return false;
      if (m_bMustFree != xOther.m_bMustFree)  return false;
      if (  m_bIsNull != xOther.m_bIsNull)    return false;

      switch (m_eType) {
        case VT_UNKNOWN:
          // Not return false here because we are comparing PlanNodes, not SQL NULLs.
          break;

        case VT_BOOLEAN:
          if (m_bBoolean != xOther.m_bBoolean)  return false;
          break;

        case VT_INT:
          if (m_iInt != xOther.m_iInt)          return false;
          break;

        case VT_UINT:
          if (m_uInt != xOther.m_uInt)          return false;
          break;

        case VT_FLOAT:
          if (m_dFloat != xOther.m_dFloat)      return false;
          break;

        case VT_STRING:
          if (m_bIsNull) {
            // Both are null. Just compare pointers.
            if (m_pText != xOther.m_pText)    return false;

          } else {
            // Compare the strings
            if (!m_pText->szGet()->bEqual(xOther.m_pText->szGet())) return false;
          }
          break;

          case VT_DATE:
            if (m_xDate.u.m_iAsI8 != xOther.m_xDate.u.m_iAsI8)  return false;
            break;

          case VT_TIME:
            if (m_xTime.m_iAsI8 != xOther.m_xTime.m_iAsI8)  return false;
            break;

          case VT_TIMESTAMP:
            if (m_xTsp.m_iAsI8 != xOther.m_xTsp.m_iAsI8)  return false;
            break;

          case VT_INTERVAL:
            NOT_IMPLEMENTED_YET

        default:
          NEVER_REACH
      }

      // We are equal
      return true;
    }

    // Inequality operator
    b1 operator!=(const Value& xOther)
    {
      return !(*this == xOther);
    }

    // Destructor
    ~Value()
    {
      if (!m_bMustFree) {
        return;
      }

      switch (m_eType) {
        case VT_STRING:
          if (m_pText) {
            delete m_pText;
          }
          m_pText = nullptr;
          break;

        default:
          NEVER_REACH
      }

      m_bMustFree = false;
    }

    // Create a deep duplicate of Value and return
    Value xDup()
    {
      Value newVal;
      newVal.m_eType = m_eType;
      newVal.m_bMustFree = m_bMustFree;
      newVal.m_bIsNull = m_bIsNull;

      switch (m_eType) {
        case VT_STRING:
          if (m_pText) {
            // copy the string
            newVal.m_pText = new Text(m_pText->szGet());
            newVal.m_bMustFree = true;

          } else {
            m_pText = nullptr;
            m_bIsNull = true;
            m_bMustFree = false;
          }
          break;

        default:
          // Assumption, the union is only 8 bytes wide
          newVal.m_iInt = m_iInt;
          break;
      }

      return newVal;
    }

    // Set the value to a null value
    void vSetNull(Value::Type eType)
    {
      vDealloc();
      m_eType = eType;
      m_bMustFree = false;
      m_bIsNull = true;
    }

    // Set a boolean into the value
    void vSet(b1 bVal)
    {
      vDealloc();
      m_eType = VT_BOOLEAN;
      m_bMustFree = false;
      m_bIsNull = false;
      m_bBoolean = bVal;
    }

    // Set an integer number into the value
    void vSet(i8 iVal)
    {
      vDealloc();
      m_eType = VT_INT;
      m_bMustFree = false;
      m_bIsNull = false;
      m_iInt = iVal;
    }

    // Set an unsigned integer number into the value
    void vSet(u8 uVal)
    {
      vDealloc();
      m_eType = VT_UINT;
      m_bMustFree = false;
      m_bIsNull = false;
      m_uInt = uVal;
    }

    // Set a floating point number into the value
    void vSet(d8 dVal)
    {
      vDealloc();
      m_eType = VT_FLOAT;
      m_bMustFree = false;
      m_bIsNull = false;
      m_dFloat = dVal;
    }

    // Set a string into the value, taking ownership of the Text*
    void vSet(Text *pVal)
    {
      if (!pVal) {
        vSetNull(VT_STRING);
        return;
      }

      vDealloc();
      m_eType = VT_STRING;
      m_bMustFree = true;
      m_bIsNull = false;
      m_pText = pVal;   // take ownership of string
    }

    // Set a string into the value, copying it.
    void vSet(Stz *szVal)
    {
      if (!szVal) {
        vSetNull(VT_STRING);
        return;
      }

      vDealloc();
      m_eType = VT_STRING;
      m_bMustFree = true;
      m_bIsNull = false;
      m_pText = new Text(szVal);
    }

    // Set a date value in struct
    void vSet(i8 iYear, i8 iMonth, i8 iDay)
    {
      vDealloc();
      m_eType = VT_DATE;
      m_bMustFree = false;
      m_bIsNull = false;
      m_xDate.vSet(iYear, iMonth, iDay);
    }

    // Set a time value in struct
    void vSet(i8 iHour, i8 iMin, i8 iSec, i8 iMicro)
    {
      vDealloc();
      m_eType = VT_TIME;
      m_bMustFree = false;
      m_bIsNull = false;
      m_xTime.vSet(iHour, iMin, iSec, iMicro);
    }

    // Set a timestamp value in struct
    void vSet(i8 iYear, i8 iMonth, i8 iDay, i8 iHour, i8 iMin, i8 iSec, i8 iMicro)
    {
      vDealloc();
      m_eType = VT_TIMESTAMP;
      m_bMustFree = false;
      m_bIsNull = false;
      m_xTsp.vSet(iYear, iMonth, iDay, iHour, iMin, iSec, iMicro);
    }

    // Deallocate any pointers, if required
    void vDealloc()
    {
      if (m_bMustFree) {

        switch (m_eType) {
          case VT_STRING:
            if (m_pText) {
              delete m_pText;
              m_pText = nullptr;
              m_bMustFree = false;
            }
            break;

          default:
            NEVER_REACH
        }
      }
    }

    // Reset the contents of the struct back to initial settings.
    // Note, no deallocation of existing pointers on purpose.
    void vReset()
    {
      // NB: no dealloc() called here, on purpose
      m_eType = VT_UNKNOWN;
      m_bMustFree = false;
      m_bIsNull = true;
      m_iInt = 0;
    }

    // Describes the value by printing it into a buffer
    void vDescribe(s2K& sBuf)
    {
      if (m_bIsNull) {
        sBuf.vCopy("(null)"_Stz);
        return;
      }

      switch (m_eType) {

        case VT_UNKNOWN:
          sBuf.vCopy("(unknown)"_Stz);
          break;

        case VT_BOOLEAN:
          sBuf.vSprint("%s"_Stz, m_bBoolean ? "true"_Stz : "false"_Stz);
          break;

        case VT_INT:
          sBuf.vSprint("%lld"_Stz, m_iInt);
          break;

        case VT_UINT:
          sBuf.vSprint("%lld"_Stz, m_uInt);
          break;

        case VT_FLOAT:
          sBuf.vSprint("%g"_Stz, m_dFloat);
          break;

        case VT_STRING:
          sBuf.vCopy(m_pText->szGet());
          break;

        case VT_DATE:
          sBuf.vSprint("%lld-%lld-%lld"_Stz, m_xDate.iYear(), m_xDate.iMonth(), m_xDate.iDay());
          break;

        case VT_TIME:
          sBuf.vSprint("%lld:%lld:%lld"_Stz, m_xTime.iHour(), m_xTime.iMin(), m_xTime.iSec());
          break;

        case VT_TIMESTAMP:
          sBuf.vSprint("%lld-%lld-%lldT%lld:%lld:%lld.%lld"_Stz,
                       m_xTsp.iYear(), m_xTsp.iMonth(), m_xTsp.iDay(),
                       m_xTsp.iHour(), m_xTsp.iMin(), m_xTsp.iSec(), m_xTsp.iMicro());
          break;

        case VT_INTERVAL:
          NOT_IMPLEMENTED_YET

        default:
          NEVER_REACH
      }
    }
  };

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  class PlanNode : public /*VIRTUAL*/ Object
  {
    friend class GraphValidator;  // to aide validation of the plan graph
    friend class GraphEditor;     // Contains extra node/graph manipulation utilities

  protected:
    s256  m_sName;                // Name of the node. Must be unique amongst its siblings
    s256  m_sType;                // Path to the node that defines this node
    Value m_xValue;               // Storage of a simple value
    PlanNodeList* m_pProperties;  // List of properties of this node
    PlanNodeList* m_pChildren;    // List of children of this node
    
  private:
    Footer m_xFooter;
    
  public:
    virtual ~PlanNode(void);

    Footer* aFooter(void);

  protected:
    // Protected constructor to force inheritence of this base class.
    PlanNode(eCN eClassNo, Stz* szName, Stz* szType, Value&& xValue,
             PlanNodeList* pProperties, PlanNodeList* pChildren);

    PlanNode(eCN eClassNo, const PlanNode& oNode);

  public:
    Stz* szName();
    Stz* szType();

    // Convenience methods for accessing the value object
    Value xValue();
    Value* pValue();
    b1   bIsNullValue();
    Value::Type eValueType();

    b1   bValue();
    i8   iValue();
    u8   uValue();
    d8   dValue();
    Stz* szValue();
    Dte  xDateVal();
    Tme  xTimeVal();
    Tsp  xTspVal();
    Ivl  xIvlVal();

    b1 bHasProperties();
    b1 bHasChildren();

    i8 nProperties();
    i8 nChildren();

    // Allocates and returns an iterator for the properties or children of this node.
    // May be null if there are none. The iterator must be deleted after use.
    PlanNodeIter* pNewPropertyIter();
    PlanNodeIter* pNewChildrenIter();

    // Convenience functions for accessing children
    PlanNode* pFirstChild();
    PlanNode* pSecondChild();
    PlanNode* pNthChild(i8 iIdx); // 0-based index
    PlanNode* pLastChild();
  };
  
  PlanNode* _pPlanNode(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
