//////////////////////////////////////////////////////////
//                                                      //
//     Shared Nearest Neighbor based Clustering         //
//     Author          : Levent Ertöz                   //
//     Project Members : Michael Steinbach              //
//                       Prof. Vipin Kumar              //
//     Last Updated    : Apr 3rd, 02                    //
//     Contact info    : ertoz@cs.umn.edu               //
//                                                      //
//////////////////////////////////////////////////////////

// modifiquei a distancia euclideana para retornar o valor, e nao o quadrado do valor da distancia

#include <strstream>
//#include <sstream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
#include <cmath>
#include <string.h>
#include <stdio.h>
#include <queue>
#include <algorithm>
#include <ctime>
#include <fstream>

#include "boost/tokenizer.hpp"

//#define real float
#define real double
#define MAXLINE 300000//1024*32
#define INF 1e20

int ncol,nrow,nnzero,co_occurance=0;

class disjoint_set {
private:
    int *root,*set,n;
public:
    int nc,*size,*rank;
    disjoint_set(int x):n(x) {
        set=new int[n];
        rank=new int[n];
        root=new int[n];
        size=new int[n];
        nc=n;
        for (int i=0;i<n;++i) makeset(i);
    }

    void makeset(int i) {set[i]=i; rank[i]=0; size[i]=1;}
    void join(int index1,int index2) {
        if(findset(index1)!=findset(index2)) {
            nc-=1;
            link(findset(index1),findset(index2));
        }
    }

    void link(int i,int j) {
        if(rank[i]>rank[j]) {set[j]=i; size[i]+=size[j];}
        else                {set[i]=j; size[j]+=size[i];}
        if(rank[i]==rank[j]) rank[j]+=1;
    }

    int findset(int i) {
        if(set[i]!=i) set[i]=findset(set[i]);
        return set[i];
    }
};

class compare {public: bool operator() (std::pair<real,int> a, std::pair<real,int> b) {return a.first > b.first;}};
bool compare_id  (std::pair<int,real> a, std::pair<int,real> b) {return a.first<b.first;}
bool compare_id2 (std::pair<int,int>a, std::pair<int,int>b)     {return a.first<b.first;}
bool compare_sim (std::pair<int,int>a, std::pair<int,int>b)     {return a.second>b.second;}
bool compare_sim2(std::pair<int,real>a, std::pair<int,real>b)   {return a.second>b.second;}

template <class A>
A abs(A a){return (a<0) ? -a : a;}

real dot(int i, int j, int *rowptr, int *colindex, real *value){
    real sum=0;
    int  *p,*q,*r,*s;
    real *pp,*qq;
    p = colindex + rowptr[i]; r = colindex + rowptr[i+1];
    q = colindex + rowptr[j]; s = colindex + rowptr[j+1];
    pp= value    + rowptr[i];
    qq= value    + rowptr[j];
    while(p<r && q<s){
        if(*p == *q) {sum += *pp * *qq; ++p; ++q; ++pp; ++qq;}
        else {
            if(*p < *q) {++p; ++pp;}
            else        {++q; ++qq;}
        }
    }
    return sum;
}

real dot_dense(int i, int j, int *rowptr, int *colindex, real *value){
    real *p=value+rowptr[i], *q=value+rowptr[j], *end=p+ncol, sum=0;
    while(p<end) {sum+= *p * *q; ++p; ++q;}
    return sum;
}

real cosine(int i, int j, int *rowptr, int *colindex, real *value){
    int *p,*q,*r,*s;
    real  *pp,*qq,norm1,norm2,sum;
    sum=norm1=norm2=0;
    p = colindex + rowptr[i]; r = colindex + rowptr[i+1];
    q = colindex + rowptr[j]; s = colindex + rowptr[j+1];
    pp= value    + rowptr[i];
    qq= value    + rowptr[j];
    while(p<r && q<s){
        if(*p == *q) {sum += *pp * *qq; norm1+=*pp * *pp; norm2+=*qq * *qq; ++p; ++q; ++pp; ++qq;}
        else {
            if(*p < *q) {norm1 += *pp * *pp; ++p; ++pp;}
            else        {norm2 += *qq * *qq; ++q; ++qq;}
        }
    }
    if(p==r) {while(q!=s) {norm2 += *qq * *qq; ++q; ++qq;}}
    else     {while(p!=r) {norm1 += *pp * *pp; ++p; ++pp;}}
    return sum/sqrt(norm1*norm2);
}

real cosine_dense(int i, int j, int *rowptr, int *colindex, real *value){
    real *p=value+rowptr[i], *q=value+rowptr[j], *end=p+ncol, sum=0, norm1=0, norm2=0;
    while(p<end) {sum+= *p * *q; norm1+= *p * *p; norm2+=*q * *q; ++p; ++q;}
    return sum/sqrt(norm1*norm2);
}

// works only in sparse format
real jaccard(int i, int j, int *rowptr, int *colindex, real *value){
    int *p,*q,*r,*s,in,un;
    in=un=0;
    p = colindex + rowptr[i]; r = colindex + rowptr[i+1];
    q = colindex + rowptr[j]; s = colindex + rowptr[j+1];
    while(p<r && q<s){
        if(*p == *q) {++in; ++p; ++q;}
        else {
            ++un;
            if(*p < *q) {++p;}
            else        {++q;}
        }
    }
    return real(in)/real(in+un);
}

real euclidean(int i, int j, int *rowptr, int *colindex, real *value){
    int *p,*q,*r,*s;
    real  *pp,*qq,sum=0;
    p = colindex + rowptr[i]; r = colindex + rowptr[i+1];
    q = colindex + rowptr[j]; s = colindex + rowptr[j+1];
    pp= value    + rowptr[i];
    qq= value    + rowptr[j];
    while(p<r && q<s){
        if(*p == *q) {sum -= (*pp - *qq)*(*pp - *qq); ++p; ++q; ++pp; ++qq;}
        else {
            if(*p < *q) {sum -= *pp * *pp; ++p; ++pp;}
            else        {sum -= *qq * *qq; ++q; ++qq;}
        }
    }
    //return -sqrt(-sum);
    return sum;
}

