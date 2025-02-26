#include <vector>
#include <map>
#include <algorithm>
#include <set>
#include <iostream>
#include <bitset>
//#include <execution>
//#include <mutex>

#include <string.h>
#include <cstdlib>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "gurobi_c++.h"

#include "time.inl"

using namespace std;

#define MEM_LIMIT (220.) 
#define TIME_LIMIT (9000.)


void printEq(vector<set<int>> const & v) {
  if (v.empty()) cout << " 0 " << endl;
  else {
    for (auto const & s : v) {
      if (s.empty()) cout << " 1 ";
      else {
        for (auto x : s) cout << "v" << x << ".";
      }
      cout << " + ";
    }
    cout << endl;
  }
}

struct cmpBitset288 {
  bool operator()(const bitset<288>& a, const bitset<288>& b) const {
    for (int i = 0; i < 288; i++) {
      if (a[i] < b[i])
        return true;
      else if (a[i] > b[i])
        return false;
    }
    return false;
  }
};

int display_result_anf(set<int> cube, map<bitset<288>, int, cmpBitset288>& countingBox){

  string str = "superpolys";
  mkdir(str.c_str(), S_IRWXU);

  {
    int i ; 
    auto it2 = countingBox.begin(); 
    while (it2 != countingBox.end()) {
      if (((*it2).second % 2) == 0) {
        cout << (*it2).second << "\t";
        bitset<288> tmp = (*it2).first;
        for (i = 0; i < 80; i++) if ((tmp[i] == 1)) 
          cout << "k" << (i ) << " ";
        /*for (i = 0; i < 80; i++) if ((tmp[93 + i] == 1)) 
          cout << "v" << (i) << " ";
        // Uncomment for the cube in the superpoly.*/
        for (i = 0; i < 80; i++) if ((tmp[93 + i] == 1 && cube.find(i)!=cube.end()))//!binary_search(cube.begin(),cube.end(),i))) 
          cout << "v" << (i) << " ";
        cout << endl;
      }
      it2++;
    }
  }

  int i ; 
  int max_deg = 0;
  auto it2 = countingBox.begin(); 
  while (it2 != countingBox.end()) {
    if (((*it2).second % 2) == 1) {
      cout << ((*it2).second % 2) << " | " << (*it2).second << "\t";
      bitset<288> tmp = (*it2).first;

      int dd = 0;
      for (i = 0; i < 80; i++) {
        if ((tmp[i] == 1)) {
          cout << "k" << (i ) << " ";
          dd++;
        }      
      }

      for (i = 285; i < 288; ++i) 
        if (tmp[i] == 1) cout << " 1";
        else cout << " 0";

      if (max_deg < dd) max_deg = dd;
        
      /*for (i = 0; i < 80; i++) if ((tmp[93 + i] == 1)) 
        cout << "v" << (i) << " ";
      // Uncomment for the cube in the superpoly.*/
      for (i = 0; i < 80; i++) if ((tmp[93 + i] == 1 && cube.find(i)!=cube.end()))//!binary_search(cube.begin(),cube.end(),i))) 
        cout << "v" << (i) << " ";
      cout << endl;
    }
    it2++;
  }

  //
  str += "/sp_cube" + to_string(cube.size()) + "_deg" + to_string(max_deg);


  vector<int> sc;
  for (auto v : cube) sc.emplace_back(v);
  sort(sc.begin(), sc.end());
  for (auto v : sc) str += "_v" + to_string(v - 93);

  FILE *out = fopen(str.c_str(), "w");
  
  it2 = countingBox.begin(); 
  while (it2 != countingBox.end()) {
    if (((*it2).second % 2) == 1) {
      bitset<288> tmp = (*it2).first;
      for (i = 0; i < 80; i++) {
        if ((tmp[i] == 1)) {
          fprintf(out, " %d", i);
        }              
      }

      // for (i = 285; i < 288; ++i) 
      //   if (tmp[i] == 1) fprintf(out, " 1");
      //   else fprintf(out, " 0");
      fprintf(out, "\n");
    }
    it2++;
  }
  fclose(out);

  return 0 ; 
}


