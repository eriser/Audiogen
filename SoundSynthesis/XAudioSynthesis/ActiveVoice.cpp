#include "pch.h"
#include "ActiveVoice.h"

using namespace SoundSynthesis::XAudioSynthesis;

void ActiveVoice::Retain() noexcept
{
	_InterlockedIncrement(&m_refs);
}

void ActiveVoice::Release() noexcept
{
	if (0 == _InterlockedDecrement(&m_refs))
	{
		delete this;
	}
}

ActiveVoice::ActiveVoice(_In_ const WAVEFORMATEX *waveFormat) noexcept
:	m_refs(1),
	m_waveFormat(waveFormat)
{
	_ASSERTE(nullptr != m_waveFormat);
}

ActiveVoice::~ActiveVoice() noexcept
{
	_ASSERTE(nullptr == m_sourceVoice);
}

STDMETHODIMP_(void) ActiveVoice::OnVoiceProcessingPassStart(UINT32 BytesRequired) noexcept
{
	XAudioFrame *frame = MakeAudioFrame(BytesRequired, m_waveFormat);

	if (frame)
	{
		//frame
	}
}

STDMETHODIMP_(void) ActiveVoice::OnVoiceProcessingPassEnd() noexcept
{
}

STDMETHODIMP_(void) ActiveVoice::OnStreamEnd() noexcept
{
	XAUDIO2_VOICE_SENDS vs = { 0 };
	//
	// Stop the voice, there will be no more data.
	//
	m_sourceVoice->Stop();
	//
	// Disconnect the voice from all outputs.
	//
	m_sourceVoice->SetOutputVoices(&vs);
	//
	// Destroy the voice.
	//
	m_sourceVoice->DestroyVoice();
	m_sourceVoice = nullptr;
}

STDMETHODIMP_(void) ActiveVoice::OnBufferStart(void *pBufferContext) noexcept
{
	UNUSED(pBufferContext);
}

STDMETHODIMP_(void) ActiveVoice::OnBufferEnd(void *pBufferContext) noexcept
{
	XAudioFrame::FromContext(pBufferContext)->Destroy();
}

STDMETHODIMP_(void) ActiveVoice::OnLoopEnd(void *pBufferContext) noexcept
{
	UNUSED(pBufferContext);
}

STDMETHODIMP_(void) ActiveVoice::OnVoiceError(void *pBufferContext, HRESULT Error) noexcept
{
	UNUSED(Error);
	XAudioFrame::FromContext(pBufferContext)->Destroy();
}
