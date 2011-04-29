#ifndef PMF_H__	 

#include <itpp/itbase.h>
#include <itpp/itstat.h>
#include <itpp/stat/misc_stat.h>

/**

 Probabalistic matrix/tensor factorization written Danny Bickson, CMU

See algrithm description and explanation in: Liang Xiong, Xi Chen, Tzu-Kuo Huang, Jeff Schneider, Jaime G. Carbonell, Temporal Collaborative Filtering with Bayesian Probabilistic Tensor Factorization. In Proceedings of SIAM Data Mining, 2010.

*/
#include <vector>
#define GL_NO_MULT_EDGES //comment this flag, if you want to have support for multiple edges in different times between the same user and movie
#define GL_NO_MCMC //comment this flag, if you want to have support for MCMC methods (BPTF)

int MAX_ITER=10; //maximal number of iterations to run
int BURN_IN =10; //burn-in priod (for MCMC sampling - optional)
int D=20;         //number of features
bool FLOAT=false; //is data in float format
double LAMBDA=1;//regularization weight

//starts for holding edge data in file
struct edge_double{
  int from;
  int to;
  double time;
  double weight;
};
struct edge_float{
  float from;
  float to;
  float time;
  float weight;
};


//typedef double  sdouble; 
using namespace itpp;

/** Vertex and edge data types **/
struct vertex_data {
  vec pvec; //vector of learned values U,V,K
  double rmse; //root of mean square error
  int num_edges; //number of edges
  vertex_data(){
    pvec = zeros(D);
    rmse = 0;
    num_edges = 0;
  }


  void save(graphlab::oarchive& archive) const {  
    archive << pvec;
    archive << rmse << num_edges; 
  }  
   
  void load(graphlab::iarchive& archive) {  
     archive >> pvec;
     archive >> rmse >> num_edges;  
  }
};

struct edge_data {
  float  weight;  //observation 
#ifndef GL_NO_MULT_EDGES
  float  time; //time of observation (for tensor algorithms)
#else
  short time;
#endif
#ifndef GL_NO_MCMC  
  float avgprd;
#endif
  edge_data(){ 
	weight = 0; 
	time = 0; 
#ifndef GL_NO_MCMC
	avgprd = 0;
#endif
}

 void save(graphlab::oarchive& archive) const {  
    archive << weight << time; 
#ifndef GL_NO_MCMC	
	archive<< avgprd;; 
#endif
  }  
   
  void load(graphlab::iarchive& archive) {  
    archive >> weight >> time;
#ifndef GL_NO_MCMC
     archive >> avgprd; 
#endif 
  }
};

//containiner for handling multiple edge
struct multiple_edges{
  std::vector<edge_data> medges;
 void save(graphlab::oarchive& archive) const {  
       archive << medges; 
  }  
   
  void load(graphlab::iarchive& archive) {  
      archive >> medges;  
  }
};
 
//methods to compute the Root mean square error (RMSE)     
inline double rmse(const vec& x1, const vec& x2, int len, double val, double & sum){
	sum = dot(x1, x2);	
	return pow(sum - val, 2);
}

inline double rmse(const vec& x1, const vec& x2, const vec *x3, int len, double val, double &sum){
	if (x3 == NULL) //matrix	
		return rmse(x1,x2,len,val,sum);

	sum = 0;
	double ret = 0;
	for (int i=0; i< len; i++){
	ret = (x1[i] * x2[i] * x3->get(i));
	sum+= ret;
	}
	return pow(sum - val, 2);
}

 double get_rmse(const vertex_data & v){
    return v.rmse;
 }


//data file types
enum testtype{
    TRAINING = 0,
    VALIDATION = 1,
    TEST = 2
};

const char * testtypename[] = {"TRAINING", "VALIDATION", "TEST"};
//run modes

enum runmodes{
   ALS_MATRIX = 0,
   BPTF_MATRIX = 1,
   BPTF_TENSOR = 2,
   BPTF_TENSOR_MULT = 3,
   ALS_TENSOR_MULT = 4
};

const char * runmodesname[] = {"ALS_MATRIX", "BPTF_MATRIX", "BPTF_TENSOR", "BPTF_TENSOR_MULT", "ALS_TENSOR_MULT"};


enum countervals{
   EDGE_TRAVERSAL=0,
   BPTF_SAMPLE_STEP=1,
   CALC_RMSE_Q=2,
   ALS_LEAST_SQUARES=3,
   BPTF_TIME_EDGES=5,
   BPTF_LEAST_SQUARES=6,
   CALC_OBJ = 7,
   BPTF_MVN_RNDEX=9,
   BPTF_LEAST_SQUARES2=10, 
};

const char * countername[] = {"EDGE_TRAVERSAL", "BPTF_SAMPLE_STEP", "CALC_RMSE_Q", "ALS_LEAST_SQUARES", "NA", \
  "BPTF_TIME_EDGES", "BPTF_LEAST_SQUARES", "CALC_OBJ", "NA", "BPTF_MVN_RNDEX", "BPTF_LEAST_SQUARES2"};
#endif

