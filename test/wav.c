#include "../include/SDL_config.h"
#include <stdio.h>
#include <stdlib.h>
#include <emscripten/emscripten.h>
#include "../include/SDL.h"
static struct{SDL_AudioSpec spec;Uint8 *sound;Uint32 soundlen;int soundpos;}wave;
static SDL_AudioDeviceID device;
static void close_audio(){if(device != 0){SDL_CloseAudioDevice(device);device=0;}}
static void quit(int rc){close_audio();SDL_FreeWAV(wave.sound);SDL_Quit();exit(rc);}
static void open_audio(){device=SDL_OpenAudioDevice(NULL,SDL_FALSE,&wave.spec,NULL,0);if(!device){SDL_FreeWAV(wave.sound);quit(2);}SDL_PauseAudioDevice(device,SDL_FALSE);}
static void reopen_audio(){close_audio();open_audio();}
void SDLCALL fillerup(void *unused,Uint8 *stream,int len){Uint8 *waveptr;int waveleft;waveptr=wave.sound+wave.soundpos;waveleft=wave.soundlen-wave.soundpos;
while (waveleft <= len){SDL_memcpy(stream,waveptr,waveleft);stream+=waveleft;len-=waveleft;waveptr=wave.sound;waveleft=wave.soundlen;wave.soundpos=0;}
SDL_memcpy(stream,waveptr,len);wave.soundpos+=len;}
static int done=0;
void loop(){if(done || (SDL_GetAudioDeviceStatus(device) != SDL_AUDIO_PLAYING))emscripten_cancel_main_loop();}
int main(){return (0);}
void pl(){int i;char filename[4096];close_audio();SDL_FreeWAV(wave.sound);wave.soundpos=0;emscripten_cancel_main_loop();
// SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_INFO);
if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_EVENTS) < 0){quit (1);}
SDL_strlcpy(filename,"/sample.wav",sizeof(filename));
if(SDL_LoadWAV(filename,&wave.spec,&wave.sound,&wave.soundlen) == NULL){quit(1);}
wave.spec.callback=fillerup;open_audio();
SDL_FlushEvents(SDL_AUDIODEVICEADDED,SDL_AUDIODEVICEREMOVED);
emscripten_set_main_loop(loop,1,0);}