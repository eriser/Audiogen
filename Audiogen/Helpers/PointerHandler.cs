namespace Audiogen.Helpers
{
    using Models;
    using Audiogen.ViewModels;
    using System.Diagnostics.Contracts;
    using System;
    using System.Diagnostics;

    class PointerHandler : IPointerHandler
    {
        private ISynthesizer _synthesizer;

        public void SetSynthesizer(ISynthesizer synthesizer)
        {
            Contract.Assert(null == _synthesizer);
            Contract.Assert(null != synthesizer);
            _synthesizer = synthesizer;
        }

        void IPointerHandler.Down(PointerPosition position)
        {
            _synthesizer.BeginTone(position);
        }

        void IPointerHandler.Move(PointerPosition position)
        {
            _synthesizer.ChangeTone(position);
        }

        void IPointerHandler.Up()
        {
            _synthesizer.EndTone();
        }
    }
}
