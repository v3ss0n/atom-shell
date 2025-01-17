# `<webview>` tag

Use the `webview` tag to embed 'guest' content (such as web pages) in your
atom-shell app. The guest content is contained within the `webview` container;
an embedder page within your app controls how the guest content is laid out and
rendered.

Different from the `iframe`, the `webview` runs in a separate process than your
app; it doesn't have the same permissions as your web page and all interactions
between your app and embedded content will be asynchronous. This keeps your app
safe from the embedded content.

## Example

To embed a web page in your app, add the `webview` tag to your app's embedder
page (this is the app page that will display the guest content). In its simplest
form, the `webview` tag includes the `src` of the web page and css styles that
control the appearance of the `webview` container:

```html
<webview id="foo" src="https://www.github.com/" style="width:640px; height:480px"></webview>
```

If you want to control the guest content in any way, you can write JavaScript
that listens for `webview` events and responds to those events using the
`webview` methods. Here's sample code with two event listeners: one that listens
for the web page to start loading, the other for the web page to stop loading,
and displays a "loading..." message during the load time:

```html
<script>
  onload = function() {
    var webview = document.getElementById("foo");
    var indicator = document.querySelector(".indicator");

    var loadstart = function() {
      indicator.innerText = "loading...";
    }
    var loadstop = function() {
      indicator.innerText = "";
    }
    webview.addEventListener("did-start-loading", loadstart);
    webview.addEventListener("did-stop-loading", loadstop);
  }
</script>
```

## Tag attributes

### src

```html
<webview src="http://www.google.com/"></webview>
```

Returns the visible URL. Writing to this attribute initiates top-level
navigation.

Assigning `src` its own value will reload the current page.

The `src` attribute can also accept data URLs, such as
`data:text/plain,Hello, world!`.

### autosize

```html
<webview src="http://www.google.com/" autosize="on" minwidth="576" minheight="432"></webview>
```

If "on", the `webview` will container will automatically resize within the
bounds specified by the attributes `minwidth`, `minheight`, `maxwidth`, and
`maxheight`. These contraints do not impact the `webview` UNLESS `autosize` is
enabled. When `autosize` is enabled, the `webview` container size cannot be less
than the minimum values or greater than the maximum.

### nodeintegration

```html
<webview src="http://www.google.com/" nodeintegration></webview>
```

If "on", the guest page in `webview` will have node integration and can use node
APIs like `require` and `process` to access low level system resources.

## Methods

### `<webview>`.getUrl()

Returns URL of guest page.

### `<webview>`.getTitle()

Returns the title of guest page.

### `<webview>`.isLoading()

Returns whether guest page is still loading resources.

### `<webview>`.isWaitingForResponse()

Returns whether guest page is waiting for a first-response for the main resource
of the page.

### `<webview>`.stop()

Stops any pending navigation.

### `<webview>`.reload()

Reloads guest page.

### `<webview>`.reloadIgnoringCache()

Reloads guest page and ignores cache.

### `<webview>`.canGoBack()

Returns whether guest page can go back.

### `<webview>`.canGoForward()

Returns whether guest page can go forward.

### `<webview>`.canGoToOffset(offset)

* `offset` Integer

Returns whether guest page can go to `offset`.

### `<webview>`.goBack()

Makes guest page go back.

### `<webview>`.goForward()

Makes guest page go forward.

### `<webview>`.goToIndex(index)

* `index` Integer

Navigates to the specified absolute index.

### `<webview>`.goToOffset(offset)

* `offset` Integer

Navigates to the specified offset from the "current entry".

### `<webview>`.isCrashed()

Whether the renderer process has crashed.

### `<webview>`.setUserAgent(userAgent)

* `userAgent` String

Overrides the user agent for guest page.

### `<webview>`.insertCSS(css)

* `css` String

Injects CSS into guest page.

### `<webview>`.executeJavaScript(code)

* `code` String

Evaluate `code` in guest page.

### `<webview>`.send(channel[, args...])

* `channel` String

Send `args..` to guest page via `channel` in asynchronous message, the guest
page can handle it by listening to the `channel` event of `ipc` module.

See [WebContents.send](browser-window.md#webcontentssendchannel-args) for
examples.

## DOM events

### did-finish-load

Fired when the navigation is done, i.e. the spinner of the tab will stop
spinning, and the `onload` event was dispatched.

### did-fail-load

* `errorCode` Integer
* `errorDescription` String

This event is like `did-finish-load`, but fired when the load failed or was
cancelled, e.g. `window.stop()` is invoked.

### did-frame-finish-load

* `isMainFrame` Boolean

Fired when a frame has done navigation.

### did-start-loading

Corresponds to the points in time when the spinner of the tab starts spinning.

### did-stop-loading

Corresponds to the points in time when the spinner of the tab stops spinning.

### did-get-redirect-request

* `oldUrl` String
* `newUrl` String
* `isMainFrame` Boolean

Fired when a redirect was received while requesting a resource.

### console-message

* `level` Integer
* `message` String
* `line` Integer
* `sourceId` String

Fired when the guest window logs a console message.

The following example code forwards all log messages to the embedder's console
without regard for log level or other properties.

```javascript
webview.addEventListener('console-message', function(e) {
  console.log('Guest page logged a message: ', e.message);
});
```

### new-window

* `url` String
* `frameName` String

Fired when the guest page attempts to open a new browser window.

The following example code opens the new url in system's default browser.

```javascript
webview.addEventListener('new-window', function(e) {
  require('shell').openExternal(e.url);
});
```

### close

Fired when the guest window attempts to close itself.

The following example code navigates the `webview` to `about:blank` when the
guest attempts to close itself.

```javascript
webview.addEventListener('close', function() {
  webview.src = 'about:blank';
});
```

### crashed

Fired when the renderer process is crashed.

### destroyed

Fired when the WebContents is destroyed.
