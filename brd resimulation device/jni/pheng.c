#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <android/sensor.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <android/asset_manager.h>
#include <time.h>

void mymemcpy(float *x, float *y, float fr, int n){
	while(n--){
		x[n]=fr * y[n];}
}
void mymemadd(float *x, float *y, float fr, int n){
	while(n--){
		x[n]+=fr * y[n];}
}
void mymemcpy2(float *x, float *y, float fr, float *y2, float fr2, int n){
	while(n--){
		x[n]=fr * y[n] + fr2 * y2[n];}
}

extern GLsizei *vrtxnor;
extern GLsizei *trinor;
extern GLsizei *bnnor;
extern GLsizei *sttxbnnor;

extern GLushort *vrtxno;
extern GLsizei *trino;
extern GLushort *bnno;
extern GLushort *sttno;

extern GLfloat *vrtxPos;
extern GLfloat *vrtxClr;
extern GLushort *fcidx;
extern GLushort *bnidx;

extern GLfloat *bnorg;
extern GLushort *bnprnt;
extern GLfloat *statei_n;

#define maxbnno 20
#define maxobjno 30

extern float ax, ay, az, cpx, cpy, cpz, ner ,rgt, lft, top ,btm ,far;
extern int enwnobj[maxobjno+1];
extern GLfloat anim[maxobjno*3*maxbnno];
extern float cntr[3*maxobjno];

float dffx=0, dffy=0;
extern clock_t clck, clckld;
float dclck=0;

int mMiii =0,mMiiipry=0;
int mMiiild=0,mMiiildpry=0;
float mMt1=0,mMs1=0,mMs1pry=0,mMs2=0,mMaf1[10],mMaf1pry[10];
int mManm1[10],mManm1pry[10];
int mMltch=0,mMltch2=1,mMltchpry=0,mMltch2pry=1;
int mMii=0,mMiipry=0;
float mMay=0;
float mMldax=0,mMlday=0;

float prysttx=0,prystty=0,prysttxl=0,prysttyl=0,prysttl=1,prysttf=1,prysttxc=0,prysttyc=0;
float prypnc=0;
char customisation=0;
extern float invangx,invangy,swapangxy,invtchx,invtchy,swaptchxy;
float tmpswap;

struct saved_state {
		char pntrstt;
    float anglex;
    float angley;
    float anglez;
    float x;
    float y;
    float ldx;
    float ldy;
};

struct engine {
    struct android_app* app;

    ASensorManager* sensorManager;
    const ASensor* accelerometerSensor;
    ASensorEventQueue* sensorEventQueue;

    int animating;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int32_t width;
    int32_t height;
    struct saved_state state;
};

struct engine engine;

