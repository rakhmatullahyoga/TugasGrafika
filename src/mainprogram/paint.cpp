#include "FrameBuffer.h"
#include "DrawingObject.h"
#include "RGBcolor.h"
#include "Point.h"
#include "Rectangle.h"
#include "Circle.h"
#include "Object.h"

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <pthread.h>
#include <termios.h>

using namespace std;

// GLOBAL VARIABLES
// variables for handling framebuffer
FrameBuffer frame;
DrawingObject draw, canvas;
RGBcolor WHITE;
RGBcolor BLACK;
RGBcolor RED;
RGBcolor GREEN;
RGBcolor BLUE;

// variable pointer data
int xd,yd; //x/y movement delta
int xo,yo; //x/y overflow (out of range -255 to +255)
int lb,mb,rb,hs,vs; //left/middle/right mousebutton

// variables for cache pointer data a moment before
int xd0,yd0;
int xo0,yo0;
int lb0,mb0,rb0,hs0,vs0; 

//variables for handle pointer
int resX, resY; // monitor resolution
int x, y; // center of monitor resolution
int margin; // margin of bound
Point ref1, ref2;

// loop variable
bool run;

vector<Point> points;

// Collection of objects
vector<Rectangle> rectangles;
vector<Circle> circles;
vector<Object> objects;

// GLOBAL FUNCTIONS
void renderObjects() {
        if(!rectangles.empty()) {
                for(int i=0; i<rectangles.size(); i++) {
                        rectangles.at(i).Draw(WHITE,frame);
                        //rectangles.at(i).updateFillColor(frame);
                }
        }
        if(!circles.empty()) {
                for(int i=0; i<circles.size(); i++) {
                        circles.at(i).Draw(WHITE,frame);
                }
        }
        if(!objects.empty()) {
                for(int i=0; i<objects.size(); i++) {
                        objects.at(i).Draw(frame);
                        //objects.at(i).updateFillColor(frame);
                }
        }
}

void* mouseListener(void* param) {
        //variables for handling mouse driver
        FILE *fmouse;
        char b[3];

        Point pointer(x, y);
        Point canvas_topleft(x+margin,margin);

        fmouse = fopen("/dev/input/mice","r");
        
        canvas.drawRectangle(canvas_topleft,resY-4*margin,resX/2-2*margin,WHITE,frame);
        while(run){
                //read from mouse driver
                renderObjects();
                fread(b,sizeof(char),3,fmouse);
                draw.plotCircle(pointer, 3, BLACK, frame);
                canvas.drawRectangle(canvas_topleft,resY-4*margin,resX/2-2*margin,WHITE,frame);
                lb=(b[0]&1)>0;  //left button
                rb=(b[0]&2)>0;  //right button
                mb=(b[0]&4)>0;  //middle button
                hs=(b[0]&16)>0; //horizonal slide
                vs=(b[0]&32)>0; //vertical slide
                xo=(b[0]&64)>0; //???
                yo=(b[0]&128)>0;//???
                xd=b[1];        //delta x from a moment before
                yd=b[2];        //delya y from a moment before

                x += xd; 
                //if pointer.x out of bound
                if(x>resX-margin-3) x = resX-margin-3;
                if(x<canvas_topleft.GetAbsis()+3) x = canvas_topleft.GetAbsis()+3;
                
                y -= yd;
                //if pointer.y out of bound
                if(y>resY-3*margin-3) y = resY-3*margin-3;
                if(y<margin+3) y = margin+3;
                
                //system("clear");

                pointer = Point(x, y);
                
                if(((rb0==0) && (mb==0) && (rb==0))) {
                        draw.plotCircle(pointer, 3, WHITE, frame);
                }
                else {
                        draw.plotCircle(pointer, 3, BLACK, frame);
                }

                hs0=hs; vs0=vs; lb0=lb; rb0=rb; mb0 = mb; xo0=xo; yo0=yo; xd0=xd; yd0=yd;
        }
        fclose(fmouse);
}

int _getch() {
        struct termios oldt, newt;
        int ch;
        tcgetattr( STDIN_FILENO, &oldt );
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr( STDIN_FILENO, TCSANOW, &newt );
        ch = getchar();
        tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
        return ch;
}

