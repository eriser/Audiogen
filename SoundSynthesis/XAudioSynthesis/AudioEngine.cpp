#include "pch.h"
#include "AudioEngine.h"

using namespace SoundSynthesis::XAudioSynthesis;

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
		m_master->DestroyVoice();
		m_master = nullptr;
		m_xaudio->Release();
		m_xaudio = nullptr;
	}
}