void anim_frame(){

dffx=-engine.state.ldx+engine.state.x;
dffy=-engine.state.ldy+engine.state.y;
clck = clock();
dclck = (float)(clck - clckld) / (float)(CLOCKS_PER_SEC);
clckld = clck;

if (customisation){

if(engine.state.pntrstt==1){
      if(engine.state.y>.10 && engine.state.y<.50 && engine.state.x>.51 && engine.state.x<.65){
swaptchxy=!swaptchxy;
}else if(engine.state.y>.10 && engine.state.y<.50 && engine.state.x>.43 && engine.state.x<.51){
invtchx=invtchx*-1;
}else if(engine.state.y>.10 && engine.state.y<.50 && engine.state.x>.23 && engine.state.x<.43){
invtchy=invtchy*-1;

}else if(engine.state.y>.50 && engine.state.y<.86 && engine.state.x>.51 && engine.state.x<.65){
swapangxy=!swapangxy;
}else if(engine.state.y>.50 && engine.state.y<.86 && engine.state.x>.43 && engine.state.x<.51){
invangx=invangx*-1;
}else if(engine.state.y>.50 && engine.state.y<.86 && engine.state.x>.23 && engine.state.x<.43){
invangy=invangy*-1;
}else if(engine.state.y>.8 && engine.state.x>.8){
enwnobj[17]=0;enwnobj[18]=0;
customisation = 0;
goto ela5r;
}else{
enwnobj[17]=0;
enwnobj[18]=0;
customisation = 0;
goto ela5r;
}}

mMii=18;
enwnobj[17]=6;

if(swaptchxy){
enwnobj[mMii]=5;
cntr[mMii*3+0]=.56*2-1;cntr[mMii*3+1]=-.13*2+1;
mMii++;
}
if(invtchx==-1){
enwnobj[mMii]=5;
cntr[mMii*3+0]=.48*2-1;cntr[mMii*3+1]=-.13*2+1;
mMii++;
}
if(invtchy==-1){
enwnobj[mMii]=5;
cntr[mMii*3+0]=.38*2-1;cntr[mMii*3+1]=-.13*2+1;
mMii++;
}
if(swapangxy){
enwnobj[mMii]=5;
cntr[mMii*3+0]=.56*2-1;cntr[mMii*3+1]=-.52*2+1;
mMii++;
}
if(invangx==-1){
enwnobj[mMii]=5;
cntr[mMii*3+0]=.48*2-1;cntr[mMii*3+1]=-.52*2+1;
mMii++;
}
if(invangy==-1){
enwnobj[mMii]=5;
cntr[mMii*3+0]=.38*2-1;cntr[mMii*3+1]=-.52*2+1;
mMii++;
}
enwnobj[mMii]=0;
goto ela5r;
}
	if (enwnobj[0]==0){
		enwnobj[0]=1;
		enwnobj[1]=4;
		enwnobj[2]=2;
		enwnobj[3]=3;
		enwnobj[4]=4;
enwnobj[5]=4;enwnobj[6]=4;enwnobj[7]=4;enwnobj[8]=4;enwnobj[9]=4;enwnobj[10]=4;
enwnobj[11]=4;enwnobj[12]=4;enwnobj[13]=4;
		enwnobj[14]=0;
		enwnobj[15]=5;enwnobj[16]=5;enwnobj[17]=0;
		cntr[0*3+0]=0;cntr[0*3+1]=0;cntr[0*3+2]=2000;
		cntr[1*3+0]=0;cntr[1*3+1]=29;cntr[1*3+2]=1500;
		cntr[4*3+0]=0;cntr[4*3+1]=29;cntr[4*3+2]=1500;
		cntr[5*3+0]=0;cntr[5*3+1]=29;cntr[5*3+2]=1500;
		cntr[2*3+0]=0;cntr[2*3+1]=0;cntr[2*3+2]=0;
		cntr[3*3+0]=0;cntr[3*3+1]=0;cntr[3*3+2]=0;
	}
if(( (cntr[0*3+2]-cntr[1*3+2])<5 )&&((cntr[1*3+2]-cntr[0*3+2])<10)){
	if(( (cntr[0*3+0]-cntr[1*3+0])<100 )&&((cntr[1*3+0]-cntr[0*3+0])<100)){
		if(( (cntr[0*3+1]-cntr[1*3+1])<100 )&&((cntr[1*3+1]-cntr[0*3+1])<100)){
			prypnc=55;
		}else{if(prypnc>55)prypnc=0;prypnc+=3*dclck;}
	}else{if(prypnc>55)prypnc=0;prypnc+=3*dclck;}
}else{if(prypnc>55)prypnc=0;prypnc+=3*dclck;}
prypnc+=2*dclck;

if(prysttl>=1){prysttl=0;
	prysttyl = prystty;
	prysttxl = prysttx;
	prystty = rand()/(float)RAND_MAX;
	prysttx = rand()/(float)RAND_MAX;
	prysttf = rand()/(float)RAND_MAX;
	if(prysttx>.56)prysttx=.56;
	if(prysttx<.46)prysttx=.46;

	if(!prysttf)prysttf=.01;
}prysttl+=prysttf*dclck;

mymemcpy2 ( &prysttyc , &prystty , prysttl , &prysttyl , 1-prysttl, 1);
mymemcpy2 ( &prysttxc , &prysttx , prysttl , &prysttxl , 1-prysttl, 1);

if((prypnc>40)&&(!mMltchpry)){
	mMltchpry=1;mMs1pry=2;mMiipry=-1;
mMaf1pry[0]=2*2*6.501;
mMaf1pry[1]=1.5*2*2*6.501;
mMaf1pry[2]=2*2*4.334;
mMaf1pry[3]=1.5*2*2*4.334;
mMaf1pry[4]=2*2*4.334;
mMaf1pry[5]=2*2*.000002;
mManm1pry[0]=1;
mManm1pry[1]=2;
mManm1pry[2]=3;
mManm1pry[3]=4;
mManm1pry[4]=0;
mManm1pry[5]=-1;
}
if(mMltchpry){
	if (mMs1pry>1){
		mMiipry++;mMs1pry=0;
		mMiiildpry=mMiiipry;
		if(mManm1pry[mMiipry]==-1){mMltchpry=0;
		}else{mMiiipry=mManm1pry[mMiipry];}
	}
	mMs1pry+=dclck*mMaf1pry[mMiipry];
	if(mMltchpry==1){
		anim[1*3*20+0]+=2*.01*(prysttyc-0.5);
		anim[1*3*20+1]-=0.09*(prysttyc-0.5);
		anim[1*3*20+2]-=8*.01*(prysttxc-0.5);
		cntr[1*3+1]+=dclck*30*cos(anim[3*20+0])*cos(anim[3*20+2]);
		cntr[1*3+0]+=dclck*30*sin(anim[3*20+0])*cos(anim[3*20+2]);
		cntr[1*3+2]-=dclck*30*sin(anim[3*20+2]); mMltch2pry=1;
	}
}else{

if (mMltch2pry==1){
	mMiiildpry=0;
	mMiiipry=0;
	mMs1pry=0;
}
cntr[1*3+1]+=dclck*10*cos(anim[3*20+0])*cos(anim[3*20+2]);
cntr[1*3+0]+=dclck*10*sin(anim[3*20+0])*cos(anim[3*20+2]);
cntr[1*3+2]-=dclck*10*sin(anim[3*20+2]);
if(anim[3*20+1]>0.0)anim[3*20+1]-=.05;
if(anim[3*20+1]<0.0)anim[3*20+1]+=.05;
if(anim[3*20+2]<0.0)anim[3*20+2]+=.00032;
cntr[1*3+2]-=dclck*.5;
}
if(anim[3*20+1]<(-3.14/8.5))
	anim[3*20+1] = -3.14/8.5;
else if(anim[3*20+1]>(3.14/8.5))
	anim[3*20+1] = 3.14/8.5;

if(anim[3*20+2]<(-3.14/8.5))
	anim[3*20+2] = -3.14/8.5;
else if(anim[3*20+2]>(3.14/8.5))
	anim[3*20+2] = 3.14/8.5;
if (cntr[1*3+2] < 1000) cntr[1*3+2] = 2000;
if (cntr[1*3+2] > 2050) cntr[1*3+2] = 2050;
if ( (cntr[1*3+0] < -7000) || (cntr[1*3+0] > 7000)) cntr[1*3+0] *= -1;
if ( (cntr[1*3+1] < -7000) || (cntr[1*3+1] > 7000)) cntr[1*3+1] *= -1;

if(engine.state.pntrstt&&(!mMltch)){
if(engine.state.y>.8 && engine.state.x>.8){
customisation=1;
}else if(engine.state.y<.1 && engine.state.x>0.9){
mMay = (engine.state.angley / 10.0 )-.2;
}else if(engine.state.y>.38 && engine.state.y<.62 && engine.state.x<.15){
	mMltch=2;mMs1=2;mMii=-1;
mMaf1[0]=2*6.501;
mMaf1[1]=2*6.501;
mMaf1[2]=2*4.334;
mMaf1[3]=2*4.334;
mMaf1[4]=2*4.334;
mMaf1[5]=2*2*.000002;
mManm1[0]=1;
mManm1[1]=2;
mManm1[2]=3;
mManm1[3]=4;
mManm1[4]=11;
mManm1[5]=-1;
}else{
	mMltch=1;mMs1=2;mMii=-1;
mMaf1[0]=2*2*6.501*1.6;
mMaf1[1]=2*2*6.501*1.6;
mMaf1[2]=2*2*4.334*1.6;
mMaf1[3]=2*2*4.334*1.6;
mMaf1[4]=2*2*4.334*1.6;
mMaf1[5]=2*2*.000002;
mManm1[0]=1;
mManm1[1]=2;
mManm1[2]=3;
mManm1[3]=4;
mManm1[4]=0;
mManm1[5]=-1;
}
}

if(mMltch){
	if (mMs1>1){
		mMii++;mMs1=0;
		mMiiild=mMiii;
		if(mManm1[mMii]==-1){mMltch=0;
		}else{mMiii=mManm1[mMii];}
	}
	mMs1+=dclck*mMaf1[mMii];
	if(mMltch==1){
		anim[0]+=2*.01*(engine.state.y-0.5);
		anim[1]-=0.09*(engine.state.y-0.5);
		anim[2]-=8*.01*(engine.state.x-0.5);
		cntr[0*3+1]+=dclck*30*cos(anim[0])*cos(anim[2]);
		cntr[0*3+0]+=dclck*30*sin(anim[0])*cos(anim[2]);
		cntr[0*3+2]-=dclck*30*sin(anim[2]); mMltch2=1;
	}else	if(mMltch==2){
		if(anim[2]<(3.1416/2.101))
		anim[2]+=.03; mMltch2=2;
	}
}else{

if (mMltch2==1){
	mMiiild=0;
	mMiii=0;
	mMs1=0;
}else if (mMltch2==2){
	mMiiild=11;
	mMiii=11;
	mMs1=0;
		if(anim[2]<(3.1416/2.101))
		anim[2]+=.03;
}
cntr[0*3+1]+=dclck*10*cos(anim[0])*cos(anim[2]);
cntr[0*3+0]+=dclck*10*sin(anim[0])*cos(anim[2]);
cntr[0*3+2]-=dclck*10*sin(anim[2]);
if(anim[1]>0.0)anim[1]-=.05;
if(anim[1]<0.0)anim[1]+=.05;
if(anim[2]<0.0)anim[2]+=.00032;
if (mMltch2==2){cntr[0*3+2]-=1;}
cntr[0*3+2]-=dclck*.5;
}

if(anim[1]<(-3.14/8.5))
	anim[1] = -3.14/8.5;
else if(anim[1]>(3.14/8.5))
	anim[1] = 3.14/8.5;

if(anim[2]<(-3.14/2.01))
	anim[2] = -3.14/2.01;
else if(anim[2]>(3.14/2.01))
	anim[2] = 3.14/2.01;

if (cntr[0*3+2] < 1000) cntr[0*3+2] = 2000;
if (cntr[0*3+2] > 2050) cntr[0*3+2] = 2050;
if ( (cntr[0*3+0] < -7000) || (cntr[0*3+0] > 7000)) cntr[0*3+0] *= -1;
if ( (cntr[0*3+1] < -7000) || (cntr[0*3+1] > 7000)) cntr[0*3+1] *= -1;

mymemcpy2 ( &anim[0*3*20+3] , &statei_n[sttxbnnor[0]+mMiii*bnno[0]*3+3] , mMs1 , &statei_n[sttxbnnor[0]+mMiiild*bnno[0]*3+3] , 1-mMs1, (bnno[0]-1)*3);
mymemcpy2 ( &anim[1*3*20+3] , &statei_n[sttxbnnor[0]+mMiiipry*bnno[0]*3+3] , mMs1pry , &statei_n[sttxbnnor[0]+mMiiildpry*bnno[0]*3+3] , 1-mMs1pry, (bnno[0]-1)*3);

mymemcpy2 ( &anim[4*3*20+3] , &statei_n[sttxbnnor[0]+mMiiipry*bnno[0]*3+3] , mMs1pry+.1 , &statei_n[sttxbnnor[0]+mMiiildpry*bnno[0]*3+3] , 1-mMs1pry, (bnno[0]-1)*3);
mymemcpy2 ( &anim[5*3*20+3] , &statei_n[sttxbnnor[0]+mMiiipry*bnno[0]*3+3] , mMs1pry-.2 , &statei_n[sttxbnnor[0]+mMiiildpry*bnno[0]*3+3] , 1-mMs1pry, (bnno[0]-1)*3);
mymemcpy2 ( &anim[6*3*20+3] , &statei_n[sttxbnnor[0]+mMiiipry*bnno[0]*3+3] , mMs1pry+.05 , &statei_n[sttxbnnor[0]+mMiiildpry*bnno[0]*3+3] , 1-mMs1pry, (bnno[0]-1)*3);
mymemcpy2 ( &anim[7*3*20+3] , &statei_n[sttxbnnor[0]+mMiiipry*bnno[0]*3+3] , mMs1pry+.04 , &statei_n[sttxbnnor[0]+mMiiildpry*bnno[0]*3+3] , 1-mMs1pry, (bnno[0]-1)*3);
mymemcpy2 ( &anim[8*3*20+3] , &statei_n[sttxbnnor[0]+mMiiipry*bnno[0]*3+3] , mMs1pry-.7 , &statei_n[sttxbnnor[0]+mMiiildpry*bnno[0]*3+3] , 1-mMs1pry, (bnno[0]-1)*3);
mymemcpy2 ( &anim[9*3*20+3] , &statei_n[sttxbnnor[0]+mMiiipry*bnno[0]*3+3] , mMs1pry-.01 , &statei_n[sttxbnnor[0]+mMiiildpry*bnno[0]*3+3] , 1-mMs1pry, (bnno[0]-1)*3);
mymemcpy2 ( &anim[10*3*20+3] , &statei_n[sttxbnnor[0]+mMiiipry*bnno[0]*3+3] , mMs1pry+.08 , &statei_n[sttxbnnor[0]+mMiiildpry*bnno[0]*3+3] , 1-mMs1pry, (bnno[0]-1)*3);
mymemcpy2 ( &anim[11*3*20+3] , &statei_n[sttxbnnor[0]+mMiiipry*bnno[0]*3+3] , mMs1pry+.9 , &statei_n[sttxbnnor[0]+mMiiildpry*bnno[0]*3+3] , 1-mMs1pry, (bnno[0]-1)*3);
mymemcpy2 ( &anim[12*3*20+3] , &statei_n[sttxbnnor[0]+mMiiipry*bnno[0]*3+3] , mMs1pry+.8 , &statei_n[sttxbnnor[0]+mMiiildpry*bnno[0]*3+3] , 1-mMs1pry, (bnno[0]-1)*3);
mymemcpy2 ( &anim[13*3*20+3] , &statei_n[sttxbnnor[0]+mMiiipry*bnno[0]*3+3] , mMs1pry+.4 , &statei_n[sttxbnnor[0]+mMiiildpry*bnno[0]*3+3] , 1-mMs1pry, (bnno[0]-1)*3);



anim[4*3*20+0] = anim[5*3*20+0] = anim[6*3*20+0] = anim[7*3*20+0] = anim[8*3*20+0] = anim[9*3*20+0] = anim[10*3*20+0] = anim[11*3*20+0] = anim[12*3*20+0] = anim[13*3*20+0] = anim[1*3*20+0];
anim[4*3*20+1] = anim[5*3*20+1] = anim[6*3*20+1] = anim[7*3*20+1] = anim[8*3*20+1] = anim[9*3*20+1] = anim[10*3*20+1] = anim[11*3*20+1] = anim[12*3*20+1] = anim[13*3*20+1] = anim[1*3*20+1];
anim[4*3*20+2] = anim[5*3*20+2] = anim[6*3*20+2] = anim[7*3*20+2] = anim[8*3*20+2] = anim[9*3*20+2] = anim[10*3*20+2] = anim[11*3*20+2] = anim[12*3*20+2] = anim[13*3*20+2] = anim[1*3*20+2];
cntr[5*3+0] = cntr[1*3+0] +30;
cntr[5*3+1] = cntr[1*3+1] +30;
cntr[5*3+2] = cntr[1*3+2] -.5;
cntr[4*3+0] = cntr[5*3+0]-10.5;
cntr[4*3+1] = cntr[5*3+1]-10.5;
cntr[4*3+2] = cntr[5*3+2]-0.5;
cntr[6*3+0] = cntr[5*3+0]-15.5;
cntr[6*3+1] = cntr[5*3+1]-20.5;
cntr[6*3+2] = cntr[5*3+2]-0.5;
cntr[7*3+0] = cntr[6*3+0]+20.5;
cntr[7*3+1] = cntr[6*3+1]-10.5;
cntr[7*3+2] = cntr[6*3+2]-0.5;
cntr[8*3+0] = cntr[7*3+0]-10.5;
cntr[8*3+1] = cntr[7*3+1]-10.5;
cntr[8*3+2] = cntr[7*3+2]-0.5;
cntr[9*3+0] = cntr[8*3+0]+10.5;
cntr[9*3+1] = cntr[8*3+1]-10.5;
cntr[9*3+2] = cntr[8*3+2]-0.5;
cntr[10*3+0] = cntr[9*3+0]+10.5;
cntr[10*3+1] = cntr[9*3+1]-10.5;
cntr[10*3+2] = cntr[9*3+2]-0.5;
cntr[11*3+0] = cntr[10*3+0]-10.5;
cntr[11*3+1] = cntr[10*3+1]-10.5;
cntr[11*3+2] = cntr[10*3+2]-0.5;
cntr[12*3+0] = cntr[11*3+0]+20.5;
cntr[12*3+1] = cntr[11*3+1]-10.5;
cntr[12*3+2] = cntr[11*3+2]-0.5;
cntr[13*3+0] = cntr[12*3+0]-10.5;
cntr[13*3+1] = cntr[12*3+1]-10.5;
cntr[13*3+2] = cntr[12*3+2]-0.5;

engine.state.anglex -= .8* (engine.state.anglex - mMldax);
engine.state.angley -= .8* (engine.state.angley - mMlday);

ax=0 + (engine.state.anglex / 10.0 ); ay=anim[2]-3.1416/2+.2 +.45-(engine.state.angley / 10.0 )+mMay; az=-3.1416/2.0 + anim[0]; //ylfo2 wet7t dma8k
mMldax = engine.state.anglex;
mMlday = engine.state.angley;

cpx= -cntr[0*3+0] + 2 * sin(anim[0])*cos(anim[2])  - 1 * sin(anim[2]) * sin(anim[0]);
cpy= -cntr[0*3+1] + 2 * cos(anim[0])*cos(anim[2])  - 1 * sin(anim[2]) * cos(anim[0]);
cpz= -cntr[0*3+2] - 2 * sin(anim[2])               - 1 * cos(anim[2]) ;

float fctr=.1*2.0*.6*2*.9;
ner=.4; far=-7-10-50; rgt=1*fctr; lft=-1*fctr;
top=((float)(engine.height)/(float)(engine.width))*fctr;
btm=-((float)(engine.height)/(float)(engine.width))*fctr;

cntr[15*3+0] = .90;
cntr[15*3+1] = 0.9;
cntr[15*3+2] = 0;
cntr[16*3+0] = .90;
cntr[16*3+1] = -0.9;
cntr[16*3+2] = 0;
ela5r:
{}
}



