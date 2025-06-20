//
// Created by wzy on 2021/8/7.
//

#include "../../include/Utils/TimeSeriesUtil.h"
#include "../../include/Utils/FileUtil.h"
#include "../../include/DSTree/DSTreeNode.h"
#include "../../include/Const.h"
#include <immintrin.h>
#include <cmath>
#include <cassert>
#include <memory>
#include <bitset>
#include "./compute_simd.h"

#define distPeng(x,y) ((x-y)*(x-y))

using namespace std;

void TimeSeriesUtil::heap_data_copy(vector<PqItemSeries *> &heap){
    for(auto *pis:heap)
        pis->copyData();
}

bool TimeSeriesUtil::isSame(float* ts1, float* ts2, int ts1_length, int ts2_length)
{
    if(ts1_length!=ts2_length)
        return false;
    for(int i = 0; i < ts1_length; ++i) {
        if(abs(ts1[i]-ts2[i])>1e-5)
            return false;
    }
    return true;
}

bool TimeSeriesUtil::isSame(const PqItemSeries& ts1, const PqItemSeries& ts2)
{
    return isSame(ts1.ts, ts2.ts, Const::tsLength, Const::tsLength);
}

//bool TimeSeriesUtil::isSame(const PqItemSeriesVector& ts1, const PqItemSeriesVector& ts2)
//{
//
//    assert(ts1.ts->size() == ts2.ts->size());
//    vector<float>&t1 = *(ts1.ts), &t2 = *(ts2.ts);
//    for(int i = 0; i < ts1.ts->size(); ++i) {
//        if(abs(t1[i] - t2[i])>1e-5)
//            return false;
//    }
//    return true;;
//}
//
//bool TimeSeriesUtil::isSame(const PqItemSeriesVector* ts1, const PqItemSeriesVector* ts2)
//{
//
//    assert(ts1->ts->size() == ts2->ts->size());
//    vector<float>&t1 = *(ts1->ts), &t2 = *(ts2->ts);
//    for(int i = 0; i < ts1->ts->size(); ++i) {
//        if(abs(t1[i] - t2[i])>1e-5)
//            return false;
//    }
//    return true;;
//}
//
//bool TimeSeriesUtil::isSame(const PqItemSeriesVector *ts1, float *ts2)
//{
//
//    vector<float>&t1 = *(ts1->ts);
//    for(int i = 0; i < ts1->ts->size(); ++i) {
//        if(abs(t1[i] - ts2[i])>1e-5)
//            return false;
//    }
//    return true;;
//}

bool TimeSeriesUtil::isSame(const PqItemSeries *ts1, const float *ts2)
{

    float *t1 = (ts1->ts);
    for(int i = 0; i < Const::tsLength; ++i) {
        if(abs(t1[i] - ts2[i])>1e-5)
            return false;
    }
    return true;;
}


int TimeSeriesUtil::intersectionTsSetsCardinality(const vector<PqItemSeries *> &tsSet1, const vector<PqItemSeries *> &tsSet2){
    int intersectionNum = 0;
    for(PqItemSeries* currentTs : tsSet1)
        for (PqItemSeries* targetTs : tsSet2)
            if (isSame(*currentTs, *targetTs)) {
                intersectionNum += 1;
                break;
            }
    return intersectionNum;
}

int TimeSeriesUtil::intersectionTsSetsCardinality(const vector<PqItemSeries *> *tsSet1, vector<float *> *tsSet2){
    int intersectionNum = 0, size= tsSet1->size();
    unordered_set<int>has_used;
    for(const PqItemSeries* currentTs : *tsSet1)
        for(int i=0;i<size;++i){
            if(has_used.count(i) > 0)   continue;
            if(isSame(currentTs, (*tsSet2)[i])) {
                intersectionNum++;
                has_used.insert(i);
                break;
            }
        }

    return intersectionNum;
}

double TimeSeriesUtil::GetAP(const vector<PqItemSeries *> *approx,
                             const vector<float *> *res) {
  auto approx_sz = approx->size();
  assert(approx_sz <= res->size());

  double ap = 0;
  size_t ap_num = 0;
  const auto begin = res->begin();
  const auto end = begin + approx_sz;
  size_t idx = 0;
  while (idx < approx_sz) {
    const auto &item = approx->at(idx);
    bool found = false;
    for (auto it = begin; it != end; it++) {
      if (!isSame(item, *it)) {
        continue;
      }
      ap += 1.0 * ++ap_num / ++idx;
      found = true;
      break;
    }
    if (!found) {
      idx++;
    }
  }
  ap /= approx_sz;

  return ap;
}

