/* team members: 1.Mohammed Abdelsalam ID:20242296 
                 2.Amir Mostafa        ID:20240092
                 3.Sayed Mamdouh       ID:20240256 
Doc's demo:
1.grey

Mohammed:
2.BW
3.invert
4.rotate
5.purple(bonus)
6.edge detection
made the menu and the load,save,exit functions

Amir: 
7.crop 
8.blur
9.darken lighten
edited the menu and made undo function

Sayed:
10.flip
11.resize
12.add frame
13.infrared (bonus)*/

#include <iostream>
#include <string>
#include "Image_Class.h"
#include <stack>
using namespace std;

void grey(Image& img);
void BW(Image& img);
void invert(Image& img);
void rotate(Image& img);
void brightness(Image& image);
void crop(Image& image);
void blur(Image& image1);
void purple(Image& img);
void edges(Image& img);
void flip(Image &img);
void resize(Image &img);
void addFrame(Image& img);
void infrared(Image& img);
void merge(Image& img);
void sunlight(Image& img);

bool extentions(const string& f);
void load();
void save();

stack<Image> st;
bool loaded = false;
string fname;
Image img;

int main(){
    load();
    while (true){
        cout<<"\n1.load\n2.grey\n3.black and white\n4.invert\n5.rotate\n6.brightness\n"
              "7.crop\n8.blur\n9.purple\n10.edge detection\n11.flip\n12.resize\n"
              "13.add frame\n14.infrared\n15.sunlight\n16.merge\n17.save\n18.undo\n19.exit\n";
        int ch;
        cin>>ch;
        if (ch==1) load();
        else if (ch==2) grey(img);
        else if (ch==3) BW(img);
        else if (ch==4) invert(img);
        else if (ch==5) rotate(img);
        else if (ch==6) brightness(img);
        else if (ch == 7) crop(img);
        else if (ch == 8) blur(img);
        else if (ch==9) purple(img);
        else if (ch==10) edges(img);
        else if (ch==11) flip(img);
        else if (ch==12) resize(img);
        else if (ch==13) addFrame(img);
        else if (ch==14) infrared(img);
        else if (ch==15) sunlight(img);
        else if (ch==16) merge(img);
        else if (ch==17) save();
        else if (ch== 18) {
            if (!st.empty()) {
                img = st.top();
                st.pop();
            }
        }
        else if (ch==19){
            char c;cout<<"do you want to save changes? y/n:";cin>>c;
            if (c=='y') save();
            break;
        }
    }
    return 0;
}

void grey(Image& img) {
    st.push(img);
    for(int i=0;i<img.width;i++)
        for(int j=0;j<img.height;j++){
            unsigned int a=0;
            for(int k=0;k<3;k++)
                a+=img(i,j,k);
            a/=3;
            for (int k=0;k<3;k++)
                img(i,j,k)=a;
        }
}

void BW(Image& img){
    st.push(img);
    for(int i=0;i<img.width;i++)
        for(int j=0;j<img.height;j++){
            unsigned int a=0;
            for(int k=0;k<3;k++)
                a+=img(i,j,k);
            a/=3;
            int v=(a>127?255 :0);
            for(int k=0;k<3;k++) img(i,j,k)=v;
        }
}

void invert(Image& img){
    st.push(img);
    for(int i=0;i<img.width;i++)
        for(int j=0;j<img.height;j++)
            for(int k=0;k<3;k++)
                img(i,j,k)=255-img(i,j,k);
}

void rotate(Image& img) {
    st.push(img);
    cout<<"Enter rotate angle: \n";
    cout<<"1)90 degree 2)180 degree 3)270 degree 4)360 degree\n";
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
}

bool extentions(const string& f){
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
            while(!extentions(s)) {
                cout<<"Bad ext: ";
                cin>>s;
            }
            img.saveImage(s);
        }
    }
    cout<<"file: ";
    cin>>fname;
    while(!extentions(fname)) {
        cout<<"Bad ext: ";
        cin>>fname;
    }
    img=Image(fname);
    loaded=true;
    while (!st.empty()) st.pop();
    cout<<"done.\n";
}

void save() {
    if(!loaded) {
        cout<<"no file.\n";return;}
    cout<<"same name? y/n: ";
    char c;
    cin>>c;
    if (c=='y'){
        img.saveImage(fname);cout<<"saved.\n";}
    else {
        cout<<"the new name: ";
        string s;
        cin>>s;
        while(!extentions(s)) {
            cout<<"Bad ext: ";
            cin>>s;
        }
        img.saveImage(s);
        cout<<"saved.\n";}}

void crop (Image& image) {
    st.push(img);
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
}

void blur (Image& image1) {
    st.push(img);
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
}