void printSys(map<int, vector<set<int>>> const & sys) {
  for (auto const & p : sys) {
    cout << "v" << p.first << " = ";
    printEq(p.second);
  }
}


// Return ：the first round position or the other round A(or B or C)'s first position
// The first round: return
// Other round the A(or B or C)'s first position: return
// Other: recursion to previous round
int getVarTrivium(int x) {
  if (x < 288) return x;
  auto xmod = x%288;
  if (xmod == 0 || xmod == 93 || xmod == 177) return x;
  return getVarTrivium(x - 288 - 1);
}

map<int, vector<set<int>>> generateTrivium(int R) {
  map<int, vector<set<int>>> sys;
  int root = 288*(R+1);
  auto & root_eq = sys[root];
  vector<int> indexes_root = {65, 92, 161, 176, 242, 287};
  set<int> waiting;
  for (auto i : indexes_root) {
    root_eq.emplace_back(set<int> ({getVarTrivium(288*R + i)}));
    waiting.emplace(getVarTrivium(288*R + i));
  }
  while (!waiting.empty()) {
    auto x = *(waiting.rbegin());// take the last element from waiting
    waiting.erase(x);// erase the x from waiting
    if (x/288 == 0) continue;
    auto & eq = sys[x];
    switch (x%288) {
      case 0:
        x -= 288;
        eq.emplace_back(set<int>({getVarTrivium(x + 68)}));// emplace_back is similar to push_back
        eq.emplace_back(set<int>({getVarTrivium(x + 242)}));
        eq.emplace_back(set<int>({getVarTrivium(x + 287)}));
        eq.emplace_back(set<int>({getVarTrivium(x + 285), getVarTrivium(x + 286)}));
        waiting.emplace(getVarTrivium(x + 242));
        waiting.emplace(getVarTrivium(x + 287));
        waiting.emplace(getVarTrivium(x + 68));
        waiting.emplace(getVarTrivium(x + 285));
        waiting.emplace(getVarTrivium(x + 286));
        break;
      case 93:
        x -= 288 + 93;
        eq.emplace_back(set<int>({getVarTrivium(x + 170)}));
        eq.emplace_back(set<int>({getVarTrivium(x + 65)}));
        eq.emplace_back(set<int>({getVarTrivium(x + 92)}));
        eq.emplace_back(set<int>({getVarTrivium(x + 90), getVarTrivium(x + 91)}));
        waiting.emplace(getVarTrivium(x + 65));
        waiting.emplace(getVarTrivium(x + 92));
        waiting.emplace(getVarTrivium(x + 170));
        waiting.emplace(getVarTrivium(x + 90));
        waiting.emplace(getVarTrivium(x + 91));
        break;
      case 177:
        x -= 288 + 177;
        eq.emplace_back(set<int>({getVarTrivium(x + 263)}));
        eq.emplace_back(set<int>({getVarTrivium(x + 161)}));
        eq.emplace_back(set<int>({getVarTrivium(x + 176)}));
        eq.emplace_back(set<int>({getVarTrivium(x + 174), getVarTrivium(x + 175)}));
        waiting.emplace(getVarTrivium(x + 161));
        waiting.emplace(getVarTrivium(x + 176));
        waiting.emplace(getVarTrivium(x + 263));
        waiting.emplace(getVarTrivium(x + 174));
        waiting.emplace(getVarTrivium(x + 175));
        break;
    }
  }
  return sys;
}

map<int, vector<int>> reverseSys(map<int, vector<set<int>>> const & sys) {
  map<int, vector<int>> res;
  for (auto const & eq : sys) {
    for (auto const & s : eq.second) {
      for (auto x : s) res[x].emplace_back(eq.first);
    }
  }
  return res;
}

int getT(int x, map<int, vector<set<int>>> const & sys, map<int, int> & T, map<int, int> & TT);
int getT(set<int> const & s, map<int, vector<set<int>>> const & sys, map<int, int> & T, map<int, int> & TT);