// int TimeSeriesUtil::intersectionTsSetsCardinality(const
// vector<PqItemSeriesVector *> *tsSet1, vector<float *> *tsSet2){
//     int intersectionNum = 0, size= tsSet1->size();
//     for(const PqItemSeriesVector* currentTs : *tsSet1)
//         for(int i=0;i<size;++i){
//             if(isSame(currentTs, (*tsSet2)[i])) {
//                 intersectionNum++;
//                 break;
//             }
//         }
//
//     return intersectionNum;
// }
//
// int TimeSeriesUtil::intersectionTsSetsCardinality(const
// vector<PqItemSeriesVector *> *tsSet1, const vector<PqItemSeriesVector *>
// *tsSet2){
//     int intersectionNum = 0;
//     for(const PqItemSeriesVector* currentTs : *tsSet1)
//         for (const PqItemSeriesVector* targetTs : *tsSet2)
//             if (isSame(currentTs, targetTs)) {
//                 intersectionNum += 1;
//                 break;
//             }
//     return intersectionNum;
// }
//
// int TimeSeriesUtil::intersectionTsSetsCardinality(const
// vector<PqItemSeriesVector *> &tsSet1, const vector<PqItemSeriesVector *>
// &tsSet2){
//     int intersectionNum = 0;
//     for(const PqItemSeriesVector* currentTs : tsSet1)
//         for (const PqItemSeriesVector* targetTs : tsSet2)
//             if (isSame(currentTs, targetTs)) {
//                 intersectionNum += 1;
//                 break;
//             }
//     return intersectionNum;
// }
//
// int TimeSeriesUtil::intersectionTsSetsCardinality(const
// vector<PqItemSeriesVector> &tsSet1, const vector<PqItemSeriesVector>
// &tsSet2){
//     int intersectionNum = 0;
//     for(const PqItemSeriesVector& currentTs : tsSet1)
//         for (const PqItemSeriesVector& targetTs : tsSet2)
//             if (isSame(currentTs, targetTs)) {
//                 intersectionNum += 1;
//                 break;
//             }
//     return intersectionNum;
// }

vector<PqItemSeries> &TimeSeriesUtil::intersectionTsSets(
    const vector<PqItemSeries> &tsSet1, const vector<PqItemSeries> &tsSet2) {
  auto *res = new vector<PqItemSeries>();
  for (PqItemSeries currentTs : tsSet1)
    for (PqItemSeries targetTs : tsSet2)
      if (isSame(currentTs, targetTs)) {
        res->push_back(currentTs);
        break;
      }
  return *res;
}

vector<PqItemSeries *> & TimeSeriesUtil::intersectionTsSets(const vector<PqItemSeries *> *tsSet1, const vector<float *> *tsSet2){
    auto *res = new vector<PqItemSeries*>();
    for(auto currentTs : *tsSet1)
        for (auto targetTs : *tsSet2)
            if (isSame(currentTs, targetTs)) {
                res->push_back(currentTs);
                break;
            }
    return *res;
}

//vector<PqItemSeriesVector>& TimeSeriesUtil::intersectionTsSets(const vector<PqItemSeriesVector>& tsSet1, const vector<PqItemSeriesVector>& tsSet2){
//    auto *res = new vector<PqItemSeriesVector>();
//    for(const PqItemSeriesVector& currentTs : tsSet1)
//        for (const PqItemSeriesVector& targetTs : tsSet2)
//            if (isSame(currentTs, targetTs)) {
//                res->push_back(currentTs);
//                break;
//            }
//    return *res;
//}
//
//bool TimeSeriesUtil::containsSeries(const vector<PqItemSeriesVector *> &tss, float*q){
//        for(const PqItemSeriesVector* ts:tss)
//            if(isSame(ts, q))    return true;
//            return false;
//}

bool TimeSeriesUtil::containsSeries(const vector<PqItemSeries *> &tss, float*q){
    for(const PqItemSeries* ts:tss)
        if(isSame(ts, q))    return true;
        return false;
}

double* TimeSeriesUtil::avgBySegments(float* timeSeries, const int* segments, int segment_number) {
    auto* ret = new double[segment_number];
    int start = 0, end;
    for (int i = 0; i < segment_number; i++) {
        end = segments[i];
        ret[i] = MathUtil::avg(timeSeries, start, end);
        start = end;
    }
    return ret;
}

double* TimeSeriesUtil::devBySegments(float* timeSeries, const int* segments, int segment_number) {
    auto* ret = new double[segment_number];
    int start = 0, end;
    for (int i = 0; i < segment_number; i++) {
        end = segments[i];
        ret[i] = MathUtil::deviation(timeSeries, start, end);
        start = end;
    }
    return ret;
}

