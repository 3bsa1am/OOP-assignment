#include <iostream>
#include <string>
#include "Image_Class.h"
#include <stack>
using namespace std;

void grey(Image& img);
<<<<<<< HEAD

void Black_White(Image& img);

void invert(Image& img);

void rotate(Image& img);

void brightness(Image& image);

void crop(Image& image);

void blur(Image& image1);


bool okExt(const string& f);

void load();

void save();

stack<Image>st;
=======
void Black_White(Image& img);
void invert(Image& img);
void rotate(Image& img);
void brightness(Image& image);
void crop(Image& image);
void blur(Image& image1);
void purple(Image& img);
void edges(Image& img);

bool okExt(const string& f);
void load();
void save();

stack<Image> st;
>>>>>>> 6b4b545 (edge detection & puprle filter added and some other changes)
bool loaded = false;
string fname;
Image img;

int main(){
    load();
    while (true){
        cout<<"\n1.load\n2.grey\n3.black and white\n4.invert\n5.rotate\n6.brightness\n"
<<<<<<< HEAD
              "7.crop\n8.blur\n9.undo\n10.exit\n";
=======
              "7.crop\n8.blur\n9.purple\n10.edge detection\n11.undo\n12.exit\n";
>>>>>>> 6b4b545 (edge detection & puprle filter added and some other changes)
        int ch;
        cin>>ch;
        if (ch==1) load();
        else if (ch==2) grey(img);
        else if (ch==3) Black_White(img);
        else if (ch==4) invert(img);
        else if (ch==5) rotate(img);
        else if (ch==6) brightness(img);
<<<<<<< HEAD
        else if (ch == 7)crop(img);
        else if (ch == 8)blur(img);
        else if (ch== 9) {
=======
        else if (ch == 7) crop(img);
        else if (ch == 8) blur(img);
        else if (ch==9) purple(img);
        else if (ch==10) edges(img);
        else if (ch== 11) {
>>>>>>> 6b4b545 (edge detection & puprle filter added and some other changes)
            if (!st.empty()) {
                img = st.top();
                st.pop();
            }
        }
<<<<<<< HEAD
        else if (ch==10){
=======
        else if (ch==12){
>>>>>>> 6b4b545 (edge detection & puprle filter added and some other changes)
            char c;cout<<"do you want to save changes? y/n:";cin>>c;
            if (c=='y') save();
            break;
        }
    }
    return 0;
}
<<<<<<< HEAD
void grey(Image& img) {
=======

void grey(Image& img) {
    st.push(img);
>>>>>>> 6b4b545 (edge detection & puprle filter added and some other changes)
    for(int i=0;i<img.width;i++)
        for(int j=0;j<img.height;j++){
            unsigned int a=0;
            for(int k=0;k<3;k++)
                a+=img(i,j,k);
            a/= 3;
            for (int k=0;k<3;k++)
                img(i,j,k) = a;
        }
<<<<<<< HEAD
    st.push(img);
}

void Black_White(Image& img){
=======
}

void Black_White(Image& img){
    st.push(img);
>>>>>>> 6b4b545 (edge detection & puprle filter added and some other changes)
    for(int i=0;i<img.width;i++)
        for(int j=0;j<img.height;j++){
            unsigned int a=0;
            for(int k=0;k<3;k++)
                a+=img(i,j,k);
            a/=3;
            int v=(a>127?255 :0);
            for(int k=0;k<3;k++) img(i,j,k)=v;
        }
<<<<<<< HEAD
    st.push(img);
}

void invert(Image& img){
=======
}

void invert(Image& img){
    st.push(img);
>>>>>>> 6b4b545 (edge detection & puprle filter added and some other changes)
    for(int i=0;i<img.width;i++)
        for(int j=0;j<img.height;j++)
            for(int k=0;k<3;k++)
                img(i,j,k)=255-img(i,j,k);
<<<<<<< HEAD
    st.push(img);
}

void rotate(Image& img) {

    cout<<"Enter rotate angle: \n";
    cout<<"1)90° 2)180° 3)270° 4)360°\n";
=======
}

void rotate(Image& img) {
    st.push(img);
    cout<<"Enter rotate angle: \n";
    cout<<"1)90 degree 2)180 degree 3)270 degree 4)360 degree\n";
>>>>>>> 6b4b545 (edge detection & puprle filter added and some other changes)
    int x;
    cin>>x;
    x/=90;
    while (x--) {
        int w = img.height;
        int h = img.width;
        Image r(w,h);
        for(int i=0;i<img.width; i++)
            for(int j=0;j<img.height;j++)
                for(int k=0;k<3;k++)
                    r(j,img.width-1-i,k)=img(i,j,k);
        img=r;
    }
<<<<<<< HEAD
    st.push(img);
}

// void brightness(Image& img, bool inc) {
//     for(int i=0;i<img.width;i++)
//         for(int j=0;j<img.height;j++)
//             for(int k=0;k<3;k++){
//                 int v=img(i,j,k);
//                 if(inc)
//                     v=v+v/2;
//                 else
//                     v=v/2;
//                 if(v>255)
//                     v=255;
//                 img(i,j,k)=v;
//             }
//     st.push(img);
// }

=======
}

>>>>>>> 6b4b545 (edge detection & puprle filter added and some other changes)
bool okExt(const string& f){
    string exts[]={".jpg",".jpeg",".png",".bmp",".tga"};
    for (auto&e:exts)
        if(f.size()>=e.size()&&f.compare(f.size()-e.size(),e.size(),e)==0)
            return true;
    return false;
}

void load(){
    if (loaded){
        cout<<"do you want to save this photo? y/n: ";
        char c;
        cin>>c;
        if (c=='y'){
            cout<<"save as: ";
            string s;
            cin>>s;
            while(!okExt(s)) {
                cout<<"Bad ext: ";
                cin>>s;
            }
            img.saveImage(s);
        }
    }
    cout<<"file: ";
    cin>>fname;
    while(!okExt(fname)) {
        cout<<"Bad ext: ";
        cin>>fname;
    }
    img=Image(fname);
    loaded=true;
<<<<<<< HEAD
=======
    while (!st.empty()) st.pop();
>>>>>>> 6b4b545 (edge detection & puprle filter added and some other changes)
    cout<<"done.\n";
}

void save() {
    if(!loaded) {
        cout<<"no file.\n";return;
    }
    cout<<"same name? y/n: ";
    char c;
    cin>>c;
    if (c=='y'){
        img.saveImage(fname);cout<<"saved.\n";
    }
    else {
        cout<<"the new name: ";
        string s;
        cin>>s;
        while(!okExt(s)) {
            cout<<"Bad ext: ";
            cin>>s;
        }
        img.saveImage(s);
        cout<<"saved.\n";
    }
}

void crop (Image& image) {
<<<<<<< HEAD
=======
    st.push(img);
>>>>>>> 6b4b545 (edge detection & puprle filter added and some other changes)
    cout<<"Enter Starting Point: \n";
    int x,y;
    cin>>x>>y;

    cout << "Enter dimensions you want to crop: \n";
    int W , H ;
    cin>>W>>H;

    if (x+W > image.width || x < 0 || y < 0 || y+H > image.height) {
        cerr<<"Wrong dimensions";
        return;
    }
    Image im(W,H);
    for (int i=0;i<W;i++) {
        for (int j = 0;j<H;j++) {
            for (int k = 0;k<3;k++)
                im(i,j,k) = image (i+x ,j+y,k);
        }
    }
    img = im;
<<<<<<< HEAD
    st.push(img);
}

void blur (Image& image1) {
=======
}

void blur (Image& image1) {
    st.push(img);
>>>>>>> 6b4b545 (edge detection & puprle filter added and some other changes)
    Image image2 = image1;
    cout<<"Enter the level of blurriness: \n";
    cout<<"1)Weak "
          "2)Medium "
          "3)Strong\n";
    int t ;
    cin>>t;
    if (t==1)       t = 3;
    else if (t==2)  t = 5;
    else            t = 10;
    while (t--) {
        for (int i = 0;i<image1.width;i++) {
            for (int j=0;j<image1.height;j++) {
                int redSum = 0 , greenSum = 0 , blueSum = 0 , numberOfPixels = 0;
                for (int dx = -3;dx <= 3 ;dx++) {
                    for (int dy = -3;dy<=3;dy++){
                        if (i+dx < 0 || j+dy<0 || dx+i >= image1.width || dy+j >= image1.height)
                            continue;
                        numberOfPixels++;
                        redSum+=image1(dx+i,dy+j,0);
                        greenSum+=image1(dx+i,dy+j,1);
                        blueSum+=image1(dx+i,dy+j,2);
                    }
                }
                image2(i,j,0) = redSum/numberOfPixels;
                image2(i,j,1) = greenSum/numberOfPixels;
                image2(i,j,2) = blueSum/numberOfPixels;
            }
        }
        image1 = image2;
    }
    img = image1;
<<<<<<< HEAD
    st.push(img);
}

void brightness(Image& image) {
=======
}

void brightness(Image& image) {
    st.push(img);
>>>>>>> 6b4b545 (edge detection & puprle filter added and some other changes)
    cout<<"Do you want the image "
          "1)Lighter 2)Darker \n";
    int x;
    cin>>x;
    cout<<"Enter Percentage%: ";
    float t;
    cin>>t;
    t/=100;
    x == 1 ? t= 1 + t : t = 1 - t;
    for(int i=0;i<image.width;i++) {
        for(int j=0;j<image.height;j++) {
            for(int k=0;k<3;k++) {
                int val = image.getPixel(i,j,k) * t;
                if (val > 255)val = 255;
                else if (val < 0)val = 0;
                image.setPixel(i,j,k,val);
            }
        }
    }
    img = image;
<<<<<<< HEAD
    st.push(img);
}
=======
}

void purple(Image& img){
    for(int i=0;i<img.width;i++)
        for(int j=0;j<img.height;j++){
            int r=img(i,j,0);
            int g=img(i,j,1);
            int b=img(i,j,2);
            r=min(255,r+50);
            g=max(0,g-50);
            b=min(255,b+50);
            img(i,j,0)=r;
            img(i,j,1)=g;
            img(i,j,2)=b;}}

void edges(Image& img){
    Image temp=img;
    for(int i=1;i<img.width-1;i++){
        for(int j=1;j<img.height-1;j++){
            int contx=0,conty=0;
            for(int k=0;k<3;k++){
                contx=abs(temp(i+1,j,k)-temp(i-1,j,k));
                conty=abs(temp(i,j+1,k)-temp(i,j-1,k));}
            int cont=min(255,contx+conty);
            cont=255-cont;
            for(int k=0;k<3;k++)
                img(i,j,k)=cont;}}}
>>>>>>> 6b4b545 (edge detection & puprle filter added and some other changes)
