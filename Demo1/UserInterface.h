//
// Created by KimJunhyuk  on 2017/10/26.
//
#ifndef DEMO1_USERINTERFACE_H
#define DEMO1_USERINTERFACE_H
#include <iostream>
using namespace std;
class UI_part {
public:
    void main_UI();
    void information_UI();
    void ans_format(string &answer);
    void ensure_format(string &ensure);
    void camera_view_top();
    void camera_view_ensure(const string name,const vector<string> result);
};
void UI_part::main_UI()
{
    cout<<"--------------------------------------------------------------------------------"<<endl;
    cout<<"\n             welcome to the animal identification system!\n\n\n\n"<<endl;
    cout<<"                           \033[1m 1).Pics Identify \n"<<endl;
    cout<<"                            2).Camera Identify\n"<<endl;
    cout<<"                            3).System information\n"<<endl;
    cout<<"                            4).Exit\033[0m\n\n\n\n\n"<<endl;
    cout<<"                                                 \033[3m [8]help  [9]writers \033[0m\n"<<endl;
    cout<<"--------------------------------------------------------------------------------"<<endl;
}
void UI_part::information_UI()
{
    cout<<"--------------------------------------------------------------------------------"<<endl;
    cout<<"\n           \n\n\n\n"<<endl;
    cout<<"                     \033[1m 1).Show data information in temporary dataset \n"<<endl;
    cout<<"                      2).Show data information in training dataset\n"<<endl;
    cout<<"                      3).Updating\n"<<endl;
    cout<<"                      4).Retraining\n"<<endl;
    cout<<"                      5).Back to menue\033[0m\n\n\n"<<endl;
    cout<<"\n"<<endl;
    cout<<"--------------------------------------------------------------------------------"<<endl;
}
void UI_part::ans_format(string &answer)
{
    cout<<"--------------------------------------------------------------------------------"<<endl;
    cout<<"\n\n\n\n\n"<<endl;
    cout<<"\n"<<endl;
    cout<<" \033[1m"<<answer<<"\033[0m\n"<<endl;
    cout<<"                                 \033[1m[y]/[n]\033[0m\n"<<endl;
    cout<<"\n\n\n\n\n"<<endl;
    cout<<"\n"<<endl;
    cout<<"--------------------------------------------------------------------------------"<<endl;
}
void UI_part::ensure_format(string &ensure)
{
    cout<<"--------------------------------------------------------------------------------"<<endl;
    cout<<"\n\n\n\n\n"<<endl;
    cout<<"\n"<<endl;
    cout<<"             \033[1m"<<ensure<<"\033[0m\n"<<endl;
    cout<<"\n"<<endl;
    cout<<"\n\n\n\n\n"<<endl;
    cout<<"\n"<<endl;
    cout<<"--------------------------------------------------------------------------------"<<endl;
}
void UI_part::camera_view_top()
{
}
void UI_part::camera_view_ensure(const string name,const vector<string> result)
{
    cout<<"--------------------------------------------------------------------------------"<<endl;
    cout<<"\n\n\n\n\n"<<endl;
    cout<<"\n"<<endl;
    cout<<"   \033[1m"<<"Is that animal is the specise of "<< name.c_str()<<"?\033[0m\n"<<endl;
    cout<<"                                 \033[1m[y]/[n]\033[0m\n"<<endl;
    cout<<"   Top 5 results of CNN:\n"<<endl;
    for(int i=1;i<result.size()-1;i+=2)
    {
        cout<<"             "<<result[i]<<":"<<result[i+1]<<endl;
    }
    cout<<"--------------------------------------------------------------------------------"<<endl;
}
#endif //DEMO1_USERINTERFACE_H
