#include "specific/s_audio_sample.h"

#include "global/const.h"
#include "global/vars.h"

const struct SOUND_ADAPTER_NODE *__cdecl S_Audio_Sample_GetAdapter(GUID *guid)
{
    if (guid != NULL) {
        for (const struct SOUND_ADAPTER_NODE *adapter = g_SoundAdapterList.head;
             adapter; adapter = adapter->next) {
            if (!memcmp(&adapter->body.adapter_guid, guid, sizeof(GUID))) {
                return adapter;
            }
        }
    }

    return g_PrimarySoundAdapter;
}

void __cdecl S_Audio_Sample_CloseAllTracks(void)
{
    if (!g_IsSoundEnabled) {
        return;
    }

    for (int32_t i = 0; i < MAX_AUDIO_SAMPLE_BUFFERS; ++i) {
        if (g_SampleBuffers[i] != NULL) {
            IDirectSound_Release(g_SampleBuffers[i]);
            g_SampleBuffers[i] = NULL;
        }
    }
}

bool __cdecl S_Audio_Sample_Load(
    int32_t sample_idx, LPWAVEFORMATEX format, const void *data,
    uint32_t data_size)
{
    if (!g_DSound || !g_IsSoundEnabled
        || sample_idx >= MAX_AUDIO_SAMPLE_BUFFERS) {
        return false;
    }

    if (g_SampleBuffers[sample_idx] != NULL) {
        IDirectSound_Release(g_SampleBuffers[sample_idx]);
        g_SampleBuffers[sample_idx] = NULL;
    }

    DSBUFFERDESC desc;
    memset(&desc, 0, sizeof(desc));
    desc.dwSize = sizeof(DSBUFFERDESC);
    desc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY
        | DSBCAPS_LOCSOFTWARE;
    desc.dwBufferBytes = data_size;
    desc.dwReserved = 0;
    desc.lpwfxFormat = format;

    if (IDirectSound_CreateSoundBuffer(
            g_DSound, &desc, &g_SampleBuffers[sample_idx], NULL)
        < 0) {
        return false;
    }

    LPVOID audio_ptr;
    DWORD audio_bytes;
    if (IDirectSoundBuffer_Lock(
            g_SampleBuffers[sample_idx], 0, data_size, &audio_ptr, &audio_bytes,
            NULL, NULL, 0)
        < 0) {
        return false;
    }

    memcpy(audio_ptr, data, audio_bytes);

    if (IDirectSoundBuffer_Unlock(
            g_SampleBuffers[sample_idx], audio_ptr, audio_bytes, NULL, 0)
        < 0) {
        return false;
    }

    g_SampleFreqs[sample_idx] = format->nSamplesPerSec;
    return true;
}
