// <copyright file="DeviceServicesPageViewModel.cs" company="Microsoft Corporation">
// Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//----------------------------------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using BluetoothLEExplorer.Models;
using BluetoothLEExplorer.Views;
using Template10.Mvvm;
using Template10.Services.NavigationService;
using Windows.UI.Xaml.Navigation;

namespace BluetoothLEExplorer.ViewModels
{
    /// <summary>
    /// View Model for Device Services View
    /// </summary>
    public class DeviceServicesPageViewModel : ViewModelBase
    {
        /// <summary>
        /// App context
        /// </summary>
        private GattSampleContext context = GattSampleContext.Context;

        /// <summary>
        /// Gets the currently selected bluetooth device
        /// </summary>
        public ObservableBluetoothLEDevice Device { get; private set; } = GattSampleContext.Context.SelectedBluetoothLEDevice;

        /// <summary>
        /// Gets a value indicating if the version of Windows on this machine supports
        /// secure connections.
        /// </summary>
        public bool IsSecureConnectionSupported
        {
            get
            {
                return context.IsSecureConnectionSupported;
            }
        }

        /// <summary>
        /// Source for <see cref="ErrorText"/>
        /// </summary>
        private string errorText = String.Empty;

        /// <summary>
        /// Gets the error text to display
        /// </summary>
        public string ErrorText
        {
            get
            {
                return errorText;
            }

            private set
            {
                Set(ref errorText, value, "ErrorText");
            }
        }
        
        /// <summary>
        /// Source for <see cref="SelectedCharacteristic"/>
        /// </summary>
        private ObservableGattCharacteristics selectedCharacteristic;

        /// <summary>
        /// Gets or sets the currently selected characteristic
        /// </summary>
        public ObservableGattCharacteristics SelectedCharacteristic
        {
            get
            {
                return selectedCharacteristic;
            }

            set
            {
                Set(ref selectedCharacteristic, value, "SelectedCharacteristic");
                context.SelectedCharacteristic = SelectedCharacteristic;
                NavigationService.Navigate(typeof(CharacteristicPage));
            }
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="DeviceServicesPageViewModel" /> class.
        /// </summary>
        public DeviceServicesPageViewModel()
        {
            if (Windows.ApplicationModel.DesignMode.DesignModeEnabled)
            {
            }
        }

        /// <summary>
        /// Navigate from page
        /// </summary>
        /// <param name="parameter"></param>
        /// <param name="mode"></param>
        /// <param name="suspensionState"></param>
        /// <returns>Navigate from Task</returns>
        public override async Task OnNavigatedToAsync(object parameter, NavigationMode mode, IDictionary<string, object> suspensionState)
        {
            await Windows.ApplicationModel.Core.CoreApplication.MainView.CoreWindow.Dispatcher.RunAsync(
                Windows.UI.Core.CoreDispatcherPriority.Normal, 
                () =>
            {
            });

            await Task.CompletedTask;
        }

        /// <summary>
        /// Navigate from page
        /// </summary>
        /// <param name="suspensionState"></param>
        /// <param name="suspending"></param>
        /// <returns>Navigate from task</returns>
        public override async Task OnNavigatedFromAsync(IDictionary<string, object> suspensionState, bool suspending)
        {
            if (suspending)
            {
            }

            await Task.CompletedTask;
        }

        /// <summary>
        /// Navigate from page
        /// </summary>
        /// <param name="args"></param>
        /// <returns>Navigate from Task</returns>
        public override async Task OnNavigatingFromAsync(NavigatingEventArgs args)
        {
            args.Cancel = false;
            await Task.CompletedTask;
        }
    }
}
