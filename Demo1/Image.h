//
// Created by KimJunhyuk  on 2017/11/7.
//

#ifndef DEMO1_IMAGE_H
#define DEMO1_IMAGE_H
#include <opencv2/opencv.hpp>
#include "identification.h"
#include "UserInterface.h"
#include <stdio.h>
#include <string.h>
using namespace std;
using namespace cv;
//Camera_iden
template<typename N>
class image_iden
{
public:
    image_iden(){}
    ~image_iden(){}
    bool address_input();
    void CNN_classification();
    void SURF_classification();
    void new_species();
public:
    Mat out_image;
    char species[10];
    vector<string> rel_species;
    string nSpecies;
    int same_species_num;
    double relevant=0.0;
    string address;
    UI_part image_view;
    string tip;
};
template<typename N>
bool image_iden<N>::address_input()
{
    tip="Please input the image adress:";
    image_view.ensure_format(tip);
    safe_flush(stdin);
    cin>>address;
    cin.ignore(INT_MAX, '\n');
    out_image = imread(address);
    if(!out_image.data)
    {
        return false;
    }
    else
        return true;
}
template<typename N>
void image_iden<N>::CNN_classification()
{
    tip="Move to CNN model.......";
    image_view.ensure_format(tip);
    //CNN_BASED method
    imwrite("/Users/kimjunhyuk/CLionProjects/Demo1/temp_information/temp.jpg", out_image);
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
void image_iden<N>::SURF_classification()
{   //SURF_BASED method
    for(int i=0;i<rel_species.size();i++)
        rel_species[i]="";
    findInDir(&out_image,"/Users/kimjunhyuk/CLionProjects/Demo1/Pic_data","", &relevant,species,same_species_num);
    new_species();
    same_species_num=stastic(("/Users/kimjunhyuk/CLionProjects/Demo1/Pic_data/"+nSpecies).c_str());
}
template<typename N>
void image_iden<N>::new_species()
{
    nSpecies = species;
    string temp;
    temp=species;
    nSpecies=nSpecies.substr(0,nSpecies.find('('));
}
#endif //DEMO1_IMAGE_H
