#include "pch.h"
#include "OscillatingActiveVoice.h"

using namespace SoundSynthesis::XAudioSynthesis;
using namespace SoundSynthesis::Helpers;

OscillatingActiveVoice::OscillatingActiveVoice(_In_ const WAVEFORMATEX *waveFormat, _In_ PFOSCILLATOR oscillator) noexcept
:	ActiveVoice(waveFormat),
	m_oscillator(oscillator)
{
	_ASSERTE(nullptr != m_oscillator);
}

OscillatingActiveVoice::~OscillatingActiveVoice() noexcept
{
}

_Check_return_
XAudioFrame *OscillatingActiveVoice::MakeAudioFrame(UINT32 bytesRequired, _In_ const WAVEFORMATEX *waveFormat) noexcept
{
	UNUSED(bytesRequired);
	UNUSED(waveFormat);
	return nullptr;
}