real euclidean_dense(int i, int j, int *rowptr, int *colindex, real *value){
    real *p=value+rowptr[i], *q=value+rowptr[j], *end=p+ncol, sum=0;
    while(p<end) {sum -= (*p - *q)*(*p - *q); ++p; ++q;}
    //return -sqrt(-sum);
    return sum;
}

void normalize_stdev_col(int *rowptr, int *colindex, real *value){
// works only for full matrices
    int i;
    real *mean, *stdev, *min, *max, *range;
    mean = new real[ncol];
    stdev= new real[ncol];
    max  = new real[ncol];
    min  = new real[ncol];
    range= new real[ncol];
    for(i=0;i<ncol;++i)   {mean[i]=stdev[i]=max[i]=min[i]=range[i]=0;}
    for(i=0;i<nnzero;++i) {
        mean[i%ncol]+=  value[i];
        min [i%ncol] = (value[i] < min[i%ncol]) ? value[i] : min[i%ncol];
        max [i%ncol] = (value[i] > max[i%ncol]) ? value[i] : max[i%ncol];
    }
    for(i=0;i<ncol;++i)   {mean[i]/=real(nrow); range[i]=max[i]-min[i];}
    for(i=0;i<nnzero;++i) {value[i]-=mean[i%ncol]; stdev[i%ncol]+=value[i]*value[i];}
    for(i=0;i<ncol;++i)   stdev[i]=sqrt(stdev[i]/real(nrow));
    for(i=0;i<nnzero;++i) {
        if(range[i%ncol]==0) {value[i]=0; continue;}
        if(abs(stdev[i%ncol]/range[i%ncol])<0.001) {value[i]=0; continue;}
        value[i]/=stdev[i%ncol];
    }
}

void normalize_stdev_row(int *rowptr, int *colindex, real *value){
// works only for full matrices
// doesn't normalize by stdev but stdev*n^.5 (this is followed by 'dot' similarity function to get correlation coef.)
    int i;
    real *mean,*stdev, *min, *max, *range;
    mean = new real[nrow];
    stdev= new real[nrow];
    max  = new real[nrow];
    min  = new real[nrow];
    range= new real[nrow];
    for(i=0;i<nrow;++i)   {mean[i]=stdev[i]=max[i]=min[i]=range[i]=0;}
    for(i=0;i<nnzero;++i) {
        mean[i/ncol]+=value[i];
        min[i/ncol]  = (value[i] < min[i/ncol]) ? value[i] : min[i/ncol];
        max[i/ncol]  = (value[i] > max[i/ncol]) ? value[i] : max[i/ncol];
    }
    for(i=0;i<nrow;++i)   {mean[i]/=real(ncol); range[i]=max[i]-min[i];}
    for(i=0;i<nnzero;++i) {value[i]-=mean[i/ncol]; stdev[i/ncol]+=value[i]*value[i];}
    for(i=0;i<nrow;++i)   stdev[i]=sqrt(stdev[i]/real(nrow));
    for(i=0;i<nnzero;++i) {
        if(range[i/ncol]==0) {value[i]=0; continue;}
        if(abs(stdev[i/ncol]/range[i/ncol])<0.001) {value[i]=0; continue;}
        value[i]/=(stdev[i/ncol]*sqrt(real(nrow)));
    }
}

void read_input_dense(std::string dataset, int*& rowptr, int*& colindex, real*& value, int*& idf, std::vector<std::string> &names){
    int i;
    real x;
    char buf[MAXLINE];
    std::string line, s;
    std::vector<std::string> header;
    
    std::vector<std::vector<real> > pat;
    std::vector<real> row;

    std::ifstream file(dataset.c_str());
    if(!file) {std::cerr << "could not open input file '" << dataset << "'\n";      exit(0);}


    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep("\t ");

    // carrega dataset
    
    //std::cout << "Reading input file." << std::endl;
    file.getline(buf, MAXLINE);
    line = buf;
    tokenizer tokens(line, sep);
    for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
    {
        //std::cout << *tok_iter << " ";// << std::endl;
        header.insert(header.end(), *tok_iter);
    }

    while (!file.eof())
    {
        file.getline(buf, MAXLINE);
        line = buf;        
        //std::cout << line.size() << std::endl;
        tokens.assign(line, sep);

        if (!file.fail())
        {
            for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
            {
                if (tok_iter == tokens.begin())
                {
                   //std::cout << "ID: " << identifier << std::endl;              
                   names.push_back(*tok_iter);
                   //std::cout << *tok_iter << std::endl;
                }
                else
                {
                    s = *tok_iter; 
                    row.push_back(atof(s.c_str()));
                    //std::cout << s << std::endl;
                    //pat[identifier].insert(pat[identifier].end(), atof(s.c_str())); 
                }            
            }
            pat.push_back(row);
            row.clear();
        }
    }
    file.close();

    ncol = header.size() - 1; // -1 para tirar o rotulo das instancias
    nrow = pat.size(); 
    nnzero=nrow*ncol;
   
    //std::cout << nrow << " " << ncol << " " << nnzero << std::endl;

    rowptr   = new int[nrow+1];
    colindex = new int[nnzero];
    value    = new real [nnzero];

    idf=new int[ncol];
    for(i=0;i<ncol;++i) idf[i]=0;

    rowptr[0]=0;
    for(i=0;i<ncol;++i) idf[i]=nrow;
    for(i=1;i<=nrow;++i) rowptr[i]=i*ncol;
    int r = 0;
    for(i=0;i<nnzero;++i)
    {
        //in >> x;
        if ((i != 0) && (i%ncol == 0)) r++; 
        colindex[i]=i%ncol;
        value[i]=pat[r][colindex[i]];
//        std::cout << "i: " << i << " r: " << r 
//                  << " colindex[i]: " << colindex[i] 
//                  << " value[i]: " << value[i] << std::endl;
    }    
}

