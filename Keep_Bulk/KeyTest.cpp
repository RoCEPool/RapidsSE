/*============================================================================*/
/* KeyTest.cpp -- key library / test                                          */
/*============================================================================*/

#include "DBobjects.h"
#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* key testing                                                                */
/*----------------------------------------------------------------------------*/
void KeyTest::vTest(void)
{
  vTrace("\n\nKeyTest:*************************************************\n"_Stz);

  KeyWrk xKeyWrk;
  GenVar xVariable;
  KeyVal* aKeyVal;
  
  /*-----------------------------------------------------------------------*/
  vTrace("\n\n--------------- Single Element Key : INTEGER ---------------\n"_Stz);
  /*-----------------------------------------------------------------------*/

  // create the column
  Column* pCol = new Column("Col0"_Stz);
  pCol->vColumnType(CT_INTEGER);
  
  // create the column vector
  IdxColVec* pColVec = new IdxColVec;
  pColVec->vAdd(pCol);
  
  RelOpAry* pOpAryFind = new RelOpAry;
  
  KeyVec* pKeyVec = new KeyVec;
  
  // create a vector of key values (ascending order 0,1,2, ... 9)
  for (i8 i_Key = 0; i_Key < 10; i_Key++) {
  
    xVariable = i_Key;
    
    xKeyWrk.vReset();
    xKeyWrk.vAddDatum(xVariable, pCol);
    //xKeyWrk.vShow();
    
    aKeyVal = new KeyVal(xKeyWrk);
    
    pKeyVec->vAdd(aKeyVal);
  }
  
  i8 v;
  i8 nBytes;
  char* aCursor;
  
  // dump the key vector
  for (i8 i_Key = 0; i_Key < 10; i_Key++) {
    aKeyVal = pKeyVec->pGet(i_Key);
    aKeyVal->vShow(pColVec);
  }

  // asserts that keyval(n) < keyval(n+1)
  KeyVal* aKeyValA = pKeyVec->pGet(0);
  for (i8 i_Key = 1; i_Key < 10; i_Key++) {
    
    KeyVal* aKeyValB = pKeyVec->pGet(i_Key);
    ASSERT(aKeyValA->bLessThan(aKeyValB,pColVec));
    
    ASSERT(aKeyValA->iCmp(aKeyValB,pColVec) < 0);
    
    aKeyValA = aKeyValB;
  }
  
  // scan the key vector for key value: >= 3
  xVariable = (i8)3;
  xKeyWrk.vReset();
  xKeyWrk.vAddDatum(xVariable, pCol);
  KeyVal* aKeyValFind = new KeyVal(xKeyWrk);
  vTrace("Find Key value >= 3\n"_Stz);
  
  pOpAryFind->vClear();
  pOpAryFind->vAdd(RO_GREATER_EQUAL);
  
  aKeyVal = nullptr;
  for (i8 i_Key = 0; i_Key < 10; i_Key++) {

    aKeyVal = pKeyVec->pGet(i_Key);
    
    if (aKeyVal->bMatches(aKeyValFind,pColVec,pOpAryFind))
      break;
    
    aKeyVal = nullptr;
  }
  
  if (aKeyVal) {
    aCursor = aKeyVal->m_xKey.aWspArea();
    v = *(i8*)aKeyVal->aEachDatum(pCol,nBytes,&aCursor);
    
    vTrace("( %lld >= 3)\n"_Stz,v);
  }
  
  // scan the key vector for key value: >= 3
  xVariable = (i8)7;
  xKeyWrk.vReset();
  xKeyWrk.vAddDatum(xVariable, pCol);
  vTrace("Find Key value < 7\n"_Stz);
  
  pOpAryFind->vClear();
  pOpAryFind->vAdd(RO_LESS_THAN);
  
  aKeyVal = nullptr;
  for (i8 i_Key = 0; i_Key < 10; i_Key++) {
    
    aKeyVal = pKeyVec->pGet(i_Key);
    
    if (aKeyVal->bMatches(aKeyValFind,pColVec,pOpAryFind))
      break;
    
    aKeyVal = nullptr;
  }
  
  if (aKeyVal) {
    aCursor = aKeyVal->m_xKey.aWspArea();
    v = *(i8*)aKeyVal->aEachDatum(pCol,nBytes,&aCursor);
    
    vTrace("( %lld < 7 )\n"_Stz,v);
  }

  delete pKeyVec;
  delete pColVec;
  delete pOpAryFind;
  
  /*-----------------------------------------------------------------------*/
  vTrace("\n\n--------------- Single Element Key : VARCHAR ---------------\n"_Stz);
  /*-----------------------------------------------------------------------*/

  // create the string column
  pCol = new Column("Col0"_Stz);
  pCol->vColumnType(CT_VARCHAR);
  pCol->vDatumType(DM_VCH);
  
  // create the column vector
  pColVec = new IdxColVec;
  pColVec->vAdd(pCol);
  
  // create the operator array
  pOpAryFind = new RelOpAry;
  
  // create the key vector
  pKeyVec = new KeyVec;
  
  s256 sKey;
  
  StrVal* aStrVal;
  
  // generate random keyVal's and add to keyVec
  for (i8 i_Key = 0; i_Key < 10; i_Key++) {
    Random::vString(sKey,7);
    
    Stz* ptr = sKey.aStz();
    
    xVariable = ptr;
    aStrVal = _aStrVal(xVariable.aNestedValue());
    
    xKeyWrk.vReset();
    xKeyWrk.vAddDatum(xVariable, pCol);
    //xKeyWrk.vShow();
    
    aKeyVal = new KeyVal(xKeyWrk);
    pKeyVec->vAdd(aKeyVal);
  }
  
  // dump the key vector
  for (i8 i_Key = 0; i_Key < 10; i_Key++) {
    aKeyVal = pKeyVec->pGet(i_Key);
    aKeyVal->vShow(pColVec);
  }

  // generate a random key value to find
  Random::vString(sKey,7);
  Stz* ptr = sKey.aStz();
  vTrace("Find Key value < %s\n"_Stz, ptr);
  
  xVariable = ptr;
  aStrVal = _aStrVal(xVariable.aNestedValue());
  
  xKeyWrk.vReset();
  xKeyWrk.vAddDatum(xVariable, pCol);
  aKeyValFind = new KeyVal(xKeyWrk);
  
  // scan the key vector for key value: >= aKeyValFind
  pOpAryFind->vClear();
  pOpAryFind->vAdd(RO_LESS_THAN);
  
  aKeyVal = nullptr;
  for (i8 i_Key = 0; i_Key < 10; i_Key++) {
    
    aKeyVal = pKeyVec->pGet(i_Key);
    
    if (aKeyVal->bMatches(aKeyValFind,pColVec,pOpAryFind))
      break;
    
    aKeyVal = nullptr;
  }
  
  if (aKeyVal) {
    aCursor = aKeyVal->m_xKey.aWspArea();
    char* p = aKeyVal->aEachDatum(pCol,nBytes,&aCursor);
    
    vTrace("( %s < %s)\n"_Stz,p,aKeyValFind->m_xKey.aWspArea());
  }
  else {
    vTrace("Found nothing < %s\n"_Stz,aKeyValFind->m_xKey.aWspArea());
  }
  
  // generate a random key value to find
  Random::vString(sKey,7);
  ptr = sKey.aStz();
  vTrace("Find Key value >= %s\n"_Stz, ptr);
  
  xVariable = ptr;
  aStrVal = _aStrVal(xVariable.aNestedValue());
  
  xKeyWrk.vReset();
  xKeyWrk.vAddDatum(xVariable, pCol);
  aKeyValFind = new KeyVal(xKeyWrk);
  
  // scan the key vector for key value: < aKeyValFind
  pOpAryFind->vClear();
  pOpAryFind->vAdd(RO_GREATER_EQUAL);
  
  aKeyVal = nullptr;
  for (i8 i_Key = 0; i_Key < 10; i_Key++) {
    
    aKeyVal = pKeyVec->pGet(i_Key);
    
    if (aKeyVal->bMatches(aKeyValFind,pColVec,pOpAryFind))
      break;
    
    aKeyVal = nullptr;
  }
  
  if (aKeyVal) {
    aCursor = aKeyVal->m_xKey.aWspArea();
    char* p = aKeyVal->aEachDatum(pCol,nBytes,&aCursor);
    
    vTrace("( %s >= %s)\n"_Stz,p,aKeyValFind->m_xKey.aWspArea());
  }
  else {
    vTrace("Found nothing >= %s)\n"_Stz,aKeyValFind->m_xKey.aWspArea());
  }

  delete pKeyVec;
  delete pColVec;
  delete pOpAryFind;
  
  /*-----------------------------------------------------------------------*/
  vTrace("\n\n----------- Two Element Key : INTEGER , INTEGER -------------\n"_Stz);
  /*-----------------------------------------------------------------------*/
  
  // create the column
  Column* pCol0 = new Column("Col0"_Stz);
  pCol0->vColumnType(CT_INTEGER);
  
  Column* pCol1 = new Column("Col1"_Stz);
  pCol1->vColumnType(CT_INTEGER);
  
  // create the column vector
  pColVec = new IdxColVec;
  pColVec->vAdd(pCol0);
  pColVec->vAdd(pCol1);

  pOpAryFind = new RelOpAry;
  
  pKeyVec = new KeyVec;
  
  // create a vector of key values
  // (ascending order col0: 0,1,2, ... 9)
  //                  col1: 10, 20, 30
  for (i8 i_Key = 0; i_Key < 10; i_Key++) {
    
    for (i8 j = 10; j <= 30; j += 10) {
      xVariable = i_Key;
      xKeyWrk.vReset();
      xKeyWrk.vAddDatum(xVariable, pCol0);
      //xKeyWrk.vShow();
      
      xVariable = j;
      xKeyWrk.vAddDatum(xVariable, pCol1);
      //xKeyWrk.vShow();
      aKeyVal = new KeyVal(xKeyWrk);
      pKeyVec->vAdd(aKeyVal);
    }    
    
  }
  
  // dump the key vector
  for (i8 i_Key = 0; i_Key < 10; i_Key++) {
    
    aKeyVal = pKeyVec->pGet(i_Key);
    aKeyVal->vShow(pColVec);
    
    //vTrace("%lld: %lld %lld\n"_Stz,i_Key,v, v2);
  }
  
  // scan the key vector for key value: >= 2,10
  xVariable = (i8)2;
  xKeyWrk.vReset();
  xKeyWrk.vAddDatum(xVariable, pCol0);
  xVariable = (i8)10;
  xKeyWrk.vAddDatum(xVariable, pCol1);
  aKeyValFind = new KeyVal(xKeyWrk);
  
  vTrace("Find Key value >= 2,10\n"_Stz);
  
  pOpAryFind->vClear();
  pOpAryFind->vAdd(RO_GREATER_EQUAL);
  pOpAryFind->vAdd(RO_GREATER_EQUAL);
  
  aKeyVal = nullptr;
  for (i8 i_Key = 0; i_Key < 10; i_Key++) {
    
    aKeyVal = pKeyVec->pGet(i_Key);
    
    if (aKeyVal->bMatches(aKeyValFind,pColVec,pOpAryFind))
    break;
    
    aKeyVal = nullptr;
  }
  
  if (aKeyVal) {
    //aCursor = aKeyVal->m_xKey.aWspArea();
    //v = *(i8*)aKeyVal->aElement(pCol,nBytes,&aCursor);
    aKeyVal->vShow(pColVec);
    vTrace("( >= 2,10)\n"_Stz);
  }
  
  // scan the key vector for key value: >= 2,10
  xVariable = (i8)2;
  xKeyWrk.vReset();
  xKeyWrk.vAddDatum(xVariable, pCol0);
  xVariable = (i8)30;
  xKeyWrk.vAddDatum(xVariable, pCol1);
  aKeyValFind = new KeyVal(xKeyWrk);
  
  vTrace("Find Key value elem1 >= 2, elem2 < 30\n"_Stz);
  
  pOpAryFind->vClear();
  pOpAryFind->vAdd(RO_GREATER_EQUAL);
  pOpAryFind->vAdd(RO_LESS_THAN);
  
  aKeyVal = nullptr;
  for (i8 i_Key = 0; i_Key < 10; i_Key++) {
    
    aKeyVal = pKeyVec->pGet(i_Key);
    
    if (aKeyVal->bMatches(aKeyValFind,pColVec,pOpAryFind))
    break;
    
    aKeyVal = nullptr;
  }
  
  if (aKeyVal) {
    //aCursor = aKeyVal->m_xKey.aWspArea();
    //v = *(i8*)aKeyVal->aElement(pCol,nBytes,&aCursor);
    aKeyVal->vShow(pColVec);
    vTrace("( elem1 >= 2, elem2 < 30)\n"_Stz);
  }

  delete pKeyVec;
  delete pColVec;
  delete pOpAryFind;

  /*-----------------------------------------------------------------------*/
  vTrace("\n\n----------- Two Element Key : INTEGER , VARCHAR -------------\n"_Stz);
  /*-----------------------------------------------------------------------*/
  
  // create the column
  pCol0 = new Column("Col0"_Stz);
  pCol0->vColumnType(CT_INTEGER);
  
  pCol1 = new Column("Col1"_Stz);
  pCol1->vColumnType(CT_VARCHAR);
  pCol1->vDatumType(DM_VCH);
  
  // create the column vector
  pColVec = new IdxColVec;
  pColVec->vAdd(pCol0);
  pColVec->vAdd(pCol1);
  
  pOpAryFind = new RelOpAry;
  
  pKeyVec = new KeyVec;
  
  // create a vector of key values
  // (ascending order col0: 0,1,2, ... 9)
  //                  col1: 10, 20, 30
  for (i8 i_Key = 0; i_Key < 10; i_Key++) {
    
    for (i8 j = 10; j <= 30; j += 10) {
      xVariable = i_Key;
      xKeyWrk.vReset();
      xKeyWrk.vAddDatum(xVariable, pCol0);
      //xKeyWrk.vShow();
      
      Random::vString(sKey,7);
      Stz* ptr = sKey.aStz();
      xVariable = ptr;
      aStrVal = _aStrVal(xVariable.aNestedValue());
      xKeyWrk.vAddDatum(xVariable, pCol1);
      //xKeyWrk.vShow();
      
      aKeyVal = new KeyVal(xKeyWrk);
      
      pKeyVec->vAdd(aKeyVal);
    }
    
  }
  
  // dump the key vector
  for (i8 i_Key = 0; i_Key < 10; i_Key++) {
    aKeyVal = pKeyVec->pGet(i_Key);
    aKeyVal->vShow(pColVec);
  }

  // scan the key vector for key value: >= 2,10
  xVariable = (i8)2;
  xKeyWrk.vReset();
  xKeyWrk.vAddDatum(xVariable, pCol0);
  Random::vString(sKey,7);
  ptr = sKey.aStz();
  xVariable = ptr;
  aStrVal = _aStrVal(xVariable.aNestedValue());
  xKeyWrk.vAddDatum(xVariable, pCol1);
  
  aKeyValFind = new KeyVal(xKeyWrk);
  
  vTrace("Find Key value >= 2,%s\n"_Stz, aStrVal->aStz());
  
  pOpAryFind->vClear();
  pOpAryFind->vAdd(RO_GREATER_EQUAL);
  pOpAryFind->vAdd(RO_GREATER_EQUAL);
  
  aKeyVal = nullptr;
  for (i8 i_Key = 0; i_Key < 10; i_Key++) {
    
    aKeyVal = pKeyVec->pGet(i_Key);
    
    if (aKeyVal->bMatches(aKeyValFind,pColVec,pOpAryFind))
    break;
    
    aKeyVal = nullptr;
  }
  
  if (aKeyVal) {
    //aCursor = aKeyVal->m_xKey.aWspArea();
    //v = *(i8*)aKeyVal->aElement(pCol,nBytes,&aCursor);
    aKeyVal->vShow(pColVec);
    vTrace("( >= 2,%s)\n"_Stz, aStrVal->aStz());
  }
  
  
  vTrace("Find Key value <= 2,%s\n"_Stz, aStrVal->aStz());
  
  pOpAryFind->vClear();
  pOpAryFind->vAdd(RO_LESS_EQUAL);
  pOpAryFind->vAdd(RO_LESS_EQUAL);
  
  aKeyVal = nullptr;
  for (i8 i_Key = 0; i_Key < 10; i_Key++) {
    
    aKeyVal = pKeyVec->pGet(i_Key);
    
    if (aKeyVal->bMatches(aKeyValFind,pColVec,pOpAryFind))
    break;
    
    aKeyVal = nullptr;
  }
  
  if (aKeyVal) {
    //aCursor = aKeyVal->m_xKey.aWspArea();
    //v = *(i8*)aKeyVal->aElement(pCol,nBytes,&aCursor);
    aKeyVal->vShow(pColVec);
    vTrace("( <= 2,%s)\n"_Stz, aStrVal->aStz());
  }

  vTrace("\n\nEnd KeyTest**********************************************\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* key testing                                                                */
/*----------------------------------------------------------------------------*/
void KeyTest::vTestA(void)
{
  vTrace("\n\nKeyTest A:+++++++++++++++++++++++++++++++++++++++++++++++\n"_Stz);
  
  bool b;
  
  // create the column vector
  IdxColVec* pColVec = new IdxColVec;
  
  Column* pCol = new Column("Col0"_Stz);
  pCol->vColumnType(CT_INTEGER);
  pColVec->vAdd(pCol);
  pCol = new Column("Col1"_Stz);
  pCol->vColumnType(CT_INTEGER);
  pColVec->vAdd(pCol);
  
  // create the op array
  RelOpAry* pOpAry = new RelOpAry;
  pOpAry->vAdd(RO_LESS_EQUAL);
  pOpAry->vAdd(RO_LESS_THAN);
  
  KeyWrk xKeyWrk;
  GenVar xVariable;
  
  xKeyWrk.vReset();
  xVariable = (i8)1;
  xKeyWrk.vAddDatum(xVariable, pColVec->pGet(0));
  xVariable = (i8)10;
  xKeyWrk.vAddDatum(xVariable, pColVec->pGet(1));
  KeyVal* aKeyValThis = new KeyVal(xKeyWrk);
  
  xKeyWrk.vReset();
  xVariable = (i8)1;
  xKeyWrk.vAddDatum(xVariable, pColVec->pGet(0));
  xVariable = (i8)20;
  xKeyWrk.vAddDatum(xVariable, pColVec->pGet(1));
  KeyVal* aKeyValThat = new KeyVal(xKeyWrk);
  
  // [ ColA <= 1 , ColB < 10 ] should be before (2,20)
  b = BndLib::bIsBefore(aKeyValThis->xKey(),
                        aKeyValThat->xKey(),
                        *pOpAry,
                        *pColVec);
  assert(b);
  
  // [ ColA <= 2 , ColB < 20 ] should not be before (1,10)
  b = BndLib::bIsBefore(aKeyValThat->xKey(),
                        aKeyValThis->xKey(),
                        *pOpAry,
                        *pColVec);
  assert(!b);
  
  delete aKeyValThis;
  delete aKeyValThat;
  
  vTrace("\n\nEnd KeyTest A++++++++++++++++++++++++++++++++++++++++++++\n"_Stz);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
