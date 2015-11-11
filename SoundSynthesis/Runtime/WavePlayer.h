#pragma once

#include "..\Helpers\VoiceCallback.h"

using namespace Microsoft::WRL;

namespace SoundSynthesis { namespace Runtime
{
	public ref class WavePlayer sealed
	{
	private:
		ComPtr<IXAudio2>		m_xaudio;
		IXAudio2MasteringVoice	*m_masteringVoice;
		IXAudio2SourceVoice		*m_sourceVoice;
		Helpers::VoiceCallback	m_voiceCallback;

	public:
		virtual ~WavePlayer();

		static WavePlayer ^CreateWavePlayer();

	private:
		WavePlayer();
		bool Initialize();
	};
}}
