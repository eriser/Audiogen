namespace Audiogen.ViewModels
{
    using System.ComponentModel;
    using System.Runtime.CompilerServices;

    abstract class ViewModelBase : INotifyPropertyChanged
    {
        /// <summary>
        /// Implementation of INotifyPropertyChanged
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;

        protected void SetProperty<TPropertyType>(ref TPropertyType propertyVariable, TPropertyType newValue,
            [CallerMemberName] string propertyName = null)
        {
            if(!object.Equals(propertyVariable, newValue))
            {
                propertyVariable = newValue;
                EmitPropertyChanged(propertyName);
            }
        }
        protected void EmitPropertyChanged(string propertyName)
        {
            if (null != propertyName)
                this.PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
