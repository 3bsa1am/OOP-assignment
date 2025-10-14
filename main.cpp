/* 
team name: FITLER
team members: 1.Mohammed Abdelsalam ID:20242296 S30
                 2.Amir Mostafa        ID:20240092 S9
                 3.Sayed Mamdouh       ID:20240256 S9
Section: S30 
Work breakdown:
Doc's demo:
1.grey

Mohammed
2.B&W
3.invert
4.rotate
5.edge detection
6.purple(bonus)
-old tv
-four quarters
made the menu ,load,save,exit functions

Amir: 
7.crop 
8.blur
9.darken lighten 
10.merge
11.sunlight (bonus)
-skew
edited the menu and made undo function

Sayed:
12.flip
13.resize
14.add frame
15.infrared (bonus)
-oil painting
-graffiti
-worm cold

explanation pdf link: https://drive.google.com/file/d/1sn-_0Fqfx1kQYqh_XSp7BaX-xyePtAPZ/view?usp=drive_link

GUI video link: https://drive.google.com/file/d/1_aB4nTQwcNJPxJMdVEXKdvFQ7kdyk9U_/view?usp=drive_link

github repo link: https://github.com/3bsa1am/OOP-assignment.git

shared document link: https://docs.google.com/document/d/1o-VaxDqJPAnOOojDnYscY635XjuSHVr0/edit?usp=drive_link&ouid=106221124286685596777&rtpof=true&sd=true

part 2 vedio link: https://drive.google.com/file/d/1zFbznpp9WEzgURWg_S0waQWAu9391LR2/view?usp=drive_link

winged dragon folder link: https://drive.google.com/drive/folders/1N2F-xZ31xSGnn2pGDgwbBo4J8fekrIRL?usp=drive_link

*/

