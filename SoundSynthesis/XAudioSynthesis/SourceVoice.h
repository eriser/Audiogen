#pragma once

namespace SoundSynthesis { namespace XAudioSynthesis
{
	class AudioEngine;

	class SourceVoice sealed : IXAudio2VoiceCallback
	{
	public:
		SourceVoice(_In_ AudioEngine *engine) noexcept;
		~SourceVoice() noexcept;

		bool Initialize(_In_ IXAudio2 *xaudio, _In_ IXAudio2Voice *output) noexcept;
		void TearDown() noexcept;
		bool Start() noexcept;
		void Stop() noexcept;

	private:
		// Inherited via IXAudio2VoiceCallback
		void OnVoiceProcessingPassStart(UINT32 BytesRequired) noexcept override;
		void OnVoiceProcessingPassEnd() noexcept override;
		void OnStreamEnd() noexcept override;
		void OnBufferStart(void *pBufferContext) noexcept override;
		void OnBufferEnd(void *pBufferContext) noexcept override;
		void OnLoopEnd(void *pBufferContext) noexcept override;
		void OnVoiceError(void *pBufferContext, HRESULT error) noexcept override;

	private:
		volatile INT16		m_started;
		AudioEngine * const	m_engine;
		IXAudio2SourceVoice	*m_voice;
		IXAudio2Voice		*m_output;
		XAUDIO2_BUFFER		*m_eofBuffer;
	};

}}
