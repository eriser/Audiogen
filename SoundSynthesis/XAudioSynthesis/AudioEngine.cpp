#include "pch.h"
#include "SourceVoice.h"
#include "AudioEngine.h"

using namespace SoundSynthesis::XAudioSynthesis;

namespace
{
	struct DESTROY_SOURCE_VOICE_DATA
	{
		AudioEngine	*engine;
		SourceVoice	*voice;
	};
}

AudioEngine::AudioEngine() noexcept
:	m_xaudio(nullptr),
	m_master(nullptr)
{
	::SecureZeroMemory(&m_waveFormat, sizeof(m_waveFormat));
}

AudioEngine::~AudioEngine() noexcept
{
	_ASSERTE(nullptr == m_xaudio);
	_ASSERTE(nullptr == m_master);
}

_Check_return_
bool AudioEngine::StartEngine() noexcept
{
	bool started = WorkerThread::Start();

	if (started)
	{
		_ASSERTE(nullptr == m_xaudio);

		if (FAILED(::XAudio2Create(&m_xaudio)))
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
				m_xaudio->Release();
				m_xaudio = nullptr;
			}
		}

		if (!started)
		{
			WorkerThread::Stop();
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
		WorkerThread::Stop();
		m_master->DestroyVoice();
		m_master = nullptr;
		DestroySourceVoices();
		m_xaudio->Release();
		m_xaudio = nullptr;
	}
}

SourceVoice *AudioEngine::CreateSourceVoice() noexcept
{
	SourceVoice *voice = new(std::nothrow) SourceVoice(this);

	if (voice)
	{
		if (!voice->Initialize(m_xaudio, m_master))
		{
			delete voice;
			voice = nullptr;
		}
	}

	return voice;
}

void AudioEngine::AsyncDestroySourceVoice(_In_ SourceVoice *voice) noexcept
{
	DESTROY_SOURCE_VOICE_DATA *data = new(std::nothrow) DESTROY_SOURCE_VOICE_DATA;

	data->engine = this;
	data->voice = voice;

	WorkerThread::Queue(DestroySourceVoiceStub, reinterpret_cast<ULONG_PTR>(data));
}

void AudioEngine::DestroySourceVoices() noexcept
{
}

VOID CALLBACK AudioEngine::DestroySourceVoiceStub(ULONG_PTR parameter) noexcept
{
	DESTROY_SOURCE_VOICE_DATA *data = reinterpret_cast<DESTROY_SOURCE_VOICE_DATA*>(parameter);
	data->engine->DestroySourceVoiceProc(data->voice);
	delete data;
}

void AudioEngine::DestroySourceVoiceProc(SourceVoice *voice) noexcept
{
	voice->TearDown();
	delete voice;
}
