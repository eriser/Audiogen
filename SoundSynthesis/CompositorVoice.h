#pragma once

#include "XAudioSynthesis\IAudioSource.h"

using namespace SoundSynthesis::XAudioSynthesis;

namespace SoundSynthesis { namespace Runtime
{
	public ref class CompositorVoice sealed
	{
	public:
		virtual ~CompositorVoice();

		void Start(double x, double y);

	internal:
		static CompositorVoice ^Create(_In_ IAudioSource *voice);

	private:
		CompositorVoice(_In_ IAudioSource *voice);

	private:
		IAudioSource * const m_voice;
	};
}}
