#pragma once

#include "Retainable.h"
#include "IAudioCompositor.h"
#include "ActiveVoices.h"

namespace SoundSynthesis { namespace XAudioSynthesis
{
	class XAudioCompositor : Retainable<IAudioCompositor>
	{
	public:
		_Check_return_
		static IAudioCompositor *Create() noexcept;

	private:
		XAudioCompositor() noexcept;
		~XAudioCompositor() noexcept;
		//
		// IAudioCompositor
		//
		_Check_return_
		IAudioSource *GetOscillatingSource(_In_ Helpers::PFOSCILLATOR oscillator) noexcept override;
		//
		// Implementation
		//
		_Check_return_
		bool SetUp() noexcept;
		void FinalRelease() noexcept override;

	private:
		IXAudio2				*m_xaudio2;
		IXAudio2MasteringVoice	*m_masteringVoice;
		WAVEFORMATEX			m_waveFormat;
		ActiveVoices			m_activeVoices;
	};
}}
