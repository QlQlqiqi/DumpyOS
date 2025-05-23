//
// Created by wzy on 2021/8/7.
//

#include "../../include/Utils/SaxUtil.h"
#include "../../include/Utils/FileUtil.h"
#include "../../include/DataStructures/TimeSeries.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cassert>
#include <immintrin.h>
using namespace std;

std::string SaxUtil::breakpointsFile = "../breakpoints.txt";
std::string SaxUtil::SEPARATOR = " |\t|\n|,";
double *SaxUtil::bp8 = new double[256]{-2.660067468617458,-2.4175590162365035,-2.2662268092096522,-2.1538746940614573,-2.063527898316245,-1.9874278859298962,-1.921350774293703,-1.8627318674216515,-1.8098922384806087,-1.7616704103630665,-1.7172281175057411,-1.6759397227734438,-1.637325382768064,-1.601008664886076,-1.5666885860684134,-1.534120544352546,-1.5031029431292737,-1.4734675779471014,-1.4450725798180746,-1.4177971379962677,-1.3915374879959008,-1.3662038163720984,-1.341717841080254,-1.318010897303537,-1.2950224067058147,-1.2726986411905359,-1.2509917154625454,-1.229858759216589,-1.209261231709155,-1.189164350199337,-1.169536610207143,-1.1503493803760083,-1.131576558386188,-1.113194277160929,-1.0951806527613883,-1.0775155670402805,-1.0601804794353549,-1.0431582633184537,-1.0264330631379108,-1.0099901692495823,-0.993815907860883,-0.9778975439405418,-0.9622231952954206,-0.946781756301046,-0.9315628300071148,-0.9165566675331128,-0.9017541138301002,-0.8871465590188762,-0.8727258946270402,-0.858484474141832,-0.8444150773752572,-0.8305108782053992,-0.8167654153150912,-0.8031725655979178,-0.7897265199432658,-0.7764217611479275,-0.7632530437325706,-0.7502153754679404,-0.7373040004386545,-0.7245143834923653,-0.711842195939419,-0.69928330238322,-0.6868337485747303,-0.6744897501960819,-0.6622476824884141,-0.6501040706479954,-0.6380555809225171,-0.6260990123464211,-0.6142312890602454,-0.6024494531644237,-0.5907506580628189,-0.5791321622555561,-0.5675913235445692,-0.5561255936186916,-0.5447325129881759,-0.5334097062412806,-0.5221548775980015,-0.5109658067382474,-0.4998403448837353,-0.4887764111146696,-0.4777719889038861,-0.46682512285258965,-0.4559339156131388,-0.44509652498551644,-0.4343111611752096,-0.42357608420119974,-0.41288960144365433,-0.40225006532172536,-0.3916558710925915,-0.3811054547635565,-0.3705972911096293,-0.36012989178956945,-0.3497018035538953,-0.3393116065388173,-0.3289579126404911,-0.31863936396437526,-0.30835463134483726,-0.2981024129304869,-0.2878814328310118,-0.27769043982157676,-0.2675282061010972,-0.25739352610093835,-0.24728521534080486,-0.2372021093287877,-0.22714306250271535,-0.2171069472101298,-0.2070926527243603,-0.19709908429431236,-0.18712516222572084,-0.17716982099173986,-0.16723200837085014,-0.15731068461017073,-0.14740482161235488,-0.13751340214433597,-0.12763541906627035,-0.11776987457909531,-0.10791577948918657,-0.0980721524886611,-0.08823801944992447,-0.07841241273311222,-0.06859437050511813,-0.05878293606894307,-0.04897715720213194,-0.03917608550309764,-0.02937877574415705,-0.019584285230126924,-0.009791673161345348,0.0,0.009791673161345348,0.019584285230126924,0.02937877574415705,0.03917608550309764,0.04897715720213194,0.05878293606894307,0.06859437050511813,0.07841241273311222,0.08823801944992447,0.0980721524886611,0.10791577948918657,0.11776987457909531,0.12763541906627035,0.13751340214433597,0.14740482161235488,0.15731068461017073,0.16723200837085014,0.17716982099173986,0.18712516222572084,0.19709908429431236,0.2070926527243603,0.2171069472101298,0.22714306250271535,0.2372021093287877,0.24728521534080486,0.25739352610093835,0.2675282061010972,0.27769043982157676,0.2878814328310118,0.2981024129304869,0.30835463134483726,0.31863936396437526,0.3289579126404911,0.3393116065388173,0.3497018035538953,0.36012989178956945,0.3705972911096293,0.3811054547635565,0.3916558710925915,0.40225006532172536,0.41288960144365433,0.42357608420119974,0.4343111611752096,0.44509652498551644,0.4559339156131388,0.46682512285258965,0.4777719889038861,0.4887764111146696,0.4998403448837353,0.5109658067382474,0.5221548775980015,0.5334097062412806,0.5447325129881759,0.5561255936186916,0.5675913235445692,0.5791321622555561,0.5907506580628189,0.6024494531644237,0.6142312890602454,0.6260990123464211,0.6380555809225171,0.6501040706479954,0.6622476824884141,0.6744897501960819,0.6868337485747303,0.69928330238322,0.711842195939419,0.7245143834923653,0.7373040004386545,0.7502153754679404,0.7632530437325706,0.7764217611479275,0.7897265199432658,0.8031725655979178,0.8167654153150912,0.8305108782053992,0.8444150773752572,0.858484474141832,0.8727258946270402,0.8871465590188762,0.9017541138301002,0.9165566675331128,0.9315628300071148,0.946781756301046,0.9622231952954206,0.9778975439405418,0.993815907860883,1.0099901692495823,1.0264330631379108,1.0431582633184537,1.0601804794353549,1.0775155670402805,1.0951806527613883,1.113194277160929,1.131576558386188,1.1503493803760083,1.169536610207143,1.189164350199337,1.209261231709155,1.229858759216589,1.2509917154625454,1.2726986411905359,1.2950224067058147,1.318010897303537,1.341717841080254,1.3662038163720984,1.3915374879959008,1.4177971379962677,1.4450725798180746,1.4734675779471014,1.5031029431292737,1.534120544352546,1.5666885860684134,1.601008664886076,1.637325382768064,1.6759397227734438,1.7172281175057411,1.7616704103630665,1.8098922384806087,1.8627318674216515,1.921350774293703,1.9874278859298962,2.063527898316245,2.1538746940614573,2.2662268092096522,2.4175590162365035,2.660067468617458, numeric_limits<float>::max()};
double* SaxUtil::breakpoints = readDoubleFromFileAtOnce(SaxUtil::breakpointsFile);
float* SaxUtil::breakpoints_f = readFloatFromFileAtOnce(SaxUtil::breakpointsFile);

