#include "pch.h"
#include "CompositorVoice.h"

using namespace SoundSynthesis::Runtime;

CompositorVoice::~CompositorVoice()
{
	m_voice->Stop();
}

_Check_return_
bool CompositorVoice::Start(double x, double y)
{
	return m_voice->Start(x, y);
}

void CompositorVoice::Move(double x, double y)
{
	m_voice->Move(x, y);
}

CompositorVoice ^CompositorVoice::Create(_In_ SourceVoice *voice)
{
	return ref new CompositorVoice(voice);
}

CompositorVoice::CompositorVoice(_In_ SourceVoice *voice)
:	m_voice(voice)
{
}
