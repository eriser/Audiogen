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
		void TearDown() noexcept;
		void FinalCleanup() noexcept;
		_Check_return_
		ActiveVoice *CreateOscillatingVoice(_In_ IXAudio2 *audio,
			_In_ IXAudio2Voice *receiver,
			_In_ const WAVEFORMATEX *waveFormat,
			_In_ Helpers::PFOSCILLATOR oscillator) noexcept;

		void CleanupAsync() noexcept;

	private:
		_Check_return_
		bool AddVoice(_In_ ActiveVoice *voice) noexcept;

		static unsigned int __stdcall CleanupThreadStub(void *arg) noexcept;
		void CleanupThreadBody() noexcept;
		static VOID NTAPI CleanupAPC(_In_ ULONG_PTR parameter) noexcept;
		void CleanupBody() noexcept;

	private:
		HANDLE					m_cleanupThread;
		HANDLE					m_stopCleanupThread;	// waitable event set to stop the cleanup thread
		CRITICAL_SECTION		m_monitor;
		std::set<ActiveVoice*>	m_voices;
	};
}}
