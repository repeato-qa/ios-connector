# iOS Connector for Repeato Studio and Repeato CLI

This open source library allows remote controlling iOS devices / simulators from an automation host.
The connector library needs to be embedded into your app and takes care of transmitting and receiving data from [Repeato Studio](https://www.repeato.app) or [Repeato CLI](https://www.npmjs.com/package/@repeato/cli-testrunner).

# How does the connector work?

When the app is launched, the connector tries to connect to the host (Repeato Studio or Repeato CLI) via a websocket connection. If the connection is established, the connector starts listening for incoming commands and executes them. The connector also sends back a video feed to the host.

When the app is lauched on an iOS Simulators, the connector simply connects to localhost:1313. 
When the app is launched on a physical device however, the connector does not know where to connect to (localhost doesn't work in this case). It's going to try to connect to the host the app was originally built on, but that doesn't always work.

Therefore Repeato passes it's own host IP to the app via launch arguments. The connector then tries to connect to the host IP. 
In case of a connection error, the connector shows a dialog to the user to help them debugging the issue.

So when working with phyisical iOS devices, make sure you start the app via Repeato (either via a "Start app" step, or via the "Start last used app" button).
Manual app launches most probably will not work.

# Installation

The connector can be installed via Swift Package Manager, CocoaPods or Xamarin binding.

## Installation via Swift Package Manager

### 1. Make sure requirements are fullfilled

- Operating system: Mac OS
- Xcode installed
- Xcrun installed
- Simctl installed
- Source code of your app needs to be available on your machine. If you don't have access to the source code, ask the developer to integrate the Repeato plugin for you
- Optional (only for physical device control): [idb](https://fbidb.io/) installed

[Repeato Studio](https://www.repeato.app) provides a step by step tutorial and assists with setting things up. Just click "Troubles with connecting"...

![Repeato Studio](/docs/assets/repeato-studio-help1.png "Repeato Studio")

...and the setup wizard is started:
![Repeato Studio](/docs/assets/repeato-studio-help2.png "Repeato Studio setup wizard")

But for the sake of completenes we provide the installation instructions also here (in case you don't have Repeato Studio installed and you are integrating the plugin into the app for someone else).

👉 Please make sure your app project is opened in Xcode before proceeding. 👈

### 2. Add library to "Swift packages" in Xcode

In the screenshot below you can see how we locate the swift packages in the app "MovieSwift" and then click the "+" button:

![Locate the swift package section](/docs/assets/connect-ios1-xcode13.png "Swift package section in Xcode")

Next, paste following URL to the input field: `https://github.com/repeato-qa/ios-connector`

![Paste URL into the search field](/docs/assets/connect-ios2-xcode13.png)

Select branch "main" for "Dependency rule", then click "Add Package":

![Paste URL into the search field](/docs/assets/connect-ios3-xcode13.png)

In the next step just click "Add Package" once more.

### 3. Build an launch app

That's it. Just build and run your project via Xcode on the simulator. Repeato connector will find your Repeato instance, and connect to it.

## Installation via CocoaPods

Add following line to your Podfile:

```
platform :ios, '11.0'
use_frameworks!
pod 'Repeato', :git => 'https://github.com/repeato-qa/ios-connector.git', :branch => 'main'
```

By default cocoa pod dependencies are installed in all the build configurations of the target. Since **we don't recommend to include the connector into a production build** of your app, there is a way to only use it in certain configurations.

```
platform :ios, '11.0'
use_frameworks!
pod 'Repeato', :git => 'https://github.com/repeato-qa/ios-connector.git', :configurations => ['Debug', 'Beta'], :branch => 'main'
```

## Installation for Xamarin Apps on iOS

On Android Repeato connects to the app automatically. No need to install a connector.
On iOS however, the connector needs to be installed into the app:

1. Download the [RepeatoCapture.dll](/xamarin-bindings/Test/Test/Libraries/RepeatoCapture.dll) from our repository.
2. Open your Xamarin project in Xamarin or Visual Studio
3. Right click "References" in the "solution" pane and select "Add project reference"
4. Open the ".NET Assembly" tab
5. Select the RepeatoCapture.dll you downloaded in step 1
6. Rebuild and run your project

# Implementation details
(mostly internal documentation for our dev team)

## Connection debug dialog shown on device - how does it work?
The connector shows a dialog on launch of the app to simplify debugging for the user in case of a connection issue. As soon as the connector connects to Repeato, the dialog is automatically closed.

Here are the details of how this dialog operates:

1. If no connection possible on real device: show dialog with countdown and close app
2. If no connection possible on simulator: show dialog with log, but no countdown and no app close
3. If connection established: hide dialog and stop logging to text view
4. If connection breaks on real device: show dialog with countdown and close app (in the future, we should try to reconnect automatically)
5. If connection breaks on simulator: show dialog, but no countdown and no app close (in the future, we should try to reconnect automatically)
6. If "Cancel" button is pressed, countdown and cancel button should be hidden and and app close should be canceled. Dialog should stay open

**To sum it up**: It doesn't matter if host IP is known or not and if launch args are given or not: We show the dialog when there is no connection to the host (either because it is not yet established or it was not possible or the connection broke)
Additionally we show a countdown and close the app if no connection is there BUT only on physical devices.

## Xamarin - How to create/update the bindings library.


1. Install the latest version of sharpie: https://docs.microsoft.com/en-us/xamarin/cross-platform/macios/binding/objective-sharpie/get-started#installing
2. Download Library and rename RepeatoImpl.m to RepeatoCapture.h
3. Create Fat library (.a) with next Make file
```
XBUILD=/Applications/Xcode.app/Contents/Developer/usr/bin/xcodebuild
PROJECT_ROOT=./RepeatoCapture
PROJECT=$(PROJECT_ROOT)/RepeatoCapture.xcodeproj
TARGET=RepeatoCapture


all: lib$(TARGET).a

lib$(TARGET)-x86_64.a:
	$(XBUILD) -project $(PROJECT) -target $(TARGET) -sdk iphonesimulator -configuration Release clean build
	-mv $(PROJECT_ROOT)/build/Release-iphonesimulator/lib$(TARGET).a $@

lib$(TARGET)-arm64.a:
	$(XBUILD) -project $(PROJECT) -target $(TARGET) -sdk iphoneos -arch arm64 -configuration Release clean build
	-mv $(PROJECT_ROOT)/build/Release-iphoneos/lib$(TARGET).a $@

lib$(TARGET).a: lib$(TARGET)-x86_64.a lib$(TARGET)-arm64.a
	xcrun -sdk iphoneos lipo -create -output $@ $^

clean:
	-rm -f *.a *.dll

```
4. Call sharpie command that would gererate ApiDefinition.cs and Structs.cs
```
sharpie bind -output RepeatoCaptureLibrary -namespace  RepeatoCapture -sdk iphoneos16.2 RepeatoCapture/build/Release-iphoneos/include/RepeatoCapture/RepeatoCapture.h  -scope RepeatoCapture/build/Release-iphoneos/include/RepeatoCapture 
```

//iphoneos16.2 - change to sdk that XCode supports

5. Copy generated code into Xamarin Bindings Lib project
6. Include/Replace Fat Lib into project as well
7. Call Build command for the solution
8. Copy gererated lib from bin/iPhone and add as reference into Xamarin.iOS project
