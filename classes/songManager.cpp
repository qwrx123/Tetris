#include "../headers/songManager.h"

songManager::songManager(HWND hwnd)
    :musicVolume(50), effectVolume(50)
{
    HRESULT hr = CoInitializeEx(NULL, 0);

	hr = CoCreateInstance(CLSID_DirectSound8,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IDirectSound8,
		(LPVOID*)&lpDevice);

	hr = lpDevice->Initialize(NULL);

    lpDevice->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);

    CreateSecondaryBuffer(lpDevice, &ptitleSongBuffer, L"titlescreenSong");
	CreateSecondaryBuffer(lpDevice, &pgameSongBuffers[0], L"mainSong");
	CreateSecondaryBuffer(lpDevice, &pgameSongBuffers[1], L"mainSong2");
	CreateSecondaryBuffer(lpDevice, &pgameSongBuffers[2], L"mainSong3");
	CreateSecondaryBuffer(lpDevice, &pdeathSongBuffer, L"deathscreenSong");
	CreateSecondaryBuffer(lpDevice, &ppieceFallBuffer, L"dropPiece");

    pcurrentGameSong = pgameSongBuffers[0];
    changeSongVolume(0);
    changeEffectVolume(0);
    switchSongType(songTypes::NONE);
}

songManager::~songManager()
{

}

void songManager::switchSongType(songManager::songTypes swapType)
{
    switch (swapType)
    {
        case songTypes::NONE:
        {
            pcurrentGameSong->Stop();
            pdeathSongBuffer->Stop();
            ptitleSongBuffer->Stop();
            break;
        }
        case songTypes::TITLE:
        {
            pcurrentGameSong->Stop();
            pdeathSongBuffer->Stop();
            ptitleSongBuffer->SetCurrentPosition(0);
	        ptitleSongBuffer->Play(
		        0,  // Unused.
		        0,  // Priority for voice management.
		        DSBPLAY_LOOPING); // Flags.
            break;
        }
        case songTypes::GAME:
        {
            ptitleSongBuffer->Stop();
            pdeathSongBuffer->Stop();
            pcurrentGameSong->SetCurrentPosition(0);
	        pcurrentGameSong->Play(
		        0,  // Unused.
		        0,  // Priority for voice management.
		        DSBPLAY_LOOPING); // Flags.
            break;
        }
        case songTypes::DEATH:
        {
            pcurrentGameSong->Stop();
            ptitleSongBuffer->Stop();
            pdeathSongBuffer->SetCurrentPosition(0);
	        pdeathSongBuffer->Play(
		        0,  // Unused.
		        0,  // Priority for voice management.
		        DSBPLAY_LOOPING); // Flags.
            break;
        }
    }
}

void songManager::playEffectSound()
{
	ppieceFallBuffer->SetCurrentPosition(0);
	ppieceFallBuffer->Play(0, 0, 0);
}

void songManager::setGameSong(int type)
{
    pcurrentGameSong = pgameSongBuffers[type];
}

void songManager::changeSongVolume(int volume)
{
    if (musicVolume == volume)
    {
        return;
    }
    musicVolume = volume;
    changeVolume(ptitleSongBuffer, volume);
	changeVolume(pgameSongBuffers[0], volume);
	changeVolume(pgameSongBuffers[1], volume);
	changeVolume(pgameSongBuffers[2], volume);
	changeVolume(pdeathSongBuffer, volume);
}

void songManager::changeEffectVolume(int volume)
{
    if (effectVolume == volume)
    {
        return;
    }
    effectVolume = volume;
	changeVolume(ppieceFallBuffer, volume);
}