#include <iostream>
#include <string>
#include "Image_Class.h"
#include <stack>
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
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
void oldtv(Image& img);
void fourquarters(Image& img);
void skew(Image& img);
void oilPainting(Image& img);
void graffiti(Image& img);
void warmCold(Image& img);

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
              "13.add frame\n14.infrared\n15.sunlight\n16.merge\n17.four quarters\n18.old tv\n19.skew\n"
              "20.oil painting\n21.graffiti\n22.warm cold\n23.save\n24.undo\n25.exit\n";
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
        else if (ch==17) fourquarters(img);
        else if (ch==18) oldtv(img);
        else if (ch==19) skew(img);
        else if (ch==20) oilPainting(img);
        else if (ch==21) graffiti(img);
        else if (ch==22) warmCold(img);
        else if (ch==23) save();
        else if (ch== 24) {
            if (!st.empty()) {
                img = st.top();
                st.pop();
            }
        }
        else if (ch==25){
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


void skew(Image& img) {
    st.push(img);
    cout<<"Enter the degree of vertical skewness you want -45 -> 45: \n";
    int degree;
    cin>>degree;
    double radian = degree * M_PI / 180.0 ;
    double factor = tan(radian);
    Image image(img.width,img.height + (int)(img.width * fabs(factor)));

    for (int i = 0;i<image.width;i++) {
        for (int j = 0;j<image.height;j++) {
            double nj = j - factor*i;

            if (nj >= 0 && nj < img.height) {
                int n1 = floor(nj);
                int n2 = ceil(nj);
                double w = nj - n1;
                if (n1 == n2) {
                    for (int k = 0;k<3;k++) {
                        image(i,j,k) = img(i,n1,k);
                    }
                }
                else {
                    if (n1 >= 0 && n2 < img.height) {
                        for (int k = 0; k < 3; k++) {
                            image(i,j,k) = (1-w)*img(i,n1,k) + w*img(i,n2,k);
                        }
                    }
                }
            }
        }
    }
    img = image;
}

void oldtv(Image& img) {
    st.push(img);
    Image temp=img;
    srand(time(0));

    for (int i=0;i<img.width;i++) {
        for (int j=0;j<img.height;j++){
            for (int k=0;k<3;k++){
                int p=img(i,j,k);
                
                if (j%12<5) p*=0.15;    
                p+=(rand() %120)-60;  
                
                if (p<0) p=0;
                if (p>255) p=255;
                temp(i,j,k)=p;
            }
        }
    }

    img=temp;
}

void fourquarters(Image& img) {
    st.push(img);
    int mw = img.width / 2;
    int mh = img.height / 2;

    Image org = img;

    for (int i = 0; i < mw; i++) {
        for (int j = 0; j < mh; j++) {
            for (int k = 0; k < 3; k++)
                img(i, j, k) = org(i, j, k);
        }
    }

    for (int i = mw; i < img.width; i++) {
        for (int j = 0; j < mh; j++) {
            for (int k = 0; k < 3; k++)
                img(i, j, k) = 255 - org(i, j, k);
        }
    }

    for (int i = 0; i < mw; i++) {
        for (int j = mh; j < img.height; j++) {
            unsigned int a = 0;
            for (int k = 0; k < 3; k++)
                a += org(i, j, k);
            a /= 3;
            for (int k = 0; k < 3; k++)
                img(i, j, k) = a;
        }
    }

    Image tmp = org;
    for (int i = mw + 1; i < img.width - 1; i++) {
        for (int j = mh + 1; j < img.height - 1; j++) {
            int cx = 0, cy = 0;
            for (int k = 0; k < 3; k++) {
                cx = abs(tmp(i + 1, j, k) - tmp(i - 1, j, k));
                cy = abs(tmp(i, j + 1, k) - tmp(i, j - 1, k));
            }
            int c = min(255, cx + cy);
            c = 255 - c;
            for (int k = 0; k < 3; k++)
                img(i, j, k) = c;
        }
    }
}

void oilPainting(Image& img) {
    st.push(img);

    cout << "Enter intensity level (recommended 1-> 3): ";
    int radius=radius+2;
    cin >> radius;

    Image result(img.width, img.height);

    for (int x = 0; x < img.width; x++) {
        for (int y = 0; y < img.height; y++) {

            const int bins = 256;
            int intensityCount[bins] = {0};
            int avgR[bins] = {0};
            int avgG[bins] = {0};
            int avgB[bins] = {0};

            for (int dx = -radius; dx <= radius; dx++) {
                for (int dy = -radius; dy <= radius; dy++) {
                    int nx = x + dx;
                    int ny = y + dy;

                    if (nx < 0 || ny < 0 || nx >= img.width || ny >= img.height) 
                        continue;

                    int r = img(nx, ny, 0);
                    int g = img(nx, ny, 1);
                    int b = img(nx, ny, 2);

                    int intensity = (r + g + b) / 3; 
                    intensity = intensity * (bins - 1) / 255; 

                    intensityCount[intensity]++;
                    avgR[intensity] += r;
                    avgG[intensity] += g;
                    avgB[intensity] += b;
                }
            }

            int maxCount = 0, maxIndex = 0;
            for (int i = 0; i < bins; i++) {
                if (intensityCount[i] > maxCount) {
                    maxCount = intensityCount[i];
                    maxIndex = i;
                }
            }

            if (maxCount > 0) {
                result(x, y, 0) = avgR[maxIndex] / maxCount;
                result(x, y, 1) = avgG[maxIndex] / maxCount;
                result(x, y, 2) = avgB[maxIndex] / maxCount;
            }
        }
    }

    img = result;
    cout << "Oil Painting filter applied.\n";
}


void graffiti(Image& img) {
    st.push(img);
    Image result(img.width, img.height);

    int levels = 4;
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            for (int k = 0; k < 3; k++) {
                int v = img(x, y, k);
                v = (v / (256 / levels)) * (256 / levels);
                result(x, y, k) = v;
            }
        }
    }

    Image edge(img.width, img.height);

    int gx[3][3] = {{-1, 0, 1},
                    {-2, 0, 2},
                    {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1},
                    { 0,  0,  0},
                    { 1,  2,  1}};

    for (int y = 1; y < img.height - 1; y++) {
        for (int x = 1; x < img.width - 1; x++) {
            double sumX = 0, sumY = 0;

            for (int j = -1; j <= 1; j++) {
                for (int i = -1; i <= 1; i++) {
                    int gray = (img(x+i, y+j, 0) + img(x+i, y+j, 1) + img(x+i, y+j, 2)) / 3;
                    sumX += gx[j+1][i+1] * gray;
                    sumY += gy[j+1][i+1] * gray;
                }
            }

            int mag = sqrt(sumX*sumX + sumY*sumY);
            if (mag > 90) mag = 255; else mag = 0;
            for (int k = 0; k < 3; k++) edge(x, y, k) = mag;
        }
    }

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            int edgeVal = edge(x, y, 0);
            if (edgeVal > 200) {
                result(x, y, 0) = 0;
                result(x, y, 1) = 0;
                result(x, y, 2) = 0;
            }
        }
    }

    img = result;
    cout << "Anime filter applied successfully.\n";
}


void warmCold(Image& img) {
    st.push(img);
    cout << "Choose tone: (1) Warm  (2) Cold: ";
    int choice; cin >> choice;

    cout << "Enter filter Strength (1 = light, 2 = medium, 3 = strong): ";
    int str; cin >> str;

    double factor;
    switch (str) {
        case 1: factor = 0.5; break;
        case 2: factor = 1.0; break;
        case 3: factor = 1.5; break;
        default: factor = 1.0; cout << "Default: medium Strength applied.\n"; break;
    }

    Image result = img;

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            int r = img(x, y, 0);
            int g = img(x, y, 1);
            int b = img(x, y, 2);

            if (choice == 1) {
                r = min(255, int(r * (1.05 + 0.1 * factor) + 5 * factor));
                g = min(255, int(g * (1.02 + 0.05 * factor)));
                b = max(0,   int(b * (1.0 - 0.1 * factor) - 5 * factor));
            } 
            else if (choice == 2) {
                r = max(0,   int(r * (1.0 - 0.1 * factor) - 5 * factor));
                g = min(255, int(g * (1.0)));
                b = min(255, int(b * (1.05 + 0.1 * factor) + 5 * factor));
            }
        result(x, y, 0) = (unsigned char)r;
        result(x, y, 1) = (unsigned char)g;
        result(x, y, 2) = (unsigned char)b;
        }
    }

    img = result;
    cout << ((choice == 1) ? "Warm" : "Cold") << " tone applied with Strength level " << str << ".\n";
}
