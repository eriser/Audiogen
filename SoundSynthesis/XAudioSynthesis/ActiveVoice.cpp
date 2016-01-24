#include "pch.h"
#include "ActiveVoice.h"
#include "ActiveVoices.h"

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

_Check_return_
bool ActiveVoice::CreateSourceVoice(_In_ IXAudio2 *audio) noexcept
{
	_ASSERTE(nullptr == m_sourceVoice);
	//
	// Create a source voice with the active voice as the callback and do not connect the created voice to the mastering voice.
	//
	XAUDIO2_VOICE_SENDS vs = { 0 };
	return SUCCEEDED(audio->CreateSourceVoice(&m_sourceVoice, m_waveFormat, 0, XAUDIO2_DEFAULT_FREQ_RATIO, this, &vs));
}

ActiveVoice::ActiveVoice(_In_ ActiveVoices *container, _In_ const WAVEFORMATEX *waveFormat) noexcept
:	m_refs(1),
	m_container(container),
	m_waveFormat(waveFormat),
	m_sourceVoice(nullptr)
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

	if (frame && !frame->Submit(m_sourceVoice))
	{
		frame->Destroy();
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
	// TODO: mark the voice as stopped and queue a cleanup request to the voices collection.
	//
	m_container->CleanupAsync();
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
