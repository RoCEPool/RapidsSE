


/*
 * This class to use when loading a table, can pass the key value to it
 * by calling the member function vInput, then when there are no more rows, call the member function vFinalize()
 * SamplingData SampData;
 * SampData.vInput(ulVal);
 * SampData.vInput(ulVal);
 * ...
 * SampData.vFinalize();
 *
 *
 *
 */

#include <time.h>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <functional>

#include <map>


using namespace std;
typedef unsigned long long u64;


struct tLData {
  u64 llMinVal;
  u64 llMaxVal;
};

struct tFData {
  double dMinVal;
  double dMaxVal;
};

union uMinMax {
  tLData lData;
  tFData dData;
};


#define MAX_ARRAY 10000
#define MAX_UNIQUE_VALUES 1000

class SamplingData {
private:
  bool m_bFinalized;
  long m_lUniqueCnt;
  long m_lNonUniqueCnt;
  long m_lTotalDiscreteCnt;
  long m_lTotalCnt;
  bool m_bStats;

  tLData m_lMinMax;
  tFData m_dMinMax;

  vector<u64> m_vecLVal;
  map<u64, u64> m_mapLDup;

  vector<double> m_vecDVal;
  map<double, u64> m_mapDDup;

  /*
   *
   */
  long lDiscreteVal(u64 lVal)
  {
    if (!m_bFinalized) {
      vFinalize();
    }


    long lCount = 0; //std::count (m_vecLVal.begin(), m_vecLVal.end(), lVal);
    map<u64,u64>::iterator i = m_mapLDup.find(lVal);
    if (i != m_mapLDup.end())
      lCount = i->second;
    return lCount;
  }

  /*
   *
   */
  long lDiscreteValD(double lVal)
  {
    if (!m_bFinalized) {
      vFinalizeD();
    }


    long lCount = 0; //std::count (m_vecLVal.begin(), m_vecLVal.end(), lVal);
    map<double,u64>::iterator i = m_mapDDup.find(lVal);
    if (i != m_mapDDup.end())
      lCount = i->second;
    return lCount;
  }

public:

  SamplingData() {

    m_bStats = true;
    m_lMinMax.llMaxVal = std::numeric_limits<long long>::min();
    m_lMinMax.llMinVal = std::numeric_limits<long long>::max();
    m_dMinMax.dMaxVal = std::numeric_limits<double>::min();
    m_dMinMax.dMinVal = std::numeric_limits<double>::max();

    m_bFinalized = false;
    m_lUniqueCnt = 0;
    m_lNonUniqueCnt = 0;
    m_lTotalDiscreteCnt = 0;
    m_lTotalCnt = 0;
  }

  long lGetUniqueCnt(void) { return m_lUniqueCnt; }
  long lGetNonUniqueCnt(void) { return m_lNonUniqueCnt; }
  long lGetTotalDiscreteCnt(void) { return m_lTotalDiscreteCnt; }
  long lGetTotalCnt(void) { return m_lTotalCnt; }



  /*
   * Print stats values
   */
  void vPrint(void) {
    //printf("Dup Size=%lu\n",m_mapLDup.size());

    printf("Min Value=%llu Max Value=%llu\n", m_lMinMax.llMinVal, m_lMinMax.llMaxVal);
    printf("Total values dups=%ld\n", m_lNonUniqueCnt);
    printf("Total values uniques=%ld\n", m_lUniqueCnt);
    printf("Total discretes=%ld\n",m_lTotalDiscreteCnt);
    printf("Total Rows=%ld\n", m_lTotalCnt);
  }

  /*
   * Print stats values for decimal value type
   */
  void vPrintD(void) {
    //printf("Dup Size=%lu\n",m_mapLDup.size());

    printf("Min Value=%.9lf Max Value=%.9lf\n", m_dMinMax.dMinVal, m_dMinMax.dMaxVal);
    printf("Total values dups=%ld\n", m_lNonUniqueCnt);
    printf("Total values uniques=%ld\n", m_lUniqueCnt);
    printf("Total discretes=%ld\n",m_lTotalDiscreteCnt);
    printf("Total Rows=%ld\n", m_lTotalCnt);
  }


/*
 * Insert Integer values
 */
  void vInput(u64 lVal, int iModVal=0) {

    hash<u64> lHash;
    u64 ulHVal=0;

    if (iModVal > 0)
      ulHVal = lHash(lVal) % iModVal;
    else
      ulHVal = lHash(lVal);

    //printf("lVal=%lld, hash=%llu ", lVal, ulHVal);

    m_vecLVal.push_back(ulHVal);

    if(m_lMinMax.llMaxVal < lVal)
      m_lMinMax.llMaxVal = lVal;

    if(lVal < m_lMinMax.llMinVal)
      m_lMinMax.llMinVal = lVal;
  }


