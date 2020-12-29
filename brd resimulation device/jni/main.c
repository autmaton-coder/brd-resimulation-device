#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include <android_native_app_glue.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

float invangx=1,invangy=1,swapangxy=1,invtchx=1,invtchy=1,swaptchxy=0;

clock_t clck=0, clckld=0;

void init_opngl();
void draw_glframe();
void anim_frame();
void appglfreeing(void);

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

static int engine_init_display(struct engine* engine) {

    const EGLint attribs[] = {
						EGL_RENDERABLE_TYPE,EGL_OPENGL_ES2_BIT,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
EGL_DEPTH_SIZE, 8,//16
            EGL_NONE
    };
    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

    eglChooseConfig(display, attribs, &config, 1, &numConfigs);

    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

const EGLint contatr[]={EGL_CONTEXT_CLIENT_VERSION,2,EGL_NONE};
    surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
    context = eglCreateContext(display, config, NULL, contatr);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        return -1;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    engine->display = display;
    engine->context = context;
    engine->surface = surface;
    engine->width = w;
    engine->height = h;
    engine->state.anglex = engine->state.angley = engine->state.anglez = 0;

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glEnable(GL_CULL_FACE);

glEnable(GL_DEPTH_TEST);
init_opngl();

    return 0;
}

static void engine_draw_frame(struct engine* engine) {
    if (engine->display == NULL) {
        return;
    }
anim_frame();
draw_glframe();
    eglSwapBuffers(engine->display, engine->surface);
}

static void engine_term_display(struct engine* engine) {
    if (engine->display != EGL_NO_DISPLAY) {
        eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (engine->context != EGL_NO_CONTEXT) {
            eglDestroyContext(engine->display, engine->context);
        }
        if (engine->surface != EGL_NO_SURFACE) {
            eglDestroySurface(engine->display, engine->surface);
        }
        eglTerminate(engine->display);
    }
    engine->animating = 0;
    engine->display = EGL_NO_DISPLAY;
    engine->context = EGL_NO_CONTEXT;
    engine->surface = EGL_NO_SURFACE;
}

static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
    struct engine* engine = (struct engine*)app->userData;
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {

        engine->animating = 1;
				engine->state.pntrstt = AKeyEvent_getAction(event);
				if(engine->state.pntrstt==0){engine->state.pntrstt=1;}
				else if(engine->state.pntrstt==1){engine->state.pntrstt=0;}

if (engine->state.pntrstt==0){
				if(swaptchxy){
				engine->state.y = (float)AMotionEvent_getX(event, 0)/(float)(engine->width);//0
        engine->state.x = (float)AMotionEvent_getY(event, 0)/(float)(engine->height);
				}else{
        engine->state.x = (float)AMotionEvent_getX(event, 0)/(float)(engine->width);//0
        engine->state.y = (float)AMotionEvent_getY(event, 0)/(float)(engine->height);
        }engine->state.ldx = engine->state.x;
        engine->state.ldy = engine->state.y;}
else{
        engine->state.ldx = engine->state.x;
        engine->state.ldy = engine->state.y;
				if(swaptchxy){
				engine->state.y = (float)AMotionEvent_getX(event, 0)/(float)(engine->width);
        engine->state.x = (float)AMotionEvent_getY(event, 0)/(float)(engine->height);
				}else{
        engine->state.x = (float)AMotionEvent_getX(event, 0)/(float)(engine->width);
        engine->state.y = (float)AMotionEvent_getY(event, 0)/(float)(engine->height);
				}
}
				if(invtchy==-1) engine->state.y = 1-engine->state.y;
				if(invtchx==-1) engine->state.x = 1-engine->state.x;
        return 1;
    }
    return 0;
}

static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
    struct engine* engine = (struct engine*)app->userData;
    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            engine->app->savedState = malloc(sizeof(struct saved_state));
            *((struct saved_state*)engine->app->savedState) = engine->state;
            engine->app->savedStateSize = sizeof(struct saved_state);
            break;
        case APP_CMD_INIT_WINDOW:
            if (engine->app->window != NULL) {
                engine_init_display(engine);
                engine_draw_frame(engine);
            }
            break;
        case APP_CMD_TERM_WINDOW:
appglfreeing();
            engine_term_display(engine);
            break;
        case APP_CMD_GAINED_FOCUS:
            if (engine->accelerometerSensor != NULL) {
                ASensorEventQueue_enableSensor(engine->sensorEventQueue,
                        engine->accelerometerSensor);
                ASensorEventQueue_setEventRate(engine->sensorEventQueue,
                        engine->accelerometerSensor, (1000L/60)*1000);
            }
            break;
        case APP_CMD_LOST_FOCUS:
            if (engine->accelerometerSensor != NULL) {
                ASensorEventQueue_disableSensor(engine->sensorEventQueue,
                        engine->accelerometerSensor);
            }
            engine->animating = 0;
            break;
    }
}

void android_main(struct android_app* state) {

clck=clckld=clock();
srandom(clck);

    memset(&engine, 0, sizeof(engine));
    state->userData = &engine;
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = engine_handle_input;
    engine.app = state;

    engine.sensorManager = ASensorManager_getInstance();
    engine.accelerometerSensor = ASensorManager_getDefaultSensor(engine.sensorManager,
            ASENSOR_TYPE_ACCELEROMETER);
    engine.sensorEventQueue = ASensorManager_createEventQueue(engine.sensorManager,
            state->looper, LOOPER_ID_USER, NULL, NULL);

    if (state->savedState != NULL) {
        engine.state = *(struct saved_state*)state->savedState;
    }

    while (1) {
        int ident;
        int events;
        struct android_poll_source* source;
        while ((ident=ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events,
                (void**)&source)) >= 0) {

            if (source != NULL) {
                source->process(state, source);
            }

            if (ident == LOOPER_ID_USER) {
                if (engine.accelerometerSensor != NULL) {
                    ASensorEvent event;
                    while (ASensorEventQueue_getEvents(engine.sensorEventQueue,
                            &event, 1) > 0) {
												if(swapangxy){
												engine.state.angley= event.acceleration.x;
												engine.state.anglex= event.acceleration.y;}
												else{
												engine.state.anglex= event.acceleration.x;
												engine.state.angley= event.acceleration.y;}
												engine.state.anglez=event.acceleration.z;
												if(invangx==-1)engine.state.anglex*=-1;
												if(invangy==-1)engine.state.angley*=-1;
                    }
                }
            }

            if (state->destroyRequested != 0) {
                engine_term_display(&engine);
                return;
            }
        }

        if (engine.animating) {
            engine_draw_frame(&engine);
        }
    }
}
