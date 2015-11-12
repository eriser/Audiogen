#include "pch.h"
#include "VoiceCallback.h"
#include "XAudioBuffer.h"

using namespace SoundSynthesis::Helpers;

VoiceCallback::VoiceCallback() : m_sourceVoice(nullptr)
{
	SecureZeroMemory(&m_waveFormat, sizeof(m_waveFormat));
}

void VoiceCallback::SetUp(_In_ const XAUDIO2_VOICE_DETAILS *mastringVoiceDetails)
{
	m_waveFormat.cbSize = 0;
	m_waveFormat.wFormatTag = WAVE_FORMAT_IEEE_FLOAT;
	m_waveFormat.nSamplesPerSec = mastringVoiceDetails->InputSampleRate;
	m_waveFormat.nChannels = static_cast<WORD>(mastringVoiceDetails->InputChannels);
	m_waveFormat.wBitsPerSample = sizeof(float) * 8;
	m_waveFormat.nBlockAlign = (m_waveFormat.nChannels * m_waveFormat.wBitsPerSample) / 8;
	m_waveFormat.nAvgBytesPerSec = m_waveFormat.nBlockAlign * m_waveFormat.nSamplesPerSec;
}

STDMETHODIMP_(void) VoiceCallback::OnVoiceProcessingPassStart(UINT32 BytesRequired)
{
	XAudioBuffer	*buffer = new(std::nothrow) XAudioBuffer();
	UINT32			framesNumber = BytesRequired / m_waveFormat.nBlockAlign;
	float			*samples = buffer->LockBuffer(framesNumber, m_waveFormat.nChannels);

	if (!samples)
	{
		delete buffer;
	}
	else
	{
		SecureZeroMemory(samples, framesNumber * m_waveFormat.nBlockAlign);
		m_sourceVoice->SubmitSourceBuffer(buffer->GetBuffer());
	}
}

STDMETHODIMP_(void) VoiceCallback::OnVoiceProcessingPassEnd()
{
}

STDMETHODIMP_(void) VoiceCallback::OnStreamEnd()
{
}

STDMETHODIMP_(void) VoiceCallback::OnBufferStart(void* pBufferContext)
{
	(void)pBufferContext;
}

STDMETHODIMP_(void) VoiceCallback::OnBufferEnd(void* pBufferContext)
{
	XAudioBuffer *buffer = reinterpret_cast<XAudioBuffer*>(pBufferContext);
	delete buffer;
}

STDMETHODIMP_(void) VoiceCallback::OnLoopEnd(void* pBufferContext)
{
	(void)pBufferContext;
}

STDMETHODIMP_(void) VoiceCallback::OnVoiceError(void* pBufferContext, HRESULT Error)
{
	(void)pBufferContext;
	(void)Error;
}
