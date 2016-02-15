#pragma once

#include "WorkerThread.h"

namespace SoundSynthesis { namespace XAudioSynthesis
{

	class AudioEngine sealed : public WorkerThread
	{
	public:
		AudioEngine() noexcept;
		~AudioEngine() noexcept;

		_Check_return_
		bool StartEngine() noexcept;
		void StopEngine() noexcept;

	private:
		IXAudio2				*m_xaudio;
		IXAudio2MasteringVoice	*m_master;
		WAVEFORMATEX			m_waveFormat;
	};
}}
