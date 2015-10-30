namespace Audiogen.ViewModels
{
    using System;

    public interface IPointerTracker : IDisposable
    {
        void Move(PointerPosition position);
    }
}