void read_input_csr(int *rowptr, int *colindex, real *value, std::istream &in, int *idf){
    int i,j;
    real x;
    for(i=0;i<=nrow;++i)  {in >> j; rowptr[i]=--j;}
    for(i=0;i<nnzero;++i) {in >> j; colindex[i]=--j; idf[j]++;}
    for(i=0;i<nnzero;++i) {in >> x; value[i]=x;}
}

void read_input_mat(int *rowptr, int *colindex, real *value, std::istream &in, int *idf){
    using namespace std;
    int i,size=0,numlines=0;
    real x;
    char buf[MAXLINE];
    rowptr[0]=0;
    vector< pair<int,real> > row;

    while(1){
        strstream st;
        in.getline(buf,MAXLINE,'\n');
        if(in.fail() && numlines<nrow) {cerr << "file has a longer line than MAXLINE. line #" << numlines+2 << "\n"; exit(0);}
        if(!in || numlines==nrow) break;
        st << buf;
        while(1){
            st >> i >> x;
            if(!st) break;
            row.push_back(pair<int,real>(--i,x));
        }
        sort(row.begin(),row.end(),compare_id);
        st.clear();
        for(i=0;i<row.size();++i){colindex[size]=row[i].first; value[size]=row[i].second; ++size; idf[row[i].first]++;}
        row.clear();
        ++numlines;
        if(numlines>nrow) break;
        rowptr[numlines]=size;
        if(numlines%250==0) cout << numlines << " lines read\n";
    }
    if(numlines!=nrow) {cerr << "Warning: number of rows specified does not match number of rows read\n"; cout << numlines << " " << nrow << endl; exit(0);}
}

void read_input_coor(int *rowptr, int *colindex, real *value, std::istream &in, int *idf){
    using namespace std;
    int i,j,size=0;
    real x;
    vector < vector< pair<int,real> > > temp_data(nrow);
    while(1){
        in >> i >> j >> x;
        if(!in) break;
        i--; j--;
        temp_data[i].push_back(pair<int,real>(j,x));
    }
    rowptr[0]=0;
    for(i=0;i<nrow;++i){
        sort(temp_data[i].begin(),temp_data[i].end(),compare_id);
        for(j=0;j<temp_data[i].size();++j) {
            colindex[size] = temp_data[i][j].first;
            value[size]    = temp_data[i][j].second; 
            ++size; 
            idf[temp_data[i][j].first]++;
        }
        rowptr[i+1]=size;
    }
}

void write_output_csr  (int *rowptr, int *colindex, real *value, std::string data){
    using namespace std;
    string fname=data+".csr";
    ifstream check;
    ofstream out;
    check.open(fname.c_str());
    if(check) {cerr << "File :" << fname << " already exists\n"; check.close(); return;}
    out.open(fname.c_str());
    out << nrow << " " << ncol << " " << nnzero << endl;
    int i;
    for(i=0;i<=nrow; ++i) out << rowptr[i]+1   << " "; out << endl;
    for(i=0;i<nnzero;++i) out << colindex[i]+1 << " "; out << endl;
    for(i=0;i<nnzero;++i) out << value[i]      << " "; out << endl;
    out.close();
}

void write_output_mat  (int *rowptr, int *colindex, real *value, std::string data){
    using namespace std;
    string fname=data+".mat";
    ifstream check;
    ofstream out;
    check.open(fname.c_str());
    if(check) {cerr << "File :" << fname << " already exists\n"; check.close(); return;}
    out.open(fname.c_str());
    out << nrow << " " << ncol << " " << nnzero << endl;
    int i,j;
    for(i=0;i<nrow;++i){
        for(j=rowptr[i];j<rowptr[i+1];++j) out << colindex[j]+1 << " " << value[j] << " ";
        out << endl;
    }
    out.close();
}
void write_output_coor (int *rowptr, int *colindex, real *value, std::string data){
    using namespace std;
    string fname=data+".coor";
    ifstream check;
    ofstream out;
    check.open(fname.c_str());
    if(check) {cerr << "File :" << fname << " already exists\n"; check.close(); return;}
    out.open(fname.c_str());
    out << nrow << " " << ncol << " " << nnzero << endl;
    int i,j;
    for(i=0;i<nrow;++i){
        for(j=rowptr[i];j<rowptr[i+1];++j) out << i+1 << " " << colindex[j]+1 << " " << value[j] << "\n";
    }
    out.close();
}

void write_output_dense (int *rowptr, int *colindex, real *value, std::string data){
    using namespace std;
    string fname=data+".dense";
    ifstream check;
    ofstream out;
    check.open(fname.c_str());
    if(check) {cerr << "File :" << fname << " already exists\n"; check.close(); return;}
    out.open(fname.c_str());
    out << nrow << " " << ncol << endl;
    int i,j;
    for(i=0;i<nrow;++i){
        for(j=0;j<ncol;++j) out << value[i*ncol+j] << " ";
        out << endl;
    }
    out.close();
}

void inverted_index(int *rowptr, int *colindex, int *rowptr2, int *colindex2){
    int i,j;
    for(i=0;i<nrow;++i){
        for(j=rowptr[i];j<rowptr[i+1];++j){
            colindex2[rowptr2[colindex[j]]]=i;
            rowptr2[colindex[j]]++;
        }
    }
    for(i=ncol;i>0;i--) rowptr2[i]=rowptr2[i-1];
    rowptr2[0]=0;
}

void transpose_csr(int *rowptr, int *colindex, real *value, int *rowptr2, int *colindex2, real *value2){
    int i,j;
    for(i=0;i<nrow;++i){
        for(j=rowptr[i];j<rowptr[i+1];++j){
            colindex2[rowptr2[colindex[j]]]=i;
            value2[rowptr2[colindex[j]]]=value[j];
            rowptr2[colindex[j]]++;
        }
    }
    for(i=ncol;i>0;i--) rowptr2[i]=rowptr2[i-1];
    rowptr2[0]=0;
}

void get_list(int p, bool *mask, int *rowptr, int *colindex, int *rowptr2, int *colindex2){
    int *start = colindex + rowptr[p], *end = colindex + rowptr[p+1], *s, *f;
    while(start<end){
        s = colindex2 + rowptr2[*start];
        f = colindex2 + rowptr2[*start + 1];
        while(s<f) {
            *(mask + *s) = 1;
            ++s;
        }
        ++start;
    }
}