double TimeSeriesUtil::euclideanDist(const float* ts_1, const float* ts_2, int len) {
    // 这里用 simd 加速
    const size_t length = len;
    return L2SqrSIMD16ExtAVX512Float(ts_1, ts_2, &length);
    // double sum = 0, dp;
    // for (int i = 0; i < len; i++) {
    //     dp = ts_1[i] - ts_2[i];
    //     sum += dp * dp;
    // }
    // return sum;
}
 
double TimeSeriesUtil::euclideanDist(const float* ts_1, const float* ts_2, int len, double bound) {
    double sum = 0, dp;
    for (int i = 0; i < len && sum < bound; i++) {
        dp = ts_1[i] - ts_2[i];
        sum += dp * dp;
    }
    return sum;
}

double TimeSeriesUtil::euclideanDist(const float *ts_1, const vector<float>& ts_2, int len, double bound) {
    double sum = 0, dp;
    for (int i = 0; i < len && sum < bound; i++) {
        dp = ts_1[i] - ts_2[i];
        sum += dp * dp;
    }
    return sum;
}

double TimeSeriesUtil::euclideanDist(const vector<float>* ts_1, const float* ts_2, int len) {
    double sum = 0;
    for (int i = 0; i < len; i++) {
        double dp = (*ts_1)[i] - ts_2[i];
        sum += dp * dp;
    }
    return sum;
}

double TimeSeriesUtil::euclideanDist(float *query_reordered, float *ts, int size, double bound, int *order)
{
    // static size_t cnt = 0;
    // if ((++cnt % 100000) == 0) {
    //     std::cout << cnt << std::endl;
    // }
    int i;
    float sum = 0;
    for ( i = 0 ; i < size && sum < bound ; i++ )
    {
        //float x = (T[(order[i]+j)]-mean)/std;
        float x = ts[order[i]];
        sum += (x - query_reordered[i]) * (x - query_reordered[i]);
    }
    return sum;
}

// size must be a multiple of 8
float TimeSeriesUtil::euclideanDist_SIMD(float * t, float * s, int size, float bound) {
    float distance = 0;
    int i =0;
    float distancef[8];

    __m256 v_t,v_s,v_d,distancev;
    while (size > 0 && distance <bound) {
        v_t=_mm256_loadu_ps (&t[i]);
        v_s=_mm256_loadu_ps (&s[i]);

        v_d= _mm256_sub_ps (v_t,v_s);

        v_d=_mm256_mul_ps (v_d,v_d);
        size-=8;

        i=i+8;
        distancev = _mm256_hadd_ps (v_d, v_d);
        distancev = _mm256_hadd_ps (distancev, distancev);
        _mm256_storeu_ps (distancef ,distancev);
        distance +=distancef[0]+distancef[4];

    }

//    distance = sqrtf(distance);

    return distance;
}

float TimeSeriesUtil::euclideanDist_SIMD(float *query_reordered, float *ts, int size, double bound, int *order) {
    float distance = 0;
    int i =0;
    float distancef[8];
    float t[size];
    for(int tmp = 0; tmp < size; ++tmp)
        t[tmp] = ts[order[tmp]];

    __m256 v_t,v_s,v_d,distancev;
    while (size > 0 && distance <bound) {
        v_t=_mm256_loadu_ps (&t[i]);
        v_s=_mm256_loadu_ps (&query_reordered[i]);

        v_d= _mm256_sub_ps (v_t,v_s);

        v_d=_mm256_mul_ps (v_d,v_d);
        size-=8;

        i=i+8;
        distancev = _mm256_hadd_ps (v_d, v_d);
        distancev = _mm256_hadd_ps (distancev, distancev);
        _mm256_storeu_ps (distancef ,distancev);
        distance +=distancef[0]+distancef[4];

    }

//    distance = sqrtf(distance);

    return distance;
}

