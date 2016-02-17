namespace Audiogen.Helpers
{
    using Audiogen.ViewModels;
    using SoundSynthesis.Runtime;
    using System;

    sealed class CompositorPointerHandler : IPointerHandler, IDisposable
    {
        private bool _disposed;
        private readonly Compositor _compositor;

        private sealed class Tracker : IPointerTracker
        {
            private bool _disposed = false;
            private readonly CompositorVoice m_voice;

            void IPointerTracker.Move(PointerPosition position)
            {
                m_voice.Move(position.X, position.Y);
            }

            void Dispose(bool disposing)
            {
                if (!_disposed)
                {
                    if (disposing)
                    {
                        m_voice.Dispose();
                    }

                    _disposed = true;
                }
            }

            public Tracker(CompositorVoice voice)
            {
                m_voice = voice;
            }

            ~Tracker()
            {
                Dispose(false);
            }

            public void Dispose()
            {
                Dispose(true);
                GC.SuppressFinalize(this);
            }
        }

        public CompositorPointerHandler()
        {
            _compositor = Compositor.CreateCompositor();
            _compositor.Start();
        }

        ~CompositorPointerHandler()
        {
            Dispose(false);
        }

        IPointerTracker IPointerHandler.TrackPointer(PointerPosition position)
        {
            Tracker tracker = null;

            CompositorVoice cv = _compositor.GetVoice();
            if(null != cv)
            {
                tracker = new Tracker(cv);
                cv.Start(position.X, position.Y);
            }
            return tracker;
        }

        void Dispose(bool disposing)
        {
            if (!_disposed)
            {
                if (disposing)
                {
                    _compositor.Stop();
                    _compositor.Dispose();
                }

                _disposed = true;
            }
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }
    }
}