void prune_features(int *rowptr, int *colindex, real *value, int prune_cl, int prune_ch, int *idf){
    int i,j,count=0,start_old=0;
    for(i=0;i<nrow;++i){
        for(j=start_old;j<rowptr[i+1];++j){
            if(idf[colindex[j]] < prune_cl || idf[colindex[j]] > prune_ch) continue;
            colindex[count] = colindex[j];
            value   [count] = value   [j];
            ++count;
        }
        start_old   = rowptr[i+1];
        rowptr[i+1] = count;
    }
    for(i=0;i<ncol;++i) {if(idf[i] < prune_cl || idf[i] > prune_ch) idf[i]=0;}
    std::cout << nnzero - count << " nonzero's removed\n";
    nnzero = count;
}

void prune_rows_weight(int *rowptr, int *colindex, real *value, int *idf, real f){
    using namespace std;
    int i,j,p,count=0,start_old=0;
    real sum,sum2;
    vector < pair<int, real> >row;
    for(i=0;i<nrow;++i){
        sum=0;
        for(j=start_old;j<rowptr[i+1];++j) {
            sum+= value[j] * value[j];
            row.push_back(pair<int,real>(colindex[j],value[j]));
        }
        if(sum){
            sum *= f*f;
            sort(row.begin(),row.end(),compare_sim2);
            sum2=0;
            for(j=0;j<row.size();++j){
                sum2 += row[j].second * row[j].second;
                if(sum2 >= sum) break;
            }
            ++j;
            sort(row.begin(),row.begin()+j,compare_id);
            for(p=j;p<row.size();++p) idf[row[p].first]--;
            for(p=0;p<j;++p){
                colindex[count] = row[p].first;
                value   [count] = row[p].second;
                ++count;
            }
        }
        start_old   = rowptr[i+1];
        rowptr[i+1] = count;
        row.clear();
    }
    cout << nnzero - count << " nonzero's that don't contribute much to the norm removed\n";
    nnzero = count;
}

void prune_rows_length(int *rowptr, int *colindex, real *value, int prune_rl, int prune_rh, int *idf){
    int i,j,start_old=0,start_new=0,count_l=0,count_h=0;
    for(i=0;i<nrow;++i){
        if(rowptr[i+1]-start_old<prune_rl || rowptr[i+1]-start_old>prune_rh) {
            for(j=start_old;j<rowptr[i+1];++j) idf[colindex[j]]--;
            start_old=rowptr[i+1];
            rowptr[i+1]=rowptr[i]; 
            if(rowptr[i+1]-start_old<prune_rl) ++count_l;
            else ++count_h;
        }
        else {
            for(j=start_old;j<rowptr[i+1];++j){
                colindex[start_new+j-start_old] = colindex[j];
                value   [start_new+j-start_old] = value   [j];
            }
            start_new += rowptr[i+1] - start_old;
            start_old  = rowptr[i+1];
            rowptr[i+1]= start_new;
        }
    }
    nnzero = rowptr[nrow];
    std::cout << "pruned " << count_l << " rows with very few attributes\n";
    std::cout << "pruned " << count_h << " rows with very many attributes\n";
}

void help(){
    using namespace std;
    cout << "Usage: snn datafile dataformat similarity-function [options]\n";
    cout << "datafile               : ascii file containing the data. the first row is always 'nrows ncols # of non-zeros'\n";
    cout << "format                 : 0-csr 1-mat 2-coordinate 3-dense\n";
    cout << "similarity function    : 0-dot 1-cosine 2-jaccard 3-euclidean 4-correlation coef. (dense format only)\n";
    cout << "-----------------------------------------------------------------------------------------------------\n";
    cout << "options :\n";
    cout << "-rlabel [filename]     : class labels for rows (colums if transposed)\n";
    cout << "-rname  [filename]     : row names\n";
    cout << "-out    [filename]     : output file name\n";
    cout << "-t                     : transposed\n";
    cout << "-rn                    : unit row normalization\n";
    cout << "-cn [1,2]              : column normalization\n";
    cout << "                         1-tfidf 2-normalize by st.dev (dense format only)\n";
    cout << "-c  [0,1,2,3]          : convert the data to (0-csr 1-mat 2-coordinate 3-dense) and exit\n";
    cout << "-prune_rl [support]    : prune rows with very few  attributes\n";
    cout << "-prune_rh [support]    : prune rows with very many attributes\n";
    cout << "-prune_cl [support]    : prune features with low  idf value\n";
    cout << "-prune_ch [support]    : prune features with high idf value\n";
    cout << "-prune_w  [float  ]    : prune features that don't contribute much to the norm\n";
    cout << "-dump    [filename]    : write the similarity matrix (nrow x NN)  to a file\n";
    cout << "-simfile [filename][M] : read similarity matrix      (nrow x M) from a file\n";
    cout << "----------------------------------------------------------------------------\n";
    cout << "if not specified, default values for the rest of the parameters will be used\n";
    cout << "----------------------------------------------------------------------------\n";
    cout << "-NN [int]              : near neighbor list size\n";
    cout << "-strong [float]        : % of weak links [0,1]\n";
    cout << "-topic  [float]        : % of representative points [0,1]\n";
    cout << "-noise  [float]        : % of noise points [0-1]\n";
    cout << "-merge  [float]        : % of links to be used for merging [0,1]\n";
    cout << "-label  [float]        : % of merge threshold for labeling background [0,1]\n";
    cout << "----------------------------------------------------------------------------\n";
}

