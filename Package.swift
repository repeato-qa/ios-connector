// swift-tools-version:5.0
// The swift-tools-version declares the minimum version of Swift required to build this package.
//  $Id: //depot/Remote/Package.swift#10 $
//

import PackageDescription
import Foundation

let hostname = Host.current().name ?? "localhost"

let package = Package(
    name: "RepeatoPlugin",
    platforms: [.iOS("10.0"), .macOS("10.12")],
    products: [
        .library(name: "RepeatoCapture", targets: ["RepeatoCapture"]),
    ],
    dependencies: [
    ],
    targets: [
        .target(name: "RepeatoCapture",
                cSettings: [.define("DEVELOPER_HOST",
                                    to: "\"\(hostname)\"")]),
    ]
)

