#include "COneNode.h"


COneNode :: COneNode (const char* s)
{
  left   = NULL;
  right  = NULL;
  dleft  = 0;
  dright = 0;
  Init(s);
  N = 100;
}

void COneNode :: InitN(int iN) {
  N = iN;
  if (left != NULL) left->InitN(N);
  if (right != NULL) right->InitN(N);  
}


COneNode :: ~COneNode()
{

  if (left  != NULL) delete left;
  if (right != NULL) delete right;
}



bool COneNode :: RemoveZero(void)
{
  bool ok = false;
  if (left != NULL && right != NULL && dleft+dright == 0)
    {
      printf("remove zero: %s\n", GetData().c_str());       
      delete left;
      left = NULL;
      dleft = 0;
      ok = true;
    }
  if (left != NULL && left->RemoveZero()) ok =  true;
  if (right != NULL && right->RemoveZero()) ok = true;
  return ok;
}


bool COneNode :: RemoveLongDist(float maxdist)
{
  bool ok = false;

  if (left != NULL && (dleft > maxdist || left->RemoveNode()))
    {
      if (left->GetNLeaf() < 5)
	{
	  printf("Remove long edge: %lf, %s\n", dleft, left->GetData().c_str());
	  delete left;
	  left = NULL;
	  ok = true;
	}
    }


  if (right != NULL && (dright > maxdist || right->RemoveNode()))
    {
      if (right->GetNLeaf() < 5)
	{
	  printf("Remove long edge: %lf, %s\n",  dright, right->GetData().c_str());
	  delete right;
	  right = NULL;
	  ok = true;
	}
    }

  if (left != NULL && left->RemoveLongDist(maxdist)) ok = true;
  if (right != NULL && right->RemoveLongDist(maxdist)) ok = true;  

  return ok;
}



bool COneNode :: RemoveNode(void) const
{
  /*
  printf("%s\n", GetData().c_str());
  
  printf("%s\n", leaf? "leaf":"non leaf");
  printf("%s\n", (left != NULL)? "left":"non left");
  printf("%s\n", (right != NULL)? "right":"non right");

  bool ok = (!leaf) && (left == NULL) && (right == NULL);
  printf("%s\n", ok? "Remove": "non remove");
  */
  return ( (!leaf) && (left == NULL) && (right == NULL));
}




bool COneNode :: Merge(void)
{
  bool ok = false;
  if (left != NULL && left->Singleton())
    {
      dleft += left->GetDist();
      left = left->GetChild(); 
      ok = true;
    }   
  if (right != NULL && right->Singleton())
    {
      dright += right->GetDist();
      right = right->GetChild(); 
      ok = true;
    }   
  if (left != NULL && left->Merge()) ok = true;
  if (right != NULL && right->Merge()) ok = true;
}



int COneNode :: GetNLeaf(void) 
{
  
  if (left == NULL && right == NULL) 
    {
      nleaf = 1;
      return 1;
    }

  nleaf = 0;
  if (left != NULL) nleaf += left->GetNLeaf();
  if (right != NULL) nleaf += right->GetNLeaf();
  return nleaf;
}


void COneNode :: RemoveChildren(int ch)
{
  if (ch == 0)
    {
      delete left;
      left = NULL;
      dleft = 0;
    }


  if (ch == 1)
    {
      delete right;
      right = NULL;
      dright = 0;
    }


}


void COneNode :: GetBreak(COneNode* &p, int &leftright, double &dist)
{
  if (left != NULL && dleft > dist &&  left->NLeaf() >= N/2 && left->NLeaf() <= N)
    {
      p = this;
      leftright = 0;
      dist = dleft;
      //      printf("N = %d,%d\n", N, left->NLeaf());
    }


  if (right != NULL && dright > dist &&   right->NLeaf() >= N/2 && right->NLeaf() <= N)
    {
      p = this;
      leftright = 1;
      dist = dright;
      //      printf("N = %d, %d\n", N, right->NLeaf());
    }

  if (left != NULL) left->GetBreak(p, leftright, dist);
  if (right != NULL) right->GetBreak(p, leftright, dist);
}




double COneNode :: GetDist(void) const { return dleft + dright;}
bool COneNode :: Singleton(void) const 
{
  if (left == NULL && right == NULL) return false;
  return (left == NULL || right == NULL);
}

COneNode * COneNode :: GetChild(void) 
{
  if (left != NULL) return left;
  return right;
}




void COneNode :: Jump(const char* &p)
{
  if (p[0] != '(') return;
  int np = 1;
  p++;
  while (np > 0 && p[0] != '\0')
    {
      if (p[0] == '(') np++;
      if (p[0] == ')') np--;
      p = p + 1;
    }
}


double COneNode :: GetNum(const char* s) const
{
  char tempc[1024];
  int i = 0;
  while ((s[i] >= '0' && s[i] <= '9') || (s[i] == '.')) i= i + 1;
  strncpy(tempc, s,i);
  tempc[i] = '\0';
  return atof(tempc);
}






void COneNode :: Init(const char* s)
{
  if (s[0] != '(')
    {
      node = "";
      int i = 0;
      while (s[i] != ',' && s[i] != ':' && s[i] != '\0') node += s[i++];
      leaf = true;
      return;
    }

  leaf = false;


  left = new COneNode (&(s[1]));
  const char* p = &(s[1]);
  if (p[0] == '(') Jump(p);
  p = strstr(p, ":") + 1;
  dleft = GetNum(p);

  p = strstr(p, ",") + 1;
  right = new COneNode(p);
  if (p[0] == '(') Jump(p);
  p = strstr(p, ":") + 1;
  dright = GetNum(p);
}


std::string COneNode :: GetData(void) const
{
  if (left == NULL || right == NULL) return node;


  char tempc[1024];
  sprintf(tempc, "%lf", dleft);

  std::string s = "(";
  s += left->GetData();
  s += ":";
  s += tempc;
  s += ",";

  sprintf(tempc, "%lf", dright);
  s += right->GetData();
  s += ":";
  s += tempc;
  s +=")";
  return s;
}




std::string COneNode :: GetDataName(void) const
{
  if (left == NULL && right == NULL) return node;
  
  std::string s = "";
  if (left != NULL) s = left->GetDataName();

  if (right != NULL)
    if (s == "")	s = right ->GetDataName();
    else
      {
	s = s + ",";
	s = s + right ->GetDataName();
      }

  return s;
}


double COneNode :: MaxDist(void) const
{
  double md = MAX(dleft, dright);
  if (left != NULL) md = MAX(md, left->MaxDist());
  if (right != NULL) md = MAX(md, right->MaxDist());
  return md;
}