HRESULT songManager::CreateSecondaryBuffer(LPDIRECTSOUND8 lpDirectSound, LPDIRECTSOUNDBUFFER8* ppDsb8, LPCWSTR lpName) {
	WAVEFORMATEX wfx;
	DSBUFFERDESC dsbdesc;
	LPDIRECTSOUNDBUFFER pDsb = NULL;
	HRESULT hr = 0;

	void* lpRes;
	HRSRC hResInfo;
	HGLOBAL hRes;

	LPVOID lpvWrite;
	DWORD  dwLength;

	waveFile myWave;
	memset(&myWave, 0, sizeof(waveFile));
	// Find the WAVE resource. 
    HINSTANCE hInstance = GetModuleHandle(NULL);
	hResInfo = FindResource(hInstance, lpName, L"WAVE");
	if (hResInfo == NULL)
		return E_FAIL;

	// Load the WAVE resource. 

	hRes = LoadResource(hInstance, hResInfo);
	if (hRes == NULL)
		return E_FAIL;

	// Lock the WAVE resource and play it. 

	lpRes = LockResource(hRes);
	char* wavMem = static_cast<char*>(lpRes);
	memcpy(&myWave, wavMem, 36);
	wavMem += 36;
	

	while (myWave.Subchunk2ID[0] != 'd' && myWave.Subchunk2ID[1] != 'a' && myWave.Subchunk2ID[2] != 't' && myWave.Subchunk2ID[3] != 'a') {
		memcpy(&myWave.Subchunk2ID, wavMem, 8);
		if (myWave.Subchunk2ID[0] != 'd' && myWave.Subchunk2ID[1] != 'a' && myWave.Subchunk2ID[2] != 't' && myWave.Subchunk2ID[3] != 'a') {
			wavMem += myWave.Subchunk2Size;
			wavMem += 8;
		}
	}
	wavMem += 8;

	char* soundData = new char[myWave.Subchunk2Size];
	memcpy(soundData, wavMem, myWave.Subchunk2Size);
	FreeResource(hRes);

	// Set up WAV format structure. 

	memset(&wfx, 0, sizeof(WAVEFORMATEX));
	wfx.wFormatTag = myWave.AudioFormat;
	wfx.nChannels = myWave.NumChannels;
	wfx.nSamplesPerSec = myWave.SampleRate;
	wfx.nBlockAlign = myWave.BlockAlign;
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
	wfx.wBitsPerSample = myWave.BitsPerSample;

	// Set up DSBUFFERDESC structure. 

	memset(&dsbdesc, 0, sizeof(DSBUFFERDESC));
	dsbdesc.dwSize = sizeof(DSBUFFERDESC);
	dsbdesc.dwFlags =
		 DSBCAPS_CTRLVOLUME | DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLFREQUENCY;
	dsbdesc.dwBufferBytes = myWave.Subchunk2Size;
	dsbdesc.lpwfxFormat = &wfx;

	hr = lpDirectSound->CreateSoundBuffer(&dsbdesc, &pDsb, NULL);
	if (SUCCEEDED(hr))
	{
		if (DS_OK == pDsb->Lock(
			0,          // Offset at which to start lock.
			0,          // Size of lock; ignored because of flag.
			&lpvWrite,  // Gets address of first part of lock.
			&dwLength,  // Gets size of first part of lock.
			NULL,       // Address of wraparound not needed. 
			NULL,       // Size of wraparound not needed.
			DSBLOCK_ENTIREBUFFER))  // Flag.
		{
			memcpy(lpvWrite, soundData, dwLength);
			pDsb->Unlock(
				lpvWrite,   // Address of lock start.
				dwLength,   // Size of lock.
				NULL,       // No wraparound portion.
				0);         // No wraparound size.
		}
		hr = pDsb->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)ppDsb8);
		pDsb->Release();
	}

	delete[] soundData;
	return hr;
}

void songManager::changeVolume(LPDIRECTSOUNDBUFFER8 theSoundBuffer, int volume) {
	if (volume == 0) {
		theSoundBuffer->SetVolume(-10000);
	}
	else {
		theSoundBuffer->SetVolume((LONG)(volume) * 50 - 5000);
	}
}