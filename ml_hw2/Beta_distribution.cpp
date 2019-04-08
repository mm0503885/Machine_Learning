#include <iostream>
#include <fstream>
#include <cstring>
#include <math.h>
#include <iomanip>

using namespace std;

/* Use online learning to learn the beta distribution of the parameter p (chance to see 1) of the coin
tossing trails in batch. */

char buffer[256];

unsigned int num_of_case;
unsigned int num_of_0;
unsigned int num_of_1;
unsigned int total_num;
unsigned int prior_a,prior_b;
double p,likehood;


double n_factorial_log(unsigned int n){  //取n階乘的log
	double temp=0;
	unsigned int x;
	for(x=1;x<=n;x++){
		temp+=log(x);
	}
	return temp;
}

int main()
{
	while(1){
	num_of_case=0;
	fstream input;
	input.open("input.txt",ios::in);

	cout<<"Enter parameter \"a\" for the initial beta prior: ";
	cin>>prior_a;
	cout<<"Enter parameter \"b\" for the initial beta prior: ";
	cin>>prior_b;
	while(input.getline(buffer,sizeof(buffer)))
	{
      num_of_case++;
      num_of_0=0;
      num_of_1=0;
      total_num=0;
      for(int i=0;i<strlen(buffer);i++)
      {
        if(buffer[i]=='0') num_of_0++;
        else num_of_1++;
        total_num++;
      }
      p=(double)num_of_1/total_num;  //最大likehood發生的點
	  
	  //對Binomial distribution 的公式取log 避免overflow
      likehood=log(pow(p,num_of_1))+log(pow(1-p,num_of_0))+n_factorial_log(total_num); 
      likehood-=n_factorial_log(num_of_1);
      likehood-=n_factorial_log(num_of_0);
	  
	  //取exp得到原本的值
      likehood=exp(likehood);

      cout<<"case "<<num_of_case<<": "<<buffer<<endl;
      cout<<"Likelihood: "<<fixed<<setprecision(15)<<likehood<<endl;
      cout<<"Beta prior:     a = "<<prior_a<<"  b = "<<prior_b<<endl;
      prior_a+=num_of_1;
      prior_b+=num_of_0;
      cout<<"Beta posterior: a = "<<prior_a<<"  b = "<<prior_b<<endl<<endl;
	}

	input.close();
	}
}
