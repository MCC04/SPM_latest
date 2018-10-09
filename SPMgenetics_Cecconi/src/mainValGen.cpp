#include <cstdio>
#include <vector>
#include <string>
#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <cstdlib>
#include <math.h>

/*I'm a function values generator!*/
int main(int argc, char const *argv[])
{
  double range_min=-40.0;
  double range_max=10.0;
  double incr=0.5;
  //double incr=0.25;
  std::vector<double> xv;
  std::vector<double> y;

  for(double x=range_min; x<range_max; x+=incr){
    xv.push_back(x);    
    y.push_back(3*x + sin(2*x));//y.push_back(exp(x) - cos(x));
  }

  std::cout << y.size() << "\n";
  std::cout <<"\n";
  for(auto item : xv)
    std::cout << item << " ";

  std::cout << "\n"<<"\n";

  for(auto item : y)
    std::cout << item << " ";
       
  std::cout<<std::endl;

  std::getchar();
  return 0;
}
