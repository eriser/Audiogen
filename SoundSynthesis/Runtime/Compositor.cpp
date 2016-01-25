#include "pch.h"
#include "Compositor.h"
#include "XAudioSynthesis\XAudioCompositor.h"
#include "Helpers\Oscillators.h"

using namespace SoundSynthesis::Runtime;

Compositor::~Compositor()
{
	m_compositor->Release();
}

Compositor ^Compositor::CreateCompositor()
{
	Compositor ^compositor = ref new Compositor();

	if (!compositor->Initialize())
	{
		delete compositor;
		compositor = nullptr;
	}

	return compositor;
}

bool Compositor::Start()
{
	bool started = m_compositor->Start();

	if (started)
	{
		IAudioSource *source1 = m_compositor->GetOscillatingSource(SoundSynthesis::Helpers::LadderOscillator);
		m_compositor->GetOscillatingSource(SoundSynthesis::Helpers::LadderOscillator);
		m_compositor->GetOscillatingSource(SoundSynthesis::Helpers::LadderOscillator);
		source1->Release();
	}

	return started;
}

void Compositor::Stop()
{

}

Compositor::Compositor()
:	m_compositor(nullptr)
{
}

bool Compositor::Initialize()
{
	m_compositor = XAudioCompositor::Create();
	return nullptr != m_compositor;
}