int main(){
        pthread_t listener;
        int input_option;
        int x_length, y_length;
        int radius;
        RGBcolor fillcolor;
        Point start;
        Point firePoint;

        // objek-objek gambar
        Rectangle rectangle;
        Circle circle;
        Object object; //polygon
        
        // variable pointer data
        xd=0; yd=0; //x/y movement delta
        xo=0; yo=0; //x/y overflow (out of range -255 to +255)
        lb=0; mb=0; rb=0; hs=0; vs=0; //left/middle/right mousebutton

        // variables for cache pointer data a moment before
        xd0=0; yd0=0;
        xo0=0; yo0=0;
        lb0=0; mb0=0; rb0=0; hs0=0; vs0=0; 

        //variables for handle pointer
        resX = 1366; resY = 768; // monitor resolution
        x=resX/2+margin+5; y=margin+5; // top-left of canvas
        margin = 10; // margin of bound
        
        WHITE.setRGB(255, 255, 255);
        BLACK.setRGB(1, 1, 1);
        RED.setRGB(255, 0, 0);
        GREEN.setRGB(0, 255, 0);
        BLUE.setRGB(0, 0, 255);

        // loop variable
        run = true;
        
        system("clear");
        if (pthread_create(&listener, NULL, mouseListener, NULL)) {
                cout << "gagal bikin thread" << endl;
                exit(1);
        }

        do {
                frame.clear();
                cout << "input keyboard:" << endl;
                cout << "1. polygon / line" << endl;
                cout << "2. rectangle" << endl;
                cout << "3. circle" << endl;
                cout << "4. fill color" << endl;
                cout << "5. clear" << endl;
                cout << "6. quit" << endl;
                cout << "input: ";
                cin >> input_option;
                switch (input_option) {
                        case 1:
                                cout << "klik kiri untuk menentukan titik" << endl;
                                cout << "klik kanan untuk selesai" << endl;
                                while(!rb){
                                        if(lb) {
                                                Point P(x,y);
                                                points.push_back(P);
                                                draw.plotListOfPoint(points, WHITE, frame);
                                        }
                                }
                                if(!points.empty()) {
                                        start = points.at(0);
                                        points.push_back(start);
                                        //x=resX/2+margin+5; y=margin+5; // top-left of canvas
                                        //draw.plotListOfPoint(points, WHITE, frame);
                                        object.setUpObject(points);
                                        object.Draw(frame);
                                        objects.push_back(object);
                                        points.clear();
                                }
                                break;
                        case 2:
                                cout << "klik kiri untuk menentukan top-left rectangle" << endl;
                                while(!lb);
                                start.SetAbsis(x);
                                start.SetOrdinat(y);
                                cout << "klik kanan untuk menentukan bottom-right rectangle" << endl;
                                while(!rb);
                                x_length = abs(x-start.GetAbsis());
                                y_length = abs(y-start.GetOrdinat());
                                //x=resX/2+margin+5; y=margin+5; // top-left of canvas
                                rectangle.setUpRectangle(start,x_length,y_length);
                                rectangle.Draw(WHITE,frame);
                                rectangles.push_back(rectangle);
                                break;
                        case 3:
                                cout << "klik untuk menentukan titik tengah lingkaran" << endl;
                                while(!lb);
                                start.SetAbsis(x);
                                start.SetOrdinat(y);
                                cout << "masukkan jari-jari lingkaran: ";
                                cin >> radius;
                                //x=resX/2+margin+5; y=margin+5; // top-left of canvas
                                circle.setUpCircle(start, radius);
                                circle.Draw(WHITE,frame);
                                circles.push_back(circle);
                                //draw.plotCircle(start,radius,WHITE,frame);
                                //firePoint = start;
                                break;
                        case 4:
                                fillcolor = frame.drawColorPicker();
                                cout << "klik kiri untuk melakukan filling" << endl;
                                cout << "klik kanan untuk selesai filling" << endl;
                                while(!rb){
                                        if(lb) {
                                                start.SetAbsis(x);
                                                start.SetOrdinat(y);
                                        }
                                }
                                draw.FloodFill(start.GetAbsis(),start.GetOrdinat(),frame.getWarna(start.GetAbsis(),start.GetOrdinat()),fillcolor,frame);
                                frame.hapusColorPelangi();
                                frame.hapusColorGradien();
                                frame.hapusPointedColor();
                                //x=resX/2+margin+5; y=margin+5; // top-left of canvas
                                //frame.hapusColorPelangiPicker();
                                //frame.hapusColorPelangiPicker();
                                break;
                        case 5:
                                rectangles.clear();
                                objects.clear();
                                circles.clear();
                                system("clear");
                                frame.clear();
                                cout << "clearing canvas . . ." << endl;
                                break;
                        case 6:
                                run = false;
                                break;
                        default:
                                break;
                }
                sleep(1);
        } while(run);


        pthread_join(listener, NULL);
        
	return 0;
}
