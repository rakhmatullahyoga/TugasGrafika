#include "FrameBuffer.h"
#include "DrawingObject.h"
#include "RGBcolor.h"
#include "Point.h"
#include "Pattern.h"
#include "Object.h"

using namespace std;

FrameBuffer frame;
void Meledak (int xpusat, int ypusat);
void antiLedak (int xpusat, int ypusat);
void gerak (Object kapal, Object pswt);
int main(int argc, char const *argv[])
{
	DrawingObject Peluru, Peluru2;
	RGBcolor black;
	black.setRGB(0,0,0);
	/*atribut Peluru*/
	RGBcolor warnaPeluru;
	Point peluruStart,peluru2Start;
	int radpeluru = 5;
	warnaPeluru.setRGB(255,255,0);

	peluruStart.SetAbsis(180);
	peluruStart.SetOrdinat(690);
	peluru2Start.SetAbsis(480);
	peluru2Start.SetOrdinat(100);

	/*Atribut Kapal*/
	Point kapalP, turretP;
	kapalP.SetAbsis(50);
	kapalP.SetOrdinat(400);
	turretP.SetAbsis(120);
	turretP.SetOrdinat(350);
	RGBcolor kapalCol;
	kapalCol.setRGB(234,34,74);
	//Kapal kapal;
	//kapal = Kapal(kapalP,turretP,50,15,50,150,kapalCol,frame);
	
	system("clear");
	bool meledak = false;
	/*Objek Pesawat*/
	Point pesawatP_top_left, pesawatP_bottom_right;
	pesawatP_top_left.SetAbsis(300);
	pesawatP_top_left.SetOrdinat(30);
	pesawatP_bottom_right.SetAbsis(560);
	pesawatP_bottom_right.SetOrdinat(140);
	
	Object pswt("pesawat2.txt");
	Pattern pattern("burung.txt");
	pswt.Draw(frame);
	//pswt.FloodFill(kapalCol,frame);
	//pswt.DrawPattern(pattern, frame, warnaPeluru);

	/*Objek Kapal*/
	
	
	Point kapalP_top_left, kapalP_bottom_right;
	kapalP_top_left.SetAbsis(370);
	kapalP_top_left.SetOrdinat(650);
	kapalP_bottom_right.SetAbsis(550);
	kapalP_bottom_right.SetOrdinat(690);
	RGBcolor kapal2Col;
	kapal2Col.setRGB(0,0,200);
	Object kapal("Kapal2.txt"); 	
	Pattern pattern2("ikan.txt");
	kapal.Draw(frame);
	//kapal.FloodFill(kapal2Col,frame);
	//kapal.DrawPattern(pattern2, frame, warnaPeluru);





/*Objek Buatan*/
	for(int i=1;i<50;i++){
		pswt.Draw(frame);
		//pswt.FloodFill(kapalCol,frame);
		//pswt.DrawPattern(pattern, frame, warnaPeluru);
		kapal.SetWarna(kapalCol);
		kapal.Draw(frame);
		//kapal.FloodFill(kapal2Col,frame);
		//kapal.DrawPattern(pattern2, frame, warnaPeluru);
		usleep(10000);
		pswt.Hapus(frame);
		kapal.Hapus(frame);
		//kapal.drawKapal();
		//kapal geser
		kapal.Geser(1,0);
		//kapal.setTurretPoint(kapal.getTurretPos().GetAbsis()+1, kapal.getTurretPos().GetOrdinat());
		pswt.Geser(-1,0);
	}

	while((peluruStart.GetOrdinat()>10) && !meledak){
		pswt.Draw(frame);
		//pswt.FloodFill(kapalCol,frame);
		//pswt.DrawPattern(pattern, frame, warnaPeluru);
		kapal.SetWarna(kapalCol);
		kapal.Draw(frame);
		//kapal.FloodFill(kapal2Col,frame);
		//kapal.DrawPattern(pattern2, frame, warnaPeluru);
		if(frame.isBlack(peluruStart.GetAbsis(),peluruStart.GetOrdinat()-radpeluru)){
			Peluru.plotCircle(peluruStart,radpeluru,warnaPeluru,frame);
			Peluru2.plotCircle(peluru2Start,radpeluru,warnaPeluru,frame);
			usleep(10000);
			Peluru.plotCircle(peluruStart,radpeluru,black,frame);
			Peluru2.plotCircle(peluru2Start,radpeluru,black,frame);
			peluruStart.SetOrdinat(peluruStart.GetOrdinat()-5);
			peluru2Start.SetOrdinat(peluru2Start.GetOrdinat()+5);
			pswt.Hapus(frame);
			pswt.Geser(-1,0);
			kapal.Hapus(frame);
			//kapal.SetWarna(black);
			//kapal.drawKapal();
			//kapal geser
			//kapal.setBodyPoint(kapal.getBodyPos().GetAbsis()+1, kapal.getBodyPos().GetOrdinat());
			//kapal.setTurretPoint(kapal.getTurretPos().GetAbsis()+1, kapal.getTurretPos().GetOrdinat());
			kapal.Geser(1,0);
		}
		else{
			Meledak(peluruStart.GetAbsis(),peluruStart.GetOrdinat());
			antiLedak(peluruStart.GetAbsis(),peluruStart.GetOrdinat());
			meledak = true;
		}
	}
	
	return 0;
}


