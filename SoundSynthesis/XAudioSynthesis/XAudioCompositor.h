#pragma once

#include "IAudioCompositor.h"

namespace SoundSynthesis { namespace XAudioSynthesis
{
	class XAudioCompositor : IAudioCompositor
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
		void Retain() noexcept override;
		void Release() noexcept override;
		_Check_return_
		IAudioSource *GetOscillatingSource(_In_ Helpers::PFOSCILLATOR oscillator) noexcept override;
		//
		// Implementation
		//
		_Check_return_
		bool SetUp() noexcept;
		void TearDown() noexcept;

	private:
		volatile LONG			m_refs;
		IXAudio2				*m_xaudio2;
		IXAudio2MasteringVoice	*m_masteringVoice;
		XAUDIO2_VOICE_DETAILS	m_voiceDetails;
	};
}}
