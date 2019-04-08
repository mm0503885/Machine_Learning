#include<iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
using namespace std;

//data set:  http://yann.lecun.com/exdb/mnist/ 


//train data
char train_pixel[60000][28][28];
unsigned int train_pixel_i[60000][28][28];
char train_ans[60000];
unsigned int train_ans_i[60000];
unsigned int train_pixel_level[60000][28][28];  //把每個pixel根據value分成32個level

unsigned int digit_count[10]={0};
double prior[10];
unsigned int likehood_count[10][28][28][32]={0};
double likehood[10][28][28][32];

//test data
char test_pixel[10000][28][28];
unsigned int test_pixel_i[10000][28][28];
char test_ans[10000];
unsigned int test_ans_i[10000];
unsigned int test_pixel_level[10000][28][28];

double Postirior[10000][10]={0};
double Postirior_continous[10000][10]={0};

unsigned int Prediction[10000];
unsigned int Prediction_continous[10000];

unsigned int error_counter=0;
unsigned int error_counter_continous=0;
double error_rate;
double error_rate_continous;

unsigned int Imaginary_counter[10][28][28][2]={0};
unsigned int Imaginary_picture[10][28][28];


double mean[10][28][28]={0};
double variance[10][28][28]={0};

int Toggle_option;



int main()
{
	//training
    fstream  train_data;
    fstream  train_label;
	train_data.open("train-images-idx3-ubyte",ios::in) ;
	train_label.open("train-labels-idx1-ubyte",ios::in) ;

    char buffer[16];
    char buffer2[8];
	
	//讀取前面用不到的資訊
	train_data.read(buffer,16);
    train_label.read(buffer2,8);
	
	//讀取每筆train data的answer,轉成unsigned int,再統計每種答案的數量
	for(int i=0;i<60000;i++)
    {
      train_label.get(train_ans[i]);
      train_ans_i[i]=(unsigned int)(unsigned char)train_ans[i];
	  digit_count[train_ans_i[i]]++;
	}

	//讀取每個pixel
	for(int i=0;i<60000;i++)
      for(int j=0;j<28;j++)
        for(int k=0;k<28;k++)
        {
          train_data.get(train_pixel[i][j][k]);
	      train_pixel_i[i][j][k]=(unsigned int)(unsigned char)train_pixel[i][j][k];
		  train_pixel_level[i][j][k]=train_pixel_i[i][j][k]/8;
		  likehood_count[train_ans_i[i]][j][k][train_pixel_level[i][j][k]]++;
		  mean[train_ans_i[i]][j][k]+=train_pixel_i[i][j][k];
		  if(0<=train_pixel_i[i][j][k] && train_pixel_i[i][j][k]<=127)
		    Imaginary_counter[train_ans_i[i]][j][k][0]++;
		  else
			Imaginary_counter[train_ans_i[i]][j][k][1]++;
        }
		
	//計算每種數字每個pixel value的平均	
    for(int i=0;i<10;i++)
      for(int j=0;j<28;j++)
        for(int k=0;k<28;k++)
		  mean[i][j][k]/=digit_count[i];
    
	//計算每種數字每個pixel value的變異數
    for(int i=0;i<60000;i++)
      for(int j=0;j<28;j++)
        for(int k=0;k<28;k++)
	      variance[train_ans_i[i]][j][k]+=pow((train_pixel_i[i][j][k]-mean[train_ans_i[i]][j][k]),2);
	
	for(int i=0;i<10;i++)
      for(int j=0;j<28;j++)
        for(int k=0;k<28;k++)
		{
		  variance[i][j][k]/=digit_count[i];
		  if(variance[i][j][k]<=10) variance[i][j][k]=10;
		}
		
	for(int i=0;i<10;i++)
      for(int j=0;j<28;j++)
        for(int k=0;k<28;k++)
          if(Imaginary_counter[i][j][k][0]>Imaginary_counter[i][j][k][1])
		    Imaginary_picture[i][j][k]=0;
		  else
			Imaginary_picture[i][j][k]=1;
	
	for(int i=0;i<10;i++)
	  prior[i]=log((float)digit_count[i]/60000);	
  
    for(int i=0;i<10;i++)
      for(int j=0;j<28;j++)
        for(int k=0;k<28;k++)
		  for(int l=0;l<32;l++)
		  {
			if(likehood_count[i][j][k][l]==0) likehood_count[i][j][k][l]++;
		    likehood[i][j][k][l]=log((double)likehood_count[i][j][k][l]/digit_count[i]);
	      }
		  
	     	

    train_data.close();
    train_label.close();
    
	
	//testing
	
	fstream  test_data;
    fstream  test_label;
	test_data.open("t10k-images-idx3-ubyte",ios::in) ;
	test_label.open("t10k-labels-idx1-ubyte",ios::in) ;
	
	char buffer3[16];
    char buffer4[8];
	test_data.read(buffer3,16);
    test_label.read(buffer4,8);
	
	for(int i=0;i<10000;i++)
    {
      test_label.get(test_ans[i]);
      test_ans_i[i]=(unsigned int)(unsigned char)test_ans[i];
	}
	
	for(int i=0;i<10000;i++)
      for(int j=0;j<28;j++)
        for(int k=0;k<28;k++)
        {
          test_data.get(test_pixel[i][j][k]);
	      test_pixel_i[i][j][k]=(unsigned int)(unsigned char)test_pixel[i][j][k];
		  test_pixel_level[i][j][k]=test_pixel_i[i][j][k]/8;
        }
		
	test_data.close();
    test_label.close();
	
	double sum,sum_2;
	unsigned int min,min_2;
	for(int i=0;i<10000;i++)
	{
	   sum=0;
	   sum_2=0;
	   min=0;
	   min_2=0;
	   for(int n=0;n<10;n++)
	   {
	     for(int j=0;j<28;j++)
           for(int k=0;k<28;k++)
		   {
		      Postirior[i][n]+=likehood[n][j][k][test_pixel_level[i][j][k]];
			  Postirior_continous[i][n]-=log(sqrt(2*M_PI*variance[n][j][k]));
			  Postirior_continous[i][n]+=((-pow((test_pixel_i[i][j][k]-mean[n][j][k]),2))/(2*variance[n][j][k]));
		   }
		 Postirior[i][n]+=prior[n];
		 Postirior_continous[i][n]+=prior[n];
		 sum+=Postirior[i][n];
		 sum_2+=Postirior_continous[i][n];
	   }
	   for(int n=0;n<10;n++)
	   {
	     Postirior[i][n]/=sum;
		 Postirior_continous[i][n]/=sum_2;
		 if(Postirior[i][n]<Postirior[i][min])
		   min=n;
	     if(Postirior_continous[i][n]<Postirior_continous[i][min_2])
		   min_2=n;
	   }
	   Prediction[i]=min;
	   Prediction_continous[i]=min_2;
	   if(Prediction[i]!=test_ans_i[i])
	   error_counter++;
       if(Prediction_continous[i]!=test_ans_i[i])
	   error_counter_continous++;
	}
	
	error_rate=(double)error_counter/10000;
	error_rate_continous=(double)error_counter_continous/10000;
	
	  cout<<"Enter the Toggle option(0 or 1): ";
	  cin>>Toggle_option;
	
	fstream output;
	output.open("output.txt",ios::out) ;
	
	

	  for(int n=0;n<10000;n++)
	  {
	    output<<"Postirior (in log scale):"<<endl;
		if(Toggle_option==1)
	    {
	      for(int i=0;i<10;i++)
	        output<<i<<": "<<fixed<< setprecision(15)<<Postirior_continous[n][i]<<endl;
          output<<"Prediction: "<<Prediction_continous[n]<<", Ans: "<<test_ans_i[n]<<endl<<endl;
		}
		else
		{
	      for(int i=0;i<10;i++)
	        output<<i<<": "<<fixed<< setprecision(15)<<Postirior[n][i]<<endl;
          output<<"Prediction: "<<Prediction[n]<<", Ans: "<<test_ans_i[n]<<endl<<endl;
		}
      }
	
	  output<<"Imagination of numbers in Bayesian classifier:"<<endl<<endl;
	  for(int n=0;n<10;n++)
	  {
	     output<<n<<":"<<endl;
	     for(int i=0;i<28;i++)
	     {
	       for(int j=0;j<28;j++)
	         output<<Imaginary_picture[n][i][j]<<" ";
	       output<<endl;
	     }
	     output<<endl;
	  }
	  if(Toggle_option==1)
	    output<<"Error rate: "<<fixed<< setprecision(4)<<error_rate_continous<<endl;
	  else
		output<<"Error rate: "<<fixed<< setprecision(4)<<error_rate<<endl;
	  output.close();
	
	
}