double TimeSeriesUtil::dtw(const float* A, const float* B, int len, int r, double bsf)
{

    double *cost;
    double *cost_prev;
    double *cost_tmp;
    int i,j,k;
    double x,y,z,min_cost;

    /// Instead of using matrix of size O(m^2) or O(mr), we will reuse two array of size O(r).
    cost = (double*)malloc(sizeof(double)*(2*r+1));
    for(k=0; k<2*r+1; k++)    cost[k]=numeric_limits<double>::max();

    cost_prev = (double*)malloc(sizeof(double)*(2*r+1));
    for(k=0; k<2*r+1; k++)    cost_prev[k]=numeric_limits<double>::max();

    for (i=0; i<len; i++)
    {
        k = max(0,r-i);
        min_cost = numeric_limits<double>::max();

        for(j=max(0,i-r); j<=min(len-1,i+r); j++, k++)
        {
            /// Initialize all row and column
            if ((i==0)&&(j==0))
            {
                cost[k]=distPeng(A[0],B[0]);
                min_cost = cost[k];
                continue;
            }

            if ((j-1<0)||(k-1<0))     y = numeric_limits<double>::max();
            else                      y = cost[k-1];
            if ((i-1<0)||(k+1>2*r))   x = numeric_limits<double>::max();
            else                      x = cost_prev[k+1];
            if ((i-1<0)||(j-1<0))     z = numeric_limits<double>::max();
            else                      z = cost_prev[k];

            /// Classic DTW calculation
            cost[k] = min( min( x, y) , z) + distPeng(A[i],B[j]);

            /// Find minimum cost in row for early abandoning (possibly to use column instead of row).
            if (cost[k] < min_cost)
            {   min_cost = cost[k];
            }
        }

        /// We can abandon early if the current cummulative distace with lower bound together are larger than bsf
        if (i+r < len-1 && min_cost  >= bsf)
        {   free(cost);
            free(cost_prev);
            return min_cost ;
        }
        /// Move current array to previous array.
        cost_tmp = cost;
        cost = cost_prev;
        cost_prev = cost_tmp;
    }
    k--;

    /// the DTW distance is in the last cell in the matrix of size O(m^2) or at the middle of our array.
    double final_dtw = cost_prev[k];
    free(cost);
    free(cost_prev);
    return final_dtw;
}

double TimeSeriesUtil::dtw(const float* A, const vector<float>& B, int len, int r, double bsf)
{

    double *cost;
    double *cost_prev;
    double *cost_tmp;
    int i,j,k;
    double x,y,z,min_cost;

    /// Instead of using matrix of size O(m^2) or O(mr), we will reuse two array of size O(r).
    cost = (double*)malloc(sizeof(double)*(2*r+1));
    for(k=0; k<2*r+1; k++)    cost[k]=numeric_limits<double>::max();

    cost_prev = (double*)malloc(sizeof(double)*(2*r+1));
    for(k=0; k<2*r+1; k++)    cost_prev[k]=numeric_limits<double>::max();

    for (i=0; i<len; i++)
    {
        k = max(0,r-i);
        min_cost = numeric_limits<double>::max();

        for(j=max(0,i-r); j<=min(len-1,i+r); j++, k++)
        {
            /// Initialize all row and column
            if ((i==0)&&(j==0))
            {
                cost[k]=distPeng(A[0],B[0]);
                min_cost = cost[k];
                continue;
            }

            if ((j-1<0)||(k-1<0))     y = numeric_limits<double>::max();
            else                      y = cost[k-1];
            if ((i-1<0)||(k+1>2*r))   x = numeric_limits<double>::max();
            else                      x = cost_prev[k+1];
            if ((i-1<0)||(j-1<0))     z = numeric_limits<double>::max();
            else                      z = cost_prev[k];

            /// Classic DTW calculation
            cost[k] = min( min( x, y) , z) + distPeng(A[i],B[j]);

            /// Find minimum cost in row for early abandoning (possibly to use column instead of row).
            if (cost[k] < min_cost)
            {   min_cost = cost[k];
            }
        }

        /// We can abandon early if the current cummulative distace with lower bound together are larger than bsf
        if (i+r < len-1 && min_cost  >= bsf)
        {   free(cost);
            free(cost_prev);
            return min_cost ;
        }
        /// Move current array to previous array.
        cost_tmp = cost;
        cost = cost_prev;
        cost_prev = cost_tmp;
    }
    k--;

    /// the DTW distance is in the last cell in the matrix of size O(m^2) or at the middle of our array.
    double final_dtw = cost_prev[k];
    free(cost);
    free(cost_prev);
    return final_dtw;
}

