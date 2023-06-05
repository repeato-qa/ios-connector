using System;

using ObjCRuntime;
using Foundation;
using UIKit;


namespace RepeatoCapture
{
    [BaseType(typeof(NSObject))]
    public interface RepeatoCapture
    {
        [Static]
        [Export("load")]
        void Load();
    }

    [BaseType(typeof(NSObject))]
    public interface InfoMessages
    {
        [Export("showAlert")]
        void ShowAlert();

        [Export("noLaunchArgumentsPassed")]
        void NoLaunchArgumentsPassed();

    }

    [BaseType(typeof(NSObject))]
    public interface Logger
    {
     
    }

}