int getTT(int x, map<int, vector<set<int>>> const & sys, map<int, int> & T, map<int, int> & TT) {
  auto it = TT.find(x);
  if (it != TT.end()) return it->second;
  auto it_eq1 = sys.find(x);
  auto it_eq2 = sys.find(x+288);
  auto & TTx = TT[x];
  if (it_eq1 == sys.end() && it_eq2 == sys.end()) {
    TTx = getT(x, sys, T, TT) + getT(x+288, sys, T, TT);
  }
  else {
    auto const & eq1 = (it_eq1 != sys.end()) ? it_eq1->second : vector<set<int>> (1, set<int> ({x}));
    auto const & eq2 = (it_eq2 != sys.end()) ? it_eq2->second : vector<set<int>> (1, set<int> ({x+288}));
    TTx = 0;
    for (auto const & s1 : eq1) {
      for (auto s2 : eq2) {
        s2.insert(s1.begin(), s1.end());
        int tmp = getT(s2, sys, T, TT);
        if (tmp > TTx) TTx = tmp;
      }
    }
  }
  return TTx;
}

int getT(set<int> const & s, map<int, vector<set<int>>> const & sys, map<int, int> & T, map<int, int> & TT) {
  if (s.size() == 0) return 0;
  if (s.size() == 1) return getT(*s.begin(), sys, T, TT);
  auto ss = s;
  auto x = *ss.begin();
  ss.erase(ss.begin());
  if (ss.count(x+288) == 0) return getT(x, sys, T, TT) + getT(ss, sys, T, TT);
  else {
    ss.erase(x+288);
    return getTT(x, sys, T, TT) + getT(ss, sys, T, TT);
  }
}

int getT(int x, map<int, vector<set<int>>> const & sys, map<int, int> & T, map<int, int> & TT) {
  auto it = T.find(x);
  if (it != T.end()) return it->second;
  auto & Tx = T[x];
  Tx = 0;
  auto it_eq = sys.find(x);
  if (it_eq != sys.end()) {
    for (auto const & s : it_eq->second) {
      int local_best = getT(s, sys, T, TT);
      if (local_best > Tx) Tx = local_best;
    }
  }
  return Tx;
}