void Meledak (int xpusat, int ypusat){
	DrawingObject Ledakan;
	RGBcolor warnaLedakan;
	warnaLedakan.setRed(255);
	warnaLedakan.setBlue(0);
	Point titikLedak;
	titikLedak.SetAbsis(xpusat);
	titikLedak.SetOrdinat(ypusat);
 	for(int radLedak=1; radLedak<=32; radLedak++){
 		warnaLedakan.setGreen(256-radLedak*7);
   		Ledakan.plotCircle(titikLedak,radLedak,warnaLedakan,frame);
   		usleep(10000);
 	}
}

void antiLedak (int xpusat, int ypusat){
	DrawingObject Ledakan;
	RGBcolor warnaLedakan;
	warnaLedakan.setRed(0);
	warnaLedakan.setGreen(0);
	warnaLedakan.setBlue(0);
	Point titikLedak;
	titikLedak.SetAbsis(xpusat);
	titikLedak.SetOrdinat(ypusat);
 	for(int radLedak=1; radLedak<=32; radLedak++){
   		Ledakan.plotCircle(titikLedak,radLedak,warnaLedakan,frame);
   		usleep(20000);
 	}
}
/*
void gerak(Kapal kapal, Pesawat pswt) {
	DrawingObject Circle;

	int i =0;
	//atribut Peluru
	RGBcolor warnaPeluru;
	Point peluruStart;
	int radpeluru = 5;
	warnaPeluru.setRed(255);
	warnaPeluru.setGreen(255);
	warnaPeluru.setBlue(0);
	peluruStart.SetAbsis(160);
	peluruStart.SetOrdinat(300);

	RGBcolor black;
	black.setRed(0);
	black.setBlue(0);
	black.setGreen(0);

	RGBcolor fore;
	fore.setRed(255);
	fore.setGreen(255);
	fore.setBlue(255);

	bool meledak = false, end = false;

	while(!end) {

		// pesawat
		pswt.Hapus();
		pswt.Geser(-1,0);
		pswt.Draw();

		// kapal
		
		kapal.SetWarna(black);
		kapal.drawKapal();
		kapal.setBodyPoint(kapal.getBodyPos().GetAbsis()+1, kapal.getBodyPos().GetOrdinat());
		kapal.setTurretPoint(kapal.getTurretPos().GetAbsis()+1, kapal.getTurretPos().GetOrdinat());
		kapal.SetWarna(fore);
		kapal.drawKapal();
		
		// peluru
		if (i>=10 && !end) {
			if (frame.isBlack(peluruStart.GetAbsis(),peluruStart.GetOrdinat()-radpeluru)) {
				Circle.plotCircle(peluruStart,radpeluru,black,frame);
				peluruStart.SetOrdinat(peluruStart.GetOrdinat()-1);
				Circle.plotCircle(peluruStart,radpeluru,warnaPeluru,frame);
			} else {
				Meledak(peluruStart.GetAbsis(),peluruStart.GetOrdinat());
				antiLedak(peluruStart.GetAbsis(),peluruStart.GetOrdinat());
				meledak = true;
				end = true;
			}
		}
		i++;
		usleep(10000);
	}
}
*/