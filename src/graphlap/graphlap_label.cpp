#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <ctime>
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml/ml.hpp>
using namespace std;
using namespace cv;
/*******************varun headers*********************/
#define MAX_SUPERPIXEL 2148
#define MAX_FEATURE_SIZE 59
#define INITIAL_VAL 0
#define VAL_ONE     1
#define MAX_BUFFER_FEATURE_FILE 2148
#define DELIMINATOR_FEATURE_FILE "   "
#define DELIMINATOR_SINGLE_SPACE " "
#define DELIMINATOR_COMMA ","
#define SIGMA_VAL    2
#define MAX_LABELS 5
/**********************varun function prototypes********/
vector<vector<double>>feature_arr(MAX_SUPERPIXEL,vector<double>(MAX_FEATURE_SIZE)) ;
void reading_features_from_file(void );
void finding_neighbour_pixels( void);
void reading_label_from_svm(Mat&);
vector<vector<int>>neighbour_list;
void SemiSupervised(Mat&,float,Mat&);
int main()
{       float alpha;
        cout<<"************************************************************************************************"<<endl;
        cout<<"Enter the value of alpha"<<endl;
        cin>>alpha;
        cout<<"Wait for the result!!!"<<endl;
        Mat label_arr(MAX_SUPERPIXEL,MAX_LABELS,CV_32FC1);
	reading_features_from_file();
	finding_neighbour_pixels();
	Mat result(MAX_LABELS,MAX_SUPERPIXEL,CV_32FC1,Scalar(INITIAL_VAL));
        reading_label_from_svm(label_arr);

	//Initialising the label vector 
//	label_arr.at<float>(0, 0) = 1.0;         

//	label_arr.at<float>(9, 0) = -1.0;  
//        for(int i=0;i<MAX_LABELS;i++)
	SemiSupervised(result,alpha,label_arr);
	return 0;
}
void SemiSupervised(Mat&result,float alpha,Mat&label_arr)
{
	float dist,dist1;
         double min,max;
	int i,j,k;
        Point loc_min,loc_max;
	Mat affinity(MAX_SUPERPIXEL,MAX_SUPERPIXEL,CV_32FC1,Scalar(0));
       ofstream outputFile;
       outputFile.open("result_label_graphlap.txt");
	for(i=INITIAL_VAL;i<MAX_SUPERPIXEL;i++)
	{
		for (j=INITIAL_VAL;j<(int)neighbour_list[i].size();j++)
		{

			//			for(k=0;k<MAX_FEATURE_SIZE;k++)
			//			{

			dist=norm(feature_arr[i], feature_arr[neighbour_list[i][j]-1], NORM_L2);
			/***************Debugging prints **************************************/
			//  dist1=0;
			//for(k=0;k<MAX_FEATURE_SIZE;k++)
			//dist1=pow((feature_arr[i][k]-feature_arr[neighbour_list[i][j]-1][k]),2)+dist1;
			//cout<<" *"<<dist1;
//			cout<<" "<<dist;
			affinity.at<float>(i,neighbour_list[i][j]-1 ) = exp((-dist) / (2 * pow(SIGMA_VAL, 2)));
			affinity.at<float>(neighbour_list[i][j]-1, i) = affinity.at<float>(i, neighbour_list[i][j]-1);
			//			}}


	}
	affinity.at<float>(i,i )=VAL_ONE;
//	cout<<endl;
}
/********************Debugging prints**************************************/
/**
   for(j=0;j<MAX_SUPERPIXEL;j++)
   {for(i=0;i<MAX_SUPERPIXEL;i++)
   cout<<"      "<<affinity.at<float>(j,i)<<" " ;
   cout<<endl;

   }
**/     
Mat Dmat(MAX_SUPERPIXEL, MAX_SUPERPIXEL, CV_32FC1,Scalar(0));      // Dmat is the rows sum of Wmat

for (int ii = INITIAL_VAL; ii < MAX_SUPERPIXEL; ii++)
{
        
	Dmat.at<float>(ii, ii) = pow(sum(affinity.col(ii))(0), -0.5);
//        cout<<"sum"<<"   "<<sum(affinity.col(ii))(0);
//        cout<<"sum"<<"   "<<Dmat.at<float>(0,ii);
}
//cout<<endl;
/******************************Debugging prints*********************************/
/*    for(j=0;j<MAX_SUPERPIXEL;j++)
      cout<<"Dmat-----"<<" "<<Dmat.at<float>(0, j);
*/
Mat Smat(MAX_SUPERPIXEL,MAX_SUPERPIXEL,CV_32FC1,Scalar(INITIAL_VAL)), DmatT(MAX_SUPERPIXEL,1,CV_32FC1,Scalar(INITIAL_VAL));    // Smat is the normalized version of Wmat

//transpose(Dmat, DmatT);
/*
for (int ii = 0; ii < MAX_SUPERPIXEL; ii++)
{
cout<<" DmatT"<<" "<<DmatT.at<float>(ii,0);
}*/
Smat = affinity* Dmat;

//cout<<"Smat"<<endl;
/*
for(j=0;j<MAX_SUPERPIXEL;j++)
   {for(i=0;i<MAX_SUPERPIXEL;i++)
   cout<<Smat.at<float>(j,i)<<" " ;
   cout<<endl;

   }
*/

Smat = Dmat* Smat;


Mat Imat = Mat::eye(MAX_SUPERPIXEL, MAX_SUPERPIXEL, CV_32FC1);   // identity matrix

/******************************Debugging prints*********************************/
/*
   for(j=0;j<MAX_SUPERPIXEL;j++)
   {for(i=0;i<MAX_SUPERPIXEL;i++)
   cout<<Smat.at<float>(j,i)<<" " ;
   cout<<endl;

   }
cout<<endl<<"Dmat"<<endl;
 for(j=0;j<MAX_SUPERPIXEL;j++)
   {for(i=0;i<MAX_SUPERPIXEL;i++)
   cout<<Dmat.at<float>(j,i)<<" " ;
   cout<<endl;

   }
 */

// Manifold Learning equation: F = (I-alpha*S)^(-1) * Y  

//alpha=(lambda/(lambda+1));
//cout<<scientific;
//cout<<endl<<alpha<<endl;
Mat Fmat(MAX_SUPERPIXEL,MAX_SUPERPIXEL,CV_32FC1,Scalar(INITIAL_VAL)),Ftemp(MAX_SUPERPIXEL,VAL_ONE,CV_32FC1,Scalar(INITIAL_VAL));
Fmat=(Imat-(VAL_ONE-alpha)*Smat);
/*
for(j=0;j<MAX_SUPERPIXEL;j++)
   { for (i=0;i<MAX_SUPERPIXEL;i++)
   cout<<"* "<<Fmat.at<float>(j,i)<<" " ;
   cout<<endl;
   }
*/


Mat TransFmat(VAL_ONE,MAX_SUPERPIXEL,CV_32FC1);

for(int index=INITIAL_VAL;index<MAX_LABELS;index++)
{/*
for (int ii = 0; ii < MAX_SUPERPIXEL; ii++)
 {
 cout<<" Label_arr "<<" "<<label_arr.at<float>(ii,index);
 }
*/
//int index=0;
//TempMat=label_arr.col(index).clone();
//label_arr.col(index).copyTo(TempMat);
//for (i=0;i<MAX_SUPERPIXEL;i++)
  // cout<<"* "<<TempMat.at<double>(i,0)<<" " ;

solve(Fmat,alpha*label_arr.col(index),Ftemp);
//Ftemp =Fmat * label_arr.col(index) ;   
//Mat result(MAX_SUPERPIXEL,1,CV_32FC1,Scalar(0));
 //for (i=0;i<MAX_SUPERPIXEL;i++)
  // cout<<"* "<<Ftemp.at<float>(i,0)<<" " ;
//cout<<endl<<endl;
transpose(Ftemp,TransFmat);
//for (i=0;i<MAX_SUPERPIXEL;i++)
 //  cout<<"* "<<TransFmat.at<float>(i,0)<<" " ;
//cout<<endl;
TransFmat.copyTo(result.row(index));
//for (i=0;i<MAX_SUPERPIXEL;i++)
  // cout<<"* "<<result.at<float>(index,i)<<" " ;
//cout <<endl;
//}
}
/******************************Debugging prints*********************************/
 for(j=INITIAL_VAL;j<MAX_SUPERPIXEL;j++){
      minMaxLoc(result.col(j),&min,&max,&loc_min,&loc_max);
 
        outputFile<<loc_max.y+1<<endl;

}
outputFile.close();
cout <<endl<< "Hurray!!! Check the file result_label_graphlap.txt to get the new labels after graph_laplacian\n";
}
void reading_label_from_svm(Mat&label_arr)
{
int i=0,j=0;
//Mat label_arr(MAX_SUPERPIXEL,MAX_LABELS,CV_32FC1);
ifstream fin;
char buff[MAX_BUFFER_FEATURE_FILE]={INITIAL_VAL};
char *temp;
fin.open("multicolumn_result_label.txt");
if(!fin.good())
                return ;
while((!fin.eof())&& (i<=MAX_SUPERPIXEL-1))
{

		fin.getline(buff,MAX_BUFFER_FEATURE_FILE);
                temp=strtok(buff,DELIMINATOR_SINGLE_SPACE);
                while(temp!=NULL)
                 {
                  
                 label_arr.at<float>(i, j++) = stof(temp,NULL);
                 temp=strtok(NULL,DELIMINATOR_SINGLE_SPACE);
//                 cout<<" "<<label_arr.at<double>(i,j-1); 


                 }
//cout<<endl;
j=0;i++;
}


}
void reading_features_from_file(void)
{
	int i=0,j=0;
	ifstream fin;
	char buff[MAX_BUFFER_FEATURE_FILE]={INITIAL_VAL};
	char *temp;
	fin.open("spdescrs.txt");
	if(!fin.good())
		return ;
	while((!fin.eof())&&(i<=(MAX_SUPERPIXEL-1)))
	{
		fin.getline(buff,MAX_BUFFER_FEATURE_FILE);
		temp=strtok(buff,DELIMINATOR_FEATURE_FILE);
		//cout<<temp<<" " <<"***"<<stod(temp,NULL);
	//	cout<<scientific;
		feature_arr[i][j++]=stof(temp,NULL);
		// cout<< feature_arr[i][j-1]<<"   ";
		while(temp!=NULL)
		{
			temp=strtok(NULL,DELIMINATOR_FEATURE_FILE);
			// cout<<temp<<" " <<"-------"<<stod(temp,NULL)<<"\n";
			if(temp!=NULL){
				feature_arr[i][j++]=stof(temp,NULL);
				//cout<<temp<<" " <<"-------"<<stod(temp,NULL)<<"\n";
				//cout<< feature_arr[i][j-1]<<"   " ;
			}
		}
		//cout <<endl;
		i++,j=0;

	}
}
void finding_neighbour_pixels(void)
{
	int i=0,temp_num=0,j=0;
	ifstream fin;
	vector<int>temp;
	char *temp_var;
	char buff[MAX_BUFFER_FEATURE_FILE]={INITIAL_VAL};
	fin.open("adjacency_list.txt");
	if(!fin.good())
		return;
	while((!fin.eof())&&(i<=(MAX_SUPERPIXEL-1)))
	{
		fin.getline(buff,MAX_BUFFER_FEATURE_FILE);
		temp_var=strtok(buff,DELIMINATOR_SINGLE_SPACE);
		while(temp_var!=NULL)
		{
			temp_var=strtok(NULL,DELIMINATOR_COMMA);
			if(temp_var!=NULL)
			{
				temp.push_back(stoi(temp_var,NULL));

			}}
		neighbour_list.push_back(temp);
		//		cout<<"size="<<neighbour_list.size()<<"i="<<i<<"\n";
//	  	for(j=0;j<neighbour_list[i].size();j++)
//			cout<<neighbour_list[i][j]<<" ";
		i++;
		temp.resize(0);
	//	cout<<endl;
	}
}
