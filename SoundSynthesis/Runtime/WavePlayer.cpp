#include "pch.h"
#include "WavePlayer.h"

using namespace SoundSynthesis::Runtime;

WavePlayer::WavePlayer()
:	m_masteringVoice(nullptr),
	m_sourceVoice(nullptr)
{
}

WavePlayer::~WavePlayer()
{
	if (m_sourceVoice)
	{
		m_sourceVoice->DestroyVoice();
		m_sourceVoice = nullptr;
	}

	if (m_masteringVoice)
	{
		m_masteringVoice->DestroyVoice();
		m_masteringVoice = nullptr;
	}

	m_xaudio.Reset();
}

WavePlayer ^WavePlayer::CreateWavePlayer()
{
	WavePlayer ^player = ref new WavePlayer();

	if (!player->Initialize())
	{
		delete player;
		player = nullptr;
	}

	return player;
}

bool WavePlayer::Initialize()
{
	bool initialized = false;

	if (SUCCEEDED(::XAudio2Create(m_xaudio.GetAddressOf(), 0, XAUDIO2_DEFAULT_PROCESSOR)))
	{
		if (SUCCEEDED(m_xaudio->CreateMasteringVoice(&m_masteringVoice)))
		{
			XAUDIO2_VOICE_DETAILS voiceDetails;

			m_masteringVoice->GetVoiceDetails(&voiceDetails);
			m_voiceCallback.SetUp(&voiceDetails);

			HRESULT hr = m_xaudio->CreateSourceVoice(&m_sourceVoice,
				m_voiceCallback.GetWaveFormat(),
				0,
				XAUDIO2_DEFAULT_FREQ_RATIO,
				&m_voiceCallback);

			if (SUCCEEDED(hr))
			{
				XAUDIO2_SEND_DESCRIPTOR	desc[1] = { 0, m_masteringVoice };
				XAUDIO2_VOICE_SENDS		voices = { _countof(desc), desc };

				m_voiceCallback.SetSourceVoice(m_sourceVoice);

				if (FAILED(m_sourceVoice->SetOutputVoices(&voices)))
				{
					m_sourceVoice->DestroyVoice();
					m_sourceVoice = nullptr;
				}
				else
				{
					initialized = true;
				}
			}

			if(!initialized)
			{
				m_masteringVoice->DestroyVoice();
				m_masteringVoice = nullptr;
			}
		}

		if (!initialized)
		{
			m_xaudio.Reset();
		}
	}

	return initialized;
}
