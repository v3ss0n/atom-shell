// Copyright (c) 2013 GitHub, Inc. All rights reserved.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef ATOM_BROWSER_API_ATOM_API_WINDOW_H_
#define ATOM_BROWSER_API_ATOM_API_WINDOW_H_

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "atom/browser/native_window_observer.h"
#include "atom/browser/api/event_emitter.h"
#include "native_mate/handle.h"

class GURL;

namespace mate {
class Arguments;
class Dictionary;
}

namespace atom {

class NativeWindow;

namespace api {

class WebContents;

class Window : public mate::EventEmitter,
               public NativeWindowObserver {
 public:
  static mate::Wrappable* New(v8::Isolate* isolate,
                              const mate::Dictionary& options);

  static void BuildPrototype(v8::Isolate* isolate,
                             v8::Handle<v8::ObjectTemplate> prototype);

  NativeWindow* window() const { return window_.get(); }

 protected:
  explicit Window(const mate::Dictionary& options);
  virtual ~Window();

  // Implementations of NativeWindowObserver:
  void OnPageTitleUpdated(bool* prevent_default,
                          const std::string& title) override;
  void WillCreatePopupWindow(const base::string16& frame_name,
                             const GURL& target_url,
                             const std::string& partition_id) override;
  void WillCloseWindow(bool* prevent_default) override;
  void OnWindowClosed() override;
  void OnWindowBlur() override;
  void OnWindowFocus() override;
  void OnRendererUnresponsive() override;
  void OnRendererResponsive() override;

 private:
  // APIs for NativeWindow.
  void Destroy();
  void Close();
  bool IsClosed();
  void Focus();
  bool IsFocused();
  void Show();
  void ShowInactive();
  void Hide();
  bool IsVisible();
  void Maximize();
  void Unmaximize();
  bool IsMaximized();
  void Minimize();
  void Restore();
  bool IsMinimized();
  void SetFullscreen(bool fullscreen);
  bool IsFullscreen();
  void SetSize(int width, int height);
  std::vector<int> GetSize();
  void SetContentSize(int width, int height);
  std::vector<int> GetContentSize();
  void SetMinimumSize(int width, int height);
  std::vector<int> GetMinimumSize();
  void SetMaximumSize(int width, int height);
  std::vector<int> GetMaximumSize();
  void SetResizable(bool resizable);
  bool IsResizable();
  void SetAlwaysOnTop(bool top);
  bool IsAlwaysOnTop();
  void Center();
  void SetPosition(int x, int y);
  std::vector<int> GetPosition();
  void SetTitle(const std::string& title);
  std::string GetTitle();
  void FlashFrame(bool flash);
  void SetSkipTaskbar(bool skip);
  void SetKiosk(bool kiosk);
  bool IsKiosk();
  void OpenDevTools();
  void CloseDevTools();
  bool IsDevToolsOpened();
  void InspectElement(int x, int y);
  void FocusOnWebView();
  void BlurWebView();
  bool IsWebViewFocused();
  void SetRepresentedFilename(const std::string& filename);
  std::string GetRepresentedFilename();
  void SetDocumentEdited(bool edited);
  bool IsDocumentEdited();
  void CapturePage(mate::Arguments* args);
  void Print(mate::Arguments* args);
  void SetProgressBar(double progress);

  // APIs for WebContents.
  mate::Handle<WebContents> GetWebContents(v8::Isolate* isolate) const;
  mate::Handle<WebContents> GetDevToolsWebContents(v8::Isolate* isolate) const;

  scoped_ptr<NativeWindow> window_;

  DISALLOW_COPY_AND_ASSIGN(Window);
};

}  // namespace api

}  // namespace atom


namespace mate {

template<>
struct Converter<atom::NativeWindow*> {
  static bool FromV8(v8::Isolate* isolate, v8::Handle<v8::Value> val,
                     atom::NativeWindow** out) {
    // null would be tranfered to NULL.
    if (val->IsNull()) {
      *out = NULL;
      return true;
    }

    atom::api::Window* window;
    if (!Converter<atom::api::Window*>::FromV8(isolate, val, &window))
      return false;
    *out = window->window();
    return true;
  }
};

}  // namespace mate

#endif  // ATOM_BROWSER_API_ATOM_API_WINDOW_H_
