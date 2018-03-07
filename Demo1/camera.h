//
// Created by KimJunhyuk  on 2017/10/13.
//
#ifndef DEMO1_CAMERA_H
#define DEMO1_CAMERA_H
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include "identification.h"
#include <iostream>
#include "UserInterface.h"
using namespace std;
using namespace cv;
template<typename N>
class camera_iden
{
public:
    camera_iden(){}
    ~camera_iden(){}
    void recording();
    int screen();
    double PSNR(Mat &I1, Mat &I2);
    void CNN_classification();
    void SURF_classification();
    void new_species();
public:
    Mat out_image[20];
    char species[10]="";
    vector<string> rel_species;
    string nSpecies;       //species name
    int same_species_num; //calculate the number of same species in temp_dataset
    double relevant;      //relevant rate
    int best=3,record_time=7;      //determine the number of images to be recorded
    UI_part camera_view;
    string tip;
};
//do the recording
template<typename N>
void camera_iden<N>::recording()
{
    VideoCapture capture(0);
    capture.set(CV_CAP_PROP_FRAME_WIDTH, 300);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 300);
    int wait_time=3000;
    int inti_time=0;
    int i=0;
    cout<<"\033[5m ●\033[0m Camera Preparing....\n"<<endl;
    while (true){
        Mat frame;
        capture >> frame;
        imshow("读取视频", frame);
        if(inti_time<wait_time){
            cout<<"█";
        }
        else if(inti_time==wait_time){
            cout<<"100% \n Preparation complete\n";
            tip="Should we start to recording?";
            camera_view.ans_format(tip);
            char opt;
            safe_flush(stdin);
            scanf("%c",&opt);
            if(opt =='y') {
                tip= "Start Recording.......";
                camera_view.ensure_format(tip);
            }
            else
                continue;
        }
        else{
            std::cout<<"█";
            if(!(inti_time%1000)) {
                out_image[i] =frame.clone(); // put the value into the output;
                i++;
                printf(" Image Number %d recording completed!\n",i);
                if(i==record_time-1){
                    capture.release();
                    tip="recording completed!";
                    camera_view.ensure_format(tip);
                    break;
                }
            }
        }
        waitKey(30);
        inti_time +=100;
    }
}
template<typename N>
double camera_iden<N>::PSNR(Mat &I1, Mat &I2){
    Mat s1;
    absdiff(I1, I2, s1);
    s1.convertTo(s1, CV_32F);//转换为32位的float类型，8位不能计算平方
    s1 = s1.mul(s1);
    Scalar s = sum(s1);  //计算每个通道的和
    double sse = s.val[0] + s.val[1] + s.val[2];
    if( sse <= 1e-10) // for small values return zero
        return 0;
    else
    {
        double mse = sse / (double)(I1.channels() * I1.total()); //  sse/(w*h*3)
        double psnr = 10.0 * log10((255*255)/mse);
        return psnr;
    }
}
template<typename N>
int camera_iden<N>::screen(){
    double p[record_time];
    for(int i=0;i<record_time-2;i++){
        p[i] = PSNR(out_image[i],out_image[i+1]);
    }
    int i=0;
    while(p[i+1])
    {
        if(p[i+1]>p[i])
            best = i+1;
        i++;
    }
    return best;
}
template<typename N>
void camera_iden<N>::CNN_classification()
{
    tip="I Think SREEN ONE is the best image recorded!!\nGo on in classification?";
    camera_view.ans_format(tip);
    imshow("SREEN ONE",out_image[best]);
    waitKey(1);
    char opt;
    safe_flush(stdin);
    scanf("%c",&opt);
    if(opt =='y') {
        tip="Move to CNN model.......";
        camera_view.ensure_format(tip);
    }
    else
        return ;
    destroyWindow("SREEN ONE");
    //CNN_BASED method
    imwrite("/Users/kimjunhyuk/CLionProjects/Demo1/temp_information/temp.jpg", out_image[best]);
    string cmd="python3 /Users/kimjunhyuk/PycharmProjects/103_project_classification/singe_image_test.py "
            "/Users/kimjunhyuk/CLionProjects/Demo1/temp_information/";
    string result;
    execmd(cmd.c_str(),result); //run the CNN in python3
    vector<string> final_result;
    final_result = split(result,',');
    rel_species = split(result,',');
    final_result[1]= trim(final_result[1]);
    nSpecies = final_result[1];
    relevant=atof(final_result[2].c_str());
    same_species_num=stastic(("/Users/kimjunhyuk/CLionProjects/Demo1/Pic_data/"+nSpecies).c_str());
}
template<typename N>
void camera_iden<N>::SURF_classification()
{   //SURF_BASED method
    for(int i=0;i<rel_species.size();i++)
        rel_species[i]="";
    findInDir(&out_image[best],"/Users/kimjunhyuk/CLionProjects/Demo1/Pic_data","", &relevant,species,same_species_num);
    new_species();
    same_species_num=stastic(("/Users/kimjunhyuk/CLionProjects/Demo1/Pic_data/"+nSpecies).c_str());
}
template<typename N>
void camera_iden<N>::new_species()
{
    nSpecies = species;
    string temp;
    temp=species;
    nSpecies=nSpecies.substr(0,nSpecies.find('('));
}
#endif //DEMO1_CAMERA_H
