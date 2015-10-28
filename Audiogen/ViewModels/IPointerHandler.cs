namespace Audiogen.ViewModels
{
    /// <summary>
    /// Interface for sending pointer events from UI elements to view models.
    /// A view model may expose the interface as its property that can be bound
    /// to a UI control that receives pointer input. The control then calls the interface
    /// to send pointer events to the view model.
    /// </summary>
    public interface IPointerHandler
    {
        void Down(PointerPosition position);
        void Move(PointerPosition position);
        void Up();
    }
}
