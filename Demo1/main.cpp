#include "camera.h"
#include "UserInterface.h"
#include "feedback.h"
#include "Image.h"
#include <stdlib.h>
///Users/kimjunhyuk/PycharmProjects/103_project_classification/Usr_data/test/Unknown-4.jpeg
using namespace cv;
using namespace std;
int main(){
    int opt_button =0;
    UI_part ui_sys;
    feedback<Mat> feed_b;
    string tip;
    string tip_1 = "Would u like to MOVE TO SURF_BASED CLASSIFICATION?";
    string tip_2 = "No relavant specise learned. Would you like to save a new specise for that?";
    string tip_3 = "No relavant specise learned. Would u like to MOVE TO SURF_BASED CLASSIFICATION?";
    string tip_4 = "New Datum has been moved to temporary dataset!";
    string tip_5 = "New Specise has been set to temporary dataset!";
    string tip_6 = "New Specise has been set to training dataset!";
    while(true)
    {
        ui_sys.main_UI();
        scanf("%d",&opt_button);
        //----------------Pics Identify------------------------------------------------------------------//
        if(opt_button==1) {
            char option;
            image_iden<Mat> iden_img;
            if(int i=iden_img.address_input()) {
                iden_img.CNN_classification();
            } else
            {
                tip = "Please input the available img address!";
                ui_sys.ensure_format(tip);
            }
            if(iden_img.relevant>0.1)
            {
                ui_sys.camera_view_ensure(iden_img.nSpecies,iden_img.rel_species);
                safe_flush(stdin);
                scanf("%c",&option);
                if(option =='y'){
                    //save to tranning dataset
                    feed_b.save_to_training(iden_img.out_image,iden_img.nSpecies);//do the update
                    ui_sys.ensure_format(tip_6);
                }
                else if(option =='n') {
                    //printf("\n Would u like to MOVE TO SURF_BASED CLASSIFICATION?\n\n");
                    ui_sys.ans_format(tip_1);
                    safe_flush(stdin);
                    scanf("%c", &option);
                    if (option == 'y') {
                        iden_img.SURF_classification();
                        if (iden_img.relevant > 0.5) {
                            ui_sys.camera_view_ensure(iden_img.nSpecies,iden_img.rel_species);/////////surf
                            safe_flush(stdin);
                            scanf("%c", &option);
                            if (option =='y') {
                                feed_b.update_exist(iden_img.out_image, iden_img.nSpecies,
                                                    iden_img.same_species_num);//do the update
                                ui_sys.ensure_format(tip_4);
                            }
                            else if(option =='n')
                            {
                                //printf("There are no relavant specise learned.\n Would you like to save a new specise for that?\n\n");
                                ui_sys.ans_format(tip_2);
                                safe_flush(stdin);
                                scanf("%c", &option);
                                if (option == 'y') {
                                    feed_b.new_saved(iden_img.out_image);//save in a new dir named
                                    ui_sys.ensure_format(tip_5);
                                }
                                else if (option == 'n') {
                                    continue;
                                }
                            }
                        }
                        else {
                            //printf("There are no relavant specise learned.\n Would you like to save a new specise for that?\n\n");
                            ui_sys.ans_format(tip_2);
                            safe_flush(stdin);
                            scanf("%c", &option);
                            if (option == 'y') {
                                feed_b.new_saved(iden_img.out_image);//save in a new dir named
                                ui_sys.ensure_format(tip_5);
                            }
                            else if (option == 'n') {
                                continue;
                            }
                        }
                    }
                    else if(option =='n') {
                        tip ="Input the correct specise name? or input [n] to back to menu.\n\n";
                        ui_sys.ans_format(tip);
                        safe_flush(stdin);
                        scanf("%c", &option);
                        if (option == 'y') {
                            string temp_name;
                            safe_flush(stdin);
                            cin>>temp_name;
                            feed_b.save_to_training(iden_img.out_image,temp_name);//do the update
                            ui_sys.ensure_format(tip_6);
                        }
                        else if (option == 'n'){
                            continue;
                        }
                    }
                }
            }
                // fail to match
            else {
                //printf("There are no relavant specise learned.\n Would u like to MOVE TO SURF_BASED CLASSIFICATION?\n\n");
                ui_sys.ans_format(tip_3);
                safe_flush(stdin);
                scanf("%c",&option);
                if(option =='y') {
                    iden_img.SURF_classification();
                    if (iden_img.relevant > 0.7) {
                        ui_sys.camera_view_ensure(iden_img.nSpecies,iden_img.rel_species);//////surf
                        safe_flush(stdin);
                        scanf("%c", &option);
                        if (option == 'y') {
                            feed_b.update_exist(iden_img.out_image, iden_img.nSpecies,
                                                iden_img.same_species_num);//do the update
                            ui_sys.ensure_format(tip_4);
                        }
                        else if(option =='n') {
                            //printf("There are no relavant specise learned.\n Would you like to save a new specise for that?\n\n");
                            ui_sys.ans_format(tip_2);
                            safe_flush(stdin);
                            scanf("%c", &option);
                            if (option == 'y') {
                                feed_b.new_saved(iden_img.out_image);//save in a new dir named
                                ui_sys.ensure_format(tip_5);
                            }
                            else if (option == 'n'){
                                continue;
                            }
                        }
                    }
                    else {
                        //printf("There are no relavant specise learned.\n Would you like to save a new specise for that?\n\n");
                        ui_sys.ans_format(tip_2);
                        safe_flush(stdin);
                        scanf("%c", &option);
                        if (option == 'y') {
                            feed_b.new_saved(iden_img.out_image);//save in a new dir named
                            ui_sys.ensure_format(tip_5);
                        }
                        else if (option == 'n') {
                            continue;
                        }
                    }
                }
                else if(option =='n') {
                    tip ="Input the correct specise name? or input [n] to back to menu.\n\n";
                    ui_sys.ans_format(tip);
                    safe_flush(stdin);
                    scanf("%c", &option);
                    if (option == 'y') {
                        string temp_name;
                        safe_flush(stdin);
                        cin>>temp_name;
                        feed_b.save_to_training(iden_img.out_image,temp_name);//do the update
                        ui_sys.ensure_format(tip_6);
                    }
                    else if (option == 'n'){
                        continue;
                    }
                }
            }
        }
            //--------------Camera Identify-------------------------------------------------------------//
        else if(opt_button==2){
            ui_sys.camera_view_top();
            camera_iden<Mat> iden_cam;
            //CNN classification
            iden_cam.recording();
            iden_cam.screen();//By PSNR
            iden_cam.CNN_classification();
           //feed-back part
            char option;
            // species finded in CNN
            if(iden_cam.relevant>0.1){
                ui_sys.camera_view_ensure(iden_cam.nSpecies,iden_cam.rel_species);
                safe_flush(stdin);
                scanf("%c",&option);
                if(option =='y'){
                    //save to tranning dataset
                    feed_b.save_to_training(iden_cam.out_image[iden_cam.best],iden_cam.nSpecies);//do the update
                    ui_sys.ensure_format(tip_6);
                }
                else if(option =='n') {
                    //printf("\n Would u like to MOVE TO SURF_BASED CLASSIFICATION?\n\n");
                    ui_sys.ans_format(tip_1);
                    safe_flush(stdin);
                    scanf("%c", &option);
                    if (option == 'y') {
                        iden_cam.SURF_classification();
                        if (iden_cam.relevant > 0.5) {
                            ui_sys.camera_view_ensure(iden_cam.nSpecies,iden_cam.rel_species);/////////surf
                            safe_flush(stdin);
                            scanf("%c", &option);
                            if (option =='y') {
                                feed_b.update_exist(iden_cam.out_image[iden_cam.best], iden_cam.nSpecies,
                                                    iden_cam.same_species_num);//do the update
                                ui_sys.ensure_format(tip_4);
                            }
                            else if(option =='n')
                            {
                                //printf("There are no relavant specise learned.\n Would you like to save a new specise for that?\n\n");
                                ui_sys.ans_format(tip_2);
                                safe_flush(stdin);
                                scanf("%c", &option);
                                if (option == 'y') {
                                    feed_b.new_saved(iden_cam.out_image[iden_cam.best]);//save in a new dir named
                                    ui_sys.ensure_format(tip_5);
                                }
                                else if (option == 'n') {
                                    continue;
                                }
                            }
                        }
                        else {
                            //printf("There are no relavant specise learned.\n Would you like to save a new specise for that?\n\n");
                            ui_sys.ans_format(tip_2);
                            safe_flush(stdin);
                            scanf("%c", &option);
                            if (option == 'y') {
                                feed_b.new_saved(iden_cam.out_image[iden_cam.best]);//save in a new dir named
                                ui_sys.ensure_format(tip_5);
                            }
                            else if (option == 'n') {
                                continue;
                            }
                        }
                    }
                    else if(option =='n') {
                        tip ="Input the correct specise name? or input [n] to back to menu.\n\n";
                        ui_sys.ans_format(tip);
                        safe_flush(stdin);
                        scanf("%c", &option);
                        if (option == 'y') {
                            string temp_name;
                            safe_flush(stdin);
                            cin>>temp_name;
                            feed_b.save_to_training(iden_cam.out_image[iden_cam.best],temp_name);//do the update
                            ui_sys.ensure_format(tip_6);
                        }
                        else if (option == 'n'){
                            continue;
                        }
                    }
                }
            }
                // fail to match
            else {
                //printf("There are no relavant specise learned.\n Would u like to MOVE TO SURF_BASED CLASSIFICATION?\n\n");
                ui_sys.ans_format(tip_3);
                safe_flush(stdin);
                scanf("%c",&option);
                if(option =='y') {
                    iden_cam.SURF_classification();
                    if (iden_cam.relevant > 0.7) {
                        ui_sys.camera_view_ensure(iden_cam.nSpecies,iden_cam.rel_species);//////surf
                        safe_flush(stdin);
                        scanf("%c", &option);
                        if (option == 'y') {
                            feed_b.update_exist(iden_cam.out_image[iden_cam.best], iden_cam.nSpecies,
                                                iden_cam.same_species_num);//do the update
                            ui_sys.ensure_format(tip_4);
                        }
                        else if(option =='n') {
                            //printf("There are no relavant specise learned.\n Would you like to save a new specise for that?\n\n");
                            ui_sys.ans_format(tip_2);
                            safe_flush(stdin);
                            scanf("%c", &option);
                            if (option == 'y') {
                                feed_b.new_saved(iden_cam.out_image[iden_cam.best]);//save in a new dir named
                                ui_sys.ensure_format(tip_5);
                            }
                            else if (option == 'n'){
                                continue;
                            }
                        }
                    }
                    else {
                        //printf("There are no relavant specise learned.\n Would you like to save a new specise for that?\n\n");
                        ui_sys.ans_format(tip_2);
                        safe_flush(stdin);
                        scanf("%c", &option);
                        if (option == 'y') {
                            feed_b.new_saved(iden_cam.out_image[iden_cam.best]);//save in a new dir named
                            ui_sys.ensure_format(tip_5);
                        }
                        else if (option == 'n') {
                            continue;
                        }
                    }
                }
                else if(option =='n') {
                    tip ="Input the correct specise name? or input [n] to back to menu.\n\n";
                    ui_sys.ans_format(tip);
                    safe_flush(stdin);
                    scanf("%c", &option);
                    if (option == 'y') {
                        string temp_name;
                        safe_flush(stdin);
                        cin>>temp_name;
                        feed_b.save_to_training(iden_cam.out_image[iden_cam.best],temp_name);//do the update
                        ui_sys.ensure_format(tip_6);
                    }
                    else if (option == 'n'){
                        continue;
                    }
                }
            }
        }
            //--------------System information-----------------------------------------------------------//
        else if(opt_button==3)
        {
            while(true){
                ui_sys.information_UI();
                safe_flush(stdin);
                scanf("%d",&opt_button);
                if(opt_button==1) {
                    feed_b.show_temp_data();
                    safe_flush(stdin);
                    cin.ignore(INT_MAX, '\n');
                    cin>>opt_button;
                    continue;
                }
                if(opt_button==2) {
                    feed_b.show_training_data();
                    safe_flush(stdin);
                    cin.ignore(INT_MAX, '\n');
                    cin>>opt_button;
                    continue;
                }
                if(opt_button==3){
                    tip="Pleas input the specise u want to move from the temporary dataset:";
                    ui_sys.ensure_format(tip);
                    string move_name;
                    safe_flush(stdin);
                    cin.ignore(INT_MAX, '\n');
                    cin>>move_name;
                    feed_b.upto_training(move_name);
                    continue;
                }
                if(opt_button==4){
                    tip="Retraining the CNN model, it will take a longtime( Seperately running the python3 will be preferred!).";
                    ui_sys.ensure_format(tip);
                    feed_b.retraining();
                    continue;
                }
                else
                    break;
            }
            continue;
        }
            //--------------Exit------------------------------------------------------------------------//
        else if(opt_button==4)
        {
            break;
        }
            //--------------help-------------------------------------------------------------------------//
        else if(opt_button==8)
        {
            cout<<"plz input the help txt\n"<<endl;
            continue;
        }
            //-------------writers----------------------------------------------------------------------//
        else if(opt_button==9)
        {
            cout<<"Made by Eickee Zou\n"<<endl;
            continue;
        }
        else{
            cout<<"Plz do the right option~\n"<<endl;
            safe_flush(stdin);
            continue;
        }

    }
    return 0;
}