#ifndef SDL_audio_h_
#define SDL_audio_h_
#include "SDL_stdinc.h"
#include "SDL_error.h"
#include "SDL_endian.h"
#include "SDL_mutex.h"
#include "SDL_thread.h"
#include "SDL_rwops.h"
#include "begin_code.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef Uint16 SDL_AudioFormat;
/* @{ */
#define SDL_AUDIO_MASK_BITSIZE       (0xFF)
#define SDL_AUDIO_MASK_DATATYPE      (1<<8)
#define SDL_AUDIO_MASK_ENDIAN        (1<<12)
#define SDL_AUDIO_MASK_SIGNED        (1<<15)
#define SDL_AUDIO_BITSIZE(x)         (x & SDL_AUDIO_MASK_BITSIZE)
#define SDL_AUDIO_ISFLOAT(x)         (x & SDL_AUDIO_MASK_DATATYPE)
#define SDL_AUDIO_ISBIGENDIAN(x)     (x & SDL_AUDIO_MASK_ENDIAN)
#define SDL_AUDIO_ISSIGNED(x)        (x & SDL_AUDIO_MASK_SIGNED)
#define SDL_AUDIO_ISINT(x)           (!SDL_AUDIO_ISFLOAT(x))
#define SDL_AUDIO_ISLITTLEENDIAN(x)  (!SDL_AUDIO_ISBIGENDIAN(x))
#define SDL_AUDIO_ISUNSIGNED(x)      (!SDL_AUDIO_ISSIGNED(x))
/* @{ */
#define AUDIO_U8        0x0008  /**< Unsigned 8-bit samples */
#define AUDIO_S8        0x8008  /**< Signed 8-bit samples */
#define AUDIO_U16LSB    0x0010  /**< Unsigned 16-bit samples */
#define AUDIO_S16LSB    0x8010  /**< Signed 16-bit samples */
#define AUDIO_U16MSB    0x1010  /**< As above, but big-endian byte order */
#define AUDIO_S16MSB    0x9010  /**< As above, but big-endian byte order */
#define AUDIO_U16       AUDIO_U16LSB
#define AUDIO_S16       AUDIO_S16LSB
/* @} */
/* @{ */
#define AUDIO_S32LSB    0x8020  /**< 32-bit integer samples */
#define AUDIO_S32MSB    0x9020  /**< As above, but big-endian byte order */
#define AUDIO_S32       AUDIO_S32LSB
/* @} */
/* @{ */
#define AUDIO_F32LSB    0x8120  /**< 32-bit floating point samples */
#define AUDIO_F32MSB    0x9120  /**< As above, but big-endian byte order */
#define AUDIO_F32       AUDIO_F32LSB
/* @} */
/* @{ */
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
#define AUDIO_U16SYS    AUDIO_U16LSB
#define AUDIO_S16SYS    AUDIO_S16LSB
#define AUDIO_S32SYS    AUDIO_S32LSB
#define AUDIO_F32SYS    AUDIO_F32LSB
#else
#define AUDIO_U16SYS    AUDIO_U16MSB
#define AUDIO_S16SYS    AUDIO_S16MSB
#define AUDIO_S32SYS    AUDIO_S32MSB
#define AUDIO_F32SYS    AUDIO_F32MSB
#endif
/* @} */
/* @{ */
#define SDL_AUDIO_ALLOW_FREQUENCY_CHANGE    0x00000001
#define SDL_AUDIO_ALLOW_FORMAT_CHANGE       0x00000002
#define SDL_AUDIO_ALLOW_CHANNELS_CHANGE     0x00000004
#define SDL_AUDIO_ALLOW_SAMPLES_CHANGE      0x00000008
#define SDL_AUDIO_ALLOW_ANY_CHANGE          (SDL_AUDIO_ALLOW_FREQUENCY_CHANGE|SDL_AUDIO_ALLOW_FORMAT_CHANGE|SDL_AUDIO_ALLOW_CHANNELS_CHANGE|SDL_AUDIO_ALLOW_SAMPLES_CHANGE)
/* @} */
/* @} *//* Audio flags */
typedef void (SDLCALL *SDL_AudioCallback)(void *userdata,Uint8 *stream,
                                          int len);
