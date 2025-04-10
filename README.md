# iOS Connector for Repeato Studio and Repeato CLI

This open source library allows remote controlling iOS devices / simulators from an automation host.

The connector library needs to be embedded into your app and takes care of transmitting and receiving data from [Repeato Studio](https://www.repeato.app) or [Repeato CLI](https://www.npmjs.com/package/@repeato/cli-testrunner).

> This is the new connector that works a bit differently than the old one (v1.2.x, see documentation [here](https://github.com/repeato-qa/ios-connector/tree/1.2.8)). 

# How does the connector work?

When the app is launched, the connector will open a server port that an automation host can connect to. As soon as a connection is established, the connector waits for incoming commands and executes them. The connector also sends back a video feed to the host (single jpeg frames).

The port that the connector listens on can be configured via the `-port`launch argument. By default, the connector listens on port 1313.

As long as there is no connection, the connector shows a dialog to the user to help them to debug potential issues.

Make sure that you always launch the app via Repeato, which will take care of passing the right `-port` argument before establishing a connection.


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

That's it. Just build and run your project via Xcode on the simulator. If you (re) start the app via Repeato, the connector will be started automatically and Repeato can connect to it.

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

## Installation for React-Native Apps

On Android Repeato connects to the app automatically. No need to install a connector.
On iOS however, the connector needs to be installed into the app.

On newer versions of react-native there isn't really much to do. Just include the swift package via the dialog in Xcode, like described above.

On older react-native projects you might have to eject the app before you can include the swift package. Here is how you do it:

1. "Eject" your react-native app to generate your native iOS app project: `npx react-native eject`
2. Open the "iOS" folder of your app 
3. Double click the "...xcworkspace" file to open the project in Xcode
4. Follow the guide above to add the swift package to your project

## Installation for Xamarin Apps on iOS

On Android Repeato connects to the app automatically. No need to install a connector.
On iOS however, the connector needs to be installed into the app:

1. Download the [RepeatoCapture.dll](/xamarin-bindings/Test/Test/Libraries/RepeatoCapture.dll) from our repository.
2. Open your Xamarin project in Xamarin or Visual Studio
3. Right click "References" in the "solution" pane and select "Add project reference"
4. Open the ".NET Assembly" tab
5. Select the RepeatoCapture.dll you downloaded in step 1
6. Rebuild and run your project

# Trouble shooting

## App does not connect to Repeato-Studio

Check: When you launch the app, is this dialog shown?


<img src="/docs/assets/ios-connector-dialog.png" width="50%" />

**If YES**: The connector seems to be installed correctly, but for some reason Repeato might not be able to connect to it. Make sure that your device and Repeato run on the same network.

**If NO**: You might need to check if the plugin is integrated properly. If you are not a developer, you might need to talk to one. The plugin is open source, so they might easily find the reason why the dialog is not shown.

# Implementation details
(mostly internal documentation for our dev team)

## Connection debug dialog shown on device - how does it work?
The connector shows a dialog on launch of the app to simplify debugging for the user in case of a connection issue. As soon as a connection is established, the dialog is automatically closed.

Here are the details of how this dialog operates:

1. If no connection possible: show dialog
2. If connection established: hide dialog and stop logging to text view
3. If connection breaks: show dialog


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

## How to change the iOS connector implemenation and test it

Instead of importing a swift dependency via the dialog where you paste the repo url, it's also possible to use a local clone.
1. In XCode select your project in the left pane
2. Navigate to "Swift Packages"
3. Click the "+" icon
4. paste the repository URL https://github.com/repeato-qa/ios-connector.git
5. Select the right branch (at time of writing that's "remote-optimised")
6. Click "Next", select "RemoteCapture" and "Finish"
7. Clone https://github.com/repeato-qa/ios-connector.git to your disk and select the right branch
8. Drag the "Remote" folder from finder into the iOS app project*
9. Change the source code in the cloned repository and run your app project
   
* There seems to be a bug though:
In some cases, the package will show up in the project pane, but without the disclosure triangle, and the library it contained wouldn't show up when I tried to choose it with "Link Binary with Libraries".
The solution I found was to close the Xcode project after dropping the Swift package. On reopening the project, the disclosure triangle appeared, and everything else worked.
This is in Xcode 12.4.