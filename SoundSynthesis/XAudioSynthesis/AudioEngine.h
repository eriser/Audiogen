#pragma once

#include "AudioFrameSource.h"

namespace SoundSynthesis { namespace XAudioSynthesis
{
	class SourceVoice;

	const int SOURCE_VOICE_POOL_SIZE = 10;

	class AudioEngine sealed
	{
	public:
		AudioEngine() noexcept;
		~AudioEngine() noexcept;

		_Check_return_
		bool StartEngine() noexcept;
		void StopEngine() noexcept;

		SourceVoice *CreateSourceVoice() noexcept;
		void RecycleSourceVoice(_In_ SourceVoice *voice) noexcept;

	private:
		_Check_return_
		SourceVoice *InternalCreateSourceVoice() noexcept;
		_Check_return_
		bool CreateSourceVoicePool() noexcept;
		void DestroySourceVoicePool() noexcept;

	private:
		CRITICAL_SECTION		m_guard;
		std::list<SourceVoice*>	m_voicePool;
		AudioFrameSource		m_frameSource;
		IXAudio2				*m_xaudio;
		IXAudio2MasteringVoice	*m_master;
		WAVEFORMATEX			m_waveFormat;
	};
}}
