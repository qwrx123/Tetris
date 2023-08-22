#pragma once
#ifndef UNICODE
#define UNICODE
#endif 

#include <Dsound.h>
#include <objbase.h>
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "ole32.lib")


class songManager
{
public:
enum songTypes
{
    NONE,
    TITLE,
    GAME,
    DEATH
};
private:
struct waveFile {
	char ChunkID[4];
	int ChunkSize;
	char Format[4];
	char Subchunk1ID[4];
	int Subchunk1Size;
	short AudioFormat;
	short NumChannels;
	int SampleRate;
	int ByteRate;
	short BlockAlign;
	short BitsPerSample;
	char Subchunk2ID[4];
	int Subchunk2Size;
};
LPDIRECTSOUND8 lpDevice;
LPDIRECTSOUNDBUFFER8 ptitleSongBuffer = NULL;
LPDIRECTSOUNDBUFFER8 pgameSongBuffers[3];
LPDIRECTSOUNDBUFFER8 pdeathSongBuffer = NULL;
LPDIRECTSOUNDBUFFER8 ppieceFallBuffer = NULL;
LPDIRECTSOUNDBUFFER8 pcurrentGameSong = NULL;
int musicVolume;
int effectVolume;
public:
songManager(HWND hwnd);
~songManager();
void changeSongVolume(int volume);
void changeEffectVolume(int volume);
void switchSongType(songManager::songTypes swapType);
void playEffectSound();
void setGameSong(int song);
private:
void changeVolume(LPDIRECTSOUNDBUFFER8 theSoundBuffer, int volume);
HRESULT CreateSecondaryBuffer(LPDIRECTSOUND8 lpDirectSound, LPDIRECTSOUNDBUFFER8* ppDsb8, LPCWSTR lpName);
};