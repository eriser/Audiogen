#include "pch.h"
#include "SourceVoice.h"
#include "AudioEngine.h"
#include "SampleProducer.h"

using namespace SoundSynthesis::XAudioSynthesis;

SourceVoice::SourceVoice(_In_ AudioEngine *engine, _In_ AudioFrameSource *frameSource) noexcept
:	m_started(0),
	m_guard(nullptr),
	m_engine(engine),
	m_frameSource(frameSource),
	m_voice(nullptr),
	m_eofSent(false),
	m_eofBytes(0),
	m_sampleProducer(nullptr),
	m_channelsNumber(0)
{
}

SourceVoice::~SourceVoice() noexcept
{
	_ASSERTE(nullptr == m_voice);
	_ASSERTE(nullptr == m_sampleProducer);
	_ASSERTE(nullptr == m_guard);
}

_Check_return_
bool SourceVoice::CreateResources(_In_ IXAudio2 *xaudio, _In_ IXAudio2Voice *output, const WAVEFORMATEX *waveFormat) noexcept
{
	_ASSERTE(nullptr == m_guard);

	const DWORD GUARD_FLAGS =
#ifdef _DEBUG
		0;
#else
		CRITICAL_SECTION_NO_DEBUG_INFO;
#endif

	if (::InitializeCriticalSectionEx(&m_guardData, 4000, GUARD_FLAGS))
	{
		XAUDIO2_SEND_DESCRIPTOR	sd[1] = { 0 };
		XAUDIO2_VOICE_SENDS		sends[1] = { _ARRAYSIZE(sd), sd };

		sd->pOutputVoice = output;
		m_eofBytes = waveFormat->nBlockAlign;
		m_channelsNumber = waveFormat->nChannels;
		m_sampleProducer = new(std::nothrow) SampleProducer(StringOscillator, waveFormat);

		if (SUCCEEDED(xaudio->CreateSourceVoice(&m_voice, waveFormat, XAUDIO2_VOICE_NOSRC, XAUDIO2_DEFAULT_FREQ_RATIO, this, sends, NULL)))
		{
			m_guard = &m_guardData;
		}
		else
		{
			::DeleteCriticalSection(&m_guardData);
		}
	}

	return nullptr != m_guard;
}

void SourceVoice::TearDown() noexcept
{
	_ASSERTE(0 == m_started);
	_ASSERTE(nullptr != m_voice);
	_ASSERTE(nullptr != m_guard);

	m_voice->DestroyVoice();
	m_voice = nullptr;
	delete m_sampleProducer;
	m_sampleProducer = nullptr;
	::DeleteCriticalSection(m_guard);
	m_guard = nullptr;
}

_Check_return_
bool SourceVoice::Start(double x, double y) noexcept
{
	_ASSERTE(nullptr != m_voice);
	UNUSED(x);
	UNUSED(y);

	bool started = false;

	if (0 == _InterlockedCompareExchange16(&m_started, 1, 0))
	{
		m_voice->FlushSourceBuffers();
		m_eofSent = false;

		::EnterCriticalSection(m_guard);
		m_sampleProducer->SetFrequency(440.0 * ::pow(2.0, 1.0 * x));
		m_sampleProducer->ResetPhase();
		::LeaveCriticalSection(m_guard);

		if (SUCCEEDED(m_voice->Start()))
		{
			started = true;
		}

		if (!started)
		{
			_InterlockedExchange16(&m_started, 0);
		}
	}

	return started;
}

void SourceVoice::Move(double x, double y) noexcept
{
	UNUSED(y);
	::EnterCriticalSection(m_guard);
	//
	// TODO: change the sample producer parameters.
	//
	m_sampleProducer->SetFrequency(440.0 * ::pow(2.0, 1.0 * x));
	::LeaveCriticalSection(m_guard);
}

void SourceVoice::Stop() noexcept
{
	if (1 == _InterlockedCompareExchange16(&m_started, 0, 1))
	{
		_ASSERTE(nullptr != m_voice);
	}
}

void SourceVoice::OnVoiceProcessingPassStart(UINT32 bytesRequired) noexcept
{
	UNUSED(bytesRequired);

	if (0 == m_started)
	{
		if (!m_eofSent)
		{
			XAUDIO2_BUFFER *buffer = m_frameSource->Allocate(XAUDIO2_END_OF_STREAM, m_eofBytes);
			m_eofSent = m_frameSource->Submit(buffer, m_voice);
		}
	}
	else
	{
		size_t samples;
		XAUDIO2_BUFFER *frame = m_frameSource->Allocate(0, bytesRequired);
		SampleWriter writer = m_frameSource->GetSampleWriter(frame, &samples);

		::EnterCriticalSection(m_guard);
		for (size_t s = 0; s < samples / m_channelsNumber; ++s)
		{
			writer.Write(0.85f * m_sampleProducer->GetSample(), m_channelsNumber);
		}
		::LeaveCriticalSection(m_guard);

		if (!m_frameSource->Submit(frame, m_voice))
		{
			//
			// TODO: report an error
			//
		}
	}
}

void SourceVoice::OnVoiceProcessingPassEnd() noexcept
{
	if (m_eofSent)
	{
		m_voice->Stop();
		m_engine->RecycleSourceVoice(this);
	}
}

void SourceVoice::OnStreamEnd() noexcept
{
}

void SourceVoice::OnBufferStart(void *pBufferContext) noexcept
{
	UNUSED(pBufferContext);
}

void SourceVoice::OnBufferEnd(void *pBufferContext) noexcept
{
	m_frameSource->Release(AudioFrameSource::FromContext(pBufferContext));
}

void SourceVoice::OnLoopEnd(void *pBufferContext) noexcept
{
	UNUSED(pBufferContext);
}

void SourceVoice::OnVoiceError(void *pBufferContext, HRESULT error) noexcept
{
	UNUSED(pBufferContext);
	UNUSED(error);
}
