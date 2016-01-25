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

void ActiveVoice::Start() noexcept
{
	XAUDIO2_VOICE_SENDS		vs = { 0 };
	XAUDIO2_SEND_DESCRIPTOR	output[1] = { 0 };

	vs.SendCount = 1;
	vs.pSends = output;
	output->pOutputVoice = m_receiverVoice;

	m_sourceVoice->SetOutputVoices(&vs);
	m_sourceVoice->Start();
}

void ActiveVoice::Recycle() noexcept
{
	DestroySourceVoice();
}

_Check_return_
bool ActiveVoice::CreateSourceVoice(_In_ IXAudio2 *audio, _In_ IXAudio2Voice *receiver) noexcept
{
	_ASSERTE(nullptr == m_sourceVoice);
	//
	// Create a source voice with the active voice as the callback and do not connect the created voice to the mastering voice.
	//
	bool				created = false;
	XAUDIO2_VOICE_SENDS	vs = { 0 };

	if (SUCCEEDED(audio->CreateSourceVoice(&m_sourceVoice, m_waveFormat, 0, XAUDIO2_DEFAULT_FREQ_RATIO, this, &vs)))
	{
		m_receiverVoice = receiver;
		created = true;
	}

	return created;
}

void ActiveVoice::DestroySourceVoice() noexcept
{
	_ASSERTE(nullptr != m_sourceVoice);
	m_sourceVoice->DestroyVoice();
	m_sourceVoice = nullptr;
}

ActiveVoice::ActiveVoice(_In_ ActiveVoices *container, _In_ const WAVEFORMATEX *waveFormat) noexcept
:	m_refs(1),
	m_isActive(1),
	m_container(container),
	m_waveFormat(waveFormat),
	m_sourceVoice(nullptr),
	m_receiverVoice(nullptr)
{
	_ASSERTE(nullptr != m_waveFormat);
}

ActiveVoice::~ActiveVoice() noexcept
{
	_ASSERTE(nullptr == m_sourceVoice);
}

XAudioFrame *ActiveVoice::MakeEndOfStreamFrame(_In_ const WAVEFORMATEX *waveFormat) noexcept
{
	UNUSED(waveFormat);
	return XAudioFrame::Create(XAUDIO2_END_OF_STREAM, waveFormat->nBlockAlign);
}

STDMETHODIMP_(void) ActiveVoice::OnVoiceProcessingPassStart(UINT32 BytesRequired) noexcept
{
	XAudioFrame *frame;

	if (IsActive())
	{
		frame = MakeAudioFrame(BytesRequired, m_waveFormat);
	}
	else
	{
		frame = MakeEndOfStreamFrame(m_waveFormat);
	}

	bool submitted = nullptr != frame;

	if (submitted)
	{
		if (!frame->Submit(m_sourceVoice))
		{
			frame->Destroy();
			submitted = false;
		}
	}

	if (!submitted)
	{
		m_sourceVoice->Stop();
		SetInactive();
		m_container->CleanupAsync();
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
	// Mark the voice as stopped and queue a cleanup request to the voices collection.
	//
	SetInactive();
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
