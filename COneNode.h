#ifndef __CONENODE__
#define __CONENODE__

#include <vector>
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>


#define JumpOver(p) {int np = 1; p ++; while (np > 0 && p [0] != '\0') { if (p[0] == '(') np ++; if (p[0] == ')') np --; p++;} }

#define MAX(a,b) ((a) > (b))?(a):(b)

class COneNode {
 private:

  std::string node;
  double dleft, dright;
  double GetNum(const char* s) const;
  int nleaf;
  int N;
  bool leaf;

 public:
  ~COneNode();

  COneNode *left, *right;

  void InitN(int iN);

  void GetBreak(COneNode* &p, int &leftright, double &dist);
  void RemoveChildren(int ch);

  bool RemoveLongDist(float);
  bool RemoveNode(void) const; //check if left == NULL && right == NULL

  int NLeaf(void) {return nleaf;}

  int GetNLeaf(void);
  void Jump(const char* &p);
  void Init(const char* s);
  COneNode (const char* s);
  void Init(void);
  std::string GetData(void) const;
  std::string GetDataName(void) const;

  bool RemoveZero(void);
  bool Merge(void);
  double GetDist(void) const;
  bool Singleton(void) const;
  COneNode * GetChild(void);
  double MaxDist(void) const;

};




#endif