typedef struct SDL_AudioSpec{
int freq;                   /**< DSP frequency -- samples per second */
SDL_AudioFormat format;     /**< Audio data format */
Uint8 channels;             /**< Number of channels: 1 mono, 2 stereo */
Uint8 silence;              /**< Audio buffer silence value (calculated) */
Uint16 samples;             /**< Audio buffer size in sample FRAMES (total samples divided by channel count) */
Uint16 padding;             /**< Necessary for some compile environments */
Uint32 size;                /**< Audio buffer size in bytes (calculated) */
SDL_AudioCallback callback; /**< Callback that feeds the audio device (NULL to use SDL_QueueAudio()). */
void *userdata;             /**< Userdata passed to callback (ignored for NULL callbacks). */
}SDL_AudioSpec;
struct SDL_AudioCVT;
typedef void (SDLCALL *SDL_AudioFilter)(struct SDL_AudioCVT *cvt,
                                        SDL_AudioFormat format);
#define SDL_AUDIOCVT_MAX_FILTERS 9
#ifdef __GNUC__
#define SDL_AUDIOCVT_PACKED __attribute__((packed))
#else
#define SDL_AUDIOCVT_PACKED
#endif
typedef struct SDL_AudioCVT{
int needed;                 /**< Set to 1 if conversion possible */
SDL_AudioFormat src_format; /**< Source audio format */
SDL_AudioFormat dst_format; /**< Target audio format */
double rate_incr;           /**< Rate conversion increment */
Uint8 *buf;                 /**< Buffer to hold entire audio data */
int len;                    /**< Length of original audio buffer */
int len_cvt;                /**< Length of converted audio buffer */
int len_mult;               /**< buffer must be len*len_mult big */
double len_ratio;           /**< Given len, final size is len*len_ratio */
SDL_AudioFilter filters[SDL_AUDIOCVT_MAX_FILTERS+1]; /**< NULL-terminated list of filter functions */
int filter_index;           /**< Current audio conversion function */
} SDL_AUDIOCVT_PACKED SDL_AudioCVT;
/* @{ */
extern DECLSPEC int SDLCALL SDL_GetNumAudioDrivers(void);
extern DECLSPEC const char *SDLCALL SDL_GetAudioDriver(int index);
/* @} */
/* @{ */
extern DECLSPEC int SDLCALL SDL_AudioInit(const char *driver_name);
extern DECLSPEC void SDLCALL SDL_AudioQuit(void);
/* @} */
extern DECLSPEC const char *SDLCALL SDL_GetCurrentAudioDriver(void);
typedef Uint32 SDL_AudioDeviceID;
extern DECLSPEC int SDLCALL SDL_GetNumAudioDevices(int iscapture);
extern DECLSPEC const char *SDLCALL SDL_GetAudioDeviceName(int index,
                                                          int iscapture);
extern DECLSPEC SDL_AudioDeviceID SDLCALL SDL_OpenAudioDevice(const char
                                                              *device,
                                                              int iscapture,
                                                              const
                                                              SDL_AudioSpec *
                                                              desired,
                                                              SDL_AudioSpec *
                                                              obtained,
                                                              int
                                                              allowed_changes);
/* @{ */
typedef enum{
SDL_AUDIO_STOPPED=0,
SDL_AUDIO_PLAYING,
SDL_AUDIO_PAUSED
}SDL_AudioStatus;
extern DECLSPEC SDL_AudioStatus SDLCALL SDL_GetAudioStatus(void);
extern DECLSPEC SDL_AudioStatus SDLCALL
SDL_GetAudioDeviceStatus(SDL_AudioDeviceID dev);
/* @} *//* Audio State */
/* @{ */
extern DECLSPEC void SDLCALL SDL_PauseAudio(int pause_on);
extern DECLSPEC void SDLCALL SDL_PauseAudioDevice(SDL_AudioDeviceID dev,
                                                  int pause_on);
