#include <iostream>
#include <string>
#include "Image_Class.h"
using namespace std;

void grey(Image& img){
    for(int i=0;i<img.width;i++)
        for(int j=0;j<img.height;j++){
            unsigned int a=0;
            for(int k=0;k<3;k++) a+=img(i,j,k);
            a/= 3;
            for (int k=0;k<3;k++) img(i,j,k) =a;}}

void bw(Image& img){
    for(int i=0;i<img.width;i++)
        for(int j=0;j<img.height;j++){
            unsigned int a=0;
            for(int k=0;k<3;k++) a+=img(i,j,k);
            a/=3;
            int v=(a>127)?255 :0;
            for(int k=0;k<3;k++) img(i,j,k)=v;}}

void invert(Image& img){
    for(int i=0;i<img.width;i++)
        for(int j=0;j<img.height;j++)
            for(int k=0;k<3;k++)
                img(i,j,k)=255-img(i,j,k);}

void rotate(Image& img){
    Image r(img.height,img.width);
    for(int i=0;i<img.width; i++)
        for(int j=0;j<img.height;j++)
            for(int k=0;k<3;k++)
                r(j,img.width-1-i,k)=img(i,j,k);
    img=r;}

bool okExt(const string& f){
    string exts[]={".jpg",".jpeg",".png",".bmp",".tga"};
    for(auto&e:exts)
        if(f.size()>=e.size()&&f.compare(f.size()-e.size(),e.size(),e)==0)
            return true;
    return false;}

bool loaded =false;
string fname;
Image img(1,1);

void load(){
    if (loaded){
        char c;cout<<"do you want to save this photo? y/n: ";cin>>c;
        if (c=='y'){
            string s;cout<<"save as: ";cin>>s;
            while(!okExt(s)){cout<<"Bad ext: ";cin>>s;}
            img.saveImage(s);}}     
    cout<<"file: ";cin>>fname;
    while(!okExt(fname)){cout<<"Bad ext: ";cin>>fname;}
    img=Image(fname);loaded=true;
    cout<<"done.\n";}

void save(){
    if(!loaded){cout<<"no file.\n";return;}
    char c;cout<<"same name? y/n: ";cin>>c;
    if (c=='y'){
        img.saveImage(fname);cout<<"saved.\n";
    } else{
        string s;cout<<"the new name: ";cin>>s;
        while(!okExt(s)){cout<<"Bad ext: ";cin>>s;}
        img.saveImage(s);cout<<"saved.\n";}}

int main(){
    load();
    while (true){
        cout<<"\n1.load\n2.grey\n3.bw\n4.invert\n5.rotate\n6.save\n7.exit\n>";
        int ch;cin>>ch;
        if(ch==1) load();
        else if(ch==2) grey(img);
        else if(ch==3) bw(img);
        else if(ch==4) invert(img);
        else if(ch==5) rotate(img);
        else if(ch==6) save();
        else if(ch==7){
    char c;cout<<"do you want to save changes? y/n:";cin>>c;
    if (c=='y') save();
    break;}}
    return 0;}