#define dist(x,y) (((x)-(y))*((x)-(y)))
float TimeSeriesUtil::lb_keogh_data_bound(float* qo, float* upperLemire, float* lowerLemire, float* cb, int len, float bsf)
{
    float lb = 0;
    float uu=0,ll=0,d=0;
    int i=0;

    int len1 = (len/8)*8;
    __m256 tu256, tl256, cb256, Q, calc1, calc2;
    __m128 temp1, temp2;
    float *cbtmp = static_cast<float *>(malloc(sizeof(float) * 8));

    for(i=0; i<len1&&lb<bsf; i+=8)
    {
        Q = _mm256_loadu_ps(&qo[i]);
        tu256 = _mm256_loadu_ps(&upperLemire[i]);
        tl256 = _mm256_loadu_ps(&lowerLemire[i]);
        //tu256 = _mm_setr_ps(tu[order[i]],tu[order[i+1]],tu[order[i+2]],tu[order[i+3]]);
        //tl256 = _mm_setr_ps(tl[order[i]],tl[order[i+1]],tl[order[i+2]],tl[order[i+3]]);
        calc1 = _mm256_min_ps(Q,tu256);
        calc1 = _mm256_sub_ps(Q,calc1);

        calc2 = _mm256_max_ps(Q,tl256);
        calc2 = _mm256_sub_ps(calc2,Q);
        calc1 = _mm256_add_ps(calc1,calc2);

        calc1 = _mm256_mul_ps(calc1,calc1);

        _mm256_storeu_ps(cbtmp,calc1);

        calc1 = _mm256_hadd_ps(calc1,calc1);
        calc1 = _mm256_hadd_ps(calc1,calc1);
        temp1 = _mm256_extractf128_ps(calc1,1);
        temp2 = _mm_add_ss(_mm256_castps256_ps128(calc1),temp1);
        lb += _mm_cvtss_f32(temp2);

        cb[i]=cbtmp[0];cb[i+1] = cbtmp[1];cb[i+2]=cbtmp[2];cb[i+3]=cbtmp[3];
        cb[i+4]=cbtmp[4];cb[i+5] = cbtmp[5];cb[i+6]=cbtmp[6];cb[i+7]=cbtmp[7];
    }

    for(;i<len&&lb<bsf;i++)
    {
        uu = upperLemire[i];
        ll = lowerLemire[i];
        d= 0;
        if(qo[i] > uu)
        {
            d = dist(qo[i],uu);
        }
        else if(qo[i] < ll)
        {
            d = dist(qo[i],ll);
        }
        lb += d;
        cb[i] = d;
    }

    free( cbtmp);
    return lb;
}

float TimeSeriesUtil::dtwsimd(const float* A, const float* B, float* cb, int len, int r, float bsf, float* tSum, float* pCost, float* rDist)
{
    int length = 2*r + 1;
    // SIMD register
    //__m256 a256, b256;

    int start, end;
    float minCost = 0.0f;
    // the first line
    for(int k=0; k<=r; k++)
    {
        rDist[k] = dist(A[0], B[k]);
    }

    tSum[0] = rDist[0];
    for(int ij=1; ij<=r; ij++)
        tSum[ij] = tSum[ij-1] + rDist[ij];


    pCost[0] = tSum[0];
    for(int ij= 1; ij<=r; ij++)
    {
        pCost[ij] = min(tSum[ij-1],tSum[ij]);
    }
    pCost[r+1] = tSum[r];

    for(int i=1; i < len - 1; i++)
    {
        start = max(0,i-r);
        end = min(len - 1, i + r);

        for(int k=start; k<=end; k++)
        {
            rDist[k-start] = dist(A[i], B[k]);
        }

        for(int k=start; k<=end; k++)
        {
            tSum[k-start] =  pCost[k-start] + rDist[k-start];
        }

        minCost = tSum[0];
        for(int k=start+1; k<=end; k++)
        {
            if(tSum[k-1-start]<pCost[k-start])
            {
                tSum[k-start] = tSum[k-1-start] + rDist[k-start];
            }
            minCost = min(minCost, tSum[k - start]);
        }
        if(i+r < len - 1 && minCost + cb[i + r + 1] >= bsf)
        {
            return minCost + cb[i+r+1];
        }

        if((end-start+1) < length && start == 0)
        {
            pCost[start-start] = tSum[start-start];
            for(int ij= start+1; ij<=end; ij++)
            {
                pCost[ij-start] = min(tSum[ij-1-start],tSum[ij-start]);
            }
            pCost[end+1-start] = tSum[end-start];
        }
        else
        {
            for(int ij= start+1; ij<=end; ij++)
            {
                pCost[ij-1-start] = min(tSum[ij-1-start],tSum[ij-start]);
            }
            pCost[end-start] = tSum[end-start];
        }
    }

    // the last line
    start = len - 1 - r;
    end = len - 1;

    for(int k=start; k<=end; k++)
    {
        rDist[k-start] = (A[len - 1] - B[k]) * (A[len - 1] - B[k]);
    }

    for(int k=start; k<=end; k++)
    {
        tSum[k-start] =  pCost[k-start] + rDist[k-start];
    }
    for(int k=start+1; k<=end; k++)
    {
        if(tSum[k-1-start]<pCost[k-start])
        {
            tSum[k-start] = tSum[k-1-start] + rDist[k-start];
        }
    }
    float ret = tSum[r];
    return ret;
}

