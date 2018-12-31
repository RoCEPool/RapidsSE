/*============================================================================*/
/* StrVar.hpp -- fixed-length string variable (template)                      */
/*============================================================================*/
#pragma once

namespace Values
{
  template <int SIZE>
  class StrVar : public StrVal
  {
  private:
    char m_cArea[SIZE]; // Stz byte area
    
    // must be last
    Footer m_xFooter;
    
  public:    
    StrVar(void);
    StrVar(Stz* aArg);
    StrVar(StrVar* pThis);
   ~StrVar(void) noexcept(false);
    
    void operator =(i8);
    void operator =(Stz* aArg);
    void operator =(StrVal& xArg);
    void operator =(StrVal* pArg);
    void operator =(ValWrk& rArg);
  
    void vTest(void);
  
    Footer* aFooter(void);
  
    void vMaxValue(void);
    void vMidValue(void);
    void vMinValue(void);
    void vNullValue(void);
  };
  
  void* _pStrVar(void* aAddr);
}

using namespace Values;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <int SIZE>
StrVar<SIZE>::StrVar(void)
: StrVal(CN_STRVAR,SIZE)
{
  _precondition_(_MAGIC_,CN_STRVAR,_FOOTER_);
  
//#ifdef _DEBUG
//  if (SIZE > C_4KB) {
//    Base::vCheckAddress(this);
//  }
//#endif
  
  _checkInitialisedSize_(SIZE);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <int SIZE>
StrVar<SIZE>::StrVar(Stz* aArg)
: StrVal(CN_STRVAR,SIZE)
{
  _precondition_(_MAGIC_,CN_STRVAR,_FOOTER_);
  
//#ifdef _DEBUG
//  if (SIZE > C_4KB) {
//    Base::vCheckAddress(this);
//  }
//#endif
 
  StrVal::vCopy(aArg);
  
  _checkInitialisedSize_(SIZE);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <int SIZE>
StrVar<SIZE>::StrVar(StrVar*)
: StrVal(CN_STRVAR,SIZE)
{
  _precondition_(_MAGIC_,CN_STRVAR,_FOOTER_);
  
//#ifdef _DEBUG
//  if (SIZE > C_4KB) {
//    Base::vCheckAddress(this);
//  }
//#endif
  
  _checkInitialisedSize_(SIZE);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <int SIZE>
StrVar<SIZE>::~StrVar(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_STRVAR,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void StrVar<SIZE>::operator =(i8 iArg)
{
  _precondition_(_MAGIC_,CN_STRVAR,_FOOTER_);
  
  StrLib::wToString(iArg,(char*)aStz());
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void StrVar<SIZE>::operator =(Stz* aArg)
{
  _precondition_(_MAGIC_,CN_STRVAR,_FOOTER_);
  
  StrVal::vCopy(aArg);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void StrVar<SIZE>::operator =(StrVal& rArg)
{
  _precondition_(_MAGIC_,CN_STRVAR,_FOOTER_);
  
  vCopy(rArg);
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void StrVar<SIZE>::operator =(StrVal* pArg)
{
  _precondition_(_MAGIC_,CN_STRVAR,_FOOTER_);
  
  vCopy(pArg->aStz());
}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void StrVar<SIZE>::operator =(ValWrk& rArg)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  vCopy(rArg.aWspArea());
  
  _postcondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* test Stz                                                                   */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void StrVar<SIZE>::vTest(void)
{
  _precondition_(_MAGIC_,CN_STRVAR);

  aStz()->vPrint();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* max value                                                                  */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void StrVar<SIZE>::vMaxValue(void)
{
  StrVal::vCopy("Z"_Stz);
}

/*----------------------------------------------------------------------------*/
/* mid value                                                                  */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void StrVar<SIZE>::vMidValue(void)
{
  StrVal::vCopy("M"_Stz);
}

/*----------------------------------------------------------------------------*/
/* min value                                                                  */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void StrVar<SIZE>::vMinValue(void)
{
  StrVal::vCopy("A"_Stz);
}

/*----------------------------------------------------------------------------*/
/* null value                                                                 */
/*----------------------------------------------------------------------------*/
template <int SIZE>
void StrVar<SIZE>::vNullValue(void)
{
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <int SIZE>
Footer* StrVar<SIZE>::aFooter(void)
{
  _precondition_(_MAGIC_,CN_STRVAR);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