void brightness(Image& image) {
    st.push(img);
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

void flip(Image &img){
    st.push(img);
    cout << "flip horizontal or vertical? h/v: ";
    char c;
    cin >> c;
    if (c == 'h'){
        for (int i = 0; i < img.width / 2; i++)
            for (int j = 0; j < img.height; j++)
                for (int k = 0; k < 3; k++){
                    unsigned char tmp = img(i, j, k);
                    img(i, j, k) = img(img.width - 1 - i, j, k);
                    img(img.width - 1 - i, j, k) = tmp;
                }
    }
    else if (c == 'v'){
        for (int i = 0; i < img.width; i++)
            for (int j = 0; j < img.height / 2; j++)
                for (int k = 0; k < 3; k++){
                    unsigned char tmp = img(i, j, k);
                    img(i, j, k) = img(i, img.height - 1 - j, k);
                    img(i, img.height - 1 - j, k) = tmp;
                }
    }
    else{
        cout << "invalid choice.\n";
    }
}


void resize(Image &img){
    st.push(img);
    int newW, newH;
    cout << "Enter New Width: ";
    cin >> newW;
    cout << "Enter New Height: ";
    cin >> newH;

    Image r(newW, newH);

    double x_ratio = (double)(img.width - 1) / newW;
    double y_ratio = (double)(img.height - 1) / newH;

    for (int i = 0; i < newW; i++){
        for (int j = 0; j < newH; j++){
            double x = i * x_ratio;
            double y = j * y_ratio;

            int xL = (int)x;
            int yL = (int)y;
            int xH = min(xL + 1, img.width - 1);
            int yH = min(yL + 1, img.height - 1);

            double xWeight = x - xL;
            double yWeight = y - yL;

            for (int k = 0; k < 3; k++){
                double a = img(xL, yL, k) * (1 - xWeight) + img(xH, yL, k) * xWeight;
                double b = img(xL, yH, k) * (1 - xWeight) + img(xH, yH, k) * xWeight;
                r(i, j, k) = (unsigned char)(a * (1 - yWeight) + b * yWeight);
            }
        }
    }

    img = r;
    cout << "Image resized to " << newW << "x" << newH << ".\n";
}

void addFrame(Image& img) {
    st.push(img);

    cout << "Choose frame style:\n";
    cout << "1) simple\n2) double Color\n3) fancy Corners\n";
    int choose;
    cin >> choose;

    cout << "Enter frame thickness (pixels-recommended 40-60): ";
    int t;
    cin >> t;

    int nWidth = img.width + 1.5 * t;
    int nHight = img.height + 1.5 * t;

    Image framed(nWidth, nHight);

    for (int i = 0; i < nWidth; i++) {
        for (int j = 0; j < nHight; j++) {
            bool inside = (i >= t && i < img.width + t &&
                           j >= t && j < img.height + t);

            if (inside) {
                for (int k = 0; k < 3; k++) {
                    framed(i, j, k) = img(i - t, j - t, k);
                }
            } else {
                if (choose == 1) {
                    framed(i, j, 0) = 0;
                    framed(i, j, 1) = 0;
                    framed(i, j, 2) = 250;
                }
                else if (choose == 2) {
                    framed(i, j, 0) = (i % 2 == 0 ? 245 : 245);
                    framed(i, j, 1) = (i % 2 == 0 ? 0 : 250);
                    framed(i, j, 2) = 0;
                }
                else {
                    if ((i + j) % 20 < 10) {
                        for (int k = 0; k < 3; k++) framed(i, j, k) = 200;
                    } else {
                        for (int k = 0; k < 3; k++) framed(i, j, k) = 15;
                    }
                }
            }
        }
    }

    img = framed;
}

void infrared(Image& img) {
    st.push(img);

    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            int r = img(i, j, 0);
            int g = img(i, j, 1);
            int b = img(i, j, 2);

            int red   = min(255, (int)(r * 1.25 + g * 0.31 + b * 0.31));
            int green = g * 0.11;
            int blue  = b * 0.11;

            img(i, j, 0) = red;
            img(i, j, 1) = green;
            img(i, j, 2) = blue;
        }
    }
}

void merge(Image& img1){
    st.push(img1);
    string name;
    cout<<"Enter the image you want to merge: \n";
    cin>>name;
    Image img2(name);
    cout<<"img1: "<<img1.width<<"X"<<img1.height;
    cout<<"\nimg2: "<<img2.width<<"X"<<img2.height<<'\n';
    if (img1.width != img2.width || img1.height != img2.height) {
        cout<<"Do you want to resize 1)image 1 or 2)image 2: \n";
        int x;
        cin>>x;
        if (x==1)
            resize(img1);
        else if (x==2)
            resize(img2);
    }
    cout<<"img1: "<<img1.width<<"X"<<img1.height;
    cout<<"\nimg2: "<<img2.width<<" "<<img2.height<<'\n';
    Image ans(img1.width,img1.height);

    for (int i = 0;i<img1.width;i++) {
        for (int j = 0;j<img1.height;j++) {
            for (int k = 0;k<3;k++) {
                ans(i,j,k) = (img1(i,j,k)+img2(i,j,k))/2;
            }
        }
    }
    img = ans;
}

void sunlight(Image& img){
    Image tmp(img);

    for (int i = 0;i<img.width;i++) {
        for (int j = 0;j<img.height;j++) {
            double red = img(i,j,0);
            double green = img(i,j,1);
            double blue = img(i,j,2);

            int newRed = 1.1 * red + 10;
            int newGreen = green * 1.05  + 5;
            int newBlue = blue * .95 ;

            tmp(i,j,0) = max(0,min(255,newRed));
            tmp(i,j,1) = max(0,min(255,newGreen));
            tmp(i,j,2) = max(0,min(255,newBlue));
        }
    }
    img = tmp;
}