/* @} *//* Pause audio functions */
extern DECLSPEC SDL_AudioSpec *SDLCALL SDL_LoadWAV_RW(SDL_RWops *src,
                                                      int freesrc,
                                                      SDL_AudioSpec *spec,
                                                      Uint8 **audio_buf,
                                                      Uint32 *audio_len);
#define SDL_LoadWAV(file,spec,audio_buf,audio_len) \
    SDL_LoadWAV_RW(SDL_RWFromFile(file, "rb"),1, spec,audio_buf,audio_len)
extern DECLSPEC void SDLCALL SDL_FreeWAV(Uint8 *audio_buf);
extern DECLSPEC int SDLCALL SDL_BuildAudioCVT(SDL_AudioCVT *cvt,
                                              SDL_AudioFormat src_format,
                                              Uint8 src_channels,
                                              int src_rate,
                                              SDL_AudioFormat dst_format,
                                              Uint8 dst_channels,
                                              int dst_rate);
extern DECLSPEC int SDLCALL SDL_ConvertAudio(SDL_AudioCVT *cvt);
struct _SDL_AudioStream;
typedef struct _SDL_AudioStream SDL_AudioStream;
extern DECLSPEC SDL_AudioStream *SDLCALL SDL_NewAudioStream(const SDL_AudioFormat src_format,
                                                            const Uint8 src_channels,
                                                            const int src_rate,
                                                            const SDL_AudioFormat dst_format,
                                                            const Uint8 dst_channels,
                                                            const int dst_rate);
extern DECLSPEC int SDLCALL SDL_AudioStreamPut(SDL_AudioStream *stream,const void *buf,int len);
extern DECLSPEC int SDLCALL SDL_AudioStreamGet(SDL_AudioStream *stream,void *buf,int len);
extern DECLSPEC int SDLCALL SDL_AudioStreamAvailable(SDL_AudioStream *stream);
extern DECLSPEC int SDLCALL SDL_AudioStreamFlush(SDL_AudioStream *stream);
extern DECLSPEC void SDLCALL SDL_AudioStreamClear(SDL_AudioStream *stream);
extern DECLSPEC void SDLCALL SDL_FreeAudioStream(SDL_AudioStream *stream);
#define SDL_MIX_MAXVOLUME 128
extern DECLSPEC void SDLCALL SDL_MixAudio(Uint8 *dst,const Uint8 *src,
                                          Uint32 len,int volume);
extern DECLSPEC void SDLCALL SDL_MixAudioFormat(Uint8 *dst,
                                                const Uint8 *src,
                                                SDL_AudioFormat format,
                                                Uint32 len,int volume);
extern DECLSPEC int SDLCALL SDL_QueueAudio(SDL_AudioDeviceID dev,const void *data,Uint32 len);
extern DECLSPEC Uint32 SDLCALL SDL_DequeueAudio(SDL_AudioDeviceID dev,void *data,Uint32 len);
extern DECLSPEC Uint32 SDLCALL SDL_GetQueuedAudioSize(SDL_AudioDeviceID dev);
extern DECLSPEC void SDLCALL SDL_ClearQueuedAudio(SDL_AudioDeviceID dev);
/* @{ */
extern DECLSPEC void SDLCALL SDL_LockAudio(void);
extern DECLSPEC void SDLCALL SDL_LockAudioDevice(SDL_AudioDeviceID dev);
extern DECLSPEC void SDLCALL SDL_UnlockAudio(void);
extern DECLSPEC void SDLCALL SDL_UnlockAudioDevice(SDL_AudioDeviceID dev);
/* @} *//* Audio lock functions */
extern DECLSPEC void SDLCALL SDL_CloseAudio(void);
extern DECLSPEC void SDLCALL SDL_CloseAudioDevice(SDL_AudioDeviceID dev);
#ifdef __cplusplus
}
#endif
#include "close_code.h"
#endif /* SDL_audio_h_ */
