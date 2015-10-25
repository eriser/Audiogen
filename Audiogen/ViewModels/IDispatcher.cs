namespace Audiogen.ViewModels
{
    using System;

    interface IDispatcher
    {
        void Dispatch(Action action);
    }
}
