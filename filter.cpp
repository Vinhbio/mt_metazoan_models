/*

compile

g++ -O3 filter.cpp

./a.out ../alignments/zinc-ribbon_6.fasta.alignment.fasta.treefile

 */


#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <string>
#include <string.h>

#include "COneNode.h"
#include "COneNode.cpp"


#define MaxLen 100000000
char *bf = (char*)calloc(MaxLen, sizeof(char));

int N = 64;

double maxdist = -1;
std::string fnout, fnin;

bool viewmaxdist = false;


int main(int ArgI, char* ArgC[])
{

  if (ArgI == 1) 
    {
      printf("./split -in <file tree> -out<file break> [-N <int>] [-md <max distance>]\n");
      return 0;
    }

  strcpy(bf,"");
  for (int i = 0; i != ArgI; ++i)
    {
      if (strcmp(ArgC[i], "-in") == 0) fnin  = ArgC[i+1];
      if (strcmp(ArgC[i], "-out") == 0) fnout = ArgC[i+1];
      if (strcmp(ArgC[i], "-N") == 0) N = atoi(ArgC[i+1]);
      if (strcmp(ArgC[i], "-md") == 0) maxdist = atof(ArgC[i+1]);
      if (strcmp(ArgC[i], "-viewmd") == 0) viewmaxdist = true;
    }


  FILE *f = fopen(fnin.c_str(), "r");
  fgets(bf, MaxLen, f);
  fclose(f);



  if (strstr(bf,";") != NULL) strstr(bf, ";")[0] = '\0';
  int i = 1;
  if (bf[i] == '(')
    {
      i = i + 1;
      int np = 1;
      while (np > 0)
	{
	  if (bf[i] == '(') np++;
	  if (bf[i] == ')') np --;
	  i = i + 1;
	}

    }

  while (bf[i] != ',') i++;


  int len = strlen(bf);
  for (int j = len+1; j-1 > i; j--) bf[j] = bf[j-1]; 
  bf[i+1] = '(';
  strcat(bf, ":0.000000);");

  COneNode *root = new COneNode(bf);
  root->InitN(N);

  if (viewmaxdist)
    {
      printf("max dist = %s %lf\n", fnin.c_str(), root->MaxDist());
      return 0;
    }

  while (root->RemoveZero() || root->Merge());
  root->GetNLeaf();


  if (maxdist > 0)
    {
      printf("maxdist = %lf\n", maxdist);
      while (1==1)
	{
	  root->GetNLeaf();
	  if (root->GetNLeaf() < 10) break;
	  if (!root->RemoveLongDist(maxdist)) break;
	}
    }


  printf("tree file: %s\n", fnin.c_str());
  printf("out file : %s\n", fnout.c_str());
  printf("N = %d\n", N);


  FILE *fo = fopen(fnout.c_str(), "wb");

  while (1==1)
    {
      root->Merge();
      int n = root->GetNLeaf();
      if (n <= N) break;
      COneNode *p = NULL;
      int leftright;
      double dist = -1;
      root->GetBreak(p, leftright, dist);

      std::string s = "NONE";

      if (dist > 0)
	{
	  int nl = 0;
	  if (leftright == 0) {
	    s = p->left->GetDataName();
	    nl = p->left->NLeaf();
	  }
	  else {
	    s = p->right->GetDataName();
	    nl = p->right->NLeaf();
	  }

	  p->RemoveChildren(leftright);	  
	  fprintf(fo, "%d\t%f\t%s\n", nl, dist, s.c_str());
	}
    }

  std::string s = root->GetDataName();
  int nl = root->NLeaf();
  float dist = 0;

  fprintf(fo, "%d\t%f\t%s\n", nl, dist, s.c_str());

  fclose(fo);
  return 0;
}