// bits cardinality are the same by default
double TimeSeriesUtil::distanceEstimation(const IPGNode* n1, const IPGNode* n2){
    double sum = 0;
    for(int i=0;i<Const::segmentNum;++i){
//        if(n1->sax[id] != n2->sax[id])
            sum += ((n1->paa_mu[i] - n2->paa_mu[i]) * (n1->paa_mu[i] - n2->paa_mu[i]));
    }
    return sum;
}

double TimeSeriesUtil::minDistEstimation(const DSTreeNode *node, InsertedSeries *queryTs) {
    double sum = 0;
    auto points = node->splitPoints;

    for (int i = 0; i < node->splitPointsLen; i++) {
        //use mean and standardDeviation to estimate the distance
        int start = DSTreeNode::getSegmentStart(points, i), end = DSTreeNode::getSegmentEnd(points, i);
        double tempDist = 0, stdDev = queryTs->getStdEv(start, end), mean = queryTs->getMean(start, end);
        //stdDev out the range of min std and max std
        if ((stdDev - (*node->nodeSegmentSketches)[i].indicators[2]) * (stdDev - (*node->nodeSegmentSketches)[i].indicators[3]) > 0) {
            tempDist += pow(min(abs(stdDev - (*node->nodeSegmentSketches)[i].indicators[2]), abs(stdDev - (*node->nodeSegmentSketches)[i].indicators[3])), 2);
        }

        //avg out the range of min mean and max mean
        if ((mean - (*node->nodeSegmentSketches)[i].indicators[0]) * (mean - (*node->nodeSegmentSketches)[i].indicators[1]) > 0) {
            tempDist += pow(min(abs(mean - (*node->nodeSegmentSketches)[i].indicators[0]), abs(mean - (*node->nodeSegmentSketches)[i].indicators[1])), 2);
        }
        sum += tempDist * node->getSegmentLength(i);
    }
    return sum;
}

double TimeSeriesUtil::minDistEstimation(InsertedSeries &q, const vector<int> &points, int pointsLen, const vector<double>&mean, const vector<double>&stdev){
    double res = 0;
    for(int i=0;i<pointsLen;++i){
        int len = DSTreeNode::getSegmentLength(points, i), start = DSTreeNode::getSegmentStart(points, i), end = DSTreeNode::getSegmentEnd(points, i);
        double mean_q = q.getMean(start, end), dev_q = q.getStdEv(start, end);
        res += len * ((mean[i] - mean_q) * (mean[i] - mean_q) + (stdev[i] - dev_q) * (stdev[i] - dev_q));
    }
    return res;
}

void TimeSeriesUtil::prepareTsWithBounds(InsertedSeries& queryTs, double bsfMin, const vector<int> &points, int pointsLen,
                                         const vector<vector<double> >&means, const vector<vector<double> >&stdevs,
                                         vector<PqItemIndex *> &res){
    // index 0 for max index
    res.clear();
    int length = means.size(), maxIndex = 0;
    for(int i=0; i<length; ++i){
        double minDist = minDistEstimation(queryTs, points, pointsLen, means[i], stdevs[i]);
        if(minDist >= bsfMin)   continue;
        res.push_back(new PqItemIndex(i, minDist));
        maxIndex = max(maxIndex, i);
    }
    sort(res.begin(),  res.end(), PqItemIndexComparator());
    res.insert(res.begin(), new PqItemIndex(maxIndex, 0));
}


void TimeSeriesUtil::prepareTsWithBoundsAndThreshold(InsertedSeries &queryTs, double bsfMin, const vector<int> &points, int pointsLen, int threshold,
                                                     const vector<vector<double>> &means, const vector<vector<double>> &stdevs,
                                                     vector<int>&res){
    vector<PqItemIndex*> pq;
    int length = means.size();
    for(int i=0; i<length; ++i){
        double minDist = minDistEstimation(queryTs, points, pointsLen, means[i], stdevs[i]);
        if(minDist >= bsfMin)   continue;
        if(pq.size() >= threshold) {
            double heapTop = pq[0]->dist;
            if(heapTop <= minDist)  continue;
            pop_heap(pq.begin(), pq.end(), PqItemIndexMaxHeap());
            delete pq.back();
            pq.pop_back();
        }
        pq.push_back((new PqItemIndex(i, minDist)));
        push_heap(pq.begin(), pq.end(), PqItemIndexMaxHeap());
    }
    int hs = pq.size(), maxIndex = 0;
    res.resize(hs+1);
    for(int i=0;i<hs;++i) {
        res[hs - i] = pq[0]->index;
        pop_heap(pq.begin(), pq.end(), PqItemIndexMaxHeap());
        delete pq.back();
        pq.pop_back();
        maxIndex = max(maxIndex, res[hs - i]);
    }
    res[0] = maxIndex;
}
void clear_pq(vector<PqItemIndex*>pq){
    for(auto * x: pq)
        delete x;
    pq.clear();
}

