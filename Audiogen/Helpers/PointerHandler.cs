namespace Audiogen.Helpers
{
    using Audiogen.ViewModels;
    using Models;
    using System.Diagnostics.Contracts;
    using System;

    class PointerHandler : IPointerHandler, IDisposable
    {
        private bool _disposedVaulue;
        private ISynthesizer _synthesizer;
        private Tracker _tracker;

        private sealed class Tracker : IPointerTracker
        {
            private readonly PointerHandler _parent;
            private readonly ISynthesizer _synthesizer;
            private bool _disposedValue = false; // To detect redundant calls

            public Tracker(PointerHandler parent, ISynthesizer synthesizer)
            {
                _parent = parent;
                _synthesizer = synthesizer;
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

            public void Start(PointerPosition position)
            {
                _synthesizer.BeginTone(position);
            }

            void IPointerTracker.Move(PointerPosition position)
            {
                _synthesizer.ChangeTone(position);
            }

            private void Dispose(bool disposing)
            {
                if (!_disposedValue)
                {
                    _disposedValue = true;

                    if (disposing)
                    {
                        // TODO: dispose managed state (managed objects).
                    }

                    _synthesizer.EndTone();
                    _parent.ReleaseTracker(this);
                }
            }
        }

        public PointerHandler()
        {
            _disposedVaulue = false;
        }

        ~PointerHandler()
        {
            Dispose(false);
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        public void SetSynthesizer(ISynthesizer synthesizer)
        {
            Contract.Assert(null == _synthesizer);
            Contract.Assert(null != synthesizer);
            _synthesizer = synthesizer;
        }

        IPointerTracker IPointerHandler.TrackPointer(PointerPosition position)
        {
            IPointerTracker newTracker = null;

            if(null == _tracker)
            {
                Tracker tracker = new Tracker(this, _synthesizer);

                _tracker = tracker;
                newTracker = tracker;
                tracker.Start(position);
            }

            return newTracker;
        }

        private void ReleaseTracker(Tracker tracker)
        {
            if(object.ReferenceEquals(_tracker, tracker))
            {
                _tracker = null;
            }
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!_disposedVaulue)
            {
                _disposedVaulue = true;

                if (disposing)
                {
                    // TODO: dispose managed state (managed objects).
                }

                if (null != _tracker)
                {
                    _tracker.Dispose();
                    Contract.Assert(null == _tracker);
                }
            }
        }
    }
}
