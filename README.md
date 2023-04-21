# iOS Connector for Repeato Studio and Repeato CLI

This library allows remote controlling iOS devices / simulators from an automation host.
This connector library needs to be embedded into your app and takes care of transmitting and receiving data from [Repeato Studio](https://www.repeato.app) or [Repeato CLI](https://www.npmjs.com/package/@repeato/cli-testrunner).

# How does the connector work?

When the app is launched, the connector tries to connect to the host (Repeato Studio or Repeato CLI) via a websocket connection. If the connection is established, the connector starts listening for incoming commands and executes them. The connector also sends back a video feed to the host.

When the app is lauched on an iOS Simulators, the connector simply connects to localhost:1313. 
When the app is launched on a physical device however, the connector does not know where to connect to (localhost doesn't work in this case). It's going to try to connect to the host the app was originally built on, but that doesn't always work.

Therefore Repeato passes it's own host IP to the app via launch arguments. The connector then tries to connect to the host IP. 
In case of a connection error, the connector shows a dialog to the user to help them debugging the issue.

So when working with phyisical iOS devices, make sure you start the app via Repeato (either via a "Start app" step, or via the "Start last used app" button).
Manual app launches most probably will not work.

# Installation

The connector can be installed via Swift Package Manager or CocoaPods.

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

Then run `pod install` in your project directory.

# Implementation details

(mostly internal documentation for our dev team)

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