//void clear_tss(vector<vector<float>*>* tss, vector<PqItemSeriesVector *> &heap){
//    bool flag = false;
//    for(auto * ts: *tss) {
//        flag = false;
//        for (auto *ts2:heap) {
//            if (ts == ts2->ts) {
//                flag = true;
//                ts2->needFree = true;
//                break;
//            }
//        }
//        if(!flag)    delete ts;
//    }
//    delete tss;
//}

void TimeSeriesUtil::knnWithBsfAndThreshold(const DSTreeNode& node, InsertedSeries& queryTs, int k, vector<PqItemSeries *> &heap, int threshold) {
    assert(k>0);
    double bsfMin;
    if(heap.size() < k)
        bsfMin = numeric_limits<double>::max();
    else
        bsfMin = heap[0]->dist;
    vector<int>pq;
    prepareTsWithBoundsAndThreshold(queryTs, bsfMin, node.splitPoints, node.splitPointsLen, threshold, node.means, node.stdevs, pq);
    int len = pq.size(), maxIndex = pq[0];
    auto* tss = node.loadTssRaw(maxIndex);
    for(int i=1;i<len;++i)
        bsfMin = processSingleSeriesInKnn(queryTs.ts, tss + (pq[i]) * Const::tsLength, k, heap, bsfMin);
    heap_data_copy(heap);
    delete[] tss;
}

void TimeSeriesUtil::knnRow(const DSTreeNode& node, InsertedSeries& q, int k, vector<PqItemSeries *> &heap, int pointsLen, double bsfMin) {
    vector<PqItemIndex*>pq;
    prepareTsWithBounds(q, bsfMin, node.splitPoints, pointsLen, node.means, node.stdevs, pq);
    int len = pq.size(), maxIndex = pq[0]->index;
    auto *tss = node.loadTssRaw(maxIndex);
    for(int i=1;i<len;++i){
        if(pq[i]->dist >= bsfMin)    break;
        bsfMin = processSingleSeriesInKnn(q.ts, tss + (pq[i]->index) * Const::tsLength, k, heap, bsfMin);
    }
    clear_pq(pq);
    heap_data_copy(heap);
    delete[] tss;
}

// only for leaf node
void TimeSeriesUtil::knnRow(const DSTreeNode& node, InsertedSeries& q, int k, vector<PqItemSeries *> &heap, int threshold) {
    double bsfMin;
    if(heap.size() < k)
        bsfMin = numeric_limits<double>::max();
    else
        bsfMin = heap[0]->dist;

    vector<PqItemIndex*>pq;
    threshold = min(threshold, node.size);
    auto *tss = node.loadTssRaw(threshold);
    for(int i=0;i<threshold;++i){
        bsfMin = processSingleSeriesInKnn(q.ts, tss + (long)i * Const::tsLength, k, heap, bsfMin);
    }
    heap_data_copy(heap);
    delete[] tss;
}


vector<PqItemSeries *> & TimeSeriesUtil::knn(const DSTreeNode& node, InsertedSeries& q, int k){
    assert(k>0);
    auto* heap = new vector<PqItemSeries*>();
    make_heap(heap->begin(),  heap->end(), PqItemSeriesMaxHeap());
    int pointsLen = node.splitPointsLen;
    double bsfMin = -numeric_limits<double>::max();
    knnRow(node, q, k, *heap, pointsLen, bsfMin);
    return *heap;
}

