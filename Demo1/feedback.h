//
// Created by KimJunhyuk  on 2017/10/26.
//

#ifndef DEMO1_FEEDBACK_H
#define DEMO1_FEEDBACK_H
#include <iostream>
using namespace std;
template <typename N>
class feedback
{
public:
    void new_saved(N& input);
    void update_exist(N& input, string name,int order);
    void upto_training(string &name);
    void show_temp_data();
    void show_training_data();
    void save_to_training(N &input, string name);
    void retraining();
};
template <typename N>
void feedback<N>::new_saved(N &input)
{
    cout<<"Input ur new species name:"<<endl;
    safe_flush(stdin);
    string name;
    string temp;
    cin>>name;
    DIR *pdir;
    struct dirent *pdirent;
    pdir = opendir("/Users/kimjunhyuk/CLionProjects/Demo1/Pic_data/");
    cout<<"The Temporary Dataset:"<<endl;
    while(pdirent = readdir(pdir)) {
        if (strcmp(pdirent->d_name, ".") == 0 || strcmp(pdirent->d_name, "..") == 0 ||
            strcmp(pdirent->d_name, ".DS_Store") == 0)
            continue;
        if(strcmp(pdirent->d_name, name.c_str())==0) {
            cout << "This specise already exited! Would u like to Update?" << endl;
            char opt;
            safe_flush(stdin);
            scanf("%c", &opt);
            if (opt == 'y') {
                int order=stastic(("/Users/kimjunhyuk/CLionProjects/Demo1/Pic_data/"+name).c_str());
                imwrite("/Users/kimjunhyuk/CLionProjects/Demo1/Pic_data/"+name+"/"+name+"("+to_string(order)+")"+".jpg",input);
            }
        }
    }
    string mk_dir="mkdir /Users/kimjunhyuk/CLionProjects/Demo1/Pic_data/"+name;
    system(mk_dir.c_str());
    imwrite("/Users/kimjunhyuk/CLionProjects/Demo1/Pic_data/"+name+"/"+name+"(1).jpg",input);
}
template <typename N>
void feedback<N>::update_exist(N &input, string name,int order)
{
    imwrite("/Users/kimjunhyuk/CLionProjects/Demo1/Pic_data/"+name+"/"+name+"("+to_string(order+1)+")"+".jpg",input);
}
template <typename N>
void feedback<N>::save_to_training(N &input, string name)
{
    string cmd="python3 /Users/kimjunhyuk/PycharmProjects/103_project_classification/stastic.py "+name;
    string result;
    execmd(cmd.c_str(),result);
    imwrite("/Users/kimjunhyuk/PycharmProjects/103_project_classification/Usr_data/103_train_data/"+name+"."+result+".jpg",input);
}
template <typename N>
void feedback<N>::upto_training(string &name)
{
    DIR *pdir;
    struct dirent *pdirent;
    int count=0;
    string path="/Users/kimjunhyuk/CLionProjects/Demo1/Pic_data/";
    pdir = opendir(path.c_str());
    if(pdir) {
        while((pdirent = readdir(pdir)))
        {
            if(strcmp(pdirent->d_name, ".") == 0 || strcmp(pdirent->d_name, "..") == 0)
                continue;
            if(strcmp(pdirent->d_name, name.c_str())==0)
            {
                DIR *name_dir;
                struct dirent *name_dirent;
                name_dir = opendir((path+pdirent->d_name+"/").c_str());
                int i=0;
                if(name_dir) {
                    while ((name_dirent = readdir(name_dir))) {
                        if(strcmp(name_dirent->d_name, ".") == 0 || strcmp(name_dirent->d_name, "..") == 0)
                            continue;
                        Mat temp =imread(path+pdirent->d_name+"/"+name_dirent->d_name);
                        string type_name = pdirent->d_name;
                        char num_char[10];
                        sprintf(num_char,"%d",i);
                        imwrite("/Users/kimjunhyuk/PycharmProjects/103_project_classification/Usr_data/103_train_data/"+type_name+"."+num_char+".jpg",temp);
                        i++;
                        //string cmd = "rm -r -f /Users/kimjunhyuk/CLionProjects/Demo1/Pic_data/"+name;
                        //system(cmd.c_str());
                    }
                }
                //change list.txt
                char buffer[128];
                string result;
                string new_type=pdirent->d_name;
                FILE* data_list = fopen("/Users/kimjunhyuk/PycharmProjects/103_project_classification/Usr_data/list.txt", "r"); //打开管道，并执行命令
                if (!data_list)
                    return ;
                while(!feof(data_list)) {
                    if(fgets(buffer, 128, data_list)){
                        result =result+buffer;
                    }
                }
                vector<string> final_result;
                final_result = split(result,',');
                int number = atoi(final_result[final_result.size()-2].c_str())+1;
                char num_char[10];
                sprintf(num_char,"%d",number);
                fclose(data_list);
                data_list = fopen("/Users/kimjunhyuk/PycharmProjects/103_project_classification/Usr_data/list.txt", "a");
                new_type = " ,"+new_type;
                new_type = num_char+new_type;
                new_type = ","+new_type;
                fwrite(new_type.c_str(),new_type.size(),1,data_list);
                fclose(data_list);
            }
        }
    }
}
template <typename N>
void feedback<N>::show_temp_data()
{
    int count=0;
    int i=0;
    //temporary dataset
    DIR *pdir;
    struct dirent *pdirent;
    cout<<"--------------------------------------------------------------------------------"<<endl;
    pdir = opendir("/Users/kimjunhyuk/CLionProjects/Demo1/Pic_data/");
    cout<<"The Temporary Dataset:"<<endl;
    while(pdirent = readdir(pdir)){
        if(strcmp(pdirent->d_name, ".") == 0 || strcmp(pdirent->d_name, ".." ) == 0 || strcmp(pdirent->d_name, ".DS_Store" ) == 0)
            continue;
        string species=pdirent->d_name;
        int count = stastic(("/Users/kimjunhyuk/CLionProjects/Demo1/Pic_data/"+species).c_str());
        cout<<i<<" . "<<pdirent->d_name<< ":" << count<<endl;
        i++;
    }
    cout<<"   \033[1mInput any keys to continue:\033[0m\n"<<endl;
}
template <typename N>
void feedback<N>::show_training_data()
{    //training dataset
    cout<<"--------------------------------------------------------------------------------"<<endl;
    string cmd="python3 /Users/kimjunhyuk/PycharmProjects/103_project_classification/Information_data.py ";
    string result;
    execmd(cmd.c_str(),result);
    cout<<"The Training Dataset:"<<endl;
    cout<<result<<endl;
    cout<<"   \033[1mInput any keys to continue:\033[0m\n"<<endl;
}
template <typename N>
void feedback<N>::retraining()
{
    string cmd="python3 /Users/kimjunhyuk/PycharmProjects/103_project_classification/train_optim.py";
    string result;
    execmd(cmd.c_str(),result); //run the CNN in python3
    cout<<result;
}
#endif //DEMO1_FEEDBACK_H
