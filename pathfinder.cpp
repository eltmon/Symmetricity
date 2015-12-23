#include "pathfinder.hpp"
#include "agony.hpp"
#include <limits>
#include <fstream>
#include "ppm.hpp"
using namespace std;
map<char, vector<Eigen::Vector3d>> pathables;
GraphAnalyzer::GraphAnalyzer(const Agony &ag) : ag(ag) {
  if (pathables.size() == 0) {
    pathables['d'].push_back(Eigen::Vector3d(1, 0, 0));
    pathables['d'].push_back(Eigen::Vector3d(-1, 0, 0));
    pathables['d'].push_back(Eigen::Vector3d(1, 1, 0));
    pathables['d'].push_back(Eigen::Vector3d(-1, 1, 0));
    pathables['d'].push_back(Eigen::Vector3d(0, 1, 0));
    pathables['d'].push_back(Eigen::Vector3d(0, -1, 0));
    pathables['d'].push_back(Eigen::Vector3d(-1, -1, 0));
    pathables['u'].push_back(Eigen::Vector3d(1, 0, 0));
    pathables['u'].push_back(Eigen::Vector3d(-1, 0, 0));
    pathables['u'].push_back(Eigen::Vector3d(1, 1, 0));
    pathables['u'].push_back(Eigen::Vector3d(-1, 1, 0));
    pathables['u'].push_back(Eigen::Vector3d(0, 1, 0));
    pathables['u'].push_back(Eigen::Vector3d(0, -1, 0));
    pathables['u'].push_back(Eigen::Vector3d(-1, -1, 0));
    pathables['u'].push_back(Eigen::Vector3d(0, 0, 1));

    pathables['j'].push_back(Eigen::Vector3d(1, 0, 0));
    pathables['j'].push_back(Eigen::Vector3d(-1, 0, 0));
    pathables['j'].push_back(Eigen::Vector3d(1, 1, 0));
    pathables['j'].push_back(Eigen::Vector3d(-1, 1, 0));
    pathables['j'].push_back(Eigen::Vector3d(0, 1, 0));
    pathables['j'].push_back(Eigen::Vector3d(0, -1, 0));
    pathables['j'].push_back(Eigen::Vector3d(-1, -1, 0));
    pathables['j'].push_back(Eigen::Vector3d(0, 0, -1));

    pathables['i'].push_back(Eigen::Vector3d(1, 0, 0));
    pathables['i'].push_back(Eigen::Vector3d(-1, 0, 0));
    pathables['i'].push_back(Eigen::Vector3d(1, 1, 0));
    pathables['i'].push_back(Eigen::Vector3d(-1, 1, 0));
    pathables['i'].push_back(Eigen::Vector3d(0, 1, 0));
    pathables['i'].push_back(Eigen::Vector3d(0, -1, 0));
    pathables['i'].push_back(Eigen::Vector3d(-1, -1, 0));
    pathables['i'].push_back(Eigen::Vector3d(0, 0, -1));
    pathables['i'].push_back(Eigen::Vector3d(0, 0, 1));
  }
}

void GraphAnalyzer::run_analysis(const std::string &filename) const {
  std::unordered_map<Eigen::Vector3d,std::unordered_map<Eigen::Vector3d,double> > costs;
  auto q=ag.allowed;
  for(auto j:q)
    for(auto i:q)
      costs[j.first][i.first]=numeric_limits<double>::infinity();
  for(auto i:q){//the distance between all vertices and themselves are zero.
    auto c=i.first;
    costs[c][c]=0;
  }
  for(auto i:q){//set all adjacent things to 1
    auto z=i.first;
    for(auto v:pathables[i.second])
      {//TODO fix stair connections to ensure that they are mutually connective.
        auto current=Eigen::Vector3d(z[0]+v[0],z[1]+v[1],z[2]+v[2]);
        if(q.find(current)!=q.end())
          costs[i.first][current]=1;
      }
  }
  for(auto k:q){
    for(auto i:q){
      for(auto j:q){
        if(costs[i.first][j.first]>costs[i.first][k.first]+costs[k.first][j.first])
          costs[i.first][j.first]=costs[i.first][k.first]+costs[k.first][j.first];
      }
    }
  }
  ofstream output(filename);
  std::unordered_map<Eigen::Vector3d,double> maxes;
  for(auto i:costs){
    auto z=i.first;
    output<<z[0]<<" "<<z[1]<<" "<<z[2]<< " ";
    double max=0;
    for(auto c:i.second){
      if(c.second>max&&c.second!=numeric_limits<double>::infinity())
        max+=c.second/double(i.second.size());
    }
    maxes[i.first]=max;
    output<<max<<endl;
  }
  auto c=ag.getBoundaries();
  write_ppm_to_file(filename,maxes,
                    c);
}