//void TimeSeriesUtil::knnRow(const DSTreeNode& node, InsertedSeries& q, int k, vector<PqItemSeriesVector *> &heap, int pointsLen, double bsfMin) {
//    vector<PqItemIndex*>pq;
//    prepareTsWithBounds(q, bsfMin, node.splitPoints, pointsLen, node.means, node.stdevs, pq);
//    int len = pq.size(), maxIndex = pq[0]->index;
//    auto *tss = node.loadTssVector(maxIndex);
//    for(int id=1;id<len;++id){
//        if(pq[id]->dist >= bsfMin)    break;
//        bsfMin = processSingleSeriesInKnn(q.ts, (*tss)[(pq[id]->index)], k, heap, bsfMin);
//    }
//    clear_pq(pq);
//    clear_tss(tss, heap);
//}
//vector<PqItemSeriesVector *> & TimeSeriesUtil::knnVector(const DSTreeNode& node, InsertedSeries& q, int k){
//    assert(k>0);
//    auto* heap = new vector<PqItemSeriesVector*>();
//    make_heap(heap->begin(),  heap->end(), PqItemSeriesVectorMaxHeap());
//    int pointsLen = node.splitPointsLen;
//    double bsfMin = numeric_limits<double>::max();
//    knnRow(node, q, k, *heap, pointsLen, bsfMin);
//    return *heap;
//}
//void TimeSeriesUtil::knnWithBsf(const DSTreeNode& node, InsertedSeries& queryTs, int k, vector<PqItemSeriesVector *> &heap) {
//    assert(k>0);
//    double bsfMin;
//    if(heap.size() < k)
//        bsfMin = numeric_limits<double>::max();
//    else
//        bsfMin = heap[0]->dist;
//    knnRow(node, queryTs, k, heap, node.splitPointsLen, bsfMin);
//}

void TimeSeriesUtil::knnWithBsf(const DSTreeNode& node, InsertedSeries& queryTs, int k, vector<PqItemSeries *> &heap) {
    assert(k>0);
    if(node.isLeafNode()){
        double bsfMin;
        if(heap.size() < k)
            bsfMin = numeric_limits<double>::max();
        else
            bsfMin = heap[0]->dist;
        node.knnRaw(queryTs, k, heap, -1);
    }else{
        if(node.left != nullptr)
            knnWithBsf(*node.left, queryTs, k, heap);
        if(node.right != nullptr)
        knnWithBsf(*node.right, queryTs, k, heap);
    }

}

double TimeSeriesUtil::processSingleSeriesInKnn(float* queryTs, float *ts, int k, vector<PqItemSeries *> &heap, double bsfMin) {
    if(heap.size()<k) {
        heap.push_back((new PqItemSeries(ts, queryTs, false, true)));
        push_heap(heap.begin(), heap.end(), PqItemSeriesMaxHeap());
        if(heap.size() < k)
            return numeric_limits<double>::max();
        else
            return heap[0]->dist;
    }
    else {
        // maxDistTs's distance<ts
        double newDist = euclideanDist(ts, queryTs, Const::tsLength, bsfMin);
        if(bsfMin > newDist){
            pop_heap(heap.begin(), heap.end(), PqItemSeriesMaxHeap());
            delete heap.back();
            heap.pop_back();
            heap.push_back((new PqItemSeries(ts, newDist, false, true)));
            push_heap(heap.begin(), heap.end(), PqItemSeriesMaxHeap());
            return heap[0]->dist;
        }
        return bsfMin;
    }
}

// only big vertex uses this function
//double
//TimeSeriesUtil::processSingleSeriesInKnn(float *queryTs, vector<float> *ts, int k, vector<PqItemSeriesVector *> &heap,
//                                         double bsfMin) {
//    if(heap.size()<k) {
//        heap.push_back((new PqItemSeriesVector(ts, queryTs, false)));       // delete useless series later
//        push_heap(heap.begin(), heap.end(), PqItemSeriesVectorMaxHeap());
//        if(heap.size() < k)
//            return numeric_limits<double>::max();
//        else
//            return heap[0]->dist;
//    }
//    else {
//        // maxDistTs's distance<ts
//        double newDist = euclideanDist(ts, queryTs, TimeSeries::tsLength);
//        if(bsfMin > newDist){
//            pop_heap(heap.begin(), heap.end(), PqItemSeriesVectorMaxHeap());
//            delete heap.back();
//            heap.pop_back();
//            heap.push_back((new PqItemSeriesVector(ts, newDist, false)));
//            push_heap(heap.begin(), heap.end(), PqItemSeriesVectorMaxHeap());
//            return heap[0]->dist;
//        }
//        return bsfMin;
//    }
//}


template<typename ... Args>
string TimeSeriesUtil::str_format(const string &format, Args ... args)
{
    auto size_buf = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1;
    std::unique_ptr<char[]> buf(new(std::nothrow) char[size_buf]);

    if (!buf)
        return string{};

    std::snprintf(buf.get(), size_buf, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size_buf - 1);
}