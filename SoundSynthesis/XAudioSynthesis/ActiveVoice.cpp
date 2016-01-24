#include "pch.h"
#include "ActiveVoice.h"

using namespace SoundSynthesis::XAudioSynthesis;

ActiveVoice::ActiveVoice(_In_ const WAVEFORMATEX *waveFormat) noexcept
:	m_waveFormat(waveFormat)
{
	_ASSERTE(nullptr != m_waveFormat);
}

ActiveVoice::~ActiveVoice() noexcept
{
}

STDMETHODIMP_(void) ActiveVoice::OnVoiceProcessingPassStart(UINT32 BytesRequired) noexcept
{
	UNUSED(BytesRequired);
}

STDMETHODIMP_(void) ActiveVoice::OnVoiceProcessingPassEnd() noexcept
{
}

STDMETHODIMP_(void) ActiveVoice::OnStreamEnd() noexcept
{
}

STDMETHODIMP_(void) ActiveVoice::OnBufferStart(void *pBufferContext) noexcept
{
	UNUSED(pBufferContext);
}

STDMETHODIMP_(void) ActiveVoice::OnBufferEnd(void *pBufferContext) noexcept
{
	UNUSED(pBufferContext);
}

STDMETHODIMP_(void) ActiveVoice::OnLoopEnd(void *pBufferContext) noexcept
{
	UNUSED(pBufferContext);
}

STDMETHODIMP_(void) ActiveVoice::OnVoiceError(void *pBufferContext, HRESULT Error) noexcept
{
	UNUSED(pBufferContext);
	UNUSED(Error);
}
