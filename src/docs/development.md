# Development

## Compilation environment
Compiling a development edition of NetSurf requires a POSIX style environment, such as GNU/Linux or \*BSD.

## Logging
The [logging](docs/logging.md) interface controls debug and error messages not output through the GUI.

## New frontend development
[Implementing a new frontend](docs/implementing-new-frontend.md) for a toolkit can be challenging and this guide provides an overview and worked example.

## Core window
The [core window API](docs/core-window-interface.md) allows frontends to use generic core code for user interface elements beyond the browser render.

## Source object caching
The [source object caching](docs/source-object-backing-store.md) provides a way for downloaded content to be kept on a persistent storage medium such as hard disc to make future retrieval of that content quickly.

## Javascript
Javascript provision is split into four parts:
* An engine that takes source code and executes it.
* Interfaces between the program and the web page.
* Browser support to retrieve and manage the source code to be executed.
* Browser support for the dispatch of events from user interface.

## Library
JavaScript is provided by integrating the duktape library. There are [instructions](docs/updating-duktape.md) on how to update the library.

## Interface binding
In order for javascript programs to to interact with the page contents it must use the Document Object Model (DOM) and Cascading Style Sheet Object Model (CSSOM) API.

These interfaces are described using web Interface Description Language (IDL) within the relevant specifications (e.g. https://dom.spec.whatwg.org/).

Each interface described by the webIDL must be bound (connected) to the browsers internal representation for the DOM or CSS, etc. The process of [writing bindings](docs/jsbinding.md) is ongoing.
