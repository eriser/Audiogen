#include "pch.h"
#include "SourceVoice.h"
#include "AudioEngine.h"

using namespace SoundSynthesis::XAudioSynthesis;

SourceVoice::SourceVoice(_In_ AudioEngine *engine) noexcept
:	m_started(0),
	m_engine(engine),
	m_voice(nullptr),
	m_output(nullptr),
	m_eofBuffer(nullptr)
{
}

SourceVoice::~SourceVoice() noexcept
{
	_ASSERTE(nullptr == m_voice);
	_ASSERTE(nullptr == m_output);
	_ASSERTE(nullptr == m_eofBuffer);
}

bool SourceVoice::Initialize(_In_ IXAudio2 *xaudio, _In_ IXAudio2Voice *output) noexcept
{
	_ASSERTE(nullptr == m_eofBuffer);

	bool initialized = false;

	m_eofBuffer = m_engine->GetFrameSource()->Allocate(XAUDIO2_END_OF_STREAM, m_engine->GetWaveFormat()->nBlockAlign);

	if (nullptr != m_eofBuffer)
	{
		XAUDIO2_VOICE_SENDS	sends[1] = { 0 };

		if (SUCCEEDED(xaudio->CreateSourceVoice(&m_voice, m_engine->GetWaveFormat(), 0, XAUDIO2_DEFAULT_FREQ_RATIO, this, sends, NULL)))
		{
			m_output = output;
			initialized = true;
		}
		else
		{
			m_engine->GetFrameSource()->Release(m_eofBuffer);
			m_eofBuffer = nullptr;
		}
	}

	return initialized;
}

void SourceVoice::TearDown() noexcept
{
	_ASSERTE(0 == m_started);
	_ASSERTE(nullptr != m_voice);
	_ASSERTE(nullptr == m_eofBuffer);
	m_voice->DestroyVoice();
	m_voice = nullptr;
	m_output = nullptr;
}

bool SourceVoice::Start() noexcept
{
	bool started = false;

	if (0 == _InterlockedCompareExchange16(&m_started, 1, 0))
	{
		_ASSERTE(nullptr != m_voice);

		XAUDIO2_SEND_DESCRIPTOR	sd[1] = { 0 };
		XAUDIO2_VOICE_SENDS	sends[1] = { 0 };

		sends->SendCount = 1;
		sends->pSends = sd;
		sd->pOutputVoice = m_output;

		if (SUCCEEDED(m_voice->SetOutputVoices(sends)))
		{
			if (FAILED(m_voice->Start()))
			{
				sends->SendCount = 0;
				sends->pSends = nullptr;
				m_voice->SetOutputVoices(sends);
			}
			else
			{
				started = true;
			}
		}

		if (!started)
		{
			_InterlockedExchange16(&m_started, 0);
		}
	}

	return started;
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
		if (nullptr != m_eofBuffer)
		{
			XAUDIO2_BUFFER *buffer = m_eofBuffer;
			m_eofBuffer = nullptr;
			m_engine->GetFrameSource()->Submit(buffer, m_voice);
		}
	}
	else
	{
		size_t samples;
		XAUDIO2_BUFFER *frame = m_engine->GetFrameSource()->Allocate(0, bytesRequired);
		SampleWriter writer = m_engine->GetFrameSource()->GetSampleWriter(frame, &samples);

		for (size_t s = 0; s < samples; ++s)
		{
			writer.Write((static_cast<float>(rand() % 100) - 50.0f) / 100.0f, 1);
		}

		m_engine->GetFrameSource()->Submit(frame, m_voice);
	}
}

void SourceVoice::OnVoiceProcessingPassEnd() noexcept
{
	if (nullptr == m_eofBuffer)
	{
		m_voice->Stop();
		m_engine->AsyncDestroySourceVoice(this);
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
	m_engine->GetFrameSource()->Release(AudioFrameSource::FromContext(pBufferContext));
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
