#include "pch.h"
#include "SourceVoice.h"
#include "AudioEngine.h"

using namespace SoundSynthesis::XAudioSynthesis;

AudioEngine::AudioEngine() noexcept
:	m_xaudio(nullptr),
	m_master(nullptr)
{
	::SecureZeroMemory(&m_waveFormat, sizeof(m_waveFormat));
	::SecureZeroMemory(&m_guard, sizeof(m_guard));
}

AudioEngine::~AudioEngine() noexcept
{
	_ASSERTE(nullptr == m_xaudio);
	_ASSERTE(nullptr == m_master);
}

_Check_return_
bool AudioEngine::StartEngine() noexcept
{
	_ASSERTE(nullptr == m_xaudio);

	bool started = true;

	if (FAILED(::XAudio2Create(&m_xaudio)))
	{
		started = false;
	}
	else
	{
		if (!::InitializeCriticalSectionEx(&m_guard, 4000, 0))
		{
			started = false;
		}
		else
		{
			if (FAILED(m_xaudio->CreateMasteringVoice(&m_master, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE,
				0, NULL, NULL, AudioCategory_GameMedia)))
			{
				started = false;
			}
			else
			{

				XAUDIO2_VOICE_DETAILS details;

				m_master->GetVoiceDetails(&details);
				m_waveFormat.cbSize = sizeof(m_waveFormat);
				m_waveFormat.wFormatTag = WAVE_FORMAT_IEEE_FLOAT;
				m_waveFormat.wBitsPerSample = sizeof(float) * 8;
				m_waveFormat.nChannels = static_cast<WORD>(details.InputChannels);
				m_waveFormat.nBlockAlign = sizeof(float) * m_waveFormat.nChannels;
				m_waveFormat.nSamplesPerSec = details.InputSampleRate;
				m_waveFormat.nAvgBytesPerSec = m_waveFormat.nSamplesPerSec * m_waveFormat.nBlockAlign;

				if (FAILED(m_xaudio->StartEngine()))
				{
					started = false;
					m_master->DestroyVoice();
					m_master = nullptr;
				}
			}

			if (!started)
			{
				::DeleteCriticalSection(&m_guard);
			}
		}

		if (!started)
		{
			m_xaudio->Release();
			m_xaudio = nullptr;
		}
	}

	return started;
}

void AudioEngine::StopEngine() noexcept
{
	if (nullptr != m_xaudio)
	{
		_ASSERTE(nullptr != m_master);
		m_xaudio->StopEngine();
		m_master->DestroyVoice();
		m_master = nullptr;
		DestroySourceVoicePool();
		m_xaudio->Release();
		m_xaudio = nullptr;
		::DeleteCriticalSection(&m_guard);
		::SecureZeroMemory(&m_guard, sizeof(m_guard));
		::SecureZeroMemory(&m_waveFormat, sizeof(m_waveFormat));
	}
}

SourceVoice *AudioEngine::CreateSourceVoice() noexcept
{
	SourceVoice *voice = nullptr;

	::EnterCriticalSection(&m_guard);

	if (!m_voicePool.empty())
	{
		voice = m_voicePool.front();
		m_voicePool.pop_front();
		::LeaveCriticalSection(&m_guard);
	}
	else
	{
		::LeaveCriticalSection(&m_guard);
		voice = InternalCreateSourceVoice();
	}

	return voice;
}

void AudioEngine::RecycleSourceVoice(_In_ SourceVoice *voice) noexcept
{
	::EnterCriticalSection(&m_guard);
	m_voicePool.push_back(voice);
	::LeaveCriticalSection(&m_guard);
}

_Check_return_
SourceVoice *AudioEngine::InternalCreateSourceVoice() noexcept
{
	SourceVoice *voice = new(std::nothrow) SourceVoice(this, &m_frameSource);

	if (nullptr != voice && !voice->CreateResources(m_xaudio, m_master, &m_waveFormat))
	{
		delete voice;
		voice = nullptr;
	}

	return voice;
}

_Check_return_
bool AudioEngine::CreateSourceVoicePool() noexcept
{
	_ASSERTE(nullptr != m_xaudio);

	bool created = true;

	for (int i = 0; created && i < SOURCE_VOICE_POOL_SIZE; ++i)
	{
		SourceVoice *voice = InternalCreateSourceVoice();

		if (nullptr != voice)
		{
			m_voicePool.push_back(voice);
		}
		else
		{
			DestroySourceVoicePool();
			created = false;
		}
	}

	return created;
}

void AudioEngine::DestroySourceVoicePool() noexcept
{
	_ASSERTE(nullptr != m_xaudio);

	while (!m_voicePool.empty())
	{
		SourceVoice *voice = m_voicePool.front();
		m_voicePool.pop_front();

		voice->TearDown();
		delete voice;
	}
}