std::vector<std::string> split(std::string& str, std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str += pattern;
    int size = str.size();
    for (int i = 0; i < size; i++)
    {
        pos = str.find(pattern, i);
        if (pos < size)
        {
            std::string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}


double* SaxUtil::readFromstring(string str) {
    vector<string> strings = split(str, ",");
    double* ret = new double[strings.size()];

    for (int i = 0; i < strings.size(); i++) {
        string s = strings[i];
        string::size_type size_type;
        if (s.length() > 0)
            ret[i] = stod(s, &size_type);
    }

    return ret;
}

float* SaxUtil::readFromstringFloat(string str) {
    vector<string> strings = split(str, ",");
    auto* ret = new float[strings.size()];

    for (int i = 0; i < strings.size(); i++) {
        string s = strings[i];
        string::size_type size_type;
        if (s.length() > 0)
            ret[i] = stof(s, &size_type);
    }

    return ret;
}

double* SaxUtil::readDoubleFromFileAtOnce(const string& fileName){

    string line;
    std::ifstream fin(fileName);
    getline(fin, line);

    auto ret = readFromstring(line);
    fin.close();
    return ret;
}

float* SaxUtil::readFloatFromFileAtOnce(const string& fileName){

    string line;
    std::ifstream fin(fileName);
    getline(fin, line);

    auto ret = readFromstringFloat(line);
    return ret;
}

void SaxUtil::id2Sax(int id, int*sax, int segment_num){
    for(int i=0;i<segment_num;++i){
        int temp = id >> 1;
        sax[segment_num - i - 1] = (temp + temp == id) ? 0 : 1;
        id = temp;
    }
}

void SaxUtil::id2Sax2(int id, unsigned short *sax, int segment_num){
    for(int i=0;i<segment_num;++i){
        int temp = id >> 1;
        sax[segment_num - i - 1] = (sax[segment_num - i - 1] << 1) + ((temp + temp == id) ? 0 : 1);
        id = temp;
    }
}

double * SaxUtil::paaFromTs(const float* ts, int tsLengthPerSegment, int segmentNum){
    // Create PAA representation
    auto* paa = new double [segmentNum];

    int s, i;
    for (s=0; s<segmentNum; s++) {
        paa[s] = 0;
        for (i=0; i<tsLengthPerSegment; i++) {
            paa[s] += ts[(s * tsLengthPerSegment)+i];
        }
        paa[s] /= tsLengthPerSegment;
    }
    return paa;
}

float * SaxUtil::paaFromTsFloat(const float* ts, int tsLengthPerSegment, int segmentNum){
    // Create PAA representation
    auto* paa = new float [segmentNum];

    int s, i;
    for (s=0; s<segmentNum; s++) {
        paa[s] = 0;
        for (i=0; i<tsLengthPerSegment; i++) {
            paa[s] += ts[(s * tsLengthPerSegment)+i];
        }
        paa[s] /= tsLengthPerSegment;
    }
    return paa;
}

void SaxUtil::paaFromTs(const float* ts, float *paa, int tsLengthPerSegment, int segmentNum){
    // Create PAA representation
    double tmp[segmentNum];
    int s, i;
    for (s=0; s<segmentNum; s++) {
        tmp[s] = 0;
        for (i=0; i<tsLengthPerSegment; i++) {
            tmp[s] += ts[(s * tsLengthPerSegment)+i];
        }
        tmp[s] /= tsLengthPerSegment;
        paa[s] = tmp[s];
    }
}

void SaxUtil::paaAndSaxFromTs(const float* ts, float *paa, unsigned short *sax, int tsLengthPerSegment, int segmentNum, int cardinality){
    // Create PAA representation
    int offset = ((cardinality - 1) * (cardinality - 2)) / 2;
    int s, i;
    for (s=0; s<segmentNum; s++) {
        paa[s] = 0;
        for (i=0; i<tsLengthPerSegment; i++) {
            paa[s] += ts[(s * tsLengthPerSegment)+i];
        }
        paa[s] /= tsLengthPerSegment;
        int index = findFirstGE(breakpoints, offset, cardinality -1, paa[s]);

        if(index >= 0)
            sax[s] = (index - offset);
        else
            cout<<"ERROR!!!!!!!";
    }
}

vector<int> * SaxUtil::saxFromTs(float*ts, int tsLengthPerSegment, int segmentNum, int cardinality)
{
    // Create PAA representation
    double * paa = paaFromTs(ts, tsLengthPerSegment, segmentNum);

    // Convert PAA to SAX
    // Note: Each cardinality has cardinality - 1 break points if c is cardinality
    //       the breakpoints can be found in the following array positions:
    //       FROM (c - 1) * (c - 2) / 2
    //       TO   (c - 1) * (c - 2) / 2 + c - 1
    int offset = ((cardinality - 1) * (cardinality - 2)) / 2;
    //printf("FROM %lf TO %lf\n", sax_breakpoints[offset], sax_breakpoints[offset + cardinality - 2]);

    auto*sax = new vector<int>(segmentNum);
//    int* sax = new int[segmentNum];
    int si;
    for (si=0; si<segmentNum; si++) {
        (*sax)[si] = 0;

        // First object = sax_breakpoints[offset]
        // Last object = sax_breakpoints[offset + cardinality - 2]
        // Size of sub-array = cardinality - 1
        int index = findFirstGE(breakpoints, offset, cardinality -1, paa[si]);

        if(index >= 0)
            (*sax)[si] = (index - offset);
        else
            cout<<"ERROR!!!!!!!";

    }

    delete[] paa;
    //sax_print(sax_out, segments, cardinality);
    return sax;
}

void SaxUtil::saxFromTs(const float*ts, unsigned short *sax, int tsLengthPerSegment, int segmentNum, int cardinality)
{
    // Create PAA representation
    double paa[segmentNum];
    int s, i;
    for (s=0; s<segmentNum; s++) {
        paa[s] = 0;
        for (i=0; i<tsLengthPerSegment; i++) {
            paa[s] += ts[(s * tsLengthPerSegment)+i];
        }
        paa[s] /= tsLengthPerSegment;
    }

    // Convert PAA to SAX
    // Note: Each cardinality has cardinality - 1 break points if c is cardinality
    //       the breakpoints can be found in the following array positions:
    //       FROM (c - 1) * (c - 2) / 2
    //       TO   (c - 1) * (c - 2) / 2 + c - 1
    int offset = ((cardinality - 1) * (cardinality - 2)) / 2;
    //printf("FROM %lf TO %lf\n", sax_breakpoints[offset], sax_breakpoints[offset + cardinality - 2]);

    int si;
    for (si=0; si<segmentNum; si++) {
        // First object = sax_breakpoints[offset]
        // Last object = sax_breakpoints[offset + cardinality - 2]
        // Size of sub-array = cardinality - 1
        int index = findFirstGE(breakpoints, offset, cardinality -1, paa[si]);

        if(index >= 0)
            sax[si] = (index - offset);
        else {
          cout << "ERROR!!!!!!!\n";
          exit(-1);
        }
    }
    //sax_print(sax_out, segments, cardinality);
}

vector<unsigned short> * SaxUtil::saxFromPaa(float *paa, int segmentNum, int cardinality)
{

    // Convert PAA to SAX
    // Note: Each cardinality has cardinality - 1 break points if c is cardinality
    //       the breakpoints can be found in the following array positions:
    //       FROM (c - 1) * (c - 2) / 2
    //       TO   (c - 1) * (c - 2) / 2 + c - 1
    int offset = ((cardinality - 1) * (cardinality - 2)) / 2;
    //printf("FROM %lf TO %lf\n", sax_breakpoints[offset], sax_breakpoints[offset + cardinality - 2]);

    auto*sax = new vector<unsigned short >(segmentNum);
    //    int* sax = new int[segmentNum];
    int si;
    for (si=0; si<segmentNum; si++) {
        (*sax)[si] = 0;

        // First object = sax_breakpoints[offset]
        // Last object = sax_breakpoints[offset + cardinality - 2]
        // Size of sub-array = cardinality - 1
        int index = findFirstGE(breakpoints, offset, cardinality -1, paa[si]);

        if(index >= 0)
            (*sax)[si] = (index - offset);
        else
            cout<<"ERROR!!!!!!!";

    }
    //sax_print(sax_out, segments, cardinality);
    return sax;
}

int SaxUtil::findFirstGE(const double* array, int start, int length, double target) // satisfy condition: array[?] >= target  and the first one
{
    int end = start + length - 1;
    while (start <= end) {
        int mid = (start + end) / 2;
        if (array[mid] < target)
            start = mid + 1;
        else if (array[mid] >= target)
            end = mid - 1;
    }
    if (end == start + length - 1)
        return -1;
    return end + 1;

}

int SaxUtil::findFirstGE(const int* array, int start, int length, int target) // satisfy condition: array[?] >= target  and the first one
{
    int end = start + length - 1;
    while (start <= end) {
        int mid = (start + end) / 2;
        if (array[mid] < target)
            start = mid + 1;
        else if (array[mid] >= target)
            end = mid - 1;
    }
    if (end == start + length - 1)
        return -1;
    return end + 1;

}

int* SaxUtil::invSaxFromSax(vector<int> *sax, int bitsCardinality, int segmentNum)
{
    int j, i, segi, invj;
    int* s = new int[bitsCardinality];
    long n;
    //char sum[segments][cardinality];//=malloc(sizeof(char *) * (segments));

    for (j=0; j < bitsCardinality; j++)
        s[j] = 0;

    segi=0;
    invj = segmentNum - 1;

    for (i=bitsCardinality-1; i>=0; i--)
    {
        for (j=0; j < segmentNum; j++)
        {
            n = (*sax)[j];
            n = n >> i;

            s[segi] |= (n % 2) << invj;

            invj--;
            if(invj==-1)
            {
                segi++;
                invj = segmentNum - 1;
            }
        }
    }
    return s;
}

void SaxUtil::invSaxFromSax(vector<int> *sax, unsigned *invSAX, int bitsCardinality, int segmentNum)
{
    int j, i, segi, invj;
    long n;
    //char sum[segments][cardinality];//=malloc(sizeof(char *) * (segments));

    for (j=0; j < bitsCardinality; j++)
        invSAX[j] = 0;

    segi=0;
    invj = segmentNum - 1;

    for (i=bitsCardinality-1; i>=0; i--)
    {
        for (j=0; j < segmentNum; j++)
        {
            n = (*sax)[j];
            n = n >> i;

            invSAX[segi] |= (n % 2) << invj;

            invj--;
            if(invj==-1)
            {
                segi++;
                invj = segmentNum - 1;
            }
        }
    }
}

string SaxUtil::invSax2String(unsigned *invsax){
    string ret;
    for(int i=0;i<Const::bitsCardinality;++i){
        ret += to_string(invsax[i]);
        ret += ".";
    }
    return ret;
}

void SaxUtil::string2invSAX(string invsax, unsigned *ret){
    int cur = 0, start = 0;
    for(int i=0;i<invsax.size();++i){
        if(invsax[i] == '.'){
            ret[cur] = atol(invsax.substr(start, i - start).c_str());
            start = i + 1;
            ++cur;
        }
    }
    if(cur < Const::bitsCardinality){
        cout <<"Not enough invSAX word!"<<endl;
        exit(-3);
    }
}

unsigned *SaxUtil::str2Invsax(string str){
    auto ret = new unsigned[Const::bitsCardinality];
    int start = 0, cur = 0;
    for(int i=0;i<str.size();++i){
        if(str[i] == '.'){
            ret[cur] = atol(str.substr(start, i - start).c_str());
            ++cur;
            start = i + 1;
            if(cur > Const::bitsCardinality)    break;
        }
    }

    return ret;
}

unsigned short *SaxUtil::invSax2Sax(const unsigned *invsax, int unit_num) {
    auto sax = new unsigned short[Const::segmentNum];
    for(int i=0;i<Const::segmentNum;++i)    sax[i] = 0;
    for(int i=0;i<unit_num;++i){
        unsigned unit = invsax[i];
        for(int j = Const::segmentNum - 1; j>=0;--j){
            unsigned tag = unit % 2;
            unit >>= 1;
            sax[j] = (sax[j] << 1) + tag;
        }
    }
    return sax;
}

string SaxUtil::invSaxHeadKFromInvSax(string invsax, int layer){
    string ret;
    int cur = 0;
    for(int i=0;i<invsax.size();++i){
        if(invsax[i] == '.'){
            ++cur;
            if(cur >= layer){
                return invsax.substr(0, i + 1);
            }
        }
    }
    Const::logPrint("Error in extract prefixes of invsax.");
    exit(-1);
}

int SaxUtil::invSaxHeadFromSax(vector<int> *sax, int bitsCardinality, int segmentNum)
{
    int i=bitsCardinality-1,s = 0,n;
    for (int j=0; j < segmentNum; j++)
    {
        n = (*sax)[j];
        n >>= i;
        s |= (n % 2) << (segmentNum - 1 - j);
    }
    return s;
}

//int SaxUtil::invSaxHeadFromSax(const int *sax, int bitsCardinality, int segmentNum)
//{
//    int i=bitsCardinality-1,s = 0,n;
//    for (int j=0; j < segmentNum; j++)
//    {
//        n = sax[j];
//        n >>= i;
//        s |= (n % 2) << (segmentNum - 1 - j);
//    }
//    return s;
//}

int SaxUtil::invSaxHeadFromSax(const unsigned short *sax, int bitsCardinality, int segmentNum)
{
    int i=bitsCardinality-1,s = 0,n;
    for (int j=0; j < segmentNum; j++)
    {
        n = sax[j];
        n >>= i;
        s |= (n % 2) << (segmentNum - 1 - j);
    }
    return s;
}

int SaxUtil::invSaxHead2FromSax(const int *sax, int bitsCardinality, int segmentNum)
{
    int i=bitsCardinality-2,s = 0,n;
    for (int j=0; j < segmentNum; j++)
    {
        n = sax[j];
        n >>= i;
        s |= (n % 2) << (segmentNum - 1 - j);
    }
    return s;
}

int SaxUtil::invSaxHeadkFromSax(const unsigned short *sax, int bitsCardinality, int segmentNum, int k)
{
    int i=bitsCardinality-k,s = 0,n;
    for (int j=0; j < segmentNum; j++)
    {
        n = sax[j];
        n >>= i;
        s |= (n % 2) << (segmentNum - 1 - j);
    }
    return s;
}

int SaxUtil::invSaxHead2FromSax(vector<int> *sax, int bitsCardinality, int segmentNum)
{
    int i=bitsCardinality-2,s = 0,n;
    for (int j=0; j < segmentNum; j++)
    {
        n = (*sax)[j];
        n >>= i;
        s |= (n % 2) << (segmentNum - 1 - j);
    }
    return s;
}

int SaxUtil::invSaxHeadFromTs(const float* ts, int tsLengthPerSegment, int segmentNum) {
    // Create PAA representation
    double paa_sim[segmentNum];

    for (int s=0; s<segmentNum; s++) {
        paa_sim[s] = 0;
        for (int i=0; i<tsLengthPerSegment; i++) {
            paa_sim[s] += ts[(s * tsLengthPerSegment)+i];
        }
    }
    int res = 0;
    for (int s=0; s<segmentNum; s++) {
        res <<= 1;
        if(paa_sim[s] >= 0)
            res++;
    }

    return res;
}

int SaxUtil::invSaxHeadFromPaa(const float *paa, int tsLengthPerSegment, int segmentNum) {
    int res = 0;
    for (int s=0; s<segmentNum; s++) {
        res <<= 1;
        if(paa[s] >= 0)
            res++;
    }
    return res;
}

int SaxUtil::invSaxHead2FromTs(const float* ts, int tsLengthPerSegment, int segmentNum) {
    double splits[]{-0.67448975019608193 * tsLengthPerSegment,0.67448975019608193 * tsLengthPerSegment};

    // Create PAA representation
    double paa_sim[segmentNum];

    for (int s=0; s<segmentNum; s++) {
        paa_sim[s] = 0;
        for (int i=0; i<tsLengthPerSegment; i++) {
            paa_sim[s] += ts[(s * tsLengthPerSegment)+i];
        }
    }
    int res = 0;
    for (int s=0; s<segmentNum; s++) {
        res <<= 1;
        if((paa_sim[s] >splits[0] && paa_sim[s] <=0) || (paa_sim[s] > splits[1]) )
            res +=1;
    }

    return res;
}

double SaxUtil::LowerBound_Paa_iSax(const float *paa, const unsigned short *sax){
    double frontCoef = (double)Const::tsLength / Const::segmentNum; // n / w
    int offset = Const::offset, cardinality = Const::cardinality;
    double paaValue, lb, ub;
    unsigned short saxValue;
    double sum = 0;

    for (int i = 0; i < Const::segmentNum; i++) {
        paaValue = paa[i];
        saxValue = sax[i];
        if(saxValue == 0){
            lb = -numeric_limits<double>::max();
            ub = breakpoints[offset];
        }
        else if(saxValue == cardinality - 1){
            lb = breakpoints[offset + cardinality - 2];
            ub = numeric_limits<double>::max();
        }
        else {
            lb = breakpoints[offset + saxValue - 1];
            ub = breakpoints[offset + saxValue];
        }

        if(paaValue < lb){
            sum += (lb - paaValue) * (lb - paaValue);
        }
        else if(paaValue > ub){
            sum += (paaValue - ub) * (paaValue - ub);
        }
    } // for

    return frontCoef * sum;
}

//double SaxUtil::LowerBound_Paa_iSax(const double* paa, const int *sax, const int* bits_cardinality, vector<int>&chosen_segs, int new_id){
//    double frontCoef = (double)Const::tsLength / Const::segmentNum; // n / w
//    double paaValue, lb, ub;
//    double sum = 0;
//    int saxValue, cur = chosen_segs.size() - 1, bc;
//
//    for (int i = 0; i < Const::segmentNum; i++) {
//        paaValue = paa[i];
//        if(chosen_segs[cur] == i){
//            saxValue = (sax[i] << 1) + (new_id % 2);
//            new_id >>= 1;
//            bc = bits_cardinality[i] + 1;
//        } else {
//            saxValue = sax[i];
//            bc = bits_cardinality[i];
//        }
//        getValueRange(saxValue, bc, &lb, &ub);
//
//        if(paaValue < lb){
//            sum += (lb - paaValue) * (lb - paaValue);
//        }
//        else if(paaValue > ub){
//            sum += (paaValue - ub) * (paaValue - ub);
//        }
//    } // for
//
//    return frontCoef * sum;
//}

double SaxUtil::LowerBound_Paa_iSax(const float *paa, const unsigned short *sax, const int* bits_cardinality, vector<int>&chosen_segs, int new_id){
    double frontCoef = (double)Const::tsLength / Const::segmentNum; // n / w
    double paaValue, lb, ub;
    double sum = 0;
    int saxValue, cur = chosen_segs.size() - 1, bc;

    for (int i = Const::segmentNum-1; i >= 0; --i) {
        paaValue = paa[i];
        if(cur >=0 && chosen_segs[cur] == i){
            saxValue = (sax[i] << 1) + (new_id % 2);
            new_id >>= 1;
            bc = bits_cardinality[i] + 1;
            --cur;
        } else {
            saxValue = sax[i];
            bc = bits_cardinality[i];
        }
        getValueRange(saxValue, bc, &lb, &ub);

        if(paaValue < lb){
            sum += (lb - paaValue) * (lb - paaValue);
        }
        else if(paaValue > ub){
            sum += (paaValue - ub) * (paaValue - ub);
        }
    } // for

    return frontCoef * sum;
}

double SaxUtil::getMidLineFromSaxSymbolbc8(unsigned short symbol){
    double numer;
    assert(symbol >=0 && symbol <=255);
    if(symbol == 0) {
        numer = SaxUtil::bp8[0] - (SaxUtil::bp8[1] - SaxUtil::bp8[0]);
    }
    else if(symbol == 255) {
        numer = SaxUtil::bp8[254] + (SaxUtil::bp8[254] - SaxUtil::bp8[253]);
    }
    else {
        numer = (SaxUtil::bp8[symbol - 1] + SaxUtil::bp8[symbol]) / 2.0;
    }
    return numer;
}

double SaxUtil::LowerBound_Paa_iSax(const float *paa, const unsigned short *sax, int bits_cardinality) {
    double frontCoef = (double)Const::tsLength / Const::segmentNum; // n / w
    long offset; int cardinality;
    if(bits_cardinality == Const::bitsCardinality)
        offset = Const::offset, cardinality = Const::cardinality;
    else
        cardinality = 1 << bits_cardinality, offset = ((long )(cardinality - 1) * (cardinality - 2)) / 2;
    double paaValue, lb, ub;
    int saxValue;
    double sum = 0;

    for (int i = 0; i < Const::segmentNum; i++) {
        paaValue = paa[i];
        saxValue = sax[i];
        if(saxValue == 0){
            lb = -numeric_limits<double>::max();
            ub = breakpoints[offset];
        }
        else if(saxValue == cardinality - 1){
            lb = breakpoints[offset + cardinality - 2];
            ub = numeric_limits<double>::max();
        }
        else {
            lb = breakpoints[offset + saxValue - 1];
            ub = breakpoints[offset + saxValue];
        }

        if(paaValue < lb){
            sum += (lb - paaValue) * (lb - paaValue);
        }
        else if(paaValue > ub){
            sum += (paaValue - ub) * (paaValue - ub);
        }
    } // for

    return frontCoef * sum;
}

// complete SAX word, bits cardinality <= 8, #segments = 16 buggy code
double  SaxUtil::LowerBound_Paa_iSax_SIMD(const float *paa, unsigned char *sax_short, unsigned char *bits_cardinality_short)
{

    int region_upper[16],region_lower[16];
    float distancef[16];
    int offset = Const::offset;

    __m256i vectorsignbit = _mm256_set1_epi32 (0xffffffff);
    __m256i vloweroffset = _mm256_set1_epi32 (offset-1);
    __m256i vupperoffset = _mm256_set1_epi32 (offset);

    //__m256i c_cv_0 = _mm256_set_epi32 ( sax_cardinalities[7] , sax_cardinalities[6] ,sax_cardinalities[5] ,sax_cardinalities[4] , sax_cardinalities[3] ,sax_cardinalities[2] ,sax_cardinalities[1],sax_cardinalities[0]);
    //__m256i c_cv_1 = _mm256_set_epi32 ( sax_cardinalities[15], sax_cardinalities[14],sax_cardinalities[13],sax_cardinalities[12], sax_cardinalities[11],sax_cardinalities[10],sax_cardinalities[9],sax_cardinalities[8]);
//    __m256i sax_cardinalitiesv16 = _mm256_lddqu_si256((const __m256i*)bits_cardinality);
    __m128i sax_cardinalitiesv8 = _mm_lddqu_si128 ((const __m128i*)bits_cardinality_short);
    __m256i sax_cardinalitiesv16= _mm256_cvtepu8_epi16 (sax_cardinalitiesv8);
    __m128i sax_cardinalitiesv16_0=_mm256_extractf128_si256 (sax_cardinalitiesv16,0);
    __m128i sax_cardinalitiesv16_1=_mm256_extractf128_si256 (sax_cardinalitiesv16,1);
    __m256i bits_cardinality_0 = _mm256_cvtepu16_epi32 (sax_cardinalitiesv16_0);
    __m256i bits_cardinality_1 = _mm256_cvtepu16_epi32 (sax_cardinalitiesv16_1);

    //__m256i v_0    = _mm256_set_epi32 (sax[7],sax[6],sax[5],sax[4],sax[3],sax[2],sax[1],sax[0]);
    //__m256i v_1    = _mm256_set_epi32 (sax[15],sax[14],sax[13],sax[12],sax[11],sax[10],sax[9],sax[8]);
    __m128i saxv8= _mm_lddqu_si128 ((const __m128i*)sax_short);
    __m256i saxv16= _mm256_cvtepu8_epi16 (saxv8);
    __m128i saxv16_0 =_mm256_extractf128_si256 (saxv16,0);
    __m128i saxv16_1=_mm256_extractf128_si256 (saxv16,1);
    __m256i sax_0= _mm256_cvtepu16_epi32 (saxv16_0);
    __m256i sax_1 = _mm256_cvtepu16_epi32 (saxv16_1);


    __m256i c_m    = _mm256_set1_epi32 (Const::bitsCardinality);
    __m256i rest_bits_card_0 = _mm256_sub_epi32 (c_m, bits_cardinality_0);
    __m256i rest_bits_card_1 = _mm256_sub_epi32 (c_m, bits_cardinality_1);

    __m256i region_lowerv_0 = _mm256_sllv_epi32 (sax_0, rest_bits_card_0);
    __m256i region_lowerv_1 = _mm256_sllv_epi32 (sax_1, rest_bits_card_1);


    __m256i v1= _mm256_set1_epi32 ((int)0x7f800000);

    __m256i region_upperv_0 = _mm256_sllv_epi32 (v1, rest_bits_card_0);
    __m256i region_upperv_1 = _mm256_sllv_epi32 (v1, rest_bits_card_1);
    region_upperv_0 = _mm256_andnot_si256 (region_upperv_0, vectorsignbit);
    region_upperv_1 = _mm256_andnot_si256 (region_upperv_1, vectorsignbit);

    region_upperv_0 = _mm256_or_si256 (region_upperv_0, region_lowerv_0);

    region_upperv_1 = _mm256_or_si256 (region_upperv_1, region_lowerv_1);

    region_lowerv_0 = _mm256_add_epi32 (region_lowerv_0, vloweroffset);
    region_lowerv_1 = _mm256_add_epi32 (region_lowerv_1, vloweroffset);
    region_upperv_0 = _mm256_add_epi32 (region_upperv_0, vupperoffset);
    region_upperv_1 = _mm256_add_epi32 (region_upperv_1, vupperoffset);
    _mm256_storeu_si256 ((__m256i_u*)&(region_lower[0]),region_lowerv_0);
    _mm256_storeu_si256 ((__m256i_u*)&(region_lower[8]),region_lowerv_1);
    _mm256_storeu_si256 ((__m256i_u*)&(region_upper[0]),region_upperv_0);
    _mm256_storeu_si256 ((__m256i_u*)&(region_upper[8]),region_upperv_1);


    //lower

    __m256i lower_juge_zerov_0 = _mm256_cmpeq_epi32 (region_lowerv_0, _mm256_setzero_si256 ());
    __m256i lower_juge_zerov_1 = _mm256_cmpeq_epi32 (region_lowerv_1, _mm256_setzero_si256 ());

    __m256i lower_juge_nzerov_0 = _mm256_andnot_si256 (lower_juge_zerov_0, vectorsignbit);
    __m256i lower_juge_nzerov_1 = _mm256_andnot_si256 (lower_juge_zerov_1, vectorsignbit);

    __m256 minvalv = _mm256_set1_ps (-1000);

    __m256 lsax_breakpoints_shiftv_0 =_mm256_i32gather_ps (breakpoints_f,region_lowerv_0, 4);
    __m256 lsax_breakpoints_shiftv_1 =_mm256_i32gather_ps (breakpoints_f,region_lowerv_1, 4);


    __m256 breakpoint_lowerv_0 = (__m256)_mm256_or_si256 (_mm256_and_si256(lower_juge_zerov_0,(__m256i)minvalv),_mm256_and_si256(lower_juge_nzerov_0,(__m256i)lsax_breakpoints_shiftv_0));
    __m256 breakpoint_lowerv_1 = (__m256)_mm256_or_si256 (_mm256_and_si256(lower_juge_zerov_1,(__m256i)minvalv),_mm256_and_si256(lower_juge_nzerov_1,(__m256i)lsax_breakpoints_shiftv_1));

    //upper
    __m256 usax_breakpoints_shiftv_0 =_mm256_i32gather_ps (breakpoints_f,region_upperv_0, 4);
    __m256 usax_breakpoints_shiftv_1 =_mm256_i32gather_ps (breakpoints_f,region_upperv_1, 4);

    __m256i upper_juge_maxv_0 = _mm256_cmpeq_epi32 (region_upperv_0,  _mm256_set1_epi32 (Const::bitsCardinality - 1));
    __m256i upper_juge_maxv_1 = _mm256_cmpeq_epi32 (region_upperv_1,  _mm256_set1_epi32 (Const::bitsCardinality - 1));

    __m256i upper_juge_nmaxv_0 = _mm256_andnot_si256 (upper_juge_maxv_0, vectorsignbit);
    __m256i upper_juge_nmaxv_1 = _mm256_andnot_si256 (upper_juge_maxv_1, vectorsignbit);

    __m256 breakpoint_upperv_0 = (__m256)_mm256_or_si256 (_mm256_and_si256(upper_juge_maxv_0,(__m256i)_mm256_set1_ps (1000)),_mm256_and_si256(upper_juge_nmaxv_0,(__m256i)usax_breakpoints_shiftv_0));
    __m256 breakpoint_upperv_1 = (__m256)_mm256_or_si256 (_mm256_and_si256(upper_juge_maxv_1,(__m256i)_mm256_set1_ps (1000)),_mm256_and_si256(upper_juge_nmaxv_1,(__m256i)usax_breakpoints_shiftv_1));

    //dis
    __m256 paav_0,paav_1;



    paav_0 =_mm256_loadu_ps (paa);
    paav_1 =_mm256_loadu_ps (&(paa[8]));

    __m256 dis_juge_upv_0=_mm256_cmp_ps (breakpoint_lowerv_0, paav_0, _CMP_GT_OS);
    __m256 dis_juge_upv_1=_mm256_cmp_ps (breakpoint_lowerv_1, paav_1, _CMP_GT_OS);

    __m256 dis_juge_lov_0=(__m256)_mm256_and_si256 ((__m256i)_mm256_cmp_ps (breakpoint_lowerv_0, paav_0, _CMP_NGT_US),(__m256i)_mm256_cmp_ps (breakpoint_upperv_0, paav_0, _CMP_LT_OS))  ;
    __m256 dis_juge_lov_1=(__m256)_mm256_and_si256 ((__m256i)_mm256_cmp_ps (breakpoint_lowerv_1, paav_1, _CMP_NGT_US),(__m256i)_mm256_cmp_ps (breakpoint_upperv_1, paav_1, _CMP_LT_OS));

    __m256 dis_juge_elv_0=(__m256)_mm256_andnot_si256 (_mm256_or_si256 ((__m256i)dis_juge_upv_0, (__m256i)dis_juge_lov_0),vectorsignbit);
    __m256 dis_juge_elv_1=(__m256)_mm256_andnot_si256 (_mm256_or_si256 ((__m256i)dis_juge_upv_1, (__m256i)dis_juge_lov_1),vectorsignbit);

    __m256 dis_lowv_0 =_mm256_sub_ps (breakpoint_lowerv_0, paav_0);
    __m256 dis_lowv_1 =_mm256_sub_ps (breakpoint_lowerv_1, paav_1);
    __m256 dis_uppv_0 =_mm256_sub_ps (breakpoint_upperv_0, paav_0);
    __m256 dis_uppv_1 =_mm256_sub_ps (breakpoint_upperv_1, paav_1);


    __m256 distancev_0=(__m256)_mm256_or_si256(_mm256_or_si256(_mm256_and_si256((__m256i)dis_juge_upv_0,(__m256i)dis_lowv_0),_mm256_and_si256((__m256i)dis_juge_lov_0,(__m256i)dis_uppv_0)),_mm256_and_si256((__m256i)dis_juge_elv_0,(__m256i)_mm256_set1_ps (0.0)));
    __m256 distancev_1=(__m256)_mm256_or_si256(_mm256_or_si256(_mm256_and_si256((__m256i)dis_juge_upv_1,(__m256i)dis_lowv_1),_mm256_and_si256((__m256i)dis_juge_lov_1,(__m256i)dis_uppv_1)),_mm256_and_si256((__m256i)dis_juge_elv_1,(__m256i)_mm256_set1_ps (0.0)));


    __m256 distancesum_0 = _mm256_dp_ps (distancev_0, distancev_0, 0xff);
    __m256 distancesum_1 = _mm256_dp_ps (distancev_1, distancev_1, 0xff);
    __m256 distancevf = _mm256_add_ps (distancesum_0, distancesum_1);

    _mm256_storeu_ps (distancef ,distancevf);

    double frontCoef = (double)Const::tsLength / Const::segmentNum; // n / w

    return (distancef[0]+distancef[4])*frontCoef ;
}

double SaxUtil::LowerBound_Paa_iSax_SIMD(const float *paa, const unsigned short *sax, const int* bits_cardinality, vector<int>&chosen_segs, int new_id) {
    unsigned char bits_cardinality_short[16] __attribute__((aligned(32)));
    unsigned char sax_short[16] __attribute__((aligned(32)));
    int cur = chosen_segs.size() - 1;

    for(int i = Const::segmentNum - 1; i >= 0; --i){
        if(cur >=0 && chosen_segs[cur] == i){
            sax_short[i] = (sax[i] << 1) + (new_id % 2);
            new_id >>= 1;
            bits_cardinality_short[i] = bits_cardinality[i] + 1;
            --cur;
        } else {
            sax_short[i] = sax[i];
            bits_cardinality_short[i] = bits_cardinality[i];
        }
    }
    return LowerBound_Paa_iSax_SIMD(paa, sax_short, bits_cardinality_short);
}

double SaxUtil::LowerBound_Paa_iSax_SIMD(const float *paa, const unsigned short *sax, const int* bits_cardinality) {
    unsigned char bits_cardinality_short[16] __attribute__((aligned(32)));
    unsigned char sax_short[16] __attribute__((aligned(32)));
    for(int i = 0; i< 16; ++i){
        sax_short[i] = sax[i];
        bits_cardinality_short[i] = bits_cardinality[i];
    }
    return minidist_paa_to_isax_rawa_SIMD(paa, sax_short, bits_cardinality_short);
}

double  SaxUtil::minidist_paa_to_isax_rawa_SIMD(const float *paa, unsigned char*sax, unsigned char*sax_cardinalities){
    int region_upper[16],region_lower[16];
    float distancef[16];
    int offset = Const::offset;

    __m256i vectorsignbit = _mm256_set1_epi32 (0xffffffff);
    __m256i vloweroffset = _mm256_set1_epi32 (offset-1);
    __m256i vupperoffset = _mm256_set1_epi32 (offset);

    //__m256i c_cv_0 = _mm256_set_epi32 ( sax_cardinalities[7] , sax_cardinalities[6] ,sax_cardinalities[5] ,sax_cardinalities[4] , sax_cardinalities[3] ,sax_cardinalities[2] ,sax_cardinalities[1],sax_cardinalities[0]);
    //__m256i c_cv_1 = _mm256_set_epi32 ( sax_cardinalities[15], sax_cardinalities[14],sax_cardinalities[13],sax_cardinalities[12], sax_cardinalities[11],sax_cardinalities[10],sax_cardinalities[9],sax_cardinalities[8]);
    __m128i sax_cardinalitiesv8 = _mm_lddqu_si128 ((const __m128i*)sax_cardinalities);
    __m256i sax_cardinalitiesv16= _mm256_cvtepu8_epi16 (sax_cardinalitiesv8);
    __m128i sax_cardinalitiesv16_0=_mm256_extractf128_si256 (sax_cardinalitiesv16,0);
    __m128i sax_cardinalitiesv16_1=_mm256_extractf128_si256 (sax_cardinalitiesv16,1);
    __m256i c_cv_0 = _mm256_cvtepu16_epi32 (sax_cardinalitiesv16_0);
    __m256i c_cv_1 = _mm256_cvtepu16_epi32 (sax_cardinalitiesv16_1);

    //__m256i v_0    = _mm256_set_epi32 (sax[7],sax[6],sax[5],sax[4],sax[3],sax[2],sax[1],sax[0]);
    //__m256i v_1    = _mm256_set_epi32 (sax[15],sax[14],sax[13],sax[12],sax[11],sax[10],sax[9],sax[8]);
    __m128i saxv8= _mm_lddqu_si128 ((const __m128i*)sax);
    __m256i saxv16= _mm256_cvtepu8_epi16 (saxv8);
    __m128i saxv16_0 =_mm256_extractf128_si256 (saxv16,0);
    __m128i saxv16_1=_mm256_extractf128_si256 (saxv16,1);
    __m256i v_0= _mm256_cvtepu16_epi32 (saxv16_0);
    __m256i v_1 = _mm256_cvtepu16_epi32 (saxv16_1);


    __m256i c_m    = _mm256_set1_epi32 (Const::bitsCardinality);
    __m256i cm_ccv_0 = _mm256_sub_epi32 (c_m, c_cv_0);
    __m256i cm_ccv_1 = _mm256_sub_epi32 (c_m, c_cv_1);

    //__m256i _mm256_set_epi32 (int e7, int e6, int e5, int e4, int e3, int e2, int e1, int e0)
    //  __m256i _mm256_set1_epi32 (int a)
    __m256i region_lowerv_0 = _mm256_srlv_epi32 (v_0, cm_ccv_0);
    __m256i region_lowerv_1 = _mm256_srlv_epi32 (v_1, cm_ccv_1);
    region_lowerv_0 =  _mm256_sllv_epi32 (region_lowerv_0, cm_ccv_0);
    region_lowerv_1 =  _mm256_sllv_epi32 (region_lowerv_1, cm_ccv_1);


    __m256i v1= _mm256_andnot_si256 (_mm256_setzero_si256 (), vectorsignbit);

    __m256i region_upperv_0 = _mm256_sllv_epi32 (v1,cm_ccv_0);
    __m256i region_upperv_1 = _mm256_sllv_epi32 (v1,cm_ccv_1);
    region_upperv_0 = _mm256_andnot_si256 (region_upperv_0, vectorsignbit);
    region_upperv_1 = _mm256_andnot_si256 (region_upperv_1, vectorsignbit);

    region_upperv_0 = _mm256_or_si256 (region_upperv_0, region_lowerv_0);

    region_upperv_1 = _mm256_or_si256 (region_upperv_1, region_lowerv_1);

    region_lowerv_0 = _mm256_add_epi32 (region_lowerv_0, vloweroffset);
    region_lowerv_1 = _mm256_add_epi32 (region_lowerv_1, vloweroffset);
    region_upperv_0 = _mm256_add_epi32 (region_upperv_0, vupperoffset);
    region_upperv_1 = _mm256_add_epi32 (region_upperv_1, vupperoffset);
    _mm256_storeu_si256 ((__m256i_u*)&(region_lower[0]),region_lowerv_0);
    _mm256_storeu_si256 ((__m256i_u*)&(region_lower[8]),region_lowerv_1);
    _mm256_storeu_si256 ((__m256i_u*)&(region_upper[0]),region_upperv_0);
    _mm256_storeu_si256 ((__m256i_u*)&(region_upper[8]),region_upperv_1);


    //lower

    __m256i lower_juge_zerov_0 = _mm256_cmpeq_epi32 (region_lowerv_0, _mm256_setzero_si256 ());
    __m256i lower_juge_zerov_1 = _mm256_cmpeq_epi32 (region_lowerv_1, _mm256_setzero_si256 ());

    __m256i lower_juge_nzerov_0 = _mm256_andnot_si256 (lower_juge_zerov_0, vectorsignbit);
    __m256i lower_juge_nzerov_1 = _mm256_andnot_si256 (lower_juge_zerov_1, vectorsignbit);

    __m256 minvalv = _mm256_set1_ps (-999999);




    __m256 lsax_breakpoints_shiftv_0 =_mm256_i32gather_ps (breakpoints_f,region_lowerv_0, 4);
    //__m256 lsax_breakpoints_shiftv_0= _mm256_set_ps (sax_breakpoints[region_lower[7]],
    //sax_breakpoints[region_lower[6]],
    //sax_breakpoints[region_lower[5]],
    //sax_breakpoints[region_lower[4]],
    //sax_breakpoints[region_lower[3]],
    //sax_breakpoints[region_lower[2]],
    //sax_breakpoints[region_lower[1]],
    //sax_breakpoints[region_lower[0]]);
    __m256 lsax_breakpoints_shiftv_1 =_mm256_i32gather_ps (breakpoints_f,region_lowerv_1, 4);
    //__m256 lsax_breakpoints_shiftv_1= _mm256_set_ps (sax_breakpoints[region_lower[15]],
    //sax_breakpoints[region_lower[14]],
    //sax_breakpoints[region_lower[13]],
    //sax_breakpoints[region_lower[12]],
    //sax_breakpoints[region_lower[11]],
    //sax_breakpoints[region_lower[10]],
    //sax_breakpoints[region_lower[9]],
    //sax_breakpoints[region_lower[8]]);


    __m256 breakpoint_lowerv_0 = (__m256)_mm256_or_si256 (_mm256_and_si256(lower_juge_zerov_0,(__m256i)minvalv),_mm256_and_si256(lower_juge_nzerov_0,(__m256i)lsax_breakpoints_shiftv_0));
    __m256 breakpoint_lowerv_1 = (__m256)_mm256_or_si256 (_mm256_and_si256(lower_juge_zerov_1,(__m256i)minvalv),_mm256_and_si256(lower_juge_nzerov_1,(__m256i)lsax_breakpoints_shiftv_1));

    //uper
    __m256 usax_breakpoints_shiftv_0 =_mm256_i32gather_ps (breakpoints_f,region_upperv_0, 4);
    //__m256 usax_breakpoints_shiftv_0= _mm256_set_ps (sax_breakpoints[region_upper[7]],
    //sax_breakpoints[region_upper[6]],
    //sax_breakpoints[region_upper[5]],
    //sax_breakpoints[region_upper[4]],
    //sax_breakpoints[region_upper[3]],
    //sax_breakpoints[region_upper[2]],
    //sax_breakpoints[region_upper[1]],
    //sax_breakpoints[region_upper[0]]);
    __m256 usax_breakpoints_shiftv_1 =_mm256_i32gather_ps (breakpoints_f,region_upperv_1, 4);
    //__m256 usax_breakpoints_shiftv_1= _mm256_set_ps (sax_breakpoints[region_upper[15]],
    //sax_breakpoints[region_upper[14]],
    //sax_breakpoints[region_upper[13]],
    //sax_breakpoints[region_upper[12]],
    //sax_breakpoints[region_upper[11]],
    //sax_breakpoints[region_upper[10]],
    //sax_breakpoints[region_upper[9]],
    //sax_breakpoints[region_upper[8]]);


    __m256i upper_juge_maxv_0 = _mm256_cmpeq_epi32 (region_upperv_0,  _mm256_set1_epi32 (Const::bitsCardinality - 1));
    __m256i upper_juge_maxv_1 = _mm256_cmpeq_epi32 (region_upperv_1,  _mm256_set1_epi32 (Const::bitsCardinality - 1));

    __m256i upper_juge_nmaxv_0 = _mm256_andnot_si256 (upper_juge_maxv_0, vectorsignbit);
    __m256i upper_juge_nmaxv_1 = _mm256_andnot_si256 (upper_juge_maxv_1, vectorsignbit);

    __m256 breakpoint_upperv_0 = (__m256)_mm256_or_si256 (_mm256_and_si256(upper_juge_maxv_0,(__m256i)_mm256_set1_ps (99999)),_mm256_and_si256(upper_juge_nmaxv_0,(__m256i)usax_breakpoints_shiftv_0));
    __m256 breakpoint_upperv_1 = (__m256)_mm256_or_si256 (_mm256_and_si256(upper_juge_maxv_1,(__m256i)_mm256_set1_ps (99999)),_mm256_and_si256(upper_juge_nmaxv_1,(__m256i)usax_breakpoints_shiftv_1));

    //dis
    __m256 paav_0,paav_1;

    paav_0 =_mm256_loadu_ps (paa);
    paav_1 =_mm256_loadu_ps (&(paa[8]));

    __m256 dis_juge_upv_0=_mm256_cmp_ps (breakpoint_lowerv_0, paav_0, _CMP_GT_OS);
    __m256 dis_juge_upv_1=_mm256_cmp_ps (breakpoint_lowerv_1, paav_1, _CMP_GT_OS);

    __m256 dis_juge_lov_0=_mm256_cmp_ps (breakpoint_upperv_0, paav_0, _CMP_LT_OS);
    __m256 dis_juge_lov_1=_mm256_cmp_ps (breakpoint_upperv_1, paav_1, _CMP_LT_OS);


    // __m256 dis_juge_lov_0=(__m256)_mm256_and_si256 ((__m256i)_mm256_cmp_ps (breakpoint_lowerv_0, paav_0, _CMP_NGT_US),(__m256i)_mm256_cmp_ps (breakpoint_upperv_0, paav_0, _CMP_LT_OS))  ;
    // __m256 dis_juge_lov_1=(__m256)_mm256_and_si256 ((__m256i)_mm256_cmp_ps (breakpoint_lowerv_1, paav_1, _CMP_NGT_US),(__m256i)_mm256_cmp_ps (breakpoint_upperv_1, paav_1, _CMP_LT_OS));

    __m256 dis_juge_elv_0=(__m256)_mm256_andnot_si256 (_mm256_or_si256 ((__m256i)dis_juge_upv_0, (__m256i)dis_juge_lov_0),vectorsignbit);
    __m256 dis_juge_elv_1=(__m256)_mm256_andnot_si256 (_mm256_or_si256 ((__m256i)dis_juge_upv_1, (__m256i)dis_juge_lov_1),vectorsignbit);

    __m256 dis_lowv_0 =_mm256_sub_ps (breakpoint_lowerv_0, paav_0);
    __m256 dis_lowv_1 =_mm256_sub_ps (breakpoint_lowerv_1, paav_1);
    __m256 dis_uppv_0 =_mm256_sub_ps (breakpoint_upperv_0, paav_0);
    __m256 dis_uppv_1 =_mm256_sub_ps (breakpoint_upperv_1, paav_1);


    __m256 distancev_0=(__m256)_mm256_or_si256(_mm256_or_si256(_mm256_and_si256((__m256i)dis_juge_upv_0,(__m256i)dis_lowv_0),_mm256_and_si256((__m256i)dis_juge_lov_0,(__m256i)dis_uppv_0)),_mm256_and_si256((__m256i)dis_juge_elv_0,(__m256i)_mm256_set1_ps (0.0)));
    __m256 distancev_1=(__m256)_mm256_or_si256(_mm256_or_si256(_mm256_and_si256((__m256i)dis_juge_upv_1,(__m256i)dis_lowv_1),_mm256_and_si256((__m256i)dis_juge_lov_1,(__m256i)dis_uppv_1)),_mm256_and_si256((__m256i)dis_juge_elv_1,(__m256i)_mm256_set1_ps (0.0)));


    __m256 distancesum_0 = _mm256_dp_ps (distancev_0, distancev_0, 0xff);
    __m256 distancesum_1 = _mm256_dp_ps (distancev_1, distancev_1, 0xff);
    __m256 distancevf = _mm256_add_ps (distancesum_0, distancesum_1);
    //__m256 distancev2 = _mm256_hadd_ps (distancev, distancev);
    //__m256 distancevf = _mm256_hadd_ps (distancev2, distancev2);
    //__m256 _mm256_dp_ps (__m256 a, __m256 b, const int imm8);

    _mm256_storeu_ps (distancef ,distancevf);
    //_mm256_storeu_ps (&checkvalue[8] ,distancev_1);

    double frontCoef = (double)Const::tsLength / Const::segmentNum; // n / w

    return (distancef[0]+distancef[4])*frontCoef ;
}

double SaxUtil::getMinDist1stLayer(const float *paa, int id){
    double ret = 0;
    double frontCoef = (double)Const::tsLength / Const::segmentNum; // n / w
    for(int i=Const::segmentNum-1;i>=0;--i){
        if(id %2 == 1){
            if(paa[i] <0)
                ret += (paa[i] * paa[i]);
        }else{
            if(paa[i] > 0)
                ret += (paa[i] * paa[i]);
        }
        id >>=1;
    }
    return frontCoef * ret;
}

//double SaxUtil::LowerBound_Paa_iSax(const double *paa, const int *sax, const int* bits_cardinality) {
//    double frontCoef = (double)Const::tsLength / Const::segmentNum; // n / w
//    double paaValue, lb, ub;
//    double sum = 0;
//
//    for (int i = 0; i < Const::segmentNum; i++) {
//        paaValue = paa[i];
//        if(bits_cardinality[i] == 0)    continue;
//        getValueRange(sax[i], bits_cardinality[i], &lb, &ub);
//
//        if(paaValue < lb){
//            sum += (lb - paaValue) * (lb - paaValue);
//        }
//        else if(paaValue > ub){
//            sum += (paaValue - ub) * (paaValue - ub);
//        }
//    } // for
//
//    return frontCoef * sum;
//}

double SaxUtil::LowerBound_Paa_iSax(const float *paa, const unsigned short *sax, const int* bits_cardinality) {
    double frontCoef = (double)Const::tsLength / Const::segmentNum; // n / w
    double paaValue, lb, ub;
    double sum = 0;

    for (int i = 0; i < Const::segmentNum; i++) {
        paaValue = paa[i];
        if(bits_cardinality[i] == 0)    continue;
        getValueRange(sax[i], bits_cardinality[i], &lb, &ub);

        if(paaValue < lb){
            sum += (lb - paaValue) * (lb - paaValue);
        }
        else if(paaValue > ub){
            sum += (paaValue - ub) * (paaValue - ub);
        }
    } // for

    return frontCoef * sum;
}

double SaxUtil::getMinDist1stLayerDTW(const double *paaU, const double *paaL, int id){
    double breakpoint_lower, breakpoint_upper;
    double distance = 0;
    for(int i=Const::segmentNum-1;i>=0;--i){
        if(id %2 == 1){
            breakpoint_lower = 0;
            breakpoint_upper = 999;
        }else{
            breakpoint_upper = 0;
            breakpoint_lower = -999;
        }
        id >>=1;

        if (breakpoint_lower > paaU[i]) {
            distance += ((breakpoint_lower - paaU[i]) * (breakpoint_lower - paaU[i]));
        }
        else if(breakpoint_upper < paaL[i]) {
            distance += ((breakpoint_upper - paaL[i]) * (breakpoint_upper - paaL[i]));
        }

    }
    return Const::tsLengthPerSegment * distance;
}

double SaxUtil::minidist_paa_to_isax_DTW(const double *paaU, const double *paaL, const unsigned short *sax,
                                         const int* bits_cardinality, vector<int>&chosen_segs, int new_id){
    double distance = 0;
    double breakpoint_lower, breakpoint_upper;
    int saxValue, cur = chosen_segs.size() - 1, bc;

    for (int i = Const::segmentNum-1; i >= 0; --i) {
        if(cur >=0 && chosen_segs[cur] == i){
            saxValue = (sax[i] << 1) + (new_id % 2);
            new_id >>= 1;
            bc = bits_cardinality[i] + 1;
            --cur;
        } else {
            saxValue = sax[i];
            bc = bits_cardinality[i];
        }
        getValueRange(saxValue, bc, &breakpoint_lower, &breakpoint_upper);

        if (breakpoint_lower > paaU[i]) {
            distance += ((breakpoint_lower - paaU[i]) * (breakpoint_lower - paaU[i]));
        }
        else if(breakpoint_upper < paaL[i]) {
            distance += ((breakpoint_upper - paaL[i]) * (breakpoint_upper - paaL[i]));
        }
    } // for

    return Const::tsLengthPerSegment * distance;
}

double SaxUtil::minidist_paa_to_isax_DTW(const double *paaU, const double *paaL ,  const unsigned short *sax,
                               const int*sax_cardinalities)
{

    double distance = 0;
    double breakpoint_lower, breakpoint_upper;
    // For each sax record find the break point
    int i;
    for (i=0; i<Const::segmentNum; i++) {

        if(sax_cardinalities[i] == 0)    continue;
        getValueRange(sax[i], sax_cardinalities[i], &breakpoint_lower, &breakpoint_upper);


        if (breakpoint_lower > paaU[i]) {
            distance += ((breakpoint_lower - paaU[i]) * (breakpoint_lower - paaU[i]));
        }
        else if(breakpoint_upper < paaL[i]) {
            distance += ((breakpoint_upper - paaL[i]) * (breakpoint_upper - paaL[i]));
        }
        //        else {
        //            printf("%lf is between: %lf and %lf\n", paa[i], breakpoint_lower, breakpoint_upper);
        //        }
    }

    //distance = ratio_sqrt * sqrtf(distance);
    distance = Const::tsLengthPerSegment * distance;
    return distance;
}

double SaxUtil::minidist_paa_to_isax_DTW(const double *paaU, const double *paaL ,  const unsigned short *sax,
                                         int bitsCardinality)
{

    double distance = 0;
    double breakpoint_lower, breakpoint_upper;
    // For each sax record find the break point
    int i;
    for (i=0; i<Const::segmentNum; i++) {

        if(bitsCardinality == 0)    continue;
        getValueRange(sax[i], bitsCardinality, &breakpoint_lower, &breakpoint_upper);


        if (breakpoint_lower > paaU[i]) {
            distance += ((breakpoint_lower - paaU[i]) * (breakpoint_lower - paaU[i]));
        }
        else if(breakpoint_upper < paaL[i]) {
            distance += ((breakpoint_upper - paaL[i]) * (breakpoint_upper - paaL[i]));
        }
        //        else {
        //            printf("%lf is between: %lf and %lf\n", paa[i], breakpoint_lower, breakpoint_upper);
        //        }
    }

    //distance = ratio_sqrt * sqrtf(distance);
    distance = Const::tsLengthPerSegment * distance;
    return distance;
}

void SaxUtil::getValueRange(int sax_single, int bits_cardinality, double *lb, double *ub){
    int cardinality = 1 << bits_cardinality;
    int offset = ((cardinality - 1) * (cardinality - 2)) / 2;
    if(sax_single == 0){
        *lb = -numeric_limits<double>::max();
        *ub = breakpoints[offset];
    }else if(sax_single == cardinality - 1){
        *lb = breakpoints[offset + sax_single - 1];
        *ub = numeric_limits<double>::max();
    }else{
        *lb = breakpoints[offset + sax_single - 1];
        *ub = breakpoints[offset + sax_single];
    }
}

// return the new id
int SaxUtil::extendSax(float *paa, const int *bits_cardinality, vector<int> &segments) {
    int res = 0, cardinality, sw, offset;
    for(int segment:segments){
        cardinality = 1 << (bits_cardinality[segment] + 1);
        offset = ((cardinality - 1) * (cardinality - 2)) / 2;
        int index = findFirstGE(breakpoints, offset, cardinality -1, paa[segment]);
        if(index >= 0)
            sw = (index - offset);
        else
            cout<<"ERROR!!!!!!!";
        res = (res << 1) + (sw % 2);
    }
    return res;
}

// return the new id
int SaxUtil::extendSax(const unsigned short *sax, const int *bits_cardinality, vector<int> &segments) {
    int res = 0, sw;
    for(int segment:segments){
        sw = sax[segment] >> (Const::bitsCardinality - bits_cardinality[segment] - 1);
        res = (res << 1) + (sw % 2);
    }
    return res;
}

// return the new id
int SaxUtil::extendSaxFuzzy(const unsigned short *sax, const unsigned short* node_sax, const int *bits_cardinality, vector<int> &segments) {
    int res = 0, sw, osw;
    for(int segment:segments){
        osw = sax[segment] >> (Const::bitsCardinality - bits_cardinality[segment]);
        if(node_sax[segment] < osw){
            res = (res << 1) + 1;
        }else if(node_sax[segment] > osw){
            res = (res << 1);
        }else{
            sw = sax[segment] >> (Const::bitsCardinality - bits_cardinality[segment] - 1);
            res = (res << 1) + (sw % 2);
        }
    }
    return res;
}


// return the new id
int SaxUtil::extendSax(const unsigned short *sax, const int *bits_cardinality, vector<int> &segments,
                       const unsigned short *parent_sax) {
    int res = 0, sw;
    for(int segment:segments){
        sw = sax[segment] >> (Const::bitsCardinality - bits_cardinality[segment] - 1);
        if((sw >> 1) == parent_sax[segment])    res = (res << 1) + (sw % 2);
        else if((sw >> 1) > parent_sax[segment])    res = (res << 1) + 1;
        else res <<= 1;
    }
    return res;
}

// return the new id
int SaxUtil::extendSax(const unsigned short *sax, const int *bits_cardinality) {
    int res = 0, sw;
    for(int segment = 0;segment < Const::segmentNum; ++segment){
        sw = sax[segment] >> (Const::bitsCardinality - bits_cardinality[segment] - 1);
        res = (res << 1) + (sw % 2);
    }
    return res;
}

void SaxUtil::extendSax(const unsigned short * parent_sax, const int*parent_bits_cardinality,
                          vector<int> &chosen_segs, int new_id,
               unsigned short* sax, int *bits_cardinality) {
    int cur = chosen_segs.size() - 1;

    for (int i = Const::segmentNum - 1; i >= 0; --i) {
        if (cur >= 0 && chosen_segs[cur] == i) {
            sax[i] = (parent_sax[i] << 1) + (new_id % 2);
            new_id >>= 1;
            bits_cardinality[i] = parent_bits_cardinality[i] + 1;
            --cur;
        } else {
            sax[i] = parent_sax[i];
            bits_cardinality[i] = parent_bits_cardinality[i];
        }
    }
}

int SaxUtil::getNewId(const float *paa, const float *split_line){
    int ret = 0;
    for(int i=0;i<Const::segmentNum;++i){
        ret <<= 1;
        if(paa[i] > split_line[i])  ret += 1;
    }
    return  ret;
}

int SaxUtil::getNewId(const float *paa, const float *split_line, vector<int>&segments){
    int ret = 0;
    for(int i:segments){
        ret <<= 1;
        if(paa[i] > split_line[i])  ret += 1;
    }
    return  ret;
}

void SaxUtil::saxPrint(int* sax, int bits_cardinality, int segment_num)
{
    int i;
    for (i=0; i < segment_num; i++) {
        cout<<i << "\t";
        printBinary(sax[i], bits_cardinality);
        cout <<"\t" << sax[i];
        cout<<endl;
    }
    cout<<endl;
}

void SaxUtil::invSaxPrintDec(int *invsax, int bc){
    for(int i=0;i<bc;++i){
        cout << i << '\t' << invsax[i] <<endl;
    }
    cout << endl;
}

void SaxUtil::printBinary(long n, int size) {
    char b[size + 1];
    b[size] = 0;
    int i;
    for (i=0; i<size; i++) {
        b[i] = '0';
    }

    for (i=0; i<size; i++, n=n/2)
        if (n%2 == 1) b[size-1-i] = '1';

    cout<<b;
}

void SaxUtil::generateSaxFile(const string &fn , const string &output){
    long fs = FileUtil::getFileSize(fn.c_str());
    int num = fs / Const::tsLengthBytes;
    cout << "Total Number is "<< num <<endl;
    float ts[Const::tsLength];
    unsigned short sax[Const::segmentNum];
    FILE *f = fopen(fn.c_str(), "rb");
    FILE *of = fopen(output.c_str(), "wb");
    for(int i=0;i<num;++i){
        // if(i % 1000000 == 0)    cout << i << endl;
        fread(ts, sizeof(float ), Const::tsLength, f);
        if(isnan(ts[0])){
            printf("ts is nan\n");
            exit(-1);
            // for(auto &t:sax)    t = 0;
            // cout << i << "," <<endl;
        }
        else    saxFromTs(ts, sax, Const::tsLengthPerSegment, Const::segmentNum, Const::cardinality);
        fwrite(sax, sizeof(unsigned short), Const::segmentNum, of);
    }
    fclose(f);
    fclose(of);
}

void SaxUtil::generatePaaFile(const string &fn, const string &output) {
    long fs = FileUtil::getFileSize(fn.c_str());
    int num = fs / Const::tsLengthBytes;
    cout << "Total number is " << num << endl;
    float ts[Const::tsLength];
    float paa[Const::segmentNum];
    FILE *f = fopen(fn.c_str(), "rb");
    FILE *of = fopen(output.c_str(), "wb");
    float paa_max[Const::segmentNum];
    fill_n(paa_max, Const::segmentNum, 0);
    for(int i=0;i<num;++i){
        // if(i % 1000000 == 0)    cout << i << endl;
        fread(ts, sizeof(float ), Const::tsLength, f);
        if(isnan(ts[0])){
            for(auto &t:paa)    t = 0;
            // cout << i << "," <<endl;
        }
        else    paaFromTs(ts, paa, Const::tsLengthPerSegment, Const::segmentNum);
        for(int j=0;j<Const::segmentNum;++j)
            paa_max[j] = max(paa_max[j], paa[j]);
        fwrite(paa, sizeof(float ), Const::segmentNum, of);
    }
    fclose(f);
    fclose(of);
}

#define max(x,y) ((x)>(y)?(x):(y))
#define dist(x,y) (((x)-(y))*((x)-(y)))
void init(dequeue *d, int capacity)
{
    d->capacity = capacity;
    d->size = 0;
    d->dq = (int *) malloc(sizeof(int)*d->capacity);
    d->f = 0;
    d->r = d->capacity-1;
}

/// Insert to the queue at the back
void push_back(struct dequeue *d, int v)
{
    d->dq[d->r] = v;
    d->r--;
    if (d->r < 0)
        d->r = d->capacity-1;
    d->size++;
}

/// Delete the current (front) element from queue
void pop_front(struct dequeue *d)
{
    d->f--;
    if (d->f < 0)
        d->f = d->capacity-1;
    d->size--;
}

/// Delete the last element from queue
void pop_back(struct dequeue *d)
{
    d->r = (d->r+1)%d->capacity;
    d->size--;
}

/// Get the value at the current position of the circular queue
int front(struct dequeue *d)
{
    int aux = d->f - 1;

    if (aux < 0)
        aux = d->capacity-1;
    return d->dq[aux];
}

/// Get the value at the last position of the circular queueint back(struct dequeue *d)
int back(struct dequeue *d)
{
    int aux = (d->r+1)%d->capacity;
    return d->dq[aux];
}

/// Check whether or not the queue is empty
int empty(struct dequeue *d)
{
    return d->size == 0;
}

/// Destroy the queue
void destroy(dequeue *d)
{
    free(d->dq);
}

void SaxUtil::lower_upper_lemire(const float *t, int len, int r, float *l, float *u)
{
    dequeue du, dl;

    init(&du, 2*r+2);
    init(&dl, 2*r+2);

    push_back(&du, 0);
    push_back(&dl, 0);
    int i;

    for (i = 1; i < len; i++)
    {
        if (i > r)
        {
            u[i-r-1] = t[front(&du)];
            l[i-r-1] = t[front(&dl)];
        }
        if (t[i] > t[i-1])
        {
            pop_back(&du);
            while (!empty(&du) && t[i] > t[back(&du)])
                pop_back(&du);
        }
        else
        {
            pop_back(&dl);
            while (!empty(&dl) && t[i] < t[back(&dl)])
                pop_back(&dl);
        }
        push_back(&du, i);
        push_back(&dl, i);
        if (i == 2 * r + 1 + front(&du))
            pop_front(&du);
        else if (i == 2 * r + 1 + front(&dl))
            pop_front(&dl);
    }
    for (i= len; i < len+r+1; i++)
    {
        u[i-r-1] = t[front(&du)];
        l[i-r-1] = t[front(&dl)];
        if (i-front(&du) >= 2 * r + 1)
            pop_front(&du);
        if (i-front(&dl) >= 2 * r + 1)
            pop_front(&dl);
    }
    destroy(&du);
    destroy(&dl);

}