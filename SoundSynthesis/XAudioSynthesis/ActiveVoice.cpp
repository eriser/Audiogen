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

void ActiveVoice::Start(float x, float y) noexcept
{
	UNUSED(x);
	UNUSED(y);

	XAUDIO2_VOICE_SENDS		vs = { 0 };
	XAUDIO2_SEND_DESCRIPTOR	output[1] = { 0 };

	vs.SendCount = 1;
	vs.pSends = output;
	output->pOutputVoice = m_receiverVoice;

	m_sourceVoice->SetOutputVoices(&vs);
	m_sourceVoice->Start();
}

void ActiveVoice::Move(float x, float y) noexcept
{
	UNUSED(x);
	UNUSED(y);
}

void ActiveVoice::Stop() noexcept
{
	if (1 == _InterlockedCompareExchange16(&m_isActive, 0, 1))
	{
		m_sourceVoice->Stop();
		m_container->CleanupAsync();
	}
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

XAUDIO2_BUFFER *ActiveVoice::MakeEndOfStreamFrame(_In_ const WAVEFORMATEX *waveFormat) noexcept
{
	UNUSED(waveFormat);
	return FrameSource()->Allocate(XAUDIO2_END_OF_STREAM, waveFormat->nBlockAlign);
}

AudioFrameSource *ActiveVoice::FrameSource() const noexcept
{
	return m_container->FrameSource();
}

STDMETHODIMP_(void) ActiveVoice::OnVoiceProcessingPassStart(UINT32 BytesRequired) noexcept
{
	XAUDIO2_BUFFER *frame;

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
		if (!AudioFrameSource::Submit(frame, m_sourceVoice))
		{
			FrameSource()->Release(frame);
			submitted = false;
		}
	}

	if (!submitted)
	{
		Stop();
	}
}

STDMETHODIMP_(void) ActiveVoice::OnVoiceProcessingPassEnd() noexcept
{
}

STDMETHODIMP_(void) ActiveVoice::OnStreamEnd() noexcept
{
	Stop();
}

STDMETHODIMP_(void) ActiveVoice::OnBufferStart(void *pBufferContext) noexcept
{
	UNUSED(pBufferContext);
}

STDMETHODIMP_(void) ActiveVoice::OnBufferEnd(void *pBufferContext) noexcept
{
	XAUDIO2_BUFFER *frame = AudioFrameSource::FromContext(pBufferContext);
	m_container->FrameSource()->Release(frame);
}

STDMETHODIMP_(void) ActiveVoice::OnLoopEnd(void *pBufferContext) noexcept
{
	UNUSED(pBufferContext);
}

STDMETHODIMP_(void) ActiveVoice::OnVoiceError(void *pBufferContext, HRESULT Error) noexcept
{
	UNUSED(Error);
	XAUDIO2_BUFFER *frame = AudioFrameSource::FromContext(pBufferContext);
	m_container->FrameSource()->Release(frame);
}