void milpTrivium(int R, set<int> vcube, set<int> v0) {
  auto sys = generateTrivium(R);
  auto rsys = reverseSys(sys);

  try {

    // Create an environment
    GRBEnv env = GRBEnv(true);
    // env.set("LogFile", "mip1.log");
		env.set(GRB_DoubleParam_PoolGap, GRB_INFINITY);
    //env.set(GRB_IntParam_OutputFlag,0);
    env.start();

    int root = sys.rbegin()->first;

    {
      set<int> tmp;
      for (auto x : vcube) tmp.emplace(x+93);// emplace is similar to insert
      vcube = move(tmp);
    }
    {
      set<int> tmp;
      for (auto x : v0) tmp.emplace(x+93);
      v0 = move(tmp);
    }

    //add 0s for state 0 Trivium
    for (int i = 80; i < 93; ++i) v0.emplace(i);
    for (int i = 93 + 80; i < 285; ++i) v0.emplace(i);

    //propagate 0s  (not really needed)
    for (auto const & p : sys) {
      bool flag0 = true;
      for (unsigned i = 0; i < 3; ++i) {
        if (v0.count(*p.second[i].begin()) == 0) flag0 = false;
      }
      auto y1 = *p.second[3].begin();
      auto y2 = *p.second[3].rbegin();
      if (v0.count(y1) == 0 && v0.count(y2) == 0) flag0 = false;
      if (flag0) v0.emplace(p.first);
    }

    //setup dynamic programming for "cost"
    map<int, int> T, TT;
    for (auto y : v0) T[y] = -100;
    for (auto y : vcube) T[y] = 1;

    vector<set<int>> vsetroot;

    for (auto const & s : sys.at(root)) {
      cout << getT(*s.begin(), sys, T, TT) << " ";
      if (getT(*s.begin(), sys, T, TT) >= vcube.size()) vsetroot.emplace_back(s);
    }
    cout << endl;

    int cptt = 0;
    int nSolutions = 0;
    map<bitset<288>, int, cmpBitset288> countingBox;


    // handle each model
    {
      uint64_t iroot = 0;
      while (iroot < vsetroot.size()) {
        auto setroot = move(vsetroot[iroot++]);
        
        if (*setroot.begin() != 221073) continue;

        // generate submodel
        auto waiting = setroot;
        map<int, vector<set<int>>> sys2;
        while (!waiting.empty()) {
          auto x = *waiting.rbegin();
          waiting.erase(x);
          auto it = sys.find(x);
          if (it == sys.end()) continue;
          sys2[x] = it->second;
          for (auto const & s : sys2[x]) waiting.insert(s.begin(), s.end());
        }

        int Nvar = 0;

        map<pair<int, int>, int> mapVars;
        for (auto const & p : sys2) {
          for (auto const & s : p.second) {
            for (auto x : s) mapVars[make_pair(p.first, x)] = Nvar;
            Nvar++;
          }
        }

        auto rsys2 = reverseSys(sys2);

        // Create an empty model
        GRBModel model = GRBModel(env);

        // Create variables and setup strategy
        vector<GRBVar> X (Nvar);
        for (auto const & p : sys2) {
          for (auto const & s : p.second) {
            auto x = *s.begin();
            {
              if (s.size() == 1 || getT(s, sys, T, TT) <= 1) X[mapVars[make_pair(p.first, x)]] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY);
              else X[mapVars[make_pair(p.first, x)]] = model.addVar(0.0, 1.0, 1.0, GRB_BINARY);
              if (getT(s, sys, T, TT) <= 0) X[mapVars[make_pair(p.first, x)]].set(GRB_IntAttr_BranchPriority, -300);
              else if (s.size() == 2) X[mapVars[make_pair(p.first, x)]].set(GRB_IntAttr_BranchPriority, getT(s, sys, T, TT));
            }
          }
          for (int i = 0; i < 3; ++i) {
            X[mapVars[make_pair(p.first, *p.second[i].begin())]].set(GRB_DoubleAttr_VarHintVal, 0.0);
          }
        }


        // error 328
        if (*setroot.begin() == 221073) {
          std::vector<std::pair<int, int>> preset_edges = {{221073, 197085}, {197373, 174909}, {197085, 170496}, {174909, 148320}, {170784, 151953}, {170496, 138993}, {151953, 127965}, {148608, 129777}, {148320, 116817}, {139281, 114225}, {138993, 115005}, {129777, 105789}, {128253, 101664}, {127965, 108864}, {117105, 93117}, {116817, 96861}, {115293, 88704}, {115005, 92541}};
          std::cout << "Task id: 328" << std::endl;
          for (auto e : preset_edges) {
            model.addConstr(X[mapVars.at(e)] == 1);
          }
        }


        // remove some impossible cases using dynamic programming
        {
          map<int, vector<int>> waiting;
          waiting[*setroot.begin()] = vector<int> (4, vcube.size());
          while (!waiting.empty()) {
            auto pw = *waiting.rbegin();
            waiting.erase(pw.first);
            if (pw.first < 288) continue;
            auto const & eq = sys2.at(pw.first);
            for (int i = 0; i <= 3; ++i) {
              auto const & s = eq[i];
              if (getT(s, sys, T, TT) < pw.second[i]) {
                // impossible to have pw.first --> eq[i]
                model.addConstr(X[mapVars[make_pair(pw.first, *s.begin())]] == 0);
              }
              else {
                if (i < 3) {
                  auto it = waiting.find(*s.begin());
                  if (it == waiting.end()) waiting.emplace(*s.begin(), vector<int> (4, pw.second[i]));
                  else {
                    for (int j = 0; j < 4; ++j) it->second[j] = min(it->second[j], pw.second[i]);
                  }
                }
                else {
                  auto x1 = *s.begin();
                  auto x2 = *s.rbegin();
                  if (x1 < 288 || x2 < 288) continue;
                  auto const & eq1 = sys2.at(x1);
                  auto const & eq2 = sys2.at(x2);
                  auto it1 = waiting.find(x1);
                  if (it1 == waiting.end()) it1 = waiting.emplace(x1, vector<int> (4, vcube.size())).first;
                  auto it2 = waiting.find(x2);
                  if (it2 == waiting.end()) it2 = waiting.emplace(x2, vector<int> (4, vcube.size())).first;
                  int m1 = 0;
                  for (int j = 0; j < 3; ++j) m1 = max(m1, getT(eq1[j], sys, T, TT));
                  int m2 = 0;
                  for (int j = 0; j < 3; ++j) m2 = max(m2, getT(eq2[j], sys, T, TT));
                  if (m1 + m2 < pw.second[i]) {
                    // At least one of the two branches has to double
                    model.addConstr(X[mapVars[make_pair(pw.first, *s.begin())]] <= X[mapVars[make_pair(x1, *eq1[3].begin())]] + X[mapVars[make_pair(x2, *eq2[3].begin())]]);
                  }
                  for (int j1 = 0; j1 < 4; ++j1) {
                    for (int j2 = 0; j2 < 4; ++j2) {
                      set<int> const & s1 = eq1[j1];
                      set<int> const & s2 = eq2[j2];
                      auto ss (s1);
                      ss.insert(s2.begin(), s2.end());
                      int cs = getT(ss, sys, T, TT);
                      if (cs < pw.second[i]) continue;
                      auto ss1 (ss);
                      for (auto x : s1) if (ss1.count(x) != 0) ss1.erase(x);
                      auto ss2 (ss);
                      for (auto x : s2) if (ss2.count(x) != 0) ss2.erase(x);
                      it1->second[j1] = min(it1->second[j1], max(0,pw.second[i] - getT(ss1, sys, T, TT)));
                      it2->second[j2] = min(it2->second[j2], max(0,pw.second[i] - getT(ss2, sys, T, TT)));
                    }
                  }
                }
              }
            }
          }
        }


        // add constraints for cube variables
        for (auto x : vcube) {
          if (setroot.count(x) == 0) {
            GRBLinExpr e = 0;
            for (auto y : rsys2[x]) e += X[mapVars.at(make_pair(y,x))];
            model.addConstr(e >= 1);
          }
        }

        // add constraints for 0s
        for (auto x : v0) {
          for (auto y : rsys2[x]) model.addConstr(X[mapVars.at(make_pair(y,x))] == 0);
        }

        cout << "bound: " << getT(setroot, sys, T, TT) << endl;
        cout << "setroot: " << setroot.size() << endl;
        cout << "vcube: " << vcube.size() << endl;



        // extra constraints from 1s
        model.addConstr(X[mapVars.at(make_pair(288,285))] == 0);
        model.addConstr(X[mapVars.at(make_pair(288,286))] == 0);
        model.addConstr(X[mapVars.at(make_pair(288,287))] == 0);

        // root
        for (auto x : setroot) {
          if (x >= 288) {
            GRBLinExpr e = 0;
            for (auto const & s : sys2.at(x)) e += X[mapVars.at(make_pair(x,*s.begin()))];
            model.addConstr(e == 1);
          }
        }

        for (auto const & p : sys2) {
          GRBLinExpr e_out = 0;
          for (auto const & s : p.second) {
            e_out += X[mapVars[make_pair(p.first, *s.begin())]];
          }
          // at most one child
          model.addConstr(e_out <= 1);


          if (setroot.count(p.first) == 0) {
            GRBLinExpr e_in = 0;
            for (auto x : rsys2[p.first]) e_in += X[mapVars[make_pair(x, p.first)]];
            model.addConstr(e_in >= e_out); // each child has at least one father
            model.addConstr(e_in <= rsys2[p.first].size()*e_out); // each father has one child
            if (p.first >= 288 && sys2.find(p.first + 288) != sys2.end() && sys2.find(p.first + 2*288) != sys2.end()) {
              vector<int> tmp;
              tmp.emplace_back(*sys2[p.first][3].begin());
              tmp.emplace_back(*sys2[p.first][3].rbegin());
              tmp.emplace_back(*sys2[p.first + 2*288][3].begin());
              tmp.emplace_back(*sys2[p.first + 2*288][3].rbegin());
              if (none_of(tmp.begin(), tmp.end(), [&v0](auto x){return v0.count(x) != 0;})) {
                auto x03 = mapVars.at(make_pair(p.first, *sys2[p.first][3].begin()));
                auto x12 = mapVars.at(make_pair(p.first + 288, *sys2[p.first + 288][2].begin()));
                auto x13 = mapVars.at(make_pair(p.first + 288, *sys2[p.first + 288][3].begin()));
                auto x23 = mapVars.at(make_pair(p.first + 2*288, *sys2[p.first + 2*288][3].begin()));
                // remove pattern "3 consecutives"
                if (sys2.find(p.first + 3*288) != sys2.end()) {
                  auto x33 = mapVars.at(make_pair(p.first + 3*288, *sys2[p.first + 3*288][3].begin()));
                  model.addConstr(2*X[x03] + 2*X[x23] + X[x12] + 2*X[x13] - X[x33] <= 4);
                }
                else model.addConstr(X[x12] + X[x13] + X[x03] + X[x23] <= 2);

              }
            }
          }
        }

        model.set(GRB_IntParam_RINS, 0);
        model.set(GRB_IntParam_VarBranch, 2);
        model.set(GRB_IntParam_MIPFocus, 3);

        model.set(GRB_IntParam_Threads, 64);

        model.set(GRB_DoubleParam_MIPGap, GRB_INFINITY);
        model.set(GRB_IntParam_PoolSearchMode, 2);
        model.set(GRB_IntParam_PoolSolutions, 2000000000);

        model.set(GRB_DoubleParam_SoftMemLimit, MEM_LIMIT);
        model.set(GRB_DoubleParam_TimeLimit, TIME_LIMIT);

        //error
#if PRESOLVE >= 0        
        model.set(GRB_IntParam_Presolve, PRESOLVE);
#endif
        model.optimize();


        if (!(model.get(GRB_IntAttr_Status) == GRB_OPTIMAL || model.get(GRB_IntAttr_Status) == GRB_INFEASIBLE)) {
          cout << "superpolys is too large to recover" << endl;
          return;
        }


        nSolutions += model.get(GRB_IntAttr_SolCount);
        cout << "\r" << "cptt: " << ++cptt << "/" << vsetroot.size() << " -- " << "solutions: " << nSolutions<<endl;

        //save superpoly
        int solCount = model.get(GRB_IntAttr_SolCount);
        if (solCount >= 2000000000) {
          cerr << "Number of solutions is too large" << endl;
          exit(0);
        }
        for (int i = 0; i < solCount; i++) {
          model.set(GRB_IntParam_SolutionNumber, i);
          bitset<288> tmp;
          for (int x = 0; x < 288; x++) {
            tmp[x] = 0;
            for (auto y : rsys2.at(x)) if (X[mapVars.at(make_pair(y,x))].get(GRB_DoubleAttr_Xn) > 0.5) 
              tmp[x]=1;
          }
          countingBox[tmp]++;
        }
      }
    }
    cout<<"SUPERPOLY"<<endl;
    display_result_anf(vcube, countingBox) ;
    cout<<endl;
  }catch(GRBException e) {
    cout  << "Error  number: " << e.getErrorCode () << endl;
    cout  << e.getMessage () << endl;
  }catch(...) {
    cout  << "Error  during  optimization" << endl;
  }
}

int main(int argc, char const *argv[]) {
  int R = 832;

  PRINTF_STAMP("recover start\n");  

  set<int> vcube, v0;
  for (int i = 1; i < argc; i++) vcube.emplace(atoi(argv[i]));

  for (int i = 0; i < 80; ++i) {
    if (vcube.count(i) == 0) v0.emplace(i);
  }

  cout << "\t\t";
  for (auto v : vcube) cout << " v" << v;
  cout << endl << endl;

  milpTrivium(R, vcube, v0);

  PRINTF_STAMP("recover done\n");

  return 0;
}

// g++ -march=native -O3 -std=c++17 src/oficial_milptrivium.cpp -o recover -I/${GUROBI_HOME}/include/ -L/${GUROBI_HOME}/lib -lgurobi_c++ -lgurobi100 -lm ; ./recover 44

