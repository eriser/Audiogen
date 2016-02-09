#include "pch.h"
#include "WhiteNoise.h"

using namespace SoundSynthesis::XAudioSynthesis;

WhiteNoise::WhiteNoise() noexcept
:	m_xaudio2(nullptr),
	m_masteringVoice(nullptr),
	m_sourceVoice(nullptr),
	m_channels(0)
{
}

WhiteNoise::~WhiteNoise() noexcept
{
}

void WhiteNoise::Start() noexcept
{
	_ASSERTE(nullptr == m_xaudio2);

	WAVEFORMATEX			vf = { 0 };
	XAUDIO2_VOICE_DETAILS	vd;
	XAUDIO2_VOICE_SENDS		vs = { 0 };
	XAUDIO2_SEND_DESCRIPTOR	sd[1] = { 0 };

	::XAudio2Create(&m_xaudio2);
	m_xaudio2->CreateMasteringVoice(&m_masteringVoice);
	m_masteringVoice->GetVoiceDetails(&vd);

	vf.cbSize = sizeof(vf);
	vf.nChannels = static_cast<WORD>(vd.InputChannels);
	m_channels = vf.nChannels;
	vf.wBitsPerSample = sizeof(float) * 8;
	vf.nSamplesPerSec = vd.InputSampleRate;
	vf.wFormatTag = WAVE_FORMAT_IEEE_FLOAT;
	vf.nBlockAlign = (vf.nChannels * vf.wBitsPerSample) / 8;
	vf.nAvgBytesPerSec = vf.nSamplesPerSec * vf.nBlockAlign;

	m_xaudio2->CreateSourceVoice(&m_sourceVoice, &vf, 0, XAUDIO2_DEFAULT_FREQ_RATIO, this, &vs);
	sd[0].Flags = 0;
	sd[0].pOutputVoice = m_masteringVoice;
	vs.SendCount = 1;
	vs.pSends = sd;
	m_sourceVoice->SetOutputVoices(&vs);

	m_xaudio2->StartEngine();
	m_sourceVoice->Start();
}

void WhiteNoise::Stop() noexcept
{
	if (m_xaudio2)
	{
		m_sourceVoice->Stop();
		m_xaudio2->StopEngine();
		m_sourceVoice->DestroyVoice();
		m_sourceVoice = nullptr;
		m_masteringVoice->DestroyVoice();
		m_masteringVoice = nullptr;
		m_xaudio2->Release();
		m_xaudio2 = nullptr;
	}
}

STDMETHODIMP_(void) WhiteNoise::OnVoiceProcessingPassStart(UINT32 BytesRequired)
{
	XAUDIO2_BUFFER *frame = m_frameSource.Allocate(0, BytesRequired);

	if (frame)
	{
		size_t count;

		SampleWriter writer = AudioFrameSource::GetSampleWriter(frame, &count);
		for (size_t s = 0; s < count; ++s)
		{
			float sample = static_cast<float>(rand() % 200) / 100.0f - 1.0f;
			writer.Write(sample, 1);
		}
		m_frameSource.Submit(frame, m_sourceVoice);
	}
}

STDMETHODIMP_(void) WhiteNoise::OnVoiceProcessingPassEnd()
{
}

STDMETHODIMP_(void) WhiteNoise::OnStreamEnd()
{
}

STDMETHODIMP_(void) WhiteNoise::OnBufferStart(THIS_ void* pBufferContext)
{
	UNUSED(pBufferContext);
}

STDMETHODIMP_(void) WhiteNoise::OnBufferEnd(THIS_ void* pBufferContext)
{
	m_frameSource.Release(AudioFrameSource::FromContext(pBufferContext));
}

STDMETHODIMP_(void) WhiteNoise::OnLoopEnd(THIS_ void* pBufferContext)
{
	UNUSED(pBufferContext);
}

STDMETHODIMP_(void) WhiteNoise::OnVoiceError(THIS_ void* pBufferContext, HRESULT Error)
{
	UNUSED(pBufferContext);
	UNUSED(Error);
}
