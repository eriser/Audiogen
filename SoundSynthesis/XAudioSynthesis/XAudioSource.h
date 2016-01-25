#pragma once

#include "Retainable.h"
#include "IAudioSource.h"
#include "IAudioCompositor.h"
#include "ActiveVoice.h"

namespace SoundSynthesis { namespace XAudioSynthesis
{
	struct XAudioSource : public Retainable<IAudioSource>
	{
	public:
		XAudioSource(_In_ IAudioCompositor *compositor, _In_ ActiveVoice *voice) noexcept;

	protected:
		~XAudioSource() noexcept;

		void FinalRelease() noexcept override;

	private:
		void Start(float x, float y) noexcept override;
		void Move(float x, float y) noexcept override;
		void Stop() noexcept override;

	private:
		IAudioCompositor * const	m_compositor;
		ActiveVoice					*m_voice;
	};
}}