  /*
   * Insert string values
   */
  void vInput(string strVal, int iModVal=0) {

    hash<string> strHash;
    u64 ulHVal=0;

    if (iModVal > 0)
      ulHVal = strHash(strVal) % iModVal;
    else
      ulHVal = strHash(strVal);

    //printf("lVal=%llu, ", ulHVal);

    m_vecLVal.push_back(ulHVal);

    if(m_lMinMax.llMaxVal < ulHVal)
      m_lMinMax.llMaxVal = ulHVal;

    if(ulHVal < m_lMinMax.llMinVal)
      m_lMinMax.llMinVal = ulHVal;
  }



  /*
   * Insert decimal (double) values
   */
  void vInput(double dVal, int iModVal=0) {
    /*hash<double> strHash;
    u64 ulHVal=0;

    if (iModVal > 0)
      ulHVal = strHash(dVal) % iModVal;
    else
      ulHVal = strHash(dVal);

    m_vecLVal.push_back(ulHVal);
     */
    if(m_dMinMax.dMaxVal < dVal)
      m_dMinMax.dMaxVal = dVal;

    if(dVal < m_dMinMax.dMinVal)
      m_dMinMax.dMinVal = dVal;

    m_vecDVal.push_back(dVal);

  }

  /*
   * Finalize decimal vaues type
   */
  void vFinalizeD(void)
  {
    if (!m_bFinalized) {
      m_lTotalCnt = m_vecDVal.size();

      sort(m_vecDVal.begin(), m_vecDVal.end());
      //m_dMinMax.dMinVal = m_vecDVal.front();
      //m_dMinMax.dMaxVal = m_vecDVal.back();

      printf("min=%.8lf , max=%.8lf\n", m_dMinMax.dMinVal, m_dMinMax.dMaxVal);

      for(auto it = std::cbegin(m_vecDVal); it != std::cend(m_vecDVal); ) {
        long dups = std::count(it, std::cend(m_vecDVal), *it);
        if ( dups > 1 ) {
          m_lNonUniqueCnt++;
          //printf("value=%.8lf is a duplicate, times=%ld\n", *it, dups);
          m_mapDDup.insert(make_pair(*it, dups));
        } else {
          //printf("value=%.8lf is not a duplicate, times=%ld\n", *it, dups);
          m_lUniqueCnt++;
          // NOTE: Here we can decide whether we would abandon the stats for this table
          // Whichever logic we need to do, for example if this table contains too many or all unique key values (like ORDERS)
          // then should we even care...
          if (m_lUniqueCnt > MAX_UNIQUE_VALUES) {
            m_mapDDup.clear();
            m_bStats = false;
            break;
          }
        }

        for(auto last = *it;*++it == last;);
      }

///////////////

      m_bFinalized = true;

      m_lTotalDiscreteCnt = m_lNonUniqueCnt + m_lUniqueCnt;

      m_vecDVal.clear();
    }

  }


  /*
   * Finalize Integer or string values type
   */
  void vFinalize(void)
  {
    if (!m_bFinalized) {
      m_lTotalCnt = m_vecLVal.size();

      sort(m_vecLVal.begin(), m_vecLVal.end());

      m_lMinMax.llMinVal = m_vecLVal.front();
      m_lMinMax.llMaxVal = m_vecLVal.back();


      for(auto it = std::cbegin(m_vecLVal); it != std::cend(m_vecLVal); ) {
        long dups = std::count(it, std::cend(m_vecLVal), *it);
        if ( dups > 1 ) {
          m_lNonUniqueCnt++;
          //printf("value=%llu is a duplicate, times=%ld\n", *it, dups);
          m_mapLDup.insert(make_pair(*it, dups));
        } else {
          m_lUniqueCnt++;
          // NOTE: Here we can decide whether we would abandon the stats for this table
          // Whichever logic we need to do, for example if this table contains too many or all unique key values (like ORDERS)
          // then should we even care...
          if (m_lUniqueCnt > MAX_UNIQUE_VALUES) {
            m_mapLDup.clear();
            m_bStats = false;
            break;
          }
        }

        for(auto last = *it;*++it == last;);
      }

///////////////

      m_bFinalized = true;

      m_lTotalDiscreteCnt = m_lNonUniqueCnt + m_lUniqueCnt;

      m_vecLVal.clear();
    }

  }


/*
 * Pass in a string/date value to get number of duplicates (zero or non zero value returns)
 */
  long lDiscrete(string strVal) {
    hash<string> strHash;
    u64 lVal = (u64)strHash(strVal);
    return lDiscreteVal(lVal);
  }

  /*
   *  Pass in a integer value to get number of duplicates (zero or non zero value returns)
   */
  long lDiscrete( u64 lVal) {
    hash<u64> ulHash;
    u64 ulVal = ulHash(lVal);
    return lDiscreteVal(ulVal);
  }

  /*
   * Pass in a decimal (double) value to get number of duplicates (zero or non zero value returns)
   */
  long lDiscrete( double lVal) {
    // hash<double> udHash;
    // u64 ulVal = udHash(lVal);
    return lDiscreteValD(lVal);
  }




};


