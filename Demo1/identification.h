//
// Created by KimJunhyuk  on 2017/10/17.
//

#ifndef DEMO1_IDENTIFICATION_H
#define DEMO1_IDENTIFICATION_H
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/xfeatures2d.hpp"
using namespace std;
using namespace cv;
//tool
void safe_flush(FILE *fp);
bool isDir(const char* path);
int stastic(const char* path);
void findInDir(Mat* input_pic ,const char *path, const string& tofind,double* avg_relvul, char* type_name, int & number);
void findInDir(Mat* input_pic ,const char *path, const string& tofind, int recursive,double* avg_relvul,char* type_name, int& number);
//brighter
//.......
//identification prior
template<typename N>
class identifier
{
public:
    identifier(){}
    ~identifier(){}
    //example-based prior
    void CompImageHist(Mat &src, MatND &b_hist, MatND &g_hist, MatND &r_hist);
    void exp_based(Mat& pic1, Mat& pic2);
    void feature_based(Mat& pic1, Mat& pic2);
    //Covolutional prior
    void CNN_based(Mat& pic1);

public:
    double rel_value;
    double relF_value;
};
//example-based method
template<typename N>
void identifier<N>::CompImageHist(Mat &src, MatND &b_hist, MatND &g_hist, MatND &r_hist)
{
    // 分割成3个单通道图像(bgr)
    vector<Mat> rgb_planes;
    split(src, rgb_planes);
    // 设定bin数目及取值范围
    int histSize = 255;
    float range[] = { 0, 255 };
    const float* histRange = { range };
    // 计算直方图
    bool uniform = true;
    bool accumulate = false;
    calcHist(&rgb_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
    calcHist(&rgb_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
    calcHist(&rgb_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);
    // 直方图归一化>>范围为[0, 1]
    normalize(r_hist, r_hist, 0, 1, NORM_MINMAX, -1/*, Mat()*/);
    normalize(g_hist, g_hist, 0, 1, NORM_MINMAX, -1/*, Mat()*/);
    normalize(b_hist, b_hist, 0, 1, NORM_MINMAX, -1/*, Mat()*/);
}
template<typename N>
void identifier<N>::exp_based(Mat& pic1,Mat& pic2)
{
    MatND hist0[3], hist1[3];
    // 计算图像直方图
    CompImageHist(pic1, hist0[0], hist0[1], hist0[2]);
    CompImageHist(pic2, hist1[0], hist1[1], hist1[2]);
    double sum[4] = { 0.0 };
    double results[4] = { 0.0 };
    char channelName[][8] = { { "蓝色" }, { "绿色" }, { "红色" } };
    // 比较直方图
    for (int i = 0; i < 3; i++)
    {
        // 相关: CV_COMP_CORREL,卡方: CV_COMP_CHISQR,相交: CV_COMP_INTERSECT,巴氏: CV_COMP_BHATTACHARYYA
        results[0] = compareHist(hist0[i], hist1[i], CV_COMP_CORREL);
        results[1] = compareHist(hist0[i], hist1[i], CV_COMP_CHISQR);
        results[2] = compareHist(hist0[i], hist1[i], CV_COMP_INTERSECT);
        results[3] = compareHist(hist0[i], hist1[i], CV_COMP_BHATTACHARYYA);
        sum[0] += results[0];
        sum[1] += results[1];
        sum[2] += results[2];
        sum[3] += results[3];
        // printf("%s-->相关: %f, 卡方: %f, 相交: %f, 巴氏: %f \n", channelName[i], results[0], results[1], results[2], results[3]);
    }
    printf("\n均值-->相关: %f, 卡方: %f, 相交: %f, 巴氏: %f \n", sum[0]/3, sum[1]/3, sum[2]/3, sum[3]/3);
    rel_value = sum[0]/3;
}
template<typename N>
void identifier<N>::feature_based(Mat& pic1,Mat& pic2)
{
    //Create SIFT class pointer
    Ptr<Feature2D> f2d = xfeatures2d::SURF::create();
    Mat srcGrayImage_1;
    cvtColor(pic1, srcGrayImage_1, CV_BGR2GRAY);
    Mat srcGrayImage_2;
    cvtColor(pic2, srcGrayImage_2, CV_BGR2GRAY);
    //Detect the keypoints
    vector<KeyPoint> keypoints_1, keypoints_2;
    f2d->detect(srcGrayImage_1, keypoints_1);
    f2d->detect(srcGrayImage_2, keypoints_2);
    //Calculate descriptors (feature vectors)
    Mat descriptors_1, descriptors_2;
    f2d->compute(srcGrayImage_1, keypoints_1, descriptors_1);
    f2d->compute(srcGrayImage_2, keypoints_2, descriptors_2);
    //cross-checking to screen
    /* BFMatcher FLMatcher;
    //因为FlannBasedMatcher类的成员函数add()的参数是一个vector<Mat>的容器，所以先定义一个这样的变量，并将原图的描述子放入容器中
    vector<Mat> g_vdescriImage1(1, descriptors_1);
    //调用FlannBasedMatcher类的成员函数add，将原图的描述子放在FlannBasedMatcher的对象FLMatcher中
    FLMatcher.add(g_vdescriImage1);
    //...........................................................
    FLMatcher.train();
    vector<vector<DMatch>> knnDMatches;
    //用之前已经存放原图描述子的对象来计算
    FLMatcher.knnMatch(descriptors_2, knnDMatches, 2);
    //采集优秀的匹配点
    vector<DMatch> goodMatches;
    int points = knnDMatches.size();
    int goodPoints = 0;
    for (size_t i = 0; i < knnDMatches.size(); i++)
    {
        //........................................................................
        if (knnDMatches[i][0].distance < 0.6 * knnDMatches[i][1].distance)
        {
            goodMatches.push_back(knnDMatches[i][0]);
            goodPoints++;
        }
    }*/
    //Matching descriptor vector using BFMatcher(basic)
    BFMatcher matcher;
    vector<DMatch> matches;
    vector<DMatch> Per_matches;
    int Matchpoints_dis = 0;
    matcher.match(descriptors_1, descriptors_2, matches);
    int points=matches.size();
    for (size_t i = 0; i < matches.size(); i++)
    {
        //........................................................................
        if (matches[i].distance <0.178)
        {
            Per_matches.push_back(matches[i]);
            Matchpoints_dis++;
        }
    }
    //绘制匹配出的关键点
    Mat img_matches;
    relF_value=2*(double)Matchpoints_dis/(double)points;
    drawMatches(pic1, keypoints_1, pic2, keypoints_2, Per_matches, img_matches);
    imshow("【match图】", img_matches);
    //等待任意按键按下
    waitKey(1);
}
//tool functions
//clear stdin
void safe_flush(FILE *fp)
{
    int ch;
    while( (ch = fgetc(fp)) != EOF && ch != '\n' );
}
// find in dir
bool isDir(const char* path)
{
    struct stat st;
    lstat(path, &st);
    return 0 != S_ISDIR(st.st_mode);
}
int stastic(const char* path)
{
    DIR *pdir;
    struct dirent *pdirent;
    int count=0;
    pdir = opendir(path);
    if(pdir) {
        while((pdirent = readdir(pdir)))
        {
            if(strcmp(pdirent->d_name, ".") == 0 || strcmp(pdirent->d_name, "..") == 0|| strcmp(pdirent->d_name, ".DS_Store" ) == 0)
                continue;
            count++;
        }
    }
    return count;
}
void findInDir( Mat* input_pic,const char *path, const string& tofind,double* avg_relvul,char* type_name, int& number)
{
    unsigned long len;
    char temp[256];
    //去掉末尾的'/'
    len = strlen(path);
    strcpy(temp, path);
    if(temp[len - 1] == '/')
        temp[len -1] = '\0';
    if(isDir(temp))
    {
        //处理目录
        int recursive = 1;
        findInDir(input_pic,temp, tofind, recursive, avg_relvul,type_name, number);
    }
    else   //输出文件
    {
        printf("path:%s\n", path);
       // myFind(path, tofind);
    }
}
void findInDir(Mat* input_pic ,const char *path, const string& tofind, int recursive,double* avg_relvul,char* type_name, int& number)
{
    DIR *pdir;
    struct dirent *pdirent;
    char temp[256];
    try {
        pdir = opendir(path);
    }
    catch(const char *str)
    {printf("failed open dir");}
    int index_count=0;
    if(pdir) {
        while((pdirent = readdir(pdir)))
        {
            //跳过"."和".."
            if(strcmp(pdirent->d_name, ".") == 0 || strcmp(pdirent->d_name, "..") == 0)
                continue;
            sprintf(temp, "%s/%s", path, pdirent->d_name);
            //当temp为目录并且recursive为1的时候递归处理子目录
            if(isDir(temp) && recursive) {
                findInDir(input_pic,temp, tofind, recursive, avg_relvul,type_name,number);
            }
            else {
                if(strcmp(pdirent->d_name,".DS_Store")) {
                    //idetentify part
                    if(index_count == 0)
                    {
                        //strcpy(type_name, pdirent->d_name);
                    }
                    identifier<Mat> iden;
                    Mat *temp_pic = new Mat(imread(temp));
                    iden.feature_based(*input_pic, *temp_pic);
                    if(iden.relF_value > *avg_relvul) {
                        *avg_relvul = iden.relF_value;
                        strcpy(type_name, pdirent->d_name);
                        index_count++;
                    }
                    cout<<*avg_relvul<<endl;
                    delete temp_pic;

                }
               // myFind(temp, tofind);
            }
        }
    }
    else{
        printf("opendir error:%s\n", path);
    }
    closedir(pdir);
}
//system cmd
int execmd(const char* cmd,string &result) {
    char buffer[128];                         //定义缓冲区
    FILE* pipe = popen(cmd, "r");            //打开管道，并执行命令
    if (!pipe)
        return 0;                      //返回0表示运行失败
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe)){             //将管道输出到result中
            result =result+ buffer;
        }
    }
    pclose(pipe);                            //关闭管道
    return 1;                                 //返回1表示运行成功
}
//string dealing
vector<string> split(string s,char token){
    istringstream iss(s);
    string word;
    vector<string> vs;
    while(getline(iss,word,token)){
        vs.push_back(word);
    }
    return vs;
} //spilt by ""
string trim(string s){
    if( !s.empty() ){
         s.erase(0,s.find_first_not_of(" "));
         s.erase(s.find_last_not_of(" ") + 1);
    }
    return s;
} //remove space
#endif //DEMO1_IDENTIFICATION_H
