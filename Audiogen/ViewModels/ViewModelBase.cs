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

        protected bool SetProperty<TPropertyType>(ref TPropertyType propertyVariable, TPropertyType newValue,
            [CallerMemberName] string propertyName = null)
        {
            bool valueChanged = !object.Equals(propertyVariable, newValue);

            if (valueChanged)
            {
                propertyVariable = newValue;
                EmitPropertyChanged(propertyName);
            }

            return valueChanged;
        }
        protected void EmitPropertyChanged(string propertyName)
        {
            if (null != propertyName)
                this.PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
