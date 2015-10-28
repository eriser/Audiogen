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
            Debug.WriteLine("Pointer down at ({0}:{1})", position.X, position.Y);
        }

        void IPointerHandler.Move(PointerPosition position)
        {
            Debug.WriteLine("Pointer moved to ({0}:{1})", position.X, position.Y);
        }

        void IPointerHandler.Up()
        {
            Debug.WriteLine("Pointer up");
        }
    }
}
