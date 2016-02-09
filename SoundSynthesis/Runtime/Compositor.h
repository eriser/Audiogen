#pragma once

#include "CompositorVoice.h"
#include "XAudioSynthesis\IAudioCompositor.h"
#include "XAudioSynthesis\WhiteNoise.h"

using namespace Microsoft::WRL;
using namespace SoundSynthesis::XAudioSynthesis;

namespace SoundSynthesis { namespace Runtime
{
	public ref class Compositor sealed
	{
	public:
		virtual ~Compositor();

		static Compositor ^CreateCompositor();

		bool Start();
		void Stop();
		CompositorVoice ^GetVoice();

		void StartWhiteNoise();
		void StopWhiteNoise();

	private:
		Compositor();
		bool Initialize();

	private:
		IAudioCompositor	*m_compositor;
		WhiteNoise			m_whiteNoise;
	};
}}