int main(int argc, char **argv){
    using namespace std;
    int q;
    int i,j,k,p;    
    int *rowptr,*colindex,*idf;
    real  *value;
    real  sum;
    char buf[MAXLINE];
    strstream st;
    clock_t t0,t1,t2;
    
    vector<string> names, rowlabels, classnames;
    map<string,int> classes;


//  clustering related parameters
    real strong,topic,noise,merge_t,label;
    int   NN = 20;
    int  NNN;
    strong   = 0.25;        // # of weak links / total # of links
    topic    = real(0.10);  // # of topics     / total # of points
    noise    = real(0.20);  // percentage of noise points
    merge_t  = real(0.10);  // percentage of links to be used for merging
    label    = real(1000);  // label*merge threshold=label threshold ... no labeling by default

    if(argc<4) {help(); return 0;}

    string dataset,fname="",rname="",rlabel="",simfile="",dump="",dummy;
    dataset=argv[1];
    
    int format      = atoi(argv[2]);    // 0-csr 1-mat 2-coordinate (i-j-value) 3-dense
    int simfunc     = atoi(argv[3]);    // 0-dot 1-cosine 2-jaccard 3-euclidean 4-correlation(dense format only)

    real prune_w=0;                     // weight based pruning for rows
    int prune_rl=0;                     // low  support threshold for row length
    int prune_rh=0;                     // high support threshold for row length
    int prune_cl=0;                     // low  support threshold for idf
    int prune_ch=0;                     // high support threshold for idf
    int conversion=-1;                  // 0-csr 1-mat 2-coordinate 3-dense
    bool transposed=0;                  // 0-no  1-yes
    bool normr=0;                       // 0-no  1-yes
    int normc;                          // 0-no  1-tfidf 2-normalize by st.dev (dense format only)

    for(i=4;i<argc;++i){
        if     (!strcmp(argv[i],"-rlabel"  )) rlabel     = argv[++i];
        else if(!strcmp(argv[i],"-rname"   )) rname      = argv[++i];
        else if(!strcmp(argv[i],"-out"     )) fname      = argv[++i];
        else if(!strcmp(argv[i],"-rn"      )) normr      = 1;
        else if(!strcmp(argv[i],"-t"       )) transposed = 1;
        else if(!strcmp(argv[i],"-cn"      )) normc      = atoi(argv[++i]);
        else if(!strcmp(argv[i],"-NN"      )) NN         = atoi(argv[++i]);
        else if(!strcmp(argv[i],"-strong"  )) strong     = atof(argv[++i]);
        else if(!strcmp(argv[i],"-topic"   )) topic      = atof(argv[++i]);
        else if(!strcmp(argv[i],"-noise"   )) noise      = atof(argv[++i]);
        else if(!strcmp(argv[i],"-merge"   )) merge_t    = atof(argv[++i]);
        else if(!strcmp(argv[i],"-label"   )) label      = atof(argv[++i]);
        else if(!strcmp(argv[i],"-c"       )) conversion = atoi(argv[++i]);
        else if(!strcmp(argv[i],"-prune_rl")) prune_rl   = atoi(argv[++i]);
        else if(!strcmp(argv[i],"-prune_rh")) prune_rh   = atoi(argv[++i]);
        else if(!strcmp(argv[i],"-prune_cl")) prune_cl   = atoi(argv[++i]);
        else if(!strcmp(argv[i],"-prune_ch")) prune_ch   = atoi(argv[++i]);
        else if(!strcmp(argv[i],"-prune_w" )) prune_w    = atof(argv[++i]);
        else if(!strcmp(argv[i],"-dump"    )) dump       = argv[++i];
        else if(!strcmp(argv[i],"-simfile" )){simfile    = argv[++i]; NNN = atoi(argv[++i]);}
        else {cout << "argument not recognized " << argv[i] << endl; exit(0);}
        if(i==argc) {cout << "parameter to option not specified\n"; exit(0);}
    }

    if(normc==2   && format!=3) {cerr << "normalization by stdev. only works in dense format\n"; exit(0);}
    if(simfunc==4 && format!=3) {cerr << "correlation only works in dense format\n"; exit(0);}
    if(simfunc==2 && format==3) {cerr << "jaccard measure only works in sparse format\n"; exit(0);}

    if(fname=="") {fname=dataset;}
    fname+=".clu";

    // le os dados no formato denso
    read_input_dense(dataset,rowptr,colindex,value,idf,names);
    
//    std::cout << nrow << " " << ncol << " " << nnzero << std::endl;
//    cout << "input read\n";  
//    for(i=0;i<nnzero;++i)
//    {
//        std::cout << " value[i]: " << value[i] << std::endl;
//    }

//  read row labels (if specified)
    ifstream check;
//    check.open(fname.c_str());
//    if(check) {cerr << "Output file " << fname.c_str() << " already exists\n"; exit(0);}
//    check.close(); check.clear();
//
//    if(dump!=""){
//        check.open(dump.c_str());
//        if(check) {cerr << "Output file " << dump << " already exists\n"; exit(0);}
//        check.close();
//    }
//    check.clear();

    if(simfile!=""){
        if(NNN<NN) {cerr << "simfile contains fewer similarity values than needed\n"; exit(0);}
        check.open(simfile.c_str());
        if(!check) {cerr << "Simfile: " << simfile << " does not exist\n"; exit(0);}
        check.close();
    }
    check.clear();

    j = transposed ? ncol : nrow;
//    if(rname!="") {
//        check.open(rname.c_str());
//        if(!check) {cerr << rname << " does not exist\n\n"; exit(0);}
//        for(i=0;i<j;++i) {
//            check >> dummy;
//            names.push_back(dummy);
//            if(!check) {cerr << "fewer row names read than expected\n"; exit(0);}
//        }
//        check.close();
//        check.clear();
//    }
    string id;
    if(rlabel!="") {
        check.open(rlabel.c_str());
        if(!check) {cerr << rlabel << " does not exist\n\n"; exit(0);}
//        for(i=0;i<j;++i) {
//            check >> dummy;
        while (!check.eof())
        {
            check >> id >> dummy;
            if (!check.fail())
            {
                rowlabels.push_back(dummy);
                if(classes[dummy]==0) {classes[dummy]=classes.size(); classnames.push_back(dummy);}                
                //if(!check) {cerr << "fewer row labels read than expected\n"; exit(0);}
            }
        }
        check.close();
        check.clear();
    }


//    idf=new int[ncol];
//    for(i=0;i<ncol;++i) idf[i]=0;
//


//    switch (format){
//    case 1:  {dataset+=".mat";
//              std::ifstream in(dataset.c_str());
//              if(!in) {cerr << "could not open input file '" << dataset << "'\n";      exit(0);}
//              read_input_mat  (rowptr,colindex,value,in,idf); break;}
//    case 2:  {dataset+=".coor";
//              std::ifstream in(dataset.c_str());
//              if(!in) {cerr << "could not open input file '" << dataset << "'\n";      exit(0);}
//              read_input_coor (rowptr,colindex,value,in,idf); break;}
//    case 3:  //read_input_dense(rowptr,colindex,value,in,idf); 
//             break;
//    default: {dataset+=".csr";
//              std::ifstream in(dataset.c_str());
//              if(!in) {cerr << "could not open input file '" << dataset << "'\n";      exit(0);}
//              read_input_csr  (rowptr,colindex,value,in,idf); break;}
//    }
//    cout << "input read\n";

    if(transposed){
        int *rowptr2,*colindex2;
        real *value2;

        value2    = new real[nnzero];
        rowptr2   = new int[ncol+1];
        colindex2 = new int[nnzero];

        rowptr2[0]=0;
        for(i=0;i<ncol;++i) rowptr2[i+1]=idf[i]+rowptr2[i];

        delete[] idf;
        idf=new int[nrow];
        for(i=0;i<nrow;++i) idf[i]=rowptr[i+1]-rowptr[i];

        transpose_csr(rowptr, colindex, value, rowptr2, colindex2, value2);
        i=nrow; nrow=ncol; ncol=i;
        delete[] value;    value    = value2;
        delete[] rowptr;   rowptr   = rowptr2;
        delete[] colindex; colindex = colindex2;
        //cout << "transpose taken\n";
    }

// row pruning
    if(format!=3 && (prune_rl || prune_rh)) {cout << "pruning rows\n"; prune_rows_length(rowptr,colindex,value,prune_rl,prune_rh,idf);}
    else          if(prune_rl || prune_rh)  {cout << "cannot prune rows in dense matrices\n";}

// feature pruning
    if(format!=3 && (prune_cl || prune_ch)) {cout << "pruning features\n"; prune_features(rowptr,colindex,value,prune_cl,prune_ch,idf);}
    else          if(prune_cl || prune_ch)  {cout << "cannot prune features in dense matrices\n";}

// column normalization (tfidf)
    switch (normc){
    case 1: for(i=0;i<nnzero;++i) value[i]*=log(real(nrow)/real(idf[colindex[i]])); cout << "column normalization done\n"; break;
    case 2: normalize_stdev_col(rowptr,colindex,value);                             cout << "column normalization done\n"; break;
    default: break;
    }

// weight pruning
    if(prune_w) prune_rows_weight(rowptr, colindex, value, idf, prune_w);

// row normalization
    if(normr){
        for(i=0;i<nrow;++i){
            sum=0;
            for(j=rowptr[i];j<rowptr[i+1];++j) sum+=value[j]*value[j];
            sum=sqrt(sum);
            for(j=rowptr[i];j<rowptr[i+1];++j) value[j]/=sum;
        }
        //cout << "row normalization done\n";
    }

    switch (conversion){
    case -1: break;
    case 0 : write_output_csr  (rowptr,colindex,value,argv[1]); cout << "file converted\n"; return 1; break;
    case 1 : write_output_mat  (rowptr,colindex,value,argv[1]); cout << "file converted\n"; return 1; break;
    case 2 : write_output_coor (rowptr,colindex,value,argv[1]); cout << "file converted\n"; return 1; break;
    case 3 : write_output_dense(rowptr,colindex,value,argv[1]); cout << "file converted\n"; return 1; break;
    default: break;
    }

// calculate similarity matrix to construct nn graph
    vector < vector < pair <int,int> > > nn(nrow,vector < pair<int,int> >(NN)); // rowid,rank
    map<int,int> *where;
    where = new map<int,int>[nrow];

    if(simfile!=""){ // read similarity matrix to construct nn graph 
        ifstream sim(simfile.c_str());
        if(sim) cout << "reading similarity matrix\n"; else {cerr << "could not open simfile\n"; exit(0);}
        for(i=0;i<nrow;i++){
            k=0;
            for(j=0;j<NNN;j++){
                sim >> p;
                if(j>=NNN-NN) {
                    if(p!=i) {
                        nn[i][k]=pair<int,int>(p,k+1); // max value = NN
                        where[i][p]=k+1;
                    }
                    else nn[i][k]=pair<int,int>(p,0);
                    ++k;
                }
            }
            sort(nn[i].begin(),nn[i].end(),compare_id2);
        }
    }
    else { // calculate the similarity matrix

        real (*func) (int, int, int*, int*, real*);
        switch(simfunc){
        case 0: if(format==3) func=dot_dense;       else func=dot;              break;
        case 3: if(format==3) func=euclidean_dense; else func = euclidean;      break;
        case 1: if(normr){if(format==3) func=dot_dense;    else func=dot;}
                    else {if(format==3) func=cosine_dense; else func=cosine;}   break;
        case 2: func = jaccard;                                                 break;
        case 4: func = dot_dense; normalize_stdev_row(rowptr,colindex,value);   break;
        default:if(normr) func=dot; else func = cosine; break;
        }

        bool *mask;
        int *rowptr3,*colindex3;

        if(simfunc<3) {
            rowptr3   = new int [ncol+1];
            colindex3 = new int [nnzero];
            mask      = new bool[nrow];
            rowptr3[0]=0;
            for(i=0;i<ncol;++i) rowptr3[i+1]=idf[i]+rowptr3[i];
            for(i=0;i<nrow;++i) mask[i]=0;
            inverted_index(rowptr, colindex, rowptr3, colindex3);
        }
        //cout << "calculating similarity matrix\n";

        int buffsize=15,*writes;
        vector< priority_queue< pair<real,int> , vector< pair<real,int> > , compare> > Q(nrow);
        vector< pair< int , pair<real,int> > > buffer(buffsize*nrow);
        vector< pair< int , pair<real,int> > >::iterator buff_it;
        writes = new int[nrow];

        vector< pair<int,real> > current_doc;
        vector<int> simlist,simlist_full;
        vector<int>::iterator simlist_it;
        real norm;

        pair<real,int> temp(-real(INF),-1);
        if(simfunc==3) {for(i=0;i<nrow;++i) {writes[i]=0; for(j=0;j<NN;++j) Q[i].push(temp);}}
        else           {for(i=0;i<nrow;++i) {writes[i]=0; for(j=0;j<NN;++j) Q[i].push(pair<real,int>(0,i));}}

        t0 = t1 = clock();
//  similarity calculation starts here
        for(i=0;i<nrow;++i){
            if(i%100==0) {
                t2 = clock();
                //cout << i << "\t" << double(t2-t1)/CLOCKS_PER_SEC << "\t" << double(t2-t0)/CLOCKS_PER_SEC << "\t" << double(t2-t0)/CLOCKS_PER_SEC/double(i) << endl;
                t1 = t2;
            }
            if(i%buffsize==0) {
                for(p=0;p<nrow;++p){
                    for(buff_it=buffer.begin()+buffsize*p;buff_it!=buffer.begin()+writes[p]+buffsize*p;++buff_it) {
                        Q[buff_it->first].push(buff_it->second); 
                        Q[buff_it->first].pop();
                    }
                    writes[p]=0;
                }
            }

            if(simfunc!=3 && format!=3) {
                get_list(i, mask, rowptr, colindex, rowptr3, colindex3);
                for(j=i+1;j<nrow;++j){
                    if(*(mask+j)){
                        ++co_occurance;
                        sum=func(i, j, rowptr, colindex, value);
                        Q[i].push(pair<real,int> (sum,j));
                        Q[i].pop();
                        buffer[buffsize*j+writes[j]]=pair<int, pair<real,int> >(j, pair<real,int>(sum,i));
                        writes[j]++;
                        *(mask+j)=0;
                    }
                }
            }
            else {
                for(j=i+1;j<nrow;++j){
                    sum=func(i, j, rowptr, colindex, value);
                    Q[i].push(pair<real,int> (sum,j));
                    Q[i].pop();
                    buffer[buffsize*j+writes[j]]=pair<int, pair<real,int> >(j, pair<real,int>(sum,i));
                    writes[j]++;
                }
            }
        }
        //cout << "co_occurance " << co_occurance << endl;
        for(p=0;p<nrow;++p){
            for(buff_it=buffer.begin()+buffsize*p;buff_it!=buffer.begin()+writes[p]+buffsize*p;++buff_it) {
                Q[buff_it->first].push(buff_it->second); Q[buff_it->first].pop();
            }
            writes[p]=0;
        }

        ofstream dumpsim;
        if(dump!="") dumpsim.open(dump.c_str());
        for(i=0;i<nrow;++i){
            for(j=0;j<NN;++j){
                temp = Q[i].top();
                Q[i].pop();
                if(temp.first!=0 || simfunc==3){
                    nn[i][j]=pair<int,int>(temp.second,j+1); // max value = NN
                    where[i][temp.second]=j+1;
                }
                else {nn[i][j]=pair<int,int>(i,0);}
                if(dump!="") dumpsim << nn[i][j].first << " ";
            }
            sort(nn[i].begin(),nn[i].end(),compare_id2);
            if(dump!="") dumpsim << "\n";
        }
    }
// construct the shared nearest neighbor graph
    //cout << "constructing snn graph\n";
    int *links;
    //std::cout << "NN: " << NN << std::endl;
    //q=(NN+1)*(1+(NN+1)*(5+NN*2))/6+1;
    q=(NN+1)*((1+(NN+1)*(5+NN*2))/(float)6)+1;
    //std::cout << "q " << q  << std::endl;
    links=new int[q];
    for(i=0;i<q;++i) links[i]=0;
    //for(i=0;i<q;++i) { if (i > 1400000000) cout << i << " "; links[i]=0;}
    //for(i=0;i<q;++i) links[i] = 0;
    //std::cout << "tam " << (sizeof(links) / sizeof(links[0]))  << std::endl;

    map<int,int> *shared;
    map<int,int>::iterator it;
    shared = new std::map<int,int>[nrow];
    //std::cout << nrow << std::endl;
    int pp;
    
    for(i=0;i<nrow;++i){
        for(pp=0;pp<NN;++pp){
            j=nn[i][pp].first;
            if(j<i){
                if(where[j].find(i)!=where[j].end()) {k=shared[j][i]; shared[i][j]=k; links[k]++;}
                else links[0]++;
                continue;
            }            
            if(where[j].find(i)!=where[j].end()) k=where[i][j]*where[j][i];
            else {links[0]++; continue;} // i is similar to j but j is not similar to i (or vica versa)
            p=q=0;
            while(p<NN && q<NN){
                if(nn[i][p].first==nn[j][q].first) {k+=nn[i][p].second*nn[j][q].second; ++p; ++q;}
                else{
                    if(nn[i][p].first<nn[j][q].first) ++p;
                    else ++q;
                }
            }
            shared[i][j]=k;
            links[k]++;
        }
	}
    //cout << "snn graph constructed\n";

//  calculate strong link threshold
    q=(NN+1)*((1+(NN+1)*(5+NN*2))/(float)6)+1;
    //std::cout << "q " << q  << std::endl; 
    p=0;
    k=nrow*NN-links[0]; // number of remaining links
    for(i=1;i<q;++i) {
        p+=links[i];
        if(p>int(k*strong)) break;
    }
    int strong_threshold=i;
    //cout << "strong\t" << strong_threshold << endl;

//  calculate merge threshold
    p=0;
    k=nrow*NN-links[0]; // number of remaining links
    for(i=1;i<q;++i) {
        p+=links[i];
        if(p>int(k*(1.-merge_t))) break;
    }
    int merge_threshold=i;
    //cout << "merge \t" << merge_threshold << endl;

    delete[] links;

//  calculate the number of strong links for each point
    int *conn,*conn_bucket;
    conn        = new int[nrow];
    conn_bucket = new int[NN+1];
    for(i=0;i<=NN;++i) conn_bucket[i]=0;
    for(i=0;i<nrow;++i){
        conn[i]=0;
        for(it=shared[i].begin();it!=shared[i].end();++it) {if(it->second>=strong_threshold) conn[i]++;}
        conn_bucket[conn[i]]++;
    }

//  calculate topic threshold
    p=0;
    k=int(nrow*topic);
    for(i=NN;i>=0;i--){
        p+=conn_bucket[i];
        if(p>=k) break;
    }
    int topic_threshold=i;
    //cout << "topic \t" << topic_threshold <<endl;

//  calculate noise threshold
    p=0;
    k=int(nrow*noise);
    for(i=0;i<=NN;++i){
        p+=conn_bucket[i];
        if(p>=k) break;
    }
    int noise_threshold=i;
    //cout << "noise \t" << noise_threshold << endl;

//  find topics
    bool *topics;
    int topic_count=0;
    topics=new bool[nrow];
    for(i=0;i<nrow;++i){
        if(conn[i]>=topic_threshold) {topics[i]=1; ++topic_count;}
        else topics[i]=0;
    }
    //cout << "number of topics\t" << topic_count << endl;

//  merging step
    disjoint_set cluster(nrow);
    for(i=0;i<nrow;++i){
        if(conn[i]>noise_threshold) {
            for(it=shared[i].begin();it!=shared[i].end();++it) {
                if(conn[it->first]>noise_threshold && it->second>=merge_threshold && (topics[i] || topics[it->first])) cluster.join(i,it->first);
            }
        }
    }

//  labeling step
    vector< vector <int> > labels(nrow);
    for(i=0;i<nrow;++i){
        if(cluster.findset(i)==i && !topics[i] && cluster.rank[i]==0 && conn[i]>noise_threshold){
            sort(nn[i].begin(),nn[i].end(),compare_sim);
            for(j=0;j<NN;++j) {
                k=nn[i][j].first;
                if(shared[i][k]>merge_threshold*label && cluster.rank[cluster.findset(k)]>0) {labels[i].push_back(k); break;}
            }
        }
    }
    for(i=0;i<nrow;++i){
        if(!labels[i].empty()) cluster.join(i,labels[i][0]);
    }

//  write output
    ofstream out(fname.c_str());
//  partition

//    int maxLabel = 0;
//    //std::cout << labels.size() << std::endl;
//    for (int l = 0; l < labels.size(); l++)
//    {   std::cout << labels[l].size() << std::endl;
//        if (!labels[l].empty())
//        {
//            std::vector<int>::iterator itMax = max_element(labels[l].begin(), labels[l].end());
//            std::cout << " maxLabel: " << maxLabel << " itMax: " << *itMax << std::endl;
//            if (*itMax > maxLabel)
//                maxLabel = *itMax;
//        }    
//    }    
    
    map<string,int> sampleLabels;
    map<int,int> clabel;
    j=0;
    for(i=0;i<nrow;++i){
        if(cluster.rank[i]==0 && cluster.findset(i)==i && !topics[i] && labels[i].empty()) 
            sampleLabels[names[i]] = -1;
            //out << names[i] << "\t"  << "0" << std::endl;
        else{
            if(clabel[cluster.findset(i)]==0) 
            {
                clabel[cluster.findset(i)]=++j; 
                sampleLabels[names[i]] = j;
                //out << names[i] << "\t" << j << std::endl;
            }
            else 
                sampleLabels[names[i]] = clabel[cluster.findset(i)];
                //out <<  names[i] << "\t"  << clabel[cluster.findset(i)] << std::endl;
        }
    }

    for (map<string,int>::iterator it = sampleLabels.begin(); it != sampleLabels.end(); it++)
    {        
        if ((*it).second == -1)
        {
            j++;
            out << (*it).first << "\t" << j << std::endl;
        }    
        else out << (*it).first << "\t" << (*it).second << std::endl;
    }    
//    for(i=0;i<nrow;++i){
//        if(cluster.rank[i]==0 && cluster.findset(i)==i && !topics[i] && labels[i].empty()) 
//            out << names[i] << "\t"  << "0" << std::endl;
//        else{
//            if(clabel[cluster.findset(i)]==0) 
//            {
//                clabel[cluster.findset(i)]=++j; 
//                out << names[i] << "\t" << j << std::endl;
//            }
//            else out <<  names[i] << "\t"  << clabel[cluster.findset(i)] << std::endl;
//        }
//    }

//    out << "\n\n";

    if(rowlabels.size()==0 && names.size()==0) return 1;
    vector < vector<int> > clusters(nrow);
    for(i=0;i<nrow;++i) clusters[cluster.findset(i)].push_back(i);
//    for(i=0;i<nrow;++i) for(j=0;j<labels[i].size();++j) clusters[cluster.findset(labels[i][j])].push_back(i);
//  confusion matrix
    if(rowlabels.size()!=0){
        out << "Confusion matrix\n";
        for(i=0;i<classnames.size();++i) out << classnames[i] << "\t"; out << endl;
        for(i=0;i<classnames.size();++i) {
            for(j=0;j<classnames[i].length();++j) out << "-";
            out << "\t";
        }
        out << endl;
        int *confusion;
        confusion = new int[classnames.size()];
        for(i=0;i<nrow;++i){
            //if(clusters[i].size()<5) continue;
	        for(j=0;j<classnames.size() ;++j) confusion[j]=0;
            for(j=0;j<clusters[i].size();++j){
                confusion[classes[rowlabels[clusters[i][j]]]-1]++;
            }
            for(j=0;j<classes.size();++j) out << confusion[j] << "\t"; out << endl;
        }
        out << "\n\n";
    }
//  output names of elements in every cluster
//    if(names.size()!=0){
//        for(i=0;i<nrow;++i){
//            //if(clusters[i].size()<5) continue;
//            for(j=0;j<clusters[i].size();++j) out << names[clusters[i][j]] << " ";
//            out << endl;
//        }
//    }

    out.close();

//    system("PAUSE");
    return EXIT_SUCCESS;
}
