#pragma once

#include "ActiveVoice.h"
#include "..\Helpers\Oscillators.h"

namespace SoundSynthesis { namespace XAudioSynthesis
{
	//
	// Collection of ActiveVoice objects.
	//
	class ActiveVoices
	{
	public:
		ActiveVoices() noexcept;
		~ActiveVoices() noexcept;

		bool Initialize() noexcept;
		_Check_return_
		ActiveVoice *CreateOscillatingVoice(_In_ const WAVEFORMATEX *waveFormat, _In_ Helpers::PFOSCILLATOR oscillator) noexcept;

	private:
		_Check_return_
		bool AddVoice(_In_ ActiveVoice *voice) noexcept;

	private:
		bool					m_initialized;
		CRITICAL_SECTION		m_monitor;
		std::set<ActiveVoice*>	m_voices;
	};
}}
