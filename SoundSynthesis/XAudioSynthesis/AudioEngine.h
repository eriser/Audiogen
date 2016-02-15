#pragma once

#include "WorkerThread.h"
#include "AudioFrameSource.h"

namespace SoundSynthesis { namespace XAudioSynthesis
{
	class SourceVoice;

	class AudioEngine sealed : public WorkerThread
	{
	public:
		AudioEngine() noexcept;
		~AudioEngine() noexcept;

		const WAVEFORMATEX	*GetWaveFormat() const noexcept { return &m_waveFormat; }
		AudioFrameSource	*GetFrameSource() noexcept { return &m_frameSource; }

		_Check_return_
		bool StartEngine() noexcept;
		void StopEngine() noexcept;
		SourceVoice *CreateSourceVoice() noexcept;
		void AsyncDestroySourceVoice(_In_ SourceVoice *voice) noexcept;

	private:
		void DestroySourceVoices() noexcept;
		static VOID CALLBACK DestroySourceVoiceStub(ULONG_PTR parameter) noexcept;
		void DestroySourceVoiceProc(SourceVoice *voice) noexcept;

	private:
		AudioFrameSource		m_frameSource;
		IXAudio2				*m_xaudio;
		IXAudio2MasteringVoice	*m_master;
		WAVEFORMATEX			m_waveFormat;
	};
}}
