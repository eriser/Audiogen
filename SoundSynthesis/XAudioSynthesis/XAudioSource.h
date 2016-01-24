#pragma once

#include "Retainable.h"
#include "IAudioSource.h"
#include "IAudioCompositor.h"

namespace SoundSynthesis { namespace XAudioSynthesis
{
	struct XAudioSource : public Retainable<IAudioSource>
	{
	public:
		XAudioSource(IAudioCompositor *compositor) noexcept;

	protected:
		~XAudioSource() noexcept;

		void FinalRelease() noexcept override;

	private:
		IAudioCompositor * const m_compositor;
	};
}}
