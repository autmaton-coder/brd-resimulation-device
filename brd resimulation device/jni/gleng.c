#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <stdio.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <android/sensor.h>
GLsizei *vrtxnor;
GLsizei *trinor;
GLsizei *bnnor;
GLsizei *sttxbnnor;

GLushort *vrtxno;
GLsizei *trino;
GLushort *bnno;
GLushort *sttno;

GLfloat *vrtxPos;
GLfloat *vrtxClr;
GLushort *fcidx;
GLfloat *bnidx;

GLfloat *bnorg;
GLfloat *bnprnt;
GLfloat *statei_n;

extern GLfloat myvrtxPos[];
extern GLfloat myvrtxClr[];
extern GLushort myfcidx[];
extern GLfloat mybnidx[];
extern GLfloat mybnorg[];
extern GLfloat mybnprnt[];
extern GLfloat mystatei_n[];

GLsizei objblprno=1;
GLsizei objdrno;

#define maxbnno 20
#define maxobjno 30

float ax, ay, az, cpx, cpy, cpz, ner ,rgt, lft, top ,btm ,far;

int enwnobj[maxobjno+1];
GLfloat anim[maxobjno*3*maxbnno];
float cntr[3*maxobjno];

GLuint bfr[4];
GLuint bnorgID, bnprntID, MatrixID, sithph=-1;
GLfloat ff[4][4]={{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
int ii;

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

void mylog(void);

GLuint compile_shaders(void) {
	GLuint vertex_shader, fragment_shader, program;

	static const GLchar * vertex_shader_source[] = {

		"uniform mat4 persXcamMatrix;\n"
		"uniform float stp[3*20];\n"
		"uniform float bnorg[3*20];\n"
		"uniform float bnprnt[20];\n"

		"attribute float bnidx;\n"
		"attribute vec3 v_position;\n"
		"attribute vec3 v_color;\n"
		"varying vec3 colorvr;\n"

		"void main(void) {\n"

			"float psi, tha, phi, x, y, z;"
			"mat4 preanim, tmpm;\n"
			"mat4 rslt=mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);\n;"
			"int tmpi=int(bnidx);\n"
			"while (tmpi > 0){"
				"tmpi=tmpi*3;"
				"psi=stp[tmpi]; tha=stp[tmpi+1]; phi=stp[tmpi+2];"
				"x=bnorg[tmpi]; y=bnorg[tmpi+1]; z=bnorg[tmpi+2];\n"

				"preanim=mat4(cos(tha)*cos(psi),-cos(tha)*sin(psi),sin(tha),0.0,"
"cos(phi)*sin(psi)+sin(phi)*sin(tha)*cos(psi),cos(phi)*cos(psi)-sin(phi)*sin(tha)*sin(psi),-sin(phi)*cos(tha),0.0,"
"sin(phi)*sin(psi)-cos(phi)*sin(tha)*cos(psi),sin(phi)*cos(psi)+cos(phi)*sin(tha)*sin(psi),cos(phi)*cos(tha),0.0,"
"x,y,z,1.0);\n"

				"tmpm=mat4(1,0,0,0,0,1,0,0,0,0,1,0,-x,-y,-z,1);\n"
				"rslt=preanim * tmpm * rslt;"
				"tmpi=tmpi/3;\n"
				"tmpi=int(bnprnt[tmpi]);}"
			"psi=stp[0]; tha=stp[1]; phi=stp[2];"
			"x=bnorg[0]; y=bnorg[1]; z=bnorg[2];\n"
			"preanim=mat4(cos(tha)*cos(psi),-cos(tha)*sin(psi),sin(tha),0.0,"
"cos(phi)*sin(psi)+sin(phi)*sin(tha)*cos(psi),cos(phi)*cos(psi)-sin(phi)*sin(tha)*sin(psi),-sin(phi)*cos(tha),0.0,"
"sin(phi)*sin(psi)-cos(phi)*sin(tha)*cos(psi),sin(phi)*cos(psi)+cos(phi)*sin(tha)*sin(psi),cos(phi)*cos(tha),0.0,"
"x,y,z,1.0);\n"

			"gl_Position = persXcamMatrix * preanim * rslt * vec4(v_position,1.0);"
			"colorvr=v_color;"

		"}"
	};

	static const GLchar * fragment_shader_source[] = {
		"varying lowp vec3 colorvr;\n"
		"void main(void) {\n"
		"gl_FragColor=vec4( colorvr, 1.0);}"
	};

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1,fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

glBindAttribLocation(program, 0, "v_position");
glBindAttribLocation(program, 1, "v_color");
glBindAttribLocation(program, 2, "bnidx");

	glLinkProgram(program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	return program;
}

void init_opngl(){

GLuint rendering_program;

rendering_program=compile_shaders();

MatrixID = glGetUniformLocation(rendering_program, "persXcamMatrix");
sithph = glGetUniformLocation(rendering_program, "stp");
bnorgID = glGetUniformLocation(rendering_program, "bnorg");
bnprntID = glGetUniformLocation(rendering_program, "bnprnt");

glGenBuffers( (GLsizei) 4, bfr);
glUseProgram(rendering_program);

objblprno=6;
    vrtxno=malloc(2*objblprno);
    trino=malloc(4*objblprno);
    bnno=malloc(2*objblprno);
    sttno=malloc(2*objblprno);

    trinor = malloc(4*(objblprno+1));
    vrtxnor = malloc(4*(objblprno+1));
		bnnor = malloc(4*(objblprno+1));
		sttxbnnor = malloc(4*(objblprno+1));

		*trinor=0;
		*vrtxnor=0;
		*bnnor=0;
		*sttxbnnor=0;

vrtxno[0]=634;vrtxno[1]=4486;vrtxno[2]=135;vrtxno[3]=747;vrtxno[4]=15;vrtxno[5]=3889;
trino[0]=805;trino[1]=8734;trino[2]=240;trino[3]=1044;trino[4]=13;trino[5]=3799;
bnno[0]=7;bnno[1]=1;bnno[2]=1;bnno[3]=7;bnno[4]=1;bnno[5]=1;
sttno[0]=12;sttno[1]=1;sttno[2]=1;sttno[3]=12;sttno[4]=1;sttno[5]=1;

    ii=1;
    while(ii<(objblprno+1)){
        vrtxnor[ii]=vrtxno[ii-1]+vrtxnor[ii-1];
        trinor[ii]=trino[ii-1]+trinor[ii-1];
        bnnor[ii]=bnno[ii-1]+bnnor[ii-1];
        sttxbnnor[ii]=sttno[ii-1]*bnno[ii-1]+sttxbnnor[ii-1];
				ii++;
    }

vrtxPos = myvrtxPos;
vrtxClr = myvrtxClr;
fcidx = myfcidx;
bnidx = mybnidx;
bnorg = mybnorg;
bnprnt = mybnprnt;
statei_n = mystatei_n;

glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bfr[0]);
glBufferData( GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr) trinor[objblprno]*3*2, (const void*) fcidx, GL_STATIC_DRAW);//DYNAMIC
glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0);

glBindBuffer( GL_ARRAY_BUFFER, bfr[1]);
glBufferData( GL_ARRAY_BUFFER, (GLsizeiptr) vrtxnor[objblprno]*3*4, (const void*) vrtxPos, GL_STATIC_DRAW);//DYNAMIC sizenot stride
glBindBuffer( GL_ARRAY_BUFFER, 0);

glBindBuffer( GL_ARRAY_BUFFER, bfr[2]);
glBufferData( GL_ARRAY_BUFFER, (GLsizeiptr) vrtxnor[objblprno]*3*4, (const void*) vrtxClr, GL_STATIC_DRAW);//DYNAMIC sizenot stride
glBindBuffer( GL_ARRAY_BUFFER, 0);

glBindBuffer( GL_ARRAY_BUFFER, bfr[3]);
glBufferData( GL_ARRAY_BUFFER, (GLsizeiptr) vrtxnor[objblprno]*4, (const void*) bnidx, GL_STATIC_DRAW);//DYNAMIC sizenot stride
glBindBuffer( GL_ARRAY_BUFFER, 0);

glBindBuffer( GL_ARRAY_BUFFER, bfr[1]);
glVertexAttribPointer((GLuint) 0, (GLint) 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
glEnableVertexAttribArray((GLuint) 0);

glBindBuffer( GL_ARRAY_BUFFER, bfr[2]);
glVertexAttribPointer((GLuint) 1, (GLint) 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
glEnableVertexAttribArray((GLuint) 1);

glBindBuffer( GL_ARRAY_BUFFER, bfr[3]);
glVertexAttribPointer((GLuint) 2, (GLint) 1, GL_FLOAT, GL_FALSE, 0, (void*)0);
glEnableVertexAttribArray((GLuint) 2);

glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bfr[0]);

glUniform1fv(bnprntID,maxbnno,(GLfloat*)&anim[0]);
glUniform1fv(bnorgID,3*maxbnno,(GLfloat*)&anim[0]);
glUniform1fv(sithph,maxbnno*3,(GLfloat*)&anim[0]);
}

void draw_glframe(){

glClearColor(0.9,0.0,0.0,1.0f);

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

ff[0][0]=2*ner*cos(ay)*cos(az)/(-lft + rgt);///xyzmnzor
ff[0][1]=-2*ner*sin(ax)*sin(ay)*cos(az)/(-btm + top) + 2*ner*sin(az)*cos(ax)/(-btm + top);
ff[0][2]=(-far - ner)*sin(ax)*sin(az)/(far - ner) + (-far - ner)*sin(ay)*cos(ax)*cos(az)/(far - ner);
ff[0][3]=-sin(ax)*sin(az) - sin(ay)*cos(ax)*cos(az);
ff[1][0]=-2*ner*sin(az)*cos(ay)/(-lft + rgt);
ff[1][1]=2*ner*sin(ax)*sin(ay)*sin(az)/(-btm + top) + 2*ner*cos(ax)*cos(az)/(-btm + top);
ff[1][2]=(-far - ner)*sin(ax)*cos(az)/(far - ner) - (-far - ner)*sin(ay)*sin(az)*cos(ax)/(far - ner);
ff[1][3]=-sin(ax)*cos(az) + sin(ay)*sin(az)*cos(ax);
ff[2][0]=-2*ner*sin(ay)/(-lft + rgt);
ff[2][1]=-2*ner*sin(ax)*cos(ay)/(-btm + top);
ff[2][2]=(-far - ner)*cos(ax)*cos(ay)/(far - ner);
ff[2][3]=-cos(ax)*cos(ay);
ff[3][0]=2*ner*cpx*cos(ay)*cos(az)/(-lft + rgt) - 2*ner*cpy*sin(az)*cos(ay)/(-lft + rgt) - 2*ner*cpz*sin(ay)/(-lft + rgt) - ner*(lft + rgt)/(-lft + rgt);
ff[3][1]=-2*ner*cpz*sin(ax)*cos(ay)/(-btm + top) - ner*(btm + top)/(-btm + top) + cpx*(-2*ner*sin(ax)*sin(ay)*cos(az)/(-btm + top) + 2*ner*sin(az)*cos(ax)/(-btm + top)) + cpy*(2*ner*sin(ax)*sin(ay)*sin(az)/(-btm + top) + 2*ner*cos(ax)*cos(az)/(-btm + top));
ff[3][2]=2*far*ner/(-far + ner) + cpx*((-far - ner)*sin(ax)*sin(az)/(far - ner) + (-far - ner)*sin(ay)*cos(ax)*cos(az)/(far - ner)) + cpy*((-far - ner)*sin(ax)*cos(az)/(far - ner) - (-far - ner)*sin(ay)*sin(az)*cos(ax)/(far - ner)) + cpz*(-far - ner)*cos(ax)*cos(ay)/(far - ner);
ff[3][3]=cpx*(-sin(ax)*sin(az) - sin(ay)*cos(ax)*cos(az)) + cpy*(-sin(ax)*cos(az) + sin(ay)*sin(az)*cos(ax)) - cpz*cos(ax)*cos(ay);

glUniformMatrix4fv(MatrixID,1,GL_FALSE,(GLfloat*)&ff[0][0]);

ii=0;
while( ii < maxobjno){
	if(enwnobj[ii]==0){break;}

	bnorg[bnnor[enwnobj[ii]-1]*3+0]=cntr[3*ii+0];
	bnorg[bnnor[enwnobj[ii]-1]*3+1]=cntr[3*ii+1];
	bnorg[bnnor[enwnobj[ii]-1]*3+2]=cntr[3*ii+2];
	glUniform1fv(bnorgID,bnno[enwnobj[ii]-1]*3,(GLfloat*)&bnorg[bnnor[enwnobj[ii]-1]*3]);
	glUniform1fv(bnprntID,bnno[enwnobj[ii]-1],(GLfloat*)&bnprnt[bnnor[enwnobj[ii]-1]]);
	glUniform1fv(sithph,maxbnno*3,(GLfloat*)&anim[ii*maxbnno*3+0]);
	glDrawElements( GL_TRIANGLES, (GLsizei) trino[enwnobj[ii]-1]*3, GL_UNSIGNED_SHORT, (void*)(0+(trinor[enwnobj[ii]-1]*3*sizeof(GLushort))));
ii++;

}

ff[0][0]=1;ff[0][1]=0;ff[0][2]=0;ff[0][3]=0;ff[1][0]=0;ff[1][1]=1;ff[1][2]=0;ff[1][3]=0;ff[2][0]=0;
ff[2][1]=0;ff[2][2]=1;ff[2][3]=0;ff[3][0]=0;ff[3][1]=0;ff[3][2]=0;ff[3][3]=1;
glUniformMatrix4fv(MatrixID,1,GL_FALSE,(GLfloat*)&ff[0][0]);
ii++;
while(enwnobj[ii]){
	if(enwnobj[ii]==0){break;}
	bnorg[bnnor[enwnobj[ii]-1]*3+0]=cntr[3*ii+0];
	bnorg[bnnor[enwnobj[ii]-1]*3+1]=cntr[3*ii+1];
	bnorg[bnnor[enwnobj[ii]-1]*3+2]=cntr[3*ii+2];
	glUniform1fv(bnorgID,bnno[enwnobj[ii]-1]*3,(GLfloat*)&bnorg[bnnor[enwnobj[ii]-1]*3]);
	glUniform1fv(bnprntID,bnno[enwnobj[ii]-1],(GLfloat*)&bnprnt[bnnor[enwnobj[ii]-1]]);
	glUniform1fv(sithph,maxbnno*3,(GLfloat*)&anim[ii*maxbnno*3+0]);
	glDrawElements( GL_TRIANGLES, (GLsizei) trino[enwnobj[ii]-1]*3, GL_UNSIGNED_SHORT, (void*)(0+(trinor[enwnobj[ii]-1]*3*sizeof(GLushort))));
ii++;
}


}

void appglfreeing(){
free(vrtxnor);
free(trinor);
free(bnnor);
free(sttxbnnor);
free(vrtxno);
free(trino);
free(bnno);
free(sttno);
}

