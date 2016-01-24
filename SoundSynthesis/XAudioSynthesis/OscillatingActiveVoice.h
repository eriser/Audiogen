#pragma once

#include "ActiveVoice.h"
#include "..\Helpers\Oscillators.h"

namespace SoundSynthesis { namespace XAudioSynthesis
{
	class OscillatingActiveVoice : public ActiveVoice
	{
	public:
		OscillatingActiveVoice(_In_ const WAVEFORMATEX *waveFormat, _In_ Helpers::PFOSCILLATOR oscillator) noexcept;

	private:
		~OscillatingActiveVoice() noexcept;

	protected:
		_Check_return_
		XAudioFrame *MakeAudioFrame(UINT32 bytesRequired, _In_ const WAVEFORMATEX *waveFormat) noexcept override;

	private:
		const Helpers::PFOSCILLATOR m_oscillator;
	};
}